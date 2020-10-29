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

#include "aadlitemmodel.h"
#include "aadlobjectcomment.h"
#include "aadlobjectconnection.h"
#include "aadlobjectconnectiongroup.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "aadlxmlreader.h"
#include "asn1modelstorage.h"
#include "baseitems/common/aadlutils.h"
#include "baseitems/graphicsview.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "commandsstack.h"
#include "context/action/actionsmanager.h"
#include "context/action/editor/dynactioneditor.h"
#include "creatortool.h"
#include "interface/aadlobjectstreeview.h"
#include "interface/colors/colormanagerdialog.h"
#include "interface/properties/dynamicpropertymanager.h"
#include "interface/properties/propertiesdialog.h"
#include "xmldocexporter.h"

#include <QAction>
#include <QBuffer>
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGuiApplication>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QMutex>
#include <QMutexLocker>
#include <QPointer>
#include <QScreen>
#include <QShortcut>
#include <QSplitter>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QToolBar>
#include <QTreeView>
#include <QUndoStack>
#include <QVBoxLayout>

namespace aadlinterface {

static const QString kDefaultFilename { QLatin1String("interfaceview.xml") };

static inline QString componentsLibraryPath()
{
    static const QString kDefaultPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("components_library") + QDir::separator();

    return qEnvironmentVariable("TASTE_COMPONENTS_LIBRARY", kDefaultPath);
}

static inline QString sharedTypesPath()
{
    static const QString kDefaultPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("shared_types") + QDir::separator();

    return qEnvironmentVariable("TASTE_SHARED_TYPES", kDefaultPath);
}

struct InterfaceDocument::InterfaceDocumentPrivate {
    QUndoStack *commandsStack { nullptr };

    QString filePath;

    QPointer<QWidget> view;
    aadlinterface::GraphicsView *graphicsView { nullptr };
    QTreeView *objectsView { nullptr };
    AADLItemModel *itemsModel { nullptr };
    aadl::AADLObjectsModel *objectsModel { nullptr };
    AADLObjectsTreeView *importView { nullptr };
    aadl::AADLObjectsModel *importModel { nullptr };

    QAction *actCreateConnectionGroup { nullptr };
    QAction *actRemove { nullptr };
    QAction *actZoomIn { nullptr };
    QAction *actZoomOut { nullptr };
    QAction *actExitToRoot { nullptr };
    QAction *actExitToParent { nullptr };
    QVector<QAction *> m_toolbarActions;

    CreatorTool *tool { nullptr };

    Asn1Acn::Asn1ModelStorage *asnDataTypes { new Asn1Acn::Asn1ModelStorage() };
    QString mscFileName;
    QString asnFileName;
};

/*!
\class aadlinterface::InterfaceDocument
\brief aadlinterface::InterfaceDocument is the document for graphical AADL data (loaded from the XML).
*/

InterfaceDocument::InterfaceDocument(QObject *parent)
    : QObject(parent)
    , d(new InterfaceDocumentPrivate)
{
    d->commandsStack = new QUndoStack(this);
    connect(d->commandsStack, &QUndoStack::cleanChanged, this, [this](bool clean) { Q_EMIT dirtyChanged(!clean); });

    d->objectsModel = new aadl::AADLObjectsModel(this);

    d->importModel = new aadl::AADLObjectsModel(this);
}

InterfaceDocument::~InterfaceDocument()
{
    delete d->asnDataTypes;
    delete d->view;
    delete d;
}

void InterfaceDocument::init()
{
    if (!d->view.isNull()) {
        return;
    }

    d->itemsModel = new AADLItemModel(d->objectsModel, this);
    connect(d->itemsModel, &AADLItemModel::itemClicked, this, &InterfaceDocument::onItemClicked);
    connect(d->itemsModel, &AADLItemModel::itemDoubleClicked, this, &InterfaceDocument::onItemDoubleClicked);

    QWidget *panelWidget = new QWidget;
    QVBoxLayout *panelLayout = new QVBoxLayout;
    panelLayout->setMargin(0);
    panelWidget->setLayout(panelLayout);

    QSplitter *panelSplitter = new QSplitter(Qt::Vertical, panelWidget);
    panelSplitter->setHandleWidth(1);
    panelSplitter->addWidget(createModelView());
    panelSplitter->addWidget(createImportView());
    panelLayout->addWidget(panelSplitter);

    d->view = new QWidget;
    auto rootLayout = new QVBoxLayout;
    rootLayout->setMargin(0);
    d->view->setLayout(rootLayout);

    auto splitter = new QSplitter(Qt::Horizontal, d->view);
    splitter->setHandleWidth(1);
    splitter->addWidget(panelWidget);
    splitter->addWidget(createGraphicsView());
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    rootLayout->addWidget(splitter);

    QTimer::singleShot(0, this, &InterfaceDocument::loadAvailableComponents);
}

void InterfaceDocument::fillToolBar(QToolBar *toolBar)
{
    if (toolBar == nullptr) {
        return;
    }

    toolBar->setUpdatesEnabled(false);
    toolBar->clear();

    const QVector<QAction *> &docActions = initActions();
    for (QAction *action : docActions) {
        toolBar->addAction(action);
    }

    toolBar->setUpdatesEnabled(true);
}

QGraphicsScene *InterfaceDocument::scene() const
{
    return d->itemsModel->scene();
}

shared::ui::GraphicsViewBase *InterfaceDocument::graphicsView() const
{
    return d->graphicsView;
}

QWidget *InterfaceDocument::view() const
{
    return d->view;
}

QUndoStack *InterfaceDocument::commandsStack() const
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
    const bool loaded = loadImpl(path);

    if (loaded) {
        setPath(path);
        d->commandsStack->clear();
    }

    return loaded;
}

bool InterfaceDocument::loadAvailableComponents()
{
    d->importModel->clear();
    QDirIterator it(componentsLibraryPath());
    while (it.hasNext()) {
        importImpl(it.next() + QDir::separator() + kDefaultFilename);
    }
    return true;
}

QString InterfaceDocument::getComponentName(const QStringList &exportNames)
{
    QString name = exportNames.join(QLatin1Char('_'));
    auto dialog = new QDialog(qobject_cast<QWidget *>(parent()));
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

QList<aadl::AADLObject *> InterfaceDocument::prepareSelectedObjectsForExport(QString &name)
{
    QList<aadl::AADLObject *> objects;
    QStringList exportNames;
    for (const auto id : d->itemsModel->selectionModel()->selection().indexes()) {
        const int role = static_cast<int>(aadl::AADLObjectsModel::AADLRoles::IdRole);
        if (aadl::AADLObject *object = d->objectsModel->getObject(id.data(role).toUuid())) {
            if (object->isFunction() && object->parentObject() == nullptr) {
                exportNames.append(object->attr(QLatin1String("name")).toString());
            }
            objects.append(object);
        }
    }

    name = getComponentName(exportNames);
    if (exportNames.size() > 1) {
        aadl::AADLObjectFunction *dummyFunction = new aadl::AADLObjectFunction(name);
        for (auto object : objects) {
            if (!object->parentObject()) {
                dummyFunction->addChild(object);
            }
        }
        objects.prepend(dummyFunction);
    }
    return objects;
}

bool InterfaceDocument::exportSelected()
{
    QString name;
    const QList<aadl::AADLObject *> objects = prepareSelectedObjectsForExport(name);
    const QString targetDir = componentsLibraryPath();

    const bool ok = shared::ensureDirExists(targetDir);
    if (!ok) {
        qWarning() << "Unable to create directory" << targetDir;
        return {};
    }

    const QString exportFilePath = QString("%1/%2").arg(targetDir, kDefaultFilename);

    const QFileInfo fi(exportFilePath);
    if (fi.exists()) {
        qWarning() << "Current object already exported: " << exportFilePath;
        return false;
    }

    QBuffer buffer;
    if (!buffer.open(QIODevice::WriteOnly)) {
        qWarning() << "Can't open buffer for exporting:" << buffer.errorString();
        return false;
    }
    if (!XmlDocExporter::exportObjects(objects, &buffer)) {
        qWarning() << "Error during component export";
        return false;
    }
    buffer.close();

    QFile file(fi.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Can't export file: " << file.errorString();
        return false;
    }
    file.write(buffer.buffer());
    file.close();
    if (QFile::copy(asn1FilePath(), targetDir + QDir::separator() + asn1FileName())) {
        qWarning() << "Error during ASN.1 file copying:" << asn1FilePath();
    }

    d->itemsModel->selectionModel()->clearSelection();
    return true;
}

bool InterfaceDocument::importImpl(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        qWarning() << Q_FUNC_INFO << "Invalid path";
        return false;
    }

    aadl::AADLXMLReader parser;
    connect(&parser, &aadl::AADLXMLReader::objectsParsed, d->importModel, &aadl::AADLObjectsModel::addObjects);
    connect(&parser, &aadl::AADLXMLReader::error, [](const QString &msg) { qWarning() << msg; });

    return parser.readFile(path);
}

bool InterfaceDocument::save(const QString &path)
{
    Q_UNUSED(path)
    return false;
}

void InterfaceDocument::close()
{
    d->objectsModel->clear();
    setPath(QString());
    d->commandsStack->clear();
}

QString InterfaceDocument::path() const
{
    return d->filePath;
}

/*!
  Sets the filename of the used asn1 file. This is only the file name without a path. The File is expected to be next
   to the aadl file
 */
void InterfaceDocument::setAsn1FileName(const QString &asnfile)
{
    if (d->asnFileName == asnfile) {
        return;
    }

    d->asnFileName = asnfile;
}

/*!
   Returns the filename of the used asn1 file. It does not contain any path.
 */
QString InterfaceDocument::asn1FileName() const
{
    return d->asnFileName;
}

QString InterfaceDocument::asn1FilePath() const
{
    QFileInfo fi(path());
    fi.setFile(fi.absolutePath() + QDir::separator() + d->asnFileName);
    return fi.absoluteFilePath();
}

/*!
   Sets the name of the associated msc file. This is only the file name without a path. The File is expected to be next
   to the aadl file
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
   next to the aadl file
 */
const QString &InterfaceDocument::mscFileName() const
{
    return d->mscFileName;
}

bool InterfaceDocument::isDirty() const
{
    return d->commandsStack && !d->commandsStack->isClean();
}

QString InterfaceDocument::title() const
{
    const QString fileName = QFileInfo(path()).fileName();
    if (fileName.isEmpty()) {
        return tr("Untitled");
    }
    return fileName;
}

QList<QAction *> InterfaceDocument::customActions() const
{
    QList<QAction *> actions;
    QAction *actCommonProps = new QAction(tr("Common Properties"));
    actions.append(actCommonProps);
    connect(actCommonProps, &QAction::triggered, this, &InterfaceDocument::onAttributesManagerRequested);
    //    QAction *actDataTypes = root->addAction(tr("Data types"));
    //    connect(actDataTypes, &QAction::triggered, this, &InterfaceDocument::onDataTypesMenuInvoked);
    QAction *actColorScheme = new QAction(tr("Color Scheme"));
    actions.append(actColorScheme);
    connect(actColorScheme, &QAction::triggered, this, &InterfaceDocument::onColorSchemeMenuInvoked);
    QAction *actDynContext = new QAction(tr("Context Actions"));
    actions.append(actDynContext);
    connect(actDynContext, &QAction::triggered, this, &InterfaceDocument::onDynContextEditorMenuInvoked);

    ActionsManager::registerAction(Q_FUNC_INFO, actCommonProps, "Edit Properties", "Show editor for common Properties");
    //    ActionsManager::registerAction(Q_FUNC_INFO, actDataTypes, "Edit Datatypes", "Show editor for common
    //    Datatypes");
    ActionsManager::registerAction(
            Q_FUNC_INFO, actColorScheme, "Edit Color scheme", "Show editor for common Color schemes");
    ActionsManager::registerAction(
            Q_FUNC_INFO, actDynContext, "Context actions", "Show editor for common custom context menu actions");

    return actions;
}

const QHash<shared::Id, aadl::AADLObject *> &InterfaceDocument::objects() const
{
    return d->objectsModel->objects();
}

aadl::AADLObjectsModel *InterfaceDocument::objectsModel() const
{
    return d->objectsModel;
}

aadl::AADLObjectsModel *InterfaceDocument::importModel() const
{
    return d->importModel;
}

Asn1Acn::Asn1ModelStorage *InterfaceDocument::asn1DataTypes() const
{
    return d->asnDataTypes;
}

QString InterfaceDocument::supportedFileExtensions() const
{
    return QStringLiteral("*.xml");
}

void InterfaceDocument::onSavedExternally(const QString &filePath, bool saved)
{
    if (saved) {
        setPath(filePath);
        d->commandsStack->setClean();
    }
}

/*!
   \brief InterfaceDocument::setObjects
 */
void InterfaceDocument::setObjects(const QVector<aadl::AADLObject *> &objects)
{
    if (d->objectsModel->initFromObjects(objects)) {
        d->objectsModel->setRootObject({});
    }
}

void InterfaceDocument::onItemClicked(shared::Id id)
{
    Q_UNUSED(id)
}

void InterfaceDocument::onItemDoubleClicked(shared::Id id)
{
    if (id.isNull()) {
        return;
    }

    if (auto entity = d->objectsModel->getObject(id)) {
        if (entity->aadlType() != aadl::AADLObject::Type::Connection) {
            showPropertyEditor(entity);
        }
    }
}

void InterfaceDocument::onAttributesManagerRequested()
{
    auto dialog = new aadlinterface::DynamicPropertyManager(qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceDocument::onDataTypesMenuInvoked()
{
    if (auto act = qobject_cast<QAction *>(sender()))
        showNIYGUI(act->text());
}

void InterfaceDocument::onColorSchemeMenuInvoked()
{
    aadlinterface::ColorManagerDialog *dialog =
            new aadlinterface::ColorManagerDialog(qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceDocument::onDynContextEditorMenuInvoked()
{
    auto dialog = new DynActionEditor(qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    dialog->open();
    if (!dialog->init())
        dialog->close();
}

void InterfaceDocument::showPropertyEditor(aadl::AADLObject *obj)
{
    aadlinterface::PropertiesDialog *dialog = new aadlinterface::PropertiesDialog(
            obj, d->asnDataTypes->asn1DataTypes(asn1FilePath()), qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceDocument::showInfoMessage(const QString &title, const QString &message)
{
    QMessageBox::information(qobject_cast<QWidget *>(parent()), title, message);
}

void InterfaceDocument::importEntity(const shared::Id &id, const QPointF &sceneDropPoint)
{
    const auto obj = d->importModel->getObject(id);
    if (!obj) {
        return;
    }
    if (d->objectsModel->getObjectByName(obj->title())) {
        return;
    }

    const QVariantList params = { QVariant::fromValue(obj), QVariant::fromValue(d->itemsModel),
        QVariant::fromValue(sceneDropPoint) };
    if (QUndoCommand *cmdImport = cmd::CommandsFactory::create(cmd::ImportEntities, params)) {
        cmd::CommandsStack::current()->push(cmdImport);
    }
}

void InterfaceDocument::setPath(const QString &path)
{
    if (path != d->filePath) {
        d->filePath = path;
        Q_EMIT titleChanged();
    }
}

bool InterfaceDocument::loadImpl(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        qWarning() << Q_FUNC_INFO << "Invalid path";
        return false;
    }

    aadl::AADLXMLReader parser;
    connect(&parser, &aadl::AADLXMLReader::objectsParsed, this, &InterfaceDocument::setObjects);
    connect(&parser, &aadl::AADLXMLReader::metaDataParsed, this, [this, path](const QVariantMap &metadata) {
        d->asnFileName = metadata["asn1file"].toString();
        setMscFileName(metadata["mscfile"].toString());
    });
    connect(&parser, &aadl::AADLXMLReader::error, [](const QString &msg) { qWarning() << msg; });

    return parser.readFile(path);
}

QVector<QAction *> InterfaceDocument::initActions()
{
    auto actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);

    if (!d->tool) {
        d->tool = new CreatorTool(d->graphicsView, d->itemsModel, this);
        connect(d->tool, &CreatorTool::created, this, [this, actionGroup]() {
            if (QAction *currentAction = actionGroup->checkedAction())
                currentAction->setChecked(false);
            d->tool->setCurrentToolType(CreatorTool::ToolType::Pointer);
        });
        connect(d->tool, &CreatorTool::propertyEditorRequest, this, &InterfaceDocument::showPropertyEditor);
        connect(d->tool, &CreatorTool::informUser, this, &InterfaceDocument::showInfoMessage);
    }

    auto actCreateFunctionType = new QAction(tr("Function Type"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateFunctionType, "Function Type", "Create FunctionType object");
    actCreateFunctionType->setCheckable(true);
    actCreateFunctionType->setActionGroup(actionGroup);
    connect(actCreateFunctionType, &QAction::triggered, this, [this]() {
        d->tool->setCurrentToolType(CreatorTool::ToolType::FunctionType);
        qWarning() << Q_FUNC_INFO << "Not implemented yet.";
    });
    actCreateFunctionType->setIcon(QIcon(":/tab_interface/toolbar/icns/function_type.svg"));

    auto actCreateFunction = new QAction(tr("Function"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateFunction, "Function", "Create Function object");
    actCreateFunction->setCheckable(true);
    actCreateFunction->setActionGroup(actionGroup);
    connect(actCreateFunction, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(CreatorTool::ToolType::Function); });
    actCreateFunction->setIcon(QIcon(":/tab_interface/toolbar/icns/function.svg"));

    auto actCreateProvidedInterface = new QAction(tr("Provided Interface"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, actCreateProvidedInterface, "Provided Interface", "Create Provided Interface object");
    actCreateProvidedInterface->setCheckable(true);
    actCreateProvidedInterface->setActionGroup(actionGroup);
    connect(actCreateProvidedInterface, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(CreatorTool::ToolType::ProvidedInterface); });
    actCreateProvidedInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/pi.svg"));

    auto actCreateRequiredInterface = new QAction(tr("Required Interface"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, actCreateRequiredInterface, "Required Interface", "Create Required Interface object");
    actCreateRequiredInterface->setCheckable(true);
    actCreateRequiredInterface->setActionGroup(actionGroup);
    connect(actCreateRequiredInterface, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(CreatorTool::ToolType::RequiredInterface); });
    actCreateRequiredInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/ri.svg"));

    auto actCreateComment = new QAction(tr("Comment"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateComment, "Comment", "Create Comment object");
    actCreateComment->setCheckable(true);
    actCreateComment->setActionGroup(actionGroup);
    connect(actCreateComment, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(CreatorTool::ToolType::Comment); });
    actCreateComment->setIcon(QIcon(":/tab_interface/toolbar/icns/comment.svg"));

    //    auto actGroupConnections = new QAction(tr("Group"));
    //    actGroupConnections->setCheckable(true);
    //    actGroupConnections->setActionGroup(actionGroup);
    //    connect(actGroupConnections, &QAction::triggered, this,
    //            [this]() { qWarning() << Q_FUNC_INFO << "Not implemented yet."; });
    //    actGroupConnections->setIcon(QIcon(":/tab_interface/toolbar/icns/group.svg"));

    auto actCreateConnection = new QAction(tr("Connection"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateConnection, "Connection", "Create Connection object");
    actCreateConnection->setCheckable(true);
    actCreateConnection->setActionGroup(actionGroup);
    connect(actCreateConnection, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(CreatorTool::ToolType::MultiPointConnection); });
    actCreateConnection->setIcon(QIcon(":/tab_interface/toolbar/icns/connection.svg"));

    d->actCreateConnectionGroup = new QAction(tr("Create Connection Group"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, d->actCreateConnectionGroup, "Connection Group", "Create Connection group");
    d->actCreateConnectionGroup->setActionGroup(actionGroup);
    connect(d->actCreateConnectionGroup, &QAction::triggered, this, [this]() { d->tool->groupSelectedItems(); });
    d->actCreateConnectionGroup->setIcon(QIcon(":/tab_interface/toolbar/icns/connection_group.svg"));

    d->actRemove = new QAction(tr("Remove"));
    ActionsManager::registerAction(Q_FUNC_INFO, d->actRemove, "Remove", "Remove selected object");
    d->actRemove->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/remove.svg")));
    d->actRemove->setEnabled(false);
    d->actRemove->setShortcut(QKeySequence::Delete);
    connect(d->actRemove, &QAction::triggered, this, [this]() { d->tool->removeSelectedItems(); });

    d->actZoomIn = new QAction(tr("Zoom In"));
    ActionsManager::registerAction(Q_FUNC_INFO, d->actZoomIn, "Zoom In", "Scale up the current scene");
    d->actZoomIn->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/zoom_in.svg")));
    d->actZoomIn->setShortcut(QKeySequence::ZoomIn);
    connect(d->actZoomIn, &QAction::triggered, this,
            [this]() { d->graphicsView->setZoom(d->graphicsView->zoom() + d->graphicsView->zoomStepPercent()); });

    d->actZoomOut = new QAction(tr("Zoom Out"));
    ActionsManager::registerAction(Q_FUNC_INFO, d->actZoomOut, "Zoom Out", "Scale down the current scene");
    d->actZoomOut->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/zoom_out.svg")));
    d->actZoomOut->setShortcut(QKeySequence::ZoomOut);
    connect(d->actZoomOut, &QAction::triggered, this,
            [this]() { d->graphicsView->setZoom(d->graphicsView->zoom() - d->graphicsView->zoomStepPercent()); });

    d->actExitToRoot = new QAction(tr("Exit to root funtion"));
    d->actExitToRoot->setActionGroup(actionGroup);
    d->actExitToRoot->setEnabled(false);
    connect(d->actExitToRoot, &QAction::triggered, this, [this]() { d->itemsModel->changeRootItem({}); });
    d->actExitToRoot->setIcon(QIcon(":/tab_interface/toolbar/icns/exit.svg"));

    d->actExitToParent = new QAction(tr("Exit to parent function"));
    d->actExitToParent->setActionGroup(actionGroup);
    d->actExitToParent->setEnabled(false);
    connect(d->actExitToParent, &QAction::triggered, this, [this]() {
        aadl::AADLObject *parentObject =
                d->objectsModel->rootObject() ? d->objectsModel->rootObject()->parentObject() : nullptr;
        d->itemsModel->changeRootItem(parentObject ? parentObject->id() : shared::InvalidId);
    });
    d->actExitToParent->setIcon(QIcon(":/tab_interface/toolbar/icns/exit_parent.svg"));

    d->m_toolbarActions = { actCreateFunctionType, actCreateFunction, actCreateProvidedInterface,
        actCreateRequiredInterface, actCreateComment, actCreateConnection, d->actCreateConnectionGroup, d->actRemove,
        d->actZoomIn, d->actZoomOut, d->actExitToRoot, d->actExitToParent };

    connect(d->objectsModel, &aadl::AADLObjectsModel::rootObjectChanged, this, [this]() {
        if (d->actExitToRoot) {
            d->actExitToRoot->setEnabled(nullptr != d->objectsModel->rootObject());
        }
        if (d->actExitToParent) {
            d->actExitToParent->setEnabled(nullptr != d->objectsModel->rootObject());
        }
    });
    connect(d->itemsModel->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            [this](const QItemSelection &selected, const QItemSelection & /*deselected*/) {
                d->actRemove->setEnabled(!selected.isEmpty());
                const QModelIndexList idxs = selected.indexes();
                auto it = std::find_if(idxs.cbegin(), idxs.cend(), [](const QModelIndex &index) {
                    return index.data(static_cast<int>(aadl::AADLObjectsModel::AADLRoles::TypeRole)).toInt()
                            == static_cast<int>(aadl::AADLObject::Type::Connection);
                });
                d->actCreateConnectionGroup->setEnabled(it != std::cend(idxs));
            });
    return d->m_toolbarActions;
}

QWidget *InterfaceDocument::createGraphicsView()
{
    if (!d->graphicsView) {
        d->graphicsView = new aadlinterface::GraphicsView;
        connect(d->graphicsView, &aadlinterface::GraphicsView::zoomChanged, this, [this](qreal percent) {
            d->itemsModel->zoomChanged();
            d->actZoomIn->setEnabled(!qFuzzyCompare(percent, d->graphicsView->maxZoomPercent()));
            d->actZoomOut->setEnabled(!qFuzzyCompare(percent, d->graphicsView->minZoomPercent()));
        });

        connect(d->graphicsView, &GraphicsView::entityDropped, this, &InterfaceDocument::importEntity);
    }
    d->graphicsView->setScene(d->itemsModel->scene());
    d->graphicsView->setUpdatesEnabled(false);
    d->itemsModel->updateSceneRect();
    d->graphicsView->setUpdatesEnabled(true);
    return d->graphicsView;
}

QTreeView *InterfaceDocument::createModelView()
{
    if (d->objectsView)
        return d->objectsView;

    d->objectsView = new QTreeView;
    d->objectsView->setObjectName(QLatin1String("AADLModelView"));
    d->objectsView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    d->objectsView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    d->objectsView->setModel(d->objectsModel);
    d->objectsView->setSelectionModel(d->itemsModel->selectionModel());
    d->objectsModel->setHeaderData(0, Qt::Horizontal, tr("AADL Structure"), Qt::DisplayRole);

    return d->objectsView;
}

QTreeView *InterfaceDocument::createImportView()
{
    if (d->importView)
        return d->importView;

    d->importView = new AADLObjectsTreeView;
    d->importView->setObjectName(QLatin1String("ImportView"));
    d->importView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    d->importView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    d->importView->setModel(d->importModel);
    d->importModel->setHeaderData(0, Qt::Horizontal, tr("Import Component"), Qt::DisplayRole);

    return d->importView;
}

void InterfaceDocument::showNIYGUI(const QString &title)
{
    QString header = title.isEmpty() ? "NIY" : title;
    QWidget *mainWindow = qobject_cast<QWidget *>(parent());
    QMessageBox::information(mainWindow, header, "Not implemented yet!");
}
}
