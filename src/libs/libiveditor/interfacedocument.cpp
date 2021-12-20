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

#include "actionsbar.h"
#include "asn1modelstorage.h"
#include "asn1systemchecks.h"
#include "colors/colormanagerdialog.h"
#include "commandsstack.h"
#include "context/action/actionsmanager.h"
#include "context/action/editor/dynactioneditor.h"
#include "errorhub.h"
#include "interface/objectstreeview.h"
#include "itemeditor/common/ivutils.h"
#include "itemeditor/graphicsitemhelpers.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "itemeditor/ivitemmodel.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivcore/abstractsystemchecks.h"
#include "ivexporter.h"
#include "ivfunction.h"
#include "ivmodel.h"
#include "ivpropertytemplateconfig.h"
#include "ivvisualizationmodelbase.h"
#include "ivxmlreader.h"
#include "propertytemplatemanager.h"
#include "propertytemplatewidget.h"

#include <QAction>
#include <QBuffer>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDir>
#include <QDirIterator>
#include <QGraphicsScene>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QSplitter>
#include <QStandardPaths>
#include <QUndoStack>
#include <QVBoxLayout>
#include <algorithm>

namespace ive {

struct InterfaceDocument::InterfaceDocumentPrivate {
    cmd::CommandsStack *commandsStack { nullptr };

    QString filePath;

    ivm::IVPropertyTemplateConfig *dynPropConfig { nullptr };
    IVItemModel *itemsModel { nullptr };
    IVVisualizationModelBase *objectsVisualizationModel { nullptr };
    QItemSelectionModel *objectsSelectionModel { nullptr };
    ivm::IVModel *objectsModel { nullptr };
    ivm::IVModel *importModel { nullptr };
    IVVisualizationModelBase *importVisualisationModel { nullptr };
    ivm::IVModel *sharedModel { nullptr };
    IVVisualizationModelBase *sharedVisualisationModel { nullptr };
    IVExporter *exporter { nullptr };
    ivm::IVModel *layersModel { nullptr };
    IVVisualizationModelBase *layerSelect { nullptr };

    Asn1Acn::Asn1SystemChecks *asnCheck { nullptr };
    ivm::AbstractSystemChecks *ivCheck { nullptr };
    QString mscFileName;
    QStringList asnFilesNames;
};

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

    d->importModel = new ivm::IVModel(d->dynPropConfig, nullptr, this);
    d->sharedModel = new ivm::IVModel(d->dynPropConfig, nullptr, this);
    d->objectsModel = new ivm::IVModel(d->dynPropConfig, d->sharedModel, this);
    d->layersModel = new ivm::IVModel(d->dynPropConfig, nullptr, this);
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
    importVisualisationModel();
    sharedVisualisationModel();
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

    const bool loaded = loadImpl(path);

    if (loaded) {
        d->commandsStack->clear();
    } else {
        setPath(oldPath);
    }

    return loaded;
}

bool InterfaceDocument::loadAvailableComponents()
{
    return reloadComponentModel() && reloadSharedTypeModel();
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
    if (ivm::IVConnectionLayerType::connectionLayers.isEmpty()) {
        ivm::IVConnectionLayerType::addDefaultConnectionLayer(layersModel()->rootObject());
    }
    for (auto &layer : ivm::IVConnectionLayerType::connectionLayers) {
        if (!layersModel()->allObjectsByType<ivm::IVConnectionLayerType>().contains(layer)) {
            layersModel()->addObject(layer);
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
        return reloadComponentModel();
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
        d->objectsSelectionModel->clearSelection();
        return reloadSharedTypeModel();
    }
    return false;
}

bool InterfaceDocument::loadComponentModel(ivm::IVModel *model, const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Invalid path"), path);
        return false;
    }

    shared::ErrorHub::setCurrentFile(path);
    ivm::IVXMLReader parser;
    if (!parser.readFile(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, parser.errorString(), path);
        shared::ErrorHub::clearCurrentFile();
        return false;
    }

    model->addObjects(parser.parsedObjects());
    shared::ErrorHub::clearCurrentFile();
    return true;
}

bool InterfaceDocument::reloadComponentModel()
{
    bool result = true;
    d->importModel->clear();
    QDirIterator importableIt(shared::componentsLibraryPath(), QDir::Dirs | QDir::NoDotAndDotDot);
    while (importableIt.hasNext()) {
        result |= loadComponentModel(
                d->importModel, importableIt.next() + QDir::separator() + shared::kDefaultInterfaceViewFileName);
    }
    return result;
}

bool InterfaceDocument::reloadSharedTypeModel()
{
    bool result = true;
    d->sharedModel->clear();
    QDirIterator instantiatableIt(shared::sharedTypesPath(), QDir::Dirs | QDir::NoDotAndDotDot);
    while (instantiatableIt.hasNext()) {
        result |= loadComponentModel(
                d->sharedModel, instantiatableIt.next() + QDir::separator() + shared::kDefaultInterfaceViewFileName);
    }
    return result;
}

void InterfaceDocument::close()
{
    d->objectsModel->clear();
    d->layersModel->clear();
    setPath(QString());
    d->commandsStack->clear();
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
  Sets the filename of the used asn1 file. This is only the file name without a path. The File is expected to be next
   to the iv file
 */
void InterfaceDocument::setAsn1FileName(const QString &newAsnfile, const QString &oldAsnfile)
{
    if (d->asnFilesNames.contains(newAsnfile)) {
        return;
    }
    if (!oldAsnfile.isEmpty()) {
        d->asnFilesNames.removeAll(oldAsnfile);
    }

    if (!newAsnfile.isEmpty()) {
        d->asnFilesNames.append(newAsnfile);
        Q_EMIT asn1FileNameChanged(newAsnfile);

        if (d->asnCheck && d->asnCheck->asn1Storage()) {
            const QDir projectPath { QFileInfo(path()).absoluteDir() };
            if (d->asnCheck->asn1Storage()->contains(projectPath.filePath(newAsnfile))) {
                checkAllInterfacesForAsn1Compliance();
            } else {
                // does load the data
                d->asnCheck->asn1Storage()->asn1DataTypes(projectPath.filePath(newAsnfile));
            }
        }
    }
}

/*!
   Returns the filenames of the used asn1 files. Result does not contain any path.
 */
QStringList InterfaceDocument::asn1FilesNames() const
{
    return d->asnFilesNames;
}

/*!
   Returns the list of ASN.1 files including full path
 */
QStringList InterfaceDocument::asn1FilesPaths() const
{
    const QDir projectDir = QFileInfo(path()).absoluteDir();
    QStringList paths;
    for (const QString &filename : d->asnFilesNames) {
        paths.append(projectDir.filePath(filename));
    }

    return paths;
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

ivm::IVModel *InterfaceDocument::importModel() const
{
    return d->importModel;
}

ivm::IVModel *InterfaceDocument::sharedModel() const
{
    return d->sharedModel;
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

QItemSelectionModel *InterfaceDocument::objectsSelectionModel() const
{
    if (!d->objectsSelectionModel) {
        d->objectsSelectionModel = new QItemSelectionModel(visualisationModel(), const_cast<InterfaceDocument *>(this));
        connect(d->objectsSelectionModel, &QItemSelectionModel::selectionChanged, this,
                &InterfaceDocument::onViewSelectionChanged);
    }
    return d->objectsSelectionModel;
}

IVVisualizationModelBase *InterfaceDocument::importVisualisationModel() const
{
    if (!d->importVisualisationModel) {
        d->importVisualisationModel = new IVVisualizationModelBase(importModel(), d->commandsStack,
                shared::DropData::Type::ImportableType, const_cast<InterfaceDocument *>(this));
        auto headerItem = new QStandardItem(tr("Import Component"));
        headerItem->setTextAlignment(Qt::AlignCenter);
        d->importVisualisationModel->setHorizontalHeaderItem(0, headerItem);
    }

    return d->importVisualisationModel;
}

IVVisualizationModelBase *InterfaceDocument::sharedVisualisationModel() const
{
    if (!d->sharedVisualisationModel) {
        d->sharedVisualisationModel = new IVVisualizationModelBase(sharedModel(), d->commandsStack,
                shared::DropData::Type::InstantiatableType, const_cast<InterfaceDocument *>(this));
        auto headerItem = new QStandardItem(tr("Shared Types"));
        headerItem->setTextAlignment(Qt::AlignCenter);
        d->sharedVisualisationModel->setHorizontalHeaderItem(0, headerItem);
    }

    return d->sharedVisualisationModel;
}

IVVisualizationModelBase *InterfaceDocument::layerVisualisationModel() const
{
    if (d->layerSelect == nullptr) {
        d->layerSelect = new IVVisualizationModelBase(
                layersModel(), d->commandsStack, shared::DropData::Type::None, const_cast<InterfaceDocument *>(this));
        auto *title = new QStandardItem(tr("IV Connection Layers"));
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
void InterfaceDocument::setObjects(const QVector<ivm::IVObject *> &objects)
{
    d->objectsModel->initFromObjects(objects);
    d->objectsModel->setRootObject({});
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

static inline bool exportObjects(
        IVExporter *exporter, const QList<shared::VEObject *> &objects, const QString &filePath)
{
    QBuffer buffer;
    if (!buffer.open(QIODevice::WriteOnly)) {
        shared::ErrorHub::addError(
                shared::ErrorItem::Error, QObject::tr("Can't open buffer for exporting: %1").arg(buffer.errorString()));
        return false;
    }
    if (!exporter->exportObjects(objects, &buffer)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, QObject::tr("Error during component export"));
        return false;
    }
    buffer.close();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        shared::ErrorHub::addError(
                shared::ErrorItem::Error, QObject::tr("Can't export file: ").arg(file.errorString()));
        return false;
    }
    const qint64 bytesWritten = file.write(buffer.buffer());
    file.close();
    return bytesWritten == buffer.size();
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
        targetPath = fi.absoluteDir().filePath(text);
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
    for (shared::VEObject *object : objects) {
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

    auto it = std::find_if(
            objects.begin(), objects.end(), [](shared::VEObject *obj) { return obj->parentObject() == nullptr; });
    if (it != objects.end()) {
        (*it)->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::name), targetDir.dirName());
    }

    if (!exportObjects(exporter(), objects, targetDir.filePath(shared::kDefaultInterfaceViewFileName))) {
        return false;
    }

    const QFileInfo ivPath(path());
    const QDir ivDir = ivPath.absoluteDir();
    for (const QString &asnFile : asn1FilesNames()) {
        if (!QFile::copy(ivDir.filePath(asnFile), targetDir.filePath(asnFile))) {
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
    return true;
}

bool InterfaceDocument::loadImpl(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Invalid path"), path);
        return false;
    }

    shared::ErrorHub::setCurrentFile(path);
    ivm::IVXMLReader parser;
    if (!parser.readFile(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, parser.errorString(), path);
        shared::ErrorHub::clearCurrentFile();
        return false;
    }

    setObjects(parser.parsedObjects());
    const QVariantMap metadata = parser.metaData();
    QVariantMap::const_iterator i = metadata.constFind("asn1file");
    while (i != metadata.end() && i.key() == "asn1file") {
        setAsn1FileName(i.value().toString());
        ++i;
    }
    setMscFileName(metadata["mscfile"].toString());
    shared::ErrorHub::clearCurrentFile();

    return true;
}

void InterfaceDocument::showNIYGUI(const QString &title)
{
    QString header = title.isEmpty() ? "NIY" : title;
    QMessageBox::information(nullptr, header, "Not implemented yet!");
}

void InterfaceDocument::onSceneSelectionChanged(const QList<shared::Id> &selectedObjects)
{
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
    auto updateSelection = [this](const QItemSelection &selection, bool value) {
        for (const QModelIndex &idx : selection.indexes()) {
            if (auto graphicsItem = itemsModel()->getItem(idx.data(IVVisualizationModelBase::IdRole).toUuid())) {
                graphicsItem->setSelected(value);
            }
        }
    };

    updateSelection(deselected, false);
    updateSelection(selected, true);
}

}
