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
    QSharedPointer<IVComponentLibrary::Component> component =
            QSharedPointer<IVComponentLibrary::Component>(new Component);
    component->componentPath = targetPath;
    component->asn1Files = asn1FilesPaths;
    for (const auto obj : objects) {
        component->rootIds.append(obj->id());
        d->components.insert(obj->id(), component);
    }
    d->watcher.addPath(component->componentPath);

    copyImplementation(ivDir, targetDir, objects);
    shared::QMakeFile::createProFileForDirectory(targetPath, asn1FilesPathsExternal);

    return resetTasteENV(targetPath);
}

void IVComponentLibrary::removeComponent(const shared::Id &id)
{
    auto component = d->components.value(id);
    for (auto id : component->rootIds) {
        d->components.remove(id);
    }
    d->watcher.removePath(component->componentPath);

    QDir dir(component->componentPath);
    dir.removeRecursively();
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

    std::unique_ptr<ivm::IVModel> model { new ivm::IVModel(ivm::IVPropertyTemplateConfig::instance()) };
    model->initFromObjects(parser.parsedObjects(), parser.externalAttributes());

    shared::ErrorHub::clearCurrentFile();
    if (!anyLoadableIVObjects(model->ivobjects().values())) {
        return nullptr;
    }

    QSharedPointer<ivm::IVComponentLibrary::Component> component { new ivm::IVComponentLibrary::Component };
    component->componentPath = path;
    component->rootIds = rootIds(model->ivobjects().values());
    component->model.swap(model);

    static const QStringList asn1extensions { QLatin1String("asn1"), QLatin1String("asn"), QLatin1String("acn") };
    const QFileInfo fi(path);
    const QDir dir = fi.absoluteDir();
    component->asn1Files = shared::QMakeFile::readFilesList(
            dir.absoluteFilePath(dir.dirName() + QLatin1String(".pro")), asn1extensions);

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

void IVComponentLibrary::addComponent(const QSharedPointer<Component> &component)
{
    for (auto id : std::as_const(component->rootIds)) {
        d->components.insert(id, component);
    }
    d->watcher.addPath(component->componentPath);
}

bool IVComponentLibrary::anyLoadableIVObjects(QVector<IVObject *> objects)
{
    return std::any_of(objects.begin(), objects.end(),
            [](IVObject *obj) { return (obj && obj->type() != ivm::IVObject::Type::InterfaceGroup); });
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
