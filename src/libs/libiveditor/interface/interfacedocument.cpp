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
#include "interface/colors/colormanagerdialog.h"
#include "interface/properties/dynamicpropertymanager.h"
#include "interface/properties/propertiesdialog.h"
#include "xmldocexporter.h"

#include <QAction>
#include <QBuffer>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGuiApplication>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QMutex>
#include <QMutexLocker>
#include <QScreen>
#include <QShortcut>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QToolBar>
#include <QTreeView>
#include <QUndoStack>
#include <QVBoxLayout>

namespace aadlinterface {

struct InterfaceDocument::InterfaceDocumentPrivate {
    QUndoStack *commandsStack { nullptr };

    QString filePath;
    int lastSavedIndex { 0 };
    bool dirty { false };

    QPointer<aadlinterface::GraphicsView> graphicsView { nullptr };
    QTreeView *objectsView { nullptr };
    AADLItemModel *model { nullptr };
    QTreeView *importView { nullptr };
    AADLItemModel *importModel { nullptr };

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
    connect(d->commandsStack, &QUndoStack::indexChanged, this, &InterfaceDocument::updateDirtyness);

    d->model = new AADLItemModel(this);
    connect(d->model, &AADLItemModel::itemClicked, this, &InterfaceDocument::onItemClicked);
    connect(d->model, &AADLItemModel::itemDoubleClicked, this, &InterfaceDocument::onItemDoubleClicked);

    d->importModel = new AADLItemModel(this);
}

InterfaceDocument::~InterfaceDocument()
{
    delete d->objectsView;
    delete d->asnDataTypes;
    delete d->graphicsView;
    delete d;
}

void InterfaceDocument::init()
{
    createView();
    createModelView();
    createImportView();
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
    return d->model->scene();
}

QWidget *InterfaceDocument::view() const
{
    return d->graphicsView;
}

QTreeView *InterfaceDocument::modelView() const
{
    return d->objectsView;
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
        resetDirtyness();
    }
    return created;
}

bool InterfaceDocument::load(const QString &path)
{
    const bool loaded = loadImpl(path);

    if (loaded) {
        setPath(path);
        d->commandsStack->clear();
        resetDirtyness();
        Q_EMIT dirtyChanged(false);
    }

    return loaded;
}

bool InterfaceDocument::import()
{
    d->importModel->clear();
    createImportView();
    const QString path =
            QStringLiteral("%1/workspace").arg(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

    QDirIterator it(path);
    while (it.hasNext()) {
        importImpl(it.next() + QDir::separator() + QLatin1String("interfaceview.xml"));
    }
    d->importView->show();
    return true;
}

bool InterfaceDocument::exportSelected()
{
    QList<aadl::AADLObject *> objects;
    QStringList exportName;
    for (const auto id : d->model->selectionModel()->selection().indexes()) {
        const int role = static_cast<int>(aadl::AADLObjectsModel::AADLRoles::IdRole);
        if (aadl::AADLObject *object = d->model->getObject(id.data(role).toUuid())) {
            if (object->isFunction() && object->parentObject() == nullptr) {
                exportName.append(object->attr(QLatin1String("name")).toString());
            }
            objects.append(object);
        }
    }

    static const QString exportFileName("interfaceview.xml");
    const QString targetDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator()
            + QLatin1String("workspace") + QDir::separator() + exportName.join(QLatin1Char('_'));

    const bool ok = shared::ensureDirExists(targetDir);
    if (!ok) {
        qWarning() << "Unable to create directory" << targetDir;
        return {};
    }

    const QString exportFilePath = QString("%1/%2").arg(targetDir, exportFileName);

    const QFileInfo fi(exportFilePath);
    if (fi.exists()) {
        qWarning() << "Current object already exported: " << exportFilePath;
        return false;
    }

    QBuffer buffer;
    if (buffer.open(QIODevice::WriteOnly)) {
        if (XmlDocExporter::exportObjects(objects, &buffer)) {
            buffer.close();
            QFile file(fi.absoluteFilePath());
            if (file.open(QIODevice::WriteOnly)) {
                file.write(buffer.buffer());
                file.close();
                return true;
            } else {
                qWarning() << "Can't export file: " << file.errorString();
            }
        }
    }
    return false;
}

bool InterfaceDocument::importImpl(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        qWarning() << Q_FUNC_INFO << "Invalid path";
        return false;
    }

    aadl::AADLXMLReader parser;
    connect(&parser, &aadl::AADLXMLReader::objectsParsed, this, [this](const QVector<aadl::AADLObject *> &objects) {
        for (const auto obj : objects) {
            d->importModel->addObject(obj);
        }
        for (auto obj : objects) {
            if (!obj->postInit()) {
                switch (obj->aadlType()) {
                case aadl::AADLObject::Type::Connection: {
                    if (aadl::AADLObjectFunction *parentFn =
                                    qobject_cast<aadl::AADLObjectFunction *>(obj->parentObject())) {
                        parentFn->removeChild(obj);
                    }
                    break;
                }
                default:
                    break;
                }

                d->importModel->removeObject(obj);
            }
        }
    });
    connect(&parser, &aadl::AADLXMLReader::metaDataParsed, this, [this, path](const QVariantMap &metadata) {
        //        setAsn1FileName(metadata["asn1file"].toString());
        //        setMscFileName(metadata["mscfile"].toString());
    });
    connect(&parser, &aadl::AADLXMLReader::error, [](const QString &msg) { qWarning() << msg; });

    return parser.readFile(path);
}

bool InterfaceDocument::save(const QString &path)
{
    return false;
}

void InterfaceDocument::close()
{
    d->model->clear();
    setPath(QString());
    d->commandsStack->clear();
    resetDirtyness();
    Q_EMIT dirtyChanged(false);
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
    Q_EMIT dirtyChanged(true);
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
    return d->dirty;
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
    return d->model->objects();
}

aadl::AADLObjectsModel *InterfaceDocument::objectsModel() const
{
    return d->model;
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
        const bool forceTitleUpdate = d->filePath != filePath && !isDirty();

        setPath(filePath);

        if (forceTitleUpdate)
            Q_EMIT dirtyChanged(false);
        else
            resetDirtyness();
    }
}

/*!
   \brief InterfaceDocument::setObjects
 */
void InterfaceDocument::setObjects(const QVector<aadl::AADLObject *> &objects)
{
    if (d->model->initFromObjects(objects)) {
        d->model->setRootObject({});
    }
}

void InterfaceDocument::updateDirtyness()
{
    const bool wasDirty = isDirty();
    d->dirty = d->lastSavedIndex != d->commandsStack->index();

    if (wasDirty != isDirty()) {
        Q_EMIT dirtyChanged(isDirty());
    }
}

void InterfaceDocument::onItemClicked(shared::Id id) { }

void InterfaceDocument::onItemDoubleClicked(shared::Id id)
{
    if (id.isNull()) {
        return;
    }

    if (auto entity = d->model->getObject(id)) {
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

void InterfaceDocument::setPath(const QString &path)
{
    if (path != d->filePath) {
        d->filePath = path;
        Q_EMIT titleChanged();
    }
}

void InterfaceDocument::resetDirtyness()
{
    d->lastSavedIndex = d->commandsStack->index();
    updateDirtyness();
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
        d->tool = new CreatorTool(d->graphicsView, d->model, this);
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
    ActionsManager::registerAction(Q_FUNC_INFO, d->actCreateConnectionGroup, "Connection", "Create Connection group");
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
    connect(d->actExitToRoot, &QAction::triggered, this, [this]() { d->model->changeRootItem({}); });
    d->actExitToRoot->setIcon(QIcon(":/tab_interface/toolbar/icns/exit.svg"));

    d->actExitToParent = new QAction(tr("Exit to parent function"));
    d->actExitToParent->setActionGroup(actionGroup);
    d->actExitToParent->setEnabled(false);
    connect(d->actExitToParent, &QAction::triggered, this, [this]() {
        aadl::AADLObject *parentObject = d->model->rootObject() ? d->model->rootObject()->parentObject() : nullptr;
        d->model->changeRootItem(parentObject ? parentObject->id() : shared::InvalidId);
    });
    d->actExitToParent->setIcon(QIcon(":/tab_interface/toolbar/icns/exit_parent.svg"));

    d->m_toolbarActions = { actCreateFunctionType, actCreateFunction, actCreateProvidedInterface,
        actCreateRequiredInterface, actCreateComment, actCreateConnection, d->actCreateConnectionGroup, d->actRemove,
        d->actZoomIn, d->actZoomOut, d->actExitToRoot, d->actExitToParent };

    connect(d->model, &AADLItemModel::rootObjectChanged, this, [this]() {
        if (d->actExitToRoot) {
            d->actExitToRoot->setEnabled(nullptr != d->model->rootObject());
        }
        if (d->actExitToParent) {
            d->actExitToParent->setEnabled(nullptr != d->model->rootObject());
        }
    });
    connect(d->model->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            [this](const QItemSelection &selected, const QItemSelection &deselected) {
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

QWidget *InterfaceDocument::createView()
{
    if (!d->graphicsView) {
        d->graphicsView = new aadlinterface::GraphicsView;
        connect(d->graphicsView, &aadlinterface::GraphicsView::zoomChanged, this, [this](qreal percent) {
            d->model->zoomChanged();
            d->actZoomIn->setEnabled(!qFuzzyCompare(percent, d->graphicsView->maxZoomPercent()));
            d->actZoomOut->setEnabled(!qFuzzyCompare(percent, d->graphicsView->minZoomPercent()));
        });
    }
    d->graphicsView->setScene(d->model->scene());
    d->graphicsView->setUpdatesEnabled(false);
    d->model->updateSceneRect();
    d->graphicsView->setUpdatesEnabled(true);
    return d->graphicsView;
}

QTreeView *InterfaceDocument::createModelView()
{
    if (d->objectsView)
        return d->objectsView;

    d->objectsView = new QTreeView;
    d->objectsView->setWindowTitle(tr("AADL Structure"));
    d->objectsView->setObjectName(QLatin1String("AADLModelView"));
    d->objectsView->setWindowFlag(Qt::Tool);
    d->objectsView->setWindowFlag(Qt::WindowStaysOnTopHint);
    d->objectsView->setHeaderHidden(true);
    d->objectsView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    d->objectsView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    d->objectsView->setModel(d->model);
    d->objectsView->setSelectionModel(d->model->selectionModel());

    return d->objectsView;
}

QTreeView *InterfaceDocument::createImportView()
{
    if (d->importView)
        return d->importView;

    d->importView = new QTreeView;
    d->importView->setWindowTitle(tr("Import AADL"));
    d->importView->setObjectName(QLatin1String("ImportView"));
    d->importView->setWindowFlag(Qt::Tool);
    d->importView->setWindowFlag(Qt::WindowStaysOnTopHint);
    d->importView->setHeaderHidden(true);
    d->importView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    d->importView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    d->importView->setModel(d->importModel);
    d->importView->setSelectionModel(d->importModel->selectionModel());

    return d->objectsView;
}

void InterfaceDocument::showNIYGUI(const QString &title)
{
    QString header = title.isEmpty() ? "NIY" : title;
    QWidget *mainWindow = qobject_cast<QWidget *>(parent());
    QMessageBox::information(mainWindow, header, "Not implemented yet!");
}
}
