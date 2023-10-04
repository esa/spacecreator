/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "interfacedocument.h"

#include "commands/cmdconnectionlayermanage.h"
#include "commandsstack.h"
#include "common.h"
#include "context/action/actionsmanager.h"
#include "context/action/editor/dynactioneditor.h"
#include "diskutils.h"
#include "itemeditor/ivitemmodel.h"
#include "ivcomponentmodel.h"
#include "ivexporter.h"
#include "ivvisualizationmodelbase.h"
#include "qmakefile.h"

#include <QAction>
#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDir>
#include <QDirIterator>
#include <QGraphicsScene>
#include <QInputDialog>
#include <QItemSelectionModel>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QSplitter>
#include <QUndoStack>
#include <QVBoxLayout>
#include <algorithm>
#include <asn1library/asn1modelstorage.h>
#include <asn1library/asn1systemchecks.h>
#include <common/exceptions/inconsistentmodelexception.h>
#include <filesystem>
#include <ivcore/abstractsystemchecks.h>
#include <ivcore/archetypes/archetypelibrary.h>
#include <ivcore/archetypes/archetypemodel.h>
#include <ivcore/archetypes/archetypeobject.h>
#include <ivcore/archetypes/archetypexmlreader.h>
#include <ivcore/archetypes/functionarchetype.h>
#include <ivcore/ivarchetypelibraryreference.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivnamevalidator.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <ivcore/ivxmlreader.h>
#include <shared/colors/colormanagerdialog.h>
#include <shared/errorhub.h>
#include <shared/propertytemplatemanager.h>
#include <shared/propertytemplatewidget.h>
#include <shared/ui/veinteractiveobject.h>

namespace ive {

struct InterfaceDocument::InterfaceDocumentPrivate {

    ~InterfaceDocumentPrivate()
    {
        if (objectsVisualizationModel) {
            objectsVisualizationModel->clear();
        }
        delete objectsVisualizationModel;
        objectsVisualizationModel = nullptr;

        if (itemsModel) {
            itemsModel->clearScene();
        }
        delete itemsModel;
        itemsModel = nullptr;

        if (objectsModel) {
            objectsModel->clear();
        }
    }

    cmd::CommandsStack *commandsStack { nullptr };

    QString filePath;
    QString creatorGitHash;
    QString requestsURL;

    ivm::IVPropertyTemplateConfig *dynPropConfig { nullptr };
    IVItemModel *itemsModel { nullptr };
    IVVisualizationModelBase *objectsVisualizationModel { nullptr };
    QItemSelectionModel *objectsSelectionModel { nullptr };
    ivm::IVModel *objectsModel { nullptr };
    IVExporter *exporter { nullptr };
    ivm::IVModel *layersModel { nullptr };
    IVVisualizationModelBase *layerSelect { nullptr };
    ivm::ArchetypeModel *archetypesModel { nullptr };
    Asn1Acn::Asn1SystemChecks *asnCheck { nullptr };
    ivm::AbstractSystemChecks *ivCheck { nullptr };
    QString projectName;
    QString mscFileName;
    QString uiFileName { shared::kDefaultInterfaceViewUIFileName };
    QStringList asnFilesNames;
    IVComponentModel *componentsModel;
    IVComponentModel *sharedTypesModel;
};

void InterfaceDocument::checkReferencedASN1Files(ivm::IVObject *object)
{
    if (!object || object->parentObject()
            || !(object->type() == ivm::IVObject::Type::Function
                       || object->type() == ivm::IVObject::Type::FunctionType)) {
        return;
    }

    const QStringList objPath = ivm::IVObject::path(object);
    const QString rootName = objPath.isEmpty() ? object->title() : objPath.front();
    const QString sourcePrefix = shared::componentsLibraryPath();
    const QDir sourceDir { sourcePrefix + QDir::separator() + rootName };
    const QDir targetDir { QFileInfo(d->filePath).absolutePath() };

    const QList<QFileInfo> asnFiles = sourceDir.entryInfoList(
            { QLatin1String("*.asn1"), QLatin1String("*.asn"), QLatin1String("*.acn") }, QDir::Files);

    QVector<QFileInfo> fileInfos;
    std::copy_if(asnFiles.begin(), asnFiles.end(), std::back_inserter(fileInfos), [targetDir](const QFileInfo &fi) {
        const QString destFilePath { targetDir.filePath(fi.fileName()) };
        return !shared::isSame(destFilePath, fi.absoluteFilePath());
    });

    QStringList newAsnFiles;
    QStringList importedAsnFiles;
    for (const QFileInfo &file : qAsConst(fileInfos)) {
        QString destFilePath { targetDir.filePath(file.fileName()) };
        const bool isNewFile = !QFile::exists(destFilePath);
        if (!isNewFile && !QFile::remove(destFilePath)) {
            importedAsnFiles.append(destFilePath);
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("%1 wasn't imported").arg(file.fileName()));
            continue;
        }
        if (shared::copyFile(file.absoluteFilePath(), destFilePath)) {
            importedAsnFiles.append(destFilePath);
            if (isNewFile)
                newAsnFiles.append(destFilePath);
        } else {
            if (QFile::exists(destFilePath))
                importedAsnFiles.append(destFilePath);
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("%1 wasn't imported").arg(file.fileName()));
        }
    }

    Q_EMIT d->commandsStack->asn1FilesImported(newAsnFiles);
}

/*!
\class ive::InterfaceDocument
\brief ive::InterfaceDocument is the document for graphical IV data (loaded from the XML).
*/

InterfaceDocument::InterfaceDocument(QObject *parent)
    : QObject(parent)
    , d(new InterfaceDocumentPrivate)
{
    d->commandsStack = new cmd::CommandsStack(this);
    connect(d->commandsStack, &cmd::CommandsStack::cleanChanged, this,
            [this](bool clean) { Q_EMIT dirtyChanged(!clean); });

    d->dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    d->dynPropConfig->init(shared::interfaceCustomAttributesFilePath());

    d->componentsModel = new IVComponentModel(IVComponentModel::Type::ComponentLibrary, tr("Components"), this);
    d->sharedTypesModel = new IVComponentModel(IVComponentModel::Type::SharedTypesLibrary, tr("Shared Types"), this);
    d->objectsModel = new ivm::IVModel(d->dynPropConfig, this);
    d->objectsModel->setObjectRequester(
            std::bind(&IVComponentModel::getObject, d->componentsModel, std::placeholders::_1));
    d->objectsModel->setSharedTypeRequester([this]() -> QVector<ivm::IVFunctionType *> {
        QVector<ivm::IVFunctionType *> fnTypes;
        for (const shared::Id &id : d->sharedTypesModel->componentIDs()) {
            const QHash<shared::Id, shared::VEObject *> objects = d->sharedTypesModel->component(id)->model->objects();
            for (auto it = objects.constBegin(); it != objects.constEnd(); ++it) {
                if (auto ivObj = qobject_cast<ivm::IVObject *>(*it)) {
                    if (ivObj->type() == ivm::IVObject::Type::FunctionType)
                        fnTypes << ivObj->as<ivm::IVFunctionType *>();
                }
            }
        }
        return fnTypes;
    });
    d->layersModel = new ivm::IVModel(d->dynPropConfig, this);
    d->archetypesModel = new ivm::ArchetypeModel(this);

    connect(d->objectsModel, &ivm::IVModel::objectsAdded, this, [this](const QVector<shared::Id> &objectsIds) {
        for (const shared::Id &id : objectsIds) {
            if (auto obj = d->objectsModel->getObject(id)) {
                if (obj->isReference()) {
                    checkReferencedASN1Files(obj);
                }
            }
        }
    });
}

InterfaceDocument::~InterfaceDocument()
{
    delete d;
}

void InterfaceDocument::init()
{
    // Create view models, as the can't handle filled models at creations
    itemsModel();
    visualisationModel();
    componentModel();
    sharedTypesModel();
    layerVisualisationModel();

    QTimer::singleShot(0, this, &InterfaceDocument::loadAvailableComponents);
}

IVExporter *InterfaceDocument::exporter() const
{
    if (!d->exporter) {
        d->exporter = new IVExporter(const_cast<InterfaceDocument *>(this));
    }
    return d->exporter;
}

QGraphicsScene *InterfaceDocument::scene() const
{
    return itemsModel()->scene();
}

QUndoStack *InterfaceDocument::undoStack() const
{
    Q_ASSERT(d->commandsStack);
    return d->commandsStack->undoStack();
}

cmd::CommandsStack *InterfaceDocument::commandsStack() const
{
    return d->commandsStack;
}

bool InterfaceDocument::create(const QString &path)
{
    bool created = false;
    if (!path.isEmpty()) {
        created = loadImpl(path);
    }
    if (created) {
        d->commandsStack->clear();
    }
    return created;
}

bool InterfaceDocument::load(const QString &path)
{
    if (!d->filePath.isEmpty()) {
        shared::ErrorHub::clearFileErrors(d->filePath);
    }
    const QString oldPath = d->filePath;
    setPath(path);

    bool loaded = false;
    try {
        loaded = loadImpl(path);
        if (loaded) {
            d->commandsStack->clear();
        } else {
            setPath(oldPath);
        }
    } catch (InconsistentModelException &e) {
        qDebug() << e.what();
        return false;
    }

    return loaded;
}

/**
 * Does load the imported and shared components.
 * If they are loaded already, nothing is done.
 */
void InterfaceDocument::loadAvailableComponents()
{
    if (d->componentsModel->libraryPath().isEmpty())
        d->componentsModel->load(shared::componentsLibraryPath());
    if (d->sharedTypesModel->libraryPath().isEmpty())
        d->sharedTypesModel->load(shared::sharedTypesPath());
}

QString InterfaceDocument::getComponentName(const QStringList &exportNames)
{
    QString name = exportNames.join(QLatin1Char('_'));
    auto dialog = new QDialog();
    dialog->setWindowTitle(tr("Export"));
    auto layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("Enter the name for exporting component:"), dialog));
    auto lineEdit = new QLineEdit(dialog);
    lineEdit->setText(name);
    lineEdit->selectAll();
    auto box = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    connect(box, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
    layout->addWidget(lineEdit);
    layout->addWidget(box);
    dialog->setLayout(layout);
    dialog->adjustSize();
    if (dialog->exec() == QDialog::Accepted) {
        name = lineEdit->text();
    }
    dialog->deleteLater();
    return name;
}

QList<shared::VEObject *> InterfaceDocument::prepareSelectedObjectsForExport(QString &name, bool silent)
{
    QList<shared::VEObject *> objects;
    QStringList exportNames;
    for (const auto id : d->objectsSelectionModel->selection().indexes()) {
        const int role = static_cast<int>(ive::IVVisualizationModelBase::IdRole);
        if (ivm::IVObject *object = d->objectsModel->getObject(id.data(role).toUuid())) {
            if ((object->isFunction() || object->isFunctionType()) && object->parentObject() == nullptr) {
                exportNames.append(object->entityAttributeValue<QString>(QLatin1String("name")));
            }
            objects.append(object);
        }
    }

    name = silent ? exportNames.join(QLatin1Char('_')) : getComponentName(exportNames);
    name = ivm::IVNameValidator::encodeName(ivm::IVObject::Type::Function, name);
    if (exportNames.size() > 1) {
        ivm::IVFunction *dummyFunction = new ivm::IVFunction;
        dummyFunction->setTitle(name);
        for (auto object : objects) {
            if (!object->parentObject()) {
                dummyFunction->addChild(object->as<ivm::IVObject *>());
            }
        }
        objects.prepend(dummyFunction);
    }
    return objects;
}

ivm::IVPropertyTemplateConfig *InterfaceDocument::dynPropConfig() const
{
    return d->dynPropConfig;
}

void InterfaceDocument::updateLayersModel() const
{
    if (layersModel() != nullptr) {
        auto layers = layersModel()->allObjectsByType<ivm::IVConnectionLayerType>();
        bool isDefaultPresent = false;
        for (auto *const layer : layers) {
            if (layer->title() == ivm::IVConnectionLayerType::DefaultLayerName) {
                isDefaultPresent = true;
            }
        }
        if (!isDefaultPresent) {
            auto *cmd = new cmd::CmdConnectionLayerCreate(layersModel(), objectsModel(), true);
            commandsStack()->push(cmd);
        }
        if (objectsModel() != nullptr) {
            objectsModel()->setConnectionLayersModel(layersModel());
        }
    }
}

bool InterfaceDocument::exportSelectedFunctions()
{
    QString name;
    const QList<shared::VEObject *> objects = prepareSelectedObjectsForExport(name);
    if (name.isEmpty()) {
        return false;
    }

    QString path = shared::componentsLibraryPath() + QDir::separator() + name;
    if (exportImpl(path, objects)) {
        d->objectsSelectionModel->clearSelection();
        return true;
    }
    return false;
}

bool InterfaceDocument::exportSelectedType()
{
    const auto indexes = d->objectsSelectionModel->selection().indexes();
    if (indexes.isEmpty()) {
        return false;
    }
    static const int role = static_cast<int>(ive::IVVisualizationModelBase::IdRole);
    ivm::IVObject *rootType = nullptr;
    for (const auto index : indexes) {
        if (ivm::IVObject *object = d->objectsModel->getObject(index.data(role).toUuid())) {
            if (object->isFunctionType() && object->parentObject() == nullptr) {
                if (rootType) {
                    return false;
                }
                rootType = object;
            }
        }
    }
    if (!rootType) {
        return false;
    }

    QString path = shared::sharedTypesPath() + QDir::separator() + rootType->title();
    if (exportImpl(path, { rootType })) {
        d->objectsModel->removeObject(rootType);
        d->objectsSelectionModel->clearSelection();
        return true;
    }
    return false;
}

bool InterfaceDocument::loadComponentModel(
        ivm::IVModel *model, const QString &path, QHash<shared::Id, QString> *pathMappings = nullptr)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        /// Just skip other folders mught be located in that library not related to component model
        // shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Invalid path"), path);
        return false;
    }

    shared::ErrorHub::setCurrentFile(path);
    ivm::IVXMLReader parser;
    if (!parser.readFile(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, parser.errorString(), path);
        shared::ErrorHub::clearCurrentFile();
        return false;
    }

    auto objects = parser.parsedObjects();
    ivm::IVObject::sortObjectList(objects);
    if (pathMappings) {
        std::for_each(objects.constBegin(), objects.constEnd(), [pathMappings, &path](ivm::IVObject *ivObj) {
            if (!ivObj->parentObject())
                pathMappings->insert(ivObj->id(), QFileInfo(path).absolutePath());
        });
    }

    model->setExtAttributes(parser.externalAttributes());
    model->addObjects(objects);
    shared::ErrorHub::clearCurrentFile();
    return true;
}

void InterfaceDocument::close()
{
    d->objectsModel->clear();
    d->layersModel->clear();
    setPath(QString());
    d->commandsStack->clear();
}

/**
 * All hidden items in the current level becomes visible
 */
void InterfaceDocument::showAll()
{
    // Find root item
    objectsModel()->setNestedObjectsVisible();
}

/**
 * hasSelectedItems returns true if the user has selected items in the main scene.
 */
bool InterfaceDocument::hasHideableItems()
{
    QList<ivm::IVObject *> selectedObjects = getSelectedObjects();
    for (ivm::IVObject *selectedObject : selectedObjects) {
        if (selectedObject->isRootObject()) {
            continue; // root object is not hideable
        }
        if (selectedObject->isFunction() || selectedObject->isFunctionType() || selectedObject->isComment()) {
            return true;
        }
    }
    return false;
}

/**
 * All items that are NOT selected in the main scene at its current level is hidden
 */
void InterfaceDocument::hideSelectedItems()
{
    QList<ivm::IVObject *> selectedObjects = getSelectedObjects();
    for (ivm::IVObject *selectedObject : selectedObjects) {
        if (selectedObject->isRootObject()) {
            continue; // root object is not hideable
        }
        bool hide = selectedObject->isFunction() || selectedObject->isFunctionType() || selectedObject->isComment();
        if (hide) {
            selectedObject->setVisible(false);
        }
    }
}

/**
 * @brief Get all the selected IVObjects, including any root object
 * @return
 */
QList<ivm::IVObject *> InterfaceDocument::getSelectedObjects()
{
    auto selectedObjects = QList<ivm::IVObject *>();
    QModelIndexList selection = objectsSelectionModel()->selectedIndexes();
    for (const QModelIndex &idx : selection) {
        QVariant id = idx.data(IVVisualizationModelBase::IdRole);
        QUuid uuid = id.toUuid();
        ivm::IVObject *selectedObject = objectsModel()->getObject(uuid);
        selectedObjects.append(selectedObject);
    }
    return selectedObjects;
}

/**
 * @brief InterfaceDocument::hasUnselectedItems
 * @return
 */
bool InterfaceDocument::hasUnselectedItems()
{
    QList<ivm::IVObject *> unselectedObjects = getUnselectedObjects();
    return !unselectedObjects.isEmpty();
}

/**
 * All items that are NOT selected in the main scene at its current level is hidden
 */
void InterfaceDocument::hideUnselectedItems()
{
    QList<ivm::IVObject *> unselectedObjects = getUnselectedObjects();
    // Hide the unselectedObjects
    for (ivm::IVObject *unselectedObject : unselectedObjects) {
        bool hide =
                unselectedObject->isFunction() || unselectedObject->isFunctionType() || unselectedObject->isComment();
        if (hide) {
            unselectedObject->setVisible(false);
        }
    }
}

/**
 * @brief get the unselected objects at the current level. Does NOT include the root object.
 * @return
 */
QList<ivm::IVObject *> InterfaceDocument::getUnselectedObjects()
{
    QList<ivm::IVObject *> selectedObjects = getSelectedObjects();
    // Find the visible unselected objects
    shared::Id rootId = objectsModel()->rootObjectId();
    auto unselectedObjects = QList<ivm::IVObject *>();
    QList<ivm::IVObject *> visibleObjects = objectsModel()->visibleObjects(rootId); // only objects at the current level
    for (ivm::IVObject *visibleObject : visibleObjects) {
        bool objectIsUnselected = !selectedObjects.contains(visibleObject);
        if (objectIsUnselected) {
            unselectedObjects.append(visibleObject);
        }
    }
    return unselectedObjects;
}

QString InterfaceDocument::path() const
{
    return d->filePath;
}

void InterfaceDocument::setPath(const QString &path)
{
    if (path != d->filePath) {
        d->filePath = path;
        Q_EMIT titleChanged();
    }
}

/*!
   Sets the name of the project.
 */
void InterfaceDocument::setProjectName(const QString &projectName)
{
    if (projectName == d->projectName) {
        return;
    }

    d->projectName = projectName;
}

/*!
   Returns the name of the project
 */
const QString &InterfaceDocument::projectName() const
{
    return d->projectName;
}

/*!
 * Sets the filename of the used asn1 file.
 * For "internal" (inside the project tree) files, this is only the file name without a path.
 */
void InterfaceDocument::setAsn1FileName(QString newAsnfile, QString oldAsnfile)
{
    QDir projectDir(QFileInfo(d->filePath).absolutePath());

    auto correctFile = [this, projectDir](QString &filename) {
        const QString projectPath = projectDir.absolutePath();
        if (filename.startsWith(projectPath)) {
            filename = projectDir.relativeFilePath(filename);
        } else {
            filename = QFileInfo(filename).canonicalFilePath();
        }
    };

    correctFile(newAsnfile);
    correctFile(oldAsnfile);

    if (d->asnFilesNames.contains(newAsnfile)) {
        return;
    }
    if (!oldAsnfile.isEmpty()) {
        d->asnFilesNames.removeAll(oldAsnfile);
    }

    if (!QFileInfo::exists(projectDir.absoluteFilePath(newAsnfile)) && !QFileInfo::exists(newAsnfile)) {
        return;
    }

    if (!newAsnfile.isEmpty()) {
        d->asnFilesNames.append(newAsnfile);
        Q_EMIT asn1FileNameChanged(newAsnfile);
    }
    d->asnFilesNames.removeDuplicates();
}

/*!
 * Returns the filenames of the used asn1 files.
 * Result does not contain any path for "internal" asn files. Files referenced from outside are with full path
 */
QStringList InterfaceDocument::asn1FilesNames() const
{
    d->asnFilesNames.removeDuplicates();
    return d->asnFilesNames;
}

/*!
 * Returns the list of ASN.1 files that are inside the project (directory tree) including full path
 */
QStringList InterfaceDocument::asn1FilesPaths() const
{
    const QDir projectDir = QFileInfo(path()).absoluteDir();
    QStringList paths;
    for (const QString &filename : d->asnFilesNames) {
        if (isProjectAsnFile(filename)) {
            paths.append(projectDir.filePath(filename));
        }
    }
    paths.removeDuplicates();
    return paths;
}

/*!
 * Returns all ASN.1 files refereced in the project, that are "extern" (outside the project tree) including the full
 * path
 */
QStringList InterfaceDocument::asn1FilesPathsExternal() const
{
    QStringList externAsns;
    for (const QString &filename : d->asnFilesNames) {
        if (!isProjectAsnFile(filename)) {
            externAsns.append(filename);
        }
    }
    externAsns.removeDuplicates();
    return externAsns;
}

/*!
   Sets the name of the associated msc file. This is only the file name without a path. The File is expected to be next
   to the iv file
 */
void InterfaceDocument::setMscFileName(const QString &mscFile)
{
    if (mscFile == d->mscFileName) {
        return;
    }

    d->mscFileName = mscFile;
    Q_EMIT mscFileNameChanged(mscFile);
}

/*!
   Returns the name of the associated msc file. This is only the file name without a path. The File is expected to be
   next to the iv file
 */
const QString &InterfaceDocument::mscFileName() const
{
    return d->mscFileName;
}

QString InterfaceDocument::mscFilePath() const
{
    if (path().isEmpty()) {
        return {};
    }

    QFileInfo fi(path());
    fi.setFile(fi.absolutePath() + QDir::separator() + d->mscFileName);
    return fi.absoluteFilePath();
}

void InterfaceDocument::setCreatorGitHash(const QString &hashStr)
{
    d->creatorGitHash = hashStr;
}

QString InterfaceDocument::creatorGitHash() const
{
    return d->creatorGitHash;
}

bool InterfaceDocument::setRequestURL(const QString &url)
{
    if (d->requestsURL != url)
    {
        d->requestsURL = url;
        Q_EMIT requestURLChanged(d->requestsURL);
        return true;
    }
    return false;


}

QString InterfaceDocument::requestsURL() const
{
    return d->requestsURL;
}

QString InterfaceDocument::uiFileName() const
{
    return d->uiFileName;
}

void InterfaceDocument::setUIFileName(const QString &filePath)
{
    if (filePath == d->uiFileName) {
        return;
    }

    d->uiFileName = filePath;
    Q_EMIT uiFileNameChanged(filePath);
}

bool InterfaceDocument::isDirty() const
{
    return d->commandsStack && !d->commandsStack->isClean();
}

QList<QAction *> InterfaceDocument::customActions() const
{
    QList<QAction *> actions;
    QAction *actCommonProps = new QAction(tr("Common Properties..."));
    actions.append(actCommonProps);
    connect(actCommonProps, &QAction::triggered, this, &InterfaceDocument::onAttributesManagerRequested);
    QAction *actColorScheme = new QAction(tr("Color Scheme..."));
    actions.append(actColorScheme);
    connect(actColorScheme, &QAction::triggered, this, &InterfaceDocument::onColorSchemeMenuInvoked);
    QAction *actDynContext = new QAction(tr("Context Actions..."));
    actions.append(actDynContext);
    connect(actDynContext, &QAction::triggered, this, &InterfaceDocument::onDynContextEditorMenuInvoked);

    ActionsManager::registerAction(Q_FUNC_INFO, actCommonProps, "Edit Properties", "Show editor for common Properties");
    ActionsManager::registerAction(
            Q_FUNC_INFO, actColorScheme, "Edit Color scheme", "Show editor for common Color schemes");
    ActionsManager::registerAction(
            Q_FUNC_INFO, actDynContext, "Context actions", "Show editor for common custom context menu actions");

    return actions;
}

QHash<shared::Id, shared::VEObject *> InterfaceDocument::objects() const
{
    return d->objectsModel->objects();
}

ivm::IVModel *InterfaceDocument::objectsModel() const
{
    return d->objectsModel;
}

IVComponentModel *InterfaceDocument::componentModel() const
{
    return d->componentsModel;
}

IVComponentModel *InterfaceDocument::sharedTypesModel() const
{
    return d->sharedTypesModel;
}

IVItemModel *InterfaceDocument::itemsModel() const
{
    if (!d->itemsModel) {
        d->itemsModel = new IVItemModel(d->objectsModel, d->commandsStack, const_cast<InterfaceDocument *>(this));
        connect(d->itemsModel, &IVItemModel::itemsSelected, this, &InterfaceDocument::onSceneSelectionChanged);
    }

    return d->itemsModel;
}

ivm::IVModel *InterfaceDocument::layersModel() const
{
    return d->layersModel;
}

QHash<shared::Id, shared::VEObject *> InterfaceDocument::layersObjects() const
{
    return d->layersModel->objects();
}

ivm::ArchetypeModel *InterfaceDocument::archetypesModel() const
{
    return d->archetypesModel;
}

/**
 * VisualizationModel for the "IV Structure" window
 */
IVVisualizationModelBase *InterfaceDocument::visualisationModel() const
{
    if (!d->objectsVisualizationModel) {
        d->objectsVisualizationModel =
                new IVVisualizationModel(d->objectsModel, d->commandsStack, const_cast<InterfaceDocument *>(this));
        auto headerItem = new QStandardItem(tr("IV Structure"));
        headerItem->setTextAlignment(Qt::AlignCenter);
        d->objectsVisualizationModel->setHorizontalHeaderItem(0, headerItem);
    }

    return d->objectsVisualizationModel;
}

/**
 * SelectionModel for the "IV Structure" window and the main graph area
 */
QItemSelectionModel *InterfaceDocument::objectsSelectionModel() const
{
    if (!d->objectsSelectionModel) {
        d->objectsSelectionModel = new QItemSelectionModel(visualisationModel(), const_cast<InterfaceDocument *>(this));
        connect(d->objectsSelectionModel, &QItemSelectionModel::selectionChanged, this,
                &InterfaceDocument::onViewSelectionChanged);
    }
    return d->objectsSelectionModel;
}

/**
 * VisualizationModel for the "Connection Layers" window
 */
IVVisualizationModelBase *InterfaceDocument::layerVisualisationModel() const
{
    if (d->layerSelect == nullptr) {
        d->layerSelect = new IVLayerVisualizationModel(
                layersModel(), d->objectsModel, d->commandsStack, const_cast<InterfaceDocument *>(this));
        auto *title = new QStandardItem(tr("Connection Layers"));
        title->setTextAlignment(Qt::AlignCenter);
        d->layerSelect->setHorizontalHeaderItem(0, title);
    }
    return d->layerSelect;
}

void InterfaceDocument::setAsn1Check(Asn1Acn::Asn1SystemChecks *check)
{
    if (d->asnCheck && d->asnCheck->asn1Storage()) {
        disconnect(d->asnCheck->asn1Storage(), nullptr, this, nullptr);
    }

    d->asnCheck = check;
    d->commandsStack->setAsn1Check(check);
    d->commandsStack->setComponentModel(d->sharedTypesModel);

    connect(d->asnCheck->asn1Storage(), &Asn1Acn::Asn1ModelStorage::dataTypesChanged, this,
            &ive::InterfaceDocument::checkAllInterfacesForAsn1Compliance, Qt::QueuedConnection);
}

Asn1Acn::Asn1SystemChecks *InterfaceDocument::asn1Check() const
{
    return d->asnCheck;
}

void InterfaceDocument::setIvCheck(ivm::AbstractSystemChecks *checks)
{
    d->ivCheck = checks;
}

ivm::AbstractSystemChecks *InterfaceDocument::ivCheck() const
{
    return d->ivCheck;
}

QString InterfaceDocument::supportedFileExtensions() const
{
    return QStringLiteral("*.xml");
}

/*!
   \brief InterfaceDocument::checkInterfaceAsn1Compliance
   \param interface
   \return
 */
bool InterfaceDocument::checkInterfaceAsn1Compliance(const ivm::IVInterface *interface) const
{
    if (!d->asnCheck) {
        return true;
    }
    const QVector<shared::InterfaceParameter> &params = interface->params();
    if (params.isEmpty()) {
        return true;
    }

    return std::any_of(params.cbegin(), params.cend(),
            [&](const shared::InterfaceParameter &param) { return d->asnCheck->hasType(param.paramTypeName()); });
}

/*!
   \brief InterfaceDocument::checkAllInterfacesForAsn1Compliance
   \param faultyInterface
   \return
 */
bool InterfaceDocument::checkAllInterfacesForAsn1Compliance()
{
    const QString ivFilenname = d->filePath;
    shared::ErrorHub::clearFileErrors(ivFilenname);

    bool ok = true;
    for (ivm::IVInterface *interface : d->objectsModel->allObjectsByType<ivm::IVInterface>()) {
        if (!checkInterfaceAsn1Compliance(interface)) {
            ok = false;
            const QString id = QString("%1.%2").arg(
                    interface->function() ? interface->function()->title() : "", interface->title());
            QString parameters;
            for (const shared::InterfaceParameter &param : interface->params()) {
                if (!parameters.isEmpty()) {
                    parameters += ", ";
                }
                parameters += param.toString();
            }
            QString error = tr("Interface ASN.1 error: %1(%2)").arg(id, parameters);
            shared::ErrorHub::addError(shared::ErrorItem::Error, error, ivFilenname);
        }
    }

    return ok;
}

void InterfaceDocument::onSavedExternally(const QString &filePath, bool saved)
{
    if (saved) {
        setPath(filePath);
        d->commandsStack->setClean();
    } else {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Export failed"), filePath);
    }
}

/*!
   \brief InterfaceDocument::setObjects
 */
void InterfaceDocument::setObjects(
        const QVector<ivm::IVObject *> &objects, const QHash<shared::Id, EntityAttributes> &externalAttrs)
{
    d->objectsModel->initFromObjects(objects, externalAttrs);
    d->objectsModel->setRootObject({});
    if (d->itemsModel)
        d->itemsModel->shrinkScene();
}

void InterfaceDocument::setLayers(const QVector<ivm::IVObject *> &layers)
{
    d->layersModel->initFromObjects(layers);
    if (d->itemsModel)
        d->itemsModel->shrinkScene();
}

void InterfaceDocument::onAttributesManagerRequested()
{
    auto dialog = new shared::PropertyTemplateManager({ d->dynPropConfig }, nullptr);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceDocument::onDataTypesMenuInvoked()
{
    if (auto act = qobject_cast<QAction *>(sender()))
        showNIYGUI(act->text());
}

void InterfaceDocument::prepareEntityNameForEditing(const shared::Id &id)
{
    if (auto entity = itemsModel()->getItem(id)) {
        if (auto iObj = qobject_cast<shared::ui::VEInteractiveObject *>(entity->toGraphicsObject())) {
            iObj->enableEditMode();
        }
    }
}

void InterfaceDocument::onColorSchemeMenuInvoked()
{
    shared::ColorManagerDialog *dialog = new shared::ColorManagerDialog(nullptr);
    dialog->setFilterGroup("IVE");
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceDocument::onDynContextEditorMenuInvoked()
{
    auto dialog = new DynActionEditor(nullptr);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    dialog->open();
    if (!dialog->init()) {
        dialog->close();
    }
}

void InterfaceDocument::showInfoMessage(const QString &title, const QString &message)
{
    QMessageBox::information(qobject_cast<QWidget *>(parent()), title, message);
}

static inline bool resolveNameConflict(QString &targetPath, QWidget *window)
{
    QMessageBox msgBox(QMessageBox::Question, QObject::tr("Exporting objects"),
            QObject::tr("Current object already exported: %1\nDo you want to proceed?").arg(targetPath),
            QMessageBox::StandardButton::NoButton, window);
    msgBox.addButton(QMessageBox::Cancel);
    msgBox.addButton(QObject::tr("Overwrite"), QMessageBox::ButtonRole::AcceptRole);
    msgBox.addButton(QObject::tr("Rename"), QMessageBox::ButtonRole::ResetRole);
    msgBox.exec();

    const QMessageBox::ButtonRole role = msgBox.buttonRole(msgBox.clickedButton());
    switch (role) {
    case QMessageBox::ButtonRole::AcceptRole:
        if (!QDir(targetPath).removeRecursively()) {
            shared::ErrorHub::addError(
                    shared::ErrorItem::Error, QObject::tr("Unable to cleanup directory %1").arg(targetPath));
            return false;
        }
        break;
    case QMessageBox::ButtonRole::ResetRole: {
        const QFileInfo fi(targetPath);
        bool ok = true;
        QString text;
        while (ok) {
            text = QInputDialog::getText(window, QObject::tr("Exporting objects"),
                    QObject::tr("Set another name for exporting object"), QLineEdit::Normal, QString(), &ok);
            if (!ok) {
                return false;
            } else if (text.isEmpty()) {
                shared::ErrorHub::addError(shared::ErrorItem::Error, QObject::tr("Component name can't be empty"));
            } else if (QFile::exists(fi.absoluteDir().filePath(text))) {
                shared::ErrorHub::addError(shared::ErrorItem::Error,
                        QObject::tr("Exported component with such name already exists, choose another one"));
            } else {
                break;
            }
        }
        targetPath = fi.absoluteDir().filePath(ivm::IVNameValidator::encodeName(ivm::IVObject::Type::Function, text));
        break;
    }
    case QMessageBox::ButtonRole::RejectRole:
    default:
        return false;
    }

    if (!shared::ensureDirExists(targetPath)) {
        shared::ErrorHub::addError(
                shared::ErrorItem::Error, QObject::tr("Unable to create directory %1").arg(targetPath));
        return false;
    }
    return true;
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

bool InterfaceDocument::exportImpl(QString &targetPath, const QList<shared::VEObject *> &objects)
{
    const bool ok = shared::ensureDirExists(targetPath);
    if (!ok) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Unable to create directory %1").arg(targetPath));
        return false;
    }

    QDir targetDir(targetPath);
    if (QFile::exists(targetDir.filePath(shared::kDefaultInterfaceViewFileName))) {
        if (!resolveNameConflict(targetPath, nullptr)) {
            return false;
        } else {
            targetDir.setPath(targetPath);
        }
    }

    if (!exporter()->exportObjectsSilently(objects, targetDir.filePath(shared::kDefaultInterfaceViewFileName),
                d->archetypesModel, exporter()->templatePath(QLatin1String("interfaceview.ui")))) {
        return false;
    }

    const QFileInfo ivPath(path());
    const QDir ivDir = ivPath.absoluteDir();
    const QString workDir = QDir(ivDir.canonicalPath() + "/work/").absolutePath();
    for (const QString &asnFile : asn1FilesPaths()) {
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

    QVector<ivm::IVObject *> children;
    std::for_each(objects.cbegin(), objects.cend(), [&children](shared::VEObject *veObj) {
        if (auto fn = veObj->as<ivm::IVObject *>()) {
            children.append(fn);
        }
    });
    copyImplementation(ivDir, targetDir, children);
    createProFile(targetPath);
    initTASTEEnv(targetPath);
    return true;
}

bool InterfaceDocument::loadImpl(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Invalid path"), path);
        return false;
    }

    loadAvailableComponents();

    shared::ErrorHub::setCurrentFile(path);
    ivm::IVXMLReader parser;
    if (!parser.readFile(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, parser.errorString(), path);
        shared::ErrorHub::clearCurrentFile();
        return false;
    }

    QVector<ivm::IVObject *> parsedObjects = parser.parsedObjects();
    ivm::IVObject::sortObjectList(parsedObjects);
    setObjects(parsedObjects, parser.externalAttributes());

    auto layers = parser.parsedLayers();
    ivm::IVObject::sortObjectListByTitle(layers);
    setLayers(layers);

    loadArchetypes();

    const QVariantMap metadata = parser.metaData();

    if (metadata.contains("projectName")) {
        setProjectName(metadata["projectName"].toString());
    }
    if (metadata.contains("asn1file")) {
        setAsn1FileName(metadata["asn1file"].toString());
    }
    setMscFileName(metadata["mscfile"].toString());
    if (metadata.contains(parser.uiFileNameTag()))
        setUIFileName(metadata[parser.uiFileNameTag()].toString());
    if (metadata.contains("requestsURL")){
        setRequestURL(metadata["requestsURL"].toString());
    }
    setCreatorGitHash(metadata["creatorHash"].toString());
    shared::ErrorHub::clearCurrentFile();

    return true;
}

void InterfaceDocument::loadArchetypes()
{
    d->archetypesModel->clear();

    for (const auto &libraryReference : d->objectsModel->getArchetypeLibraryReferences()) {
        ivm::ArchetypeXMLReader archetypeParser;
        QString absolutePath = shared::interfaceCustomArchetypesDirectoryPath() + QDir::separator()
                + libraryReference->getLibraryPath();
        if (!archetypeParser.readFile(absolutePath)) {
            shared::ErrorHub::addError(shared::ErrorItem::Error, archetypeParser.errorString(), absolutePath);
            shared::ErrorHub::clearCurrentFile();
        }
        QVector<ivm::ArchetypeObject *> archetypeObjects = archetypeParser.parsedObjects();

        generateArchetypeLibrary(archetypeObjects, libraryReference->getLibraryName());

        d->archetypesModel->addObjects(archetypeObjects);
    }
}

QString InterfaceDocument::componentPath(const shared::Id &id) const
{
    return d->componentsModel->componentPath(id);
}

QString InterfaceDocument::sharedTypePath(const shared::Id &id) const
{
    return d->sharedTypesModel->componentPath(id);
}

void InterfaceDocument::generateArchetypeLibrary(
        QVector<ivm::ArchetypeObject *> &archetypeObjects, const QString &archetypeLibraryName)
{
    ivm::ArchetypeLibrary *archetypelibrary = new ivm::ArchetypeLibrary(archetypeLibraryName);

    for (auto archetypeObject : archetypeObjects) {
        if (auto functionArchetype = archetypeObject->as<ivm::FunctionArchetype *>()) {
            archetypelibrary->addFunction(functionArchetype);
        }
    }

    archetypeObjects.append(archetypelibrary);
}

void InterfaceDocument::showNIYGUI(const QString &title)
{
    QString header = title.isEmpty() ? "NIY" : title;
    QMessageBox::information(nullptr, header, "Not implemented yet!");
}

void InterfaceDocument::createProFile(const QString &path)
{
    shared::QMakeFile::createProFileForDirectory(path, asn1FilesPathsExternal());
}

void InterfaceDocument::initTASTEEnv(const QString &path)
{
    auto initTASTECallerProcess = new QProcess(this);
    initTASTECallerProcess->setProgram("taste");
    initTASTECallerProcess->setArguments({ "reset" });
    initTASTECallerProcess->setWorkingDirectory(path);
    initTASTECallerProcess->start();
    initTASTECallerProcess->waitForFinished();
    if (initTASTECallerProcess->exitCode() != 0 || initTASTECallerProcess->exitStatus() != QProcess::NormalExit) {
        QMessageBox::warning(qApp->activeWindow(), tr("Init TASTE environment"),
                tr("Error during TASTE environment initiation for exported component!"));
    }
    initTASTECallerProcess->deleteLater();
}

bool InterfaceDocument::isProjectAsnFile(const QString &filename) const
{
    QFileInfo fi(filename);
    QDir projectDir(QFileInfo(d->filePath).absolutePath());
    return filename.startsWith(projectDir.absolutePath()) || fi.isRelative();
}

void InterfaceDocument::onSceneSelectionChanged(const QList<shared::Id> &selectedObjects)
{
    if (!d->objectsVisualizationModel || !d->objectsSelectionModel) {
        return;
    }
    QItemSelection itemSelection;
    for (auto id : selectedObjects) {
        const QModelIndex idx = d->objectsVisualizationModel->indexFromItem(d->objectsVisualizationModel->getItem(id));
        if (itemSelection.isEmpty()) {
            itemSelection.select(idx, idx);
        } else {
            itemSelection.merge(QItemSelection(idx, idx), QItemSelectionModel::SelectCurrent);
        }
    }
    d->objectsSelectionModel->select(itemSelection,
            QItemSelectionModel::Rows | QItemSelectionModel::Current | QItemSelectionModel::ClearAndSelect);
}

void InterfaceDocument::onViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    // Update the selection state of the QGraphicItems in the scene
    auto updateSelection = [this](const QItemSelection &selection, bool value) {
        for (const QModelIndex &idx : selection.indexes()) {
            QVariant id = idx.data(IVVisualizationModelBase::IdRole);
            QUuid uuid = id.toUuid();
            if (auto graphicsItem = itemsModel()->getItem(uuid)) {
                graphicsItem->setSelected(value);
            }
        }
    };

    updateSelection(deselected, false);
    updateSelection(selected, true);
}
}
