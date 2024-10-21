#include "ivcomponentlibrary.h"

#include "ivxmlreader.h"

#include <QDir>
#include <QFileSystemWatcher>
#include <archetypes/archetypemodel.h>
#include <diskutils.h>
#include <errorhub.h>
#include <externalprocess.h>
#include <ivcore/ivxmlwriter.h>
#include <ivfunction.h>
#include <ivmodel.h>
#include <ivobject.h>
#include <ivpropertytemplateconfig.h>
#include <qmakefile.h>
#include <vemodel.h>

namespace ivm {

struct IVComponentLibrary::IVComponentLibraryPrivate {
    QString modelName;
    QString libraryPath;
    QHash<shared::Id, QSharedPointer<IVComponentLibrary::Component>> components;
    QFileSystemWatcher watcher;
};

IVComponentLibrary::IVComponentLibrary(const QString &path, const QString &modelName)
    : d(new IVComponentLibraryPrivate)

{
    d->libraryPath = path;
    d->modelName = modelName;

    connect(&d->watcher, &QFileSystemWatcher::directoryChanged, this, [this](const QString &path) {
        if (path != d->libraryPath)
            return;

        /// Get all interfaceview paths from fs
        const QDir libDir(d->libraryPath);
        const QStringList entries = libDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        QSet<QString> componentsPaths;
        for (const QString &name : std::as_const(entries)) {
            const QString relInterfaceviewPath = name + QDir::separator() + shared::kDefaultInterfaceViewFileName;
            if (libDir.exists(relInterfaceviewPath)) {
                componentsPaths.insert(libDir.absoluteFilePath(relInterfaceviewPath));
            }
        }

        /// Get all loaded components with removing them in previously loaded paths
        QSet<QString> existingComponents;
        for (auto it = d->components.cbegin(); it != d->components.cend(); ++it) {
            if (!componentsPaths.remove(it.value()->componentPath)) {
                existingComponents.insert(it.value()->componentPath);
            }
        }

        /// Remove from the model non-existing in fs component
        QList<shared::Id> idsToRemove;
        for (const QString &path : std::as_const(existingComponents)) {
            for (auto it = d->components.cbegin(); it != d->components.cend(); ++it) {
                if (it.value()->componentPath == path) {
                    idsToRemove << it.key();
                    removeComponent(it.key());
                    break;
                }
            }
        }
        Q_EMIT componentsToBeRemovedFromModel(idsToRemove);

        /// Add to the model new components
        Q_EMIT componentsToBeLoaded(componentsPaths);
    });

    connect(&d->watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &path) {
        for (auto it = d->components.cbegin(); it != d->components.cend(); ++it) {
            if ((*it)->componentPath == path) {
                Q_EMIT componentUpdated(it.key());
                break;
            }
        }
    });
}

QString IVComponentLibrary::libraryPath() const
{
    return d->libraryPath;
}

static inline void copyImplementation(
        const QDir &projectDir, const QDir &targetDir, const QVector<ivm::IVObject *> &objects)
{
    for (ivm::IVObject *object : objects) {
        if (auto fn = object->as<ivm::IVFunctionType *>()) {
            const QString subPath = shared::kRootImplementationPath + QDir::separator() + object->title().toLower();
            shared::copyDir(projectDir.filePath(subPath), targetDir.filePath(subPath));
            copyImplementation(projectDir, targetDir, fn->children());
        }
    }
}

bool IVComponentLibrary::exportComponent(const QString &targetPath, const QList<IVObject *> objects,
        const QString &projectDir, QStringList asn1FilesPaths, QStringList asn1FilesPathsExternal,
        ivm::ArchetypeModel *archetypesModel)
{

    const bool ok = shared::ensureDirExists(targetPath);
    if (!ok) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Unable to create directory %1").arg(targetPath));
        return false;
    }

    QDir targetDir(targetPath);
    IVXMLWriter exporter;
    connect(&exporter, &IVXMLWriter::exported, this, &IVComponentLibrary::componentExported);
    if (!exporter.exportObjectsSilently(objects, targetDir.filePath(shared::kDefaultInterfaceViewFileName),
                archetypesModel, exporter.templatePath(QLatin1String("interfaceview.ui")))) {
        return false;
    }

    const QFileInfo ivPath(projectDir);
    const QDir ivDir = ivPath.absoluteDir();
    const QString workDir = QDir(ivDir.canonicalPath() + "/work/").absolutePath();
    for (const QString &asnFile : asn1FilesPaths) {
        if (asnFile.startsWith(workDir)) { // ignore generated .asn file
            continue;
        }
        QFileInfo fi(asnFile);
        const QString filename = fi.fileName();
        if (!QFile::copy(ivDir.filePath(filename), targetDir.filePath(filename))) {
            shared::ErrorHub::addError(
                    shared::ErrorItem::Error, tr("Error during ASN.1 file copying: %1").arg(asnFile));
        }
    }
    copyImplementation(ivDir, targetDir, objects);
    shared::QMakeFile::createProFileForDirectory(targetPath, asn1FilesPathsExternal);

    return resetTasteENV(targetPath);
}

void IVComponentLibrary::removeComponent(const shared::Id &id)
{
    auto component = d->components.take(id);
    if (!component.isNull()) {
        auto idsToRemove = component->rootIds;
        d->components.removeIf(
                [&idsToRemove](QHash<shared::Id, QSharedPointer<IVComponentLibrary::Component>>::Iterator it) {
                    return idsToRemove.contains(it.key());
                });
        d->watcher.removePath(component->componentPath);

        QDir dir(QFileInfo(component->componentPath).absolutePath());
        if (dir.exists()) {
            dir.removeRecursively();
        }
    }
}

QSharedPointer<ivm::IVComponentLibrary::Component> IVComponentLibrary::loadComponent(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        qDebug() << path << "doesn't exist";
        return nullptr;
    }

    shared::ErrorHub::setCurrentFile(path);
    ivm::IVXMLReader parser;
    if (!parser.readFile(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, parser.errorString(), path);
        shared::ErrorHub::clearCurrentFile();
        return nullptr;
    }

    QSharedPointer<ivm::IVModel> model { new ivm::IVModel(ivm::IVPropertyTemplateConfig::instance()) };
    model->initFromObjects(parser.parsedObjects(), parser.externalAttributes());

    shared::ErrorHub::clearCurrentFile();
    if (!anyLoadableIVObjects(model->ivobjects().values())) {
        return nullptr;
    }

    static const QStringList asn1extensions { QLatin1String("asn1"), QLatin1String("asn"), QLatin1String("acn") };
    const QFileInfo fi(path);
    const QDir dir = fi.absoluteDir();
    auto asn1Files = shared::QMakeFile::readFilesList(
            dir.absoluteFilePath(dir.dirName() + QLatin1String(".pro")), asn1extensions);

    auto component = createComponent(path, asn1Files, rootIds(model->ivobjects().values()), model);
    addComponent(component);
    return component;
}

QList<shared::Id> IVComponentLibrary::rootIds(QVector<IVObject *> objects)
{
    QList<shared::Id> ids;
    std::for_each(objects.constBegin(), objects.constEnd(), [&objects, &ids](IVObject *obj) {
        if (!obj->parentObject()) {
            ids.append(obj->id());
        }
    });
    return ids;
}

QSharedPointer<IVComponentLibrary::Component> IVComponentLibrary::createComponent(const QString &componentPath,
        const QStringList &asn1Files, const QList<shared::Id> &rootIds, QSharedPointer<IVModel> model)
{
    QSharedPointer<ivm::IVComponentLibrary::Component> component { new ivm::IVComponentLibrary::Component };
    component->componentPath = componentPath;
    component->asn1Files = asn1Files;
    component->rootIds = rootIds;
    component->model.swap(model);
    return component;
}

QVector<IVObject *> IVComponentLibrary::rootObjects(QVector<IVObject *> objects)
{
    QVector<IVObject *> rootObjs;
    std::for_each(objects.constBegin(), objects.constEnd(), [&objects, &rootObjs](IVObject *obj) {
        if (!obj->parentObject()) {
            rootObjs.append(obj);
        }
    });
    return rootObjs;
}

QSharedPointer<IVComponentLibrary::Component> IVComponentLibrary::component(const shared::Id &id) const
{
    auto it = d->components.constFind(id);
    if (it != d->components.constEnd()) {
        return (*it);
    }
    return nullptr;
}

QStringList IVComponentLibrary::asn1Files(const shared::Id &id) const
{
    auto comp = component(id);
    if (!comp.isNull()) {
        return comp->asn1Files;
    }
    return QStringList();
}

QString IVComponentLibrary::componentPath(const shared::Id &id) const
{
    auto comp = component(id);
    if (!comp.isNull()) {
        return comp->componentPath;
    }
    return QString();
}

QString IVComponentLibrary::modelName() const
{
    return d->modelName;
}

QList<shared::Id> IVComponentLibrary::componentsIds()
{
    return d->components.keys();
}

void IVComponentLibrary::unWatchComponent(const QString &componentPath)
{
    d->watcher.removePath(componentPath);
}

void IVComponentLibrary::addComponent(const QSharedPointer<Component> &component)
{
    for (auto id : std::as_const(component->rootIds)) {
        d->components.insert(id, component);
    }
    d->watcher.addPath(component->componentPath);
}

bool IVComponentLibrary::anyLoadableIVObjects(QVector<IVObject *> objects)
{
    return std::any_of(objects.begin(), objects.end(), [](IVObject *obj) {
        auto type = obj->type();
        return (type != ivm::IVObject::Type::InterfaceGroup);
    });
}

bool IVComponentLibrary::resetTasteENV(const QString &path)
{
    std::unique_ptr<QProcess> initTASTECallerProcess = shared::ExternalProcess::create();
    initTASTECallerProcess->setProgram("taste");
    initTASTECallerProcess->setArguments({ "reset" });
    initTASTECallerProcess->setWorkingDirectory(path);
    initTASTECallerProcess->start();
    initTASTECallerProcess->waitForFinished();
    return !(initTASTECallerProcess->exitCode() != 0 || initTASTECallerProcess->exitStatus() != QProcess::NormalExit);
}

}
