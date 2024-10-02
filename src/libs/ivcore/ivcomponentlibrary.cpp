#include "ivcomponentlibrary.h"

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

    copyImplementation(ivDir, targetDir, objects);
    shared::QMakeFile::createProFileForDirectory(targetPath, asn1FilesPathsExternal);

    return resetTasteENV(targetPath);
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
