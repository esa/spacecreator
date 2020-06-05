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

#include "interfacetabdocument.h"

#include "aadlobjectcomment.h"
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "aadlxmlreader.h"
#include "baseitems/common/aadlutils.h"
#include "baseitems/graphicsview.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "commandsstack.h"
#include "context/action/actionsmanager.h"
#include "context/action/editor/dynactioneditor.h"
#include "creatortool.h"
#include "interface/aadlcommentgraphicsitem.h"
#include "interface/aadlconnectiongraphicsitem.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlfunctiontypegraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"
#include "interface/colors/colormanagerdialog.h"
#include "interface/commenttextdialog.h"
#include "interface/properties/dynamicpropertymanager.h"
#include "interface/properties/propertiesdialog.h"
#include "interfacetabgraphicsscene.h"

#include <QAction>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGuiApplication>
#include <QMenu>
#include <QMessageBox>
#include <QMutex>
#include <QMutexLocker>
#include <QScreen>
#include <QShortcut>
#include <QToolBar>
#include <QUndoStack>

static inline void dumpItem(QObject *obj, bool strict = false)
{
#ifdef NO_AADL_ITEM_DUMP
    return;
#endif

    auto item = qobject_cast<aadlinterface::InteractiveObject *>(obj);
    if (!item)
        return;

    auto comparePolygones = [](const QVector<QPointF> &v1, const QVector<QPointF> &v2) {
        if (v1.size() != v2.size())
            return false;

        for (int idx = 0; idx < v1.size(); ++idx) {
            if (v1.at(idx).toPoint() != v2.at(idx).toPoint())
                return false;
        }
        return true;
    };

    qDebug() << item->metaObject()->className() << "\n"
             << item->aadlObject()->props() << "\n"
             << item->aadlObject()->attrs();

    if (auto iface = qobject_cast<aadlinterface::AADLInterfaceGraphicsItem *>(item)) {
        qDebug() << "\nGraphics Iface geometry:"
                 << "\n"
                 << iface->scenePos() << "\n"
                 << iface->sceneBoundingRect() << "\n";
        qDebug() << "\nInternal Iface data:"
                 << "\n"
                 << iface->entity()->title() << "\n"
                 << aadlinterface::pos(iface->entity()->coordinates()) << "\n";
        Q_ASSERT(!strict || iface->scenePos().toPoint() == aadlinterface::pos(iface->entity()->coordinates()));
    } else if (auto connection = qobject_cast<aadlinterface::AADLConnectionGraphicsItem *>(item)) {
        qDebug() << "\nGraphics Connection geometry:"
                 << "\n"
                 << connection->points() << "\n"
                 << connection->graphicsPoints() << "\n";
        qDebug() << "\nInternal Connection data:"
                 << "\n"
                 << (connection->entity()->title().isEmpty() ? QStringLiteral("Connection %1<>%2")
                                                                       .arg(connection->startItem()->entity()->title(),
                                                                               connection->endItem()->entity()->title())
                                                             : connection->entity()->title())
                 << "\n"
                 << aadlinterface::polygon(connection->entity()->coordinates()) << "\n";
        Q_ASSERT(!strict
                || comparePolygones(
                        connection->graphicsPoints(), aadlinterface::polygon(connection->entity()->coordinates())));
        Q_ASSERT(!strict
                || comparePolygones(connection->points(), aadlinterface::polygon(connection->entity()->coordinates())));
    } else if (auto rectItem = qobject_cast<aadlinterface::AADLRectGraphicsItem *>(item)) {
        qDebug() << "\nGraphics" << rectItem->metaObject()->className() << "geometry:"
                 << "\n"
                 << rectItem->sceneBoundingRect() << "\n";
        qDebug() << "\nInternal Function data:"
                 << "\n"
                 << rectItem->aadlObject()->title() << "\n"
                 << aadlinterface::rect(rectItem->aadlObject()->coordinates()) << "\n";
        Q_ASSERT(!strict
                || rectItem->sceneBoundingRect().toRect()
                        == aadlinterface::rect(rectItem->aadlObject()->coordinates()).toRect());
    } else {
        qFatal("Not implemented trace");
    }
}

namespace aadlinterface {

struct InterfaceDocument::InterfaceDocumentPrivate {
    QGraphicsScene *scene { nullptr };
    QWidget *view { nullptr };
    QUndoStack *commandsStack { nullptr };

    QString filePath;
    int lastSavedIndex { 0 };
    bool dirty { false };

    aadlinterface::InterfaceTabGraphicsScene *graphicsScene { nullptr };
    QPointer<aadlinterface::GraphicsView> graphicsView { nullptr };
    aadl::AADLObjectsModel *model { nullptr };
    QAction *actRemove { nullptr };
    QAction *actZoomIn { nullptr };
    QAction *actZoomOut { nullptr };
    QAction *actExitToRoot { nullptr };
    QAction *actExitToParent { nullptr };
    QVector<QAction *> m_toolbarActions;

    aadlinterface::CreatorTool *tool { nullptr };
    QHash<shared::Id, QGraphicsItem *> items;

    QMutex *mutex { nullptr };
    QQueue<aadl::AADLObject *> rmQueu;
    QRectF desktopGeometry;
    QRectF prevItemsRect;
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

    d->model = new aadl::AADLObjectsModel(this);
    d->mutex = new QMutex(QMutex::NonRecursive);
    d->desktopGeometry = QGuiApplication::primaryScreen()->availableGeometry();

    connect(d->commandsStack, &QUndoStack::indexChanged, this, &InterfaceDocument::updateDirtyness);
    connect(d->model, &aadl::AADLObjectsModel::modelReset, this, &InterfaceDocument::clearScene);
    connect(d->model, &aadl::AADLObjectsModel::rootObjectChanged, this, &InterfaceDocument::onRootObjectChanged,
            Qt::QueuedConnection);
    connect(d->model, &aadl::AADLObjectsModel::aadlObjectAdded, this, &InterfaceDocument::onAADLObjectAdded);
    connect(d->model, &aadl::AADLObjectsModel::aadlObjectRemoved, this, &InterfaceDocument::onAADLObjectRemoved);
}

InterfaceDocument::~InterfaceDocument()
{
    delete d->graphicsView;
    delete d->mutex;
    delete d;
}

void InterfaceDocument::init()
{
    if (QGraphicsScene *scene = createScene()) {
        d->scene = scene;
    }
    if (QWidget *view = createView()) {
        d->view = view;
    }
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
    return d->scene;
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

bool InterfaceDocument::save(const QString & /*path*/)
{
    qWarning("Save is not implemented");
    return false;
}

void InterfaceDocument::close()
{
    d->model->clear();
    updateSceneRect();
    setPath(QString());
    resetDirtyness();
    Q_EMIT dirtyChanged(false);
}

QString InterfaceDocument::path() const
{
    return d->filePath;
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

QMenu *InterfaceDocument::customMenu() const
{
    QMenu *root = new QMenu(title());
    QAction *actCommonProps = root->addAction(tr("Common Properties"));
    connect(actCommonProps, &QAction::triggered, this, &InterfaceDocument::onAttributesManagerRequested);
    QAction *actDataTypes = root->addAction(tr("Data types"));
    connect(actDataTypes, &QAction::triggered, this, &InterfaceDocument::onDataTypesMenuInvoked);
    QAction *actColorScheme = root->addAction(tr("Color Scheme"));
    connect(actColorScheme, &QAction::triggered, this, &InterfaceDocument::onColorSchemeMenuInvoked);
    QAction *actDynContext = root->addAction(tr("Context Actions"));
    connect(actDynContext, &QAction::triggered, this, &InterfaceDocument::onDynContextEditorMenuInvoked);

    ActionsManager::registerAction(Q_FUNC_INFO, actCommonProps, "Edit Properties", "Show editor for common Properties");
    ActionsManager::registerAction(Q_FUNC_INFO, actDataTypes, "Edit Datatypes", "Show editor for common Datatypes");
    ActionsManager::registerAction(
            Q_FUNC_INFO, actColorScheme, "Edit Color scheme", "Show editor for common Color schemes");
    ActionsManager::registerAction(
            Q_FUNC_INFO, actDynContext, "Context actions", "Show editor for common custom context menu actions");

    return root;
}

const QHash<shared::Id, aadl::AADLObject *> &InterfaceDocument::objects() const
{
    return d->model->objects();
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

void InterfaceDocument::onAADLObjectAdded(aadl::AADLObject *object)
{
    auto propertyChanged = [this]() {
        if (auto object = qobject_cast<aadl::AADLObject *>(sender())) {
            if (auto item = d->items.value(object->id()))
                updateItem(item);
        }
    };

    auto item = d->items.value(object->id());
    if (!item) {
        item = createItemForObject(object);
        connect(object, &aadl::AADLObject::coordinatesChanged, this, propertyChanged, Qt::QueuedConnection);
        if (auto clickable = qobject_cast<aadlinterface::InteractiveObject *>(item->toGraphicsObject())) {
            connect(clickable, &aadlinterface::InteractiveObject::clicked, this, &InterfaceDocument::onItemClicked);
            connect(clickable, &aadlinterface::InteractiveObject::doubleClicked, this,
                    &InterfaceDocument::onItemDoubleClicked, Qt::QueuedConnection);
        }
        d->items.insert(object->id(), item);
        if (d->graphicsScene != item->scene())
            d->graphicsScene->addItem(item);
    }
    updateItem(item);
}

void InterfaceDocument::onAADLObjectRemoved(aadl::AADLObject *object)
{
    d->rmQueu.enqueue(object);

    while (d->rmQueu.size()) {
        if (d->mutex->tryLock()) {
            aadl::AADLObject *obj = d->rmQueu.dequeue();
            if (auto item = d->items.take(obj->id())) {
                d->graphicsScene->removeItem(item);
                delete item;
                updateSceneRect();
            }
            d->mutex->unlock();
        }
    }
}

void InterfaceDocument::onItemClicked()
{
    dumpItem(sender());
}

void InterfaceDocument::onItemDoubleClicked()
{
    if (auto clickedItem = qobject_cast<aadlinterface::InteractiveObject *>(sender())) {
        if (auto clickedEntity = qobject_cast<aadl::AADLObject *>(clickedItem->aadlObject())) {
            if (clickedEntity->aadlType() == aadl::AADLObject::Type::Function) {
                if (auto function = qobject_cast<aadl::AADLObjectFunction *>(clickedEntity)) {
                    if (function->hasNestedChildren() && !function->isRootObject()) {
                        changeRootItem(function->id());
                        return;
                    }
                }
            } else if (clickedEntity->aadlType() == aadl::AADLObject::Type::Comment) {
                if (clickedItem->type() == aadlinterface::AADLCommentGraphicsItem::Type) {
                    auto dialog = new aadlinterface::CommentTextDialog(
                            qobject_cast<aadl::AADLObjectComment *>(clickedEntity), qobject_cast<QWidget *>(parent()));
                    dialog->setAttribute(Qt::WA_DeleteOnClose);
                    dialog->open();
                }
            } else if (clickedEntity->aadlType() != aadl::AADLObject::Type::Connection) {
                showPropertyEditor(clickedEntity);
            }
        }
    }
}

void InterfaceDocument::onRootObjectChanged(shared::Id rootId)
{
    Q_UNUSED(rootId)

    QMutexLocker lockme(d->mutex);

    d->actExitToRoot->setEnabled(nullptr != d->model->rootObject());
    d->actExitToParent->setEnabled(nullptr != d->model->rootObject());

    QList<aadl::AADLObject *> objects = d->model->visibleObjects();
    std::stable_sort(objects.begin(), objects.end(),
            [](aadl::AADLObject *obj1, aadl::AADLObject *obj2) { return obj1->aadlType() < obj2->aadlType(); });

    for (auto it = objects.cbegin(); it != objects.cend(); ++it)
        onAADLObjectAdded(*it);

    updateSceneRect();
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
    aadlinterface::PropertiesDialog *dialog =
            new aadlinterface::PropertiesDialog(obj, qobject_cast<QWidget *>(parent()));
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
    connect(&parser, &aadl::AADLXMLReader::error, [](const QString &msg) { qWarning() << msg; });

    return parser.readFile(path);
}

QVector<QAction *> InterfaceDocument::initActions()
{
    auto actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);

    if (!d->tool) {
        d->tool = new aadlinterface::CreatorTool(d->graphicsView, d->model, this);
        connect(d->tool, &aadlinterface::CreatorTool::created, this, [this, actionGroup]() {
            if (QAction *currentAction = actionGroup->checkedAction())
                currentAction->setChecked(false);
            d->tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::Pointer);
        });
        connect(d->tool, &aadlinterface::CreatorTool::propertyEditorRequest, this,
                &InterfaceDocument::showPropertyEditor);
        connect(d->tool, &aadlinterface::CreatorTool::informUser, this, &InterfaceDocument::showInfoMessage);
    }

    auto actCreateFunctionType = new QAction(tr("Function Type"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateFunctionType, "Function Type", "Create FunctionType object");
    actCreateFunctionType->setCheckable(true);
    actCreateFunctionType->setActionGroup(actionGroup);
    connect(actCreateFunctionType, &QAction::triggered, this, [this]() {
        d->tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::FunctionType);
        qWarning() << Q_FUNC_INFO << "Not implemented yet.";
    });
    actCreateFunctionType->setIcon(QIcon(":/tab_interface/toolbar/icns/function_type.svg"));

    auto actCreateFunction = new QAction(tr("Function"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateFunction, "Function", "Create Function object");
    actCreateFunction->setCheckable(true);
    actCreateFunction->setActionGroup(actionGroup);
    connect(actCreateFunction, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::Function); });
    actCreateFunction->setIcon(QIcon(":/tab_interface/toolbar/icns/function.svg"));

    auto actCreateProvidedInterface = new QAction(tr("Provided Interface"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, actCreateProvidedInterface, "Provided Interface", "Create Provided Interface object");
    actCreateProvidedInterface->setCheckable(true);
    actCreateProvidedInterface->setActionGroup(actionGroup);
    connect(actCreateProvidedInterface, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::ProvidedInterface); });
    actCreateProvidedInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/pi.svg"));

    auto actCreateRequiredInterface = new QAction(tr("Required Interface"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, actCreateRequiredInterface, "Required Interface", "Create Required Interface object");
    actCreateRequiredInterface->setCheckable(true);
    actCreateRequiredInterface->setActionGroup(actionGroup);
    connect(actCreateRequiredInterface, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::RequiredInterface); });
    actCreateRequiredInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/ri.svg"));

    auto actCreateComment = new QAction(tr("Comment"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateComment, "Comment", "Create Comment object");
    actCreateComment->setCheckable(true);
    actCreateComment->setActionGroup(actionGroup);
    connect(actCreateComment, &QAction::triggered, this,
            [this]() { d->tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::Comment); });
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
            [this]() { d->tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::MultiPointConnection); });
    actCreateConnection->setIcon(QIcon(":/tab_interface/toolbar/icns/connection.svg"));

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
    connect(d->actExitToRoot, &QAction::triggered, this, [this]() { changeRootItem({}); });
    d->actExitToRoot->setIcon(QIcon(":/tab_interface/toolbar/icns/exit.svg"));

    d->actExitToParent = new QAction(tr("Exit to parent function"));
    d->actExitToParent->setActionGroup(actionGroup);
    d->actExitToParent->setEnabled(false);
    connect(d->actExitToParent, &QAction::triggered, this, [this]() {
        aadl::AADLObject *parentObject = d->model->rootObject() ? d->model->rootObject()->parentObject() : nullptr;
        changeRootItem(parentObject ? parentObject->id() : shared::InvalidId);
    });
    d->actExitToParent->setIcon(QIcon(":/tab_interface/toolbar/icns/exit_parent.svg"));

    d->m_toolbarActions = { actCreateFunctionType, actCreateFunction, actCreateProvidedInterface,
        actCreateRequiredInterface, actCreateComment, actCreateConnection, d->actRemove, d->actZoomIn, d->actZoomOut,
        d->actExitToRoot, d->actExitToParent };
    return d->m_toolbarActions;
}

QWidget *InterfaceDocument::createView()
{
    if (!d->graphicsView) {
        d->graphicsView = new aadlinterface::GraphicsView;
        connect(d->graphicsView, &aadlinterface::GraphicsView::zoomChanged, this, [this](qreal percent) {
            for (auto item : d->graphicsScene->selectedItems()) {
                if (auto iObj = qobject_cast<aadlinterface::InteractiveObject *>(item->toGraphicsObject())) {
                    iObj->updateGripPoints();
                }
            }
            d->actZoomIn->setEnabled(!qFuzzyCompare(percent, d->graphicsView->maxZoomPercent()));
            d->actZoomOut->setEnabled(!qFuzzyCompare(percent, d->graphicsView->minZoomPercent()));
        });

        auto sc = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), qobject_cast<QWidget *>(d->graphicsView->window()));
        sc->setContext(Qt::ApplicationShortcut);
        connect(sc, &QShortcut::activated, this, [this]() {
            for (auto item : d->graphicsScene->items()) {
                dumpItem(item->toGraphicsObject(), true);
            }
        });
    }
    d->graphicsView->setScene(d->graphicsScene);
    updateSceneRect();
    return d->graphicsView;
}

QGraphicsScene *InterfaceDocument::createScene()
{
    if (!d->graphicsScene) {
        d->graphicsScene = new aadlinterface::InterfaceTabGraphicsScene(this);
        connect(d->graphicsScene, &QGraphicsScene::selectionChanged, [this]() {
            const QList<QGraphicsItem *> selectedItems = d->graphicsScene->selectedItems();
            auto it = std::find_if(selectedItems.cbegin(), selectedItems.cend(), [](QGraphicsItem *item) {
                return qobject_cast<aadlinterface::InteractiveObject *>(item->toGraphicsObject()) != nullptr;
            });
            d->actRemove->setEnabled(it != selectedItems.cend());
        });
    }
    return d->graphicsScene;
}

QGraphicsItem *InterfaceDocument::createItemForObject(aadl::AADLObject *obj)
{
    Q_ASSERT(obj);
    if (!obj)
        return nullptr;

    QGraphicsItem *parentItem = obj->parentObject() ? d->items.value(obj->parentObject()->id()) : nullptr;
    auto nestedGeomtryConnect = [this](QGraphicsItem *parentItem, aadlinterface::InteractiveObject *child) {
        if (parentItem) {
            if (auto iObjParent = qobject_cast<aadlinterface::InteractiveObject *>(parentItem->toGraphicsObject()))
                this->connect(child, &aadlinterface::InteractiveObject::boundingBoxChanged, iObjParent,
                        &aadlinterface::InteractiveObject::scheduleLayoutUpdate, Qt::QueuedConnection);
        }
    };

    switch (obj->aadlType()) {
    case aadl::AADLObject::Type::Comment: {
        auto comment =
                new aadlinterface::AADLCommentGraphicsItem(qobject_cast<aadl::AADLObjectComment *>(obj), parentItem);
        nestedGeomtryConnect(parentItem, comment);
        return comment;
    } break;
    case aadl::AADLObject::Type::RequiredInterface:
    case aadl::AADLObject::Type::ProvidedInterface:
        return new aadlinterface::AADLInterfaceGraphicsItem(qobject_cast<aadl::AADLObjectIface *>(obj), parentItem);
    case aadl::AADLObject::Type::Connection:
        if (auto connection = qobject_cast<aadl::AADLObjectConnection *>(obj)) {
            aadl::AADLObjectIface *ifaceStart = connection->sourceInterface();
            auto startItem = qgraphicsitem_cast<aadlinterface::AADLInterfaceGraphicsItem *>(
                    ifaceStart ? d->items.value(ifaceStart->id()) : nullptr);

            aadl::AADLObjectIface *ifaceEnd = connection->targetInterface();
            auto endItem = qgraphicsitem_cast<aadlinterface::AADLInterfaceGraphicsItem *>(
                    ifaceEnd ? d->items.value(ifaceEnd->id()) : nullptr);

            return new aadlinterface::AADLConnectionGraphicsItem(connection, startItem, endItem, parentItem);
        }
        break;
    case aadl::AADLObject::Type::Function: {
        auto function =
                new aadlinterface::AADLFunctionGraphicsItem(qobject_cast<aadl::AADLObjectFunction *>(obj), parentItem);
        nestedGeomtryConnect(parentItem, function);
        return function;
    } break;
    case aadl::AADLObject::Type::FunctionType: {
        auto functionType = new aadlinterface::AADLFunctionTypeGraphicsItem(
                qobject_cast<aadl::AADLObjectFunctionType *>(obj), parentItem);
        nestedGeomtryConnect(parentItem, functionType);
        return functionType;
    } break;
    default: {
        qCritical() << "Unknown object type:" << obj->aadlType();
        break;
    }
    }

    return nullptr;
}

aadlinterface::AADLFunctionGraphicsItem *InterfaceDocument::rootItem() const
{
    if (auto rootEntity = d->model->rootObject())
        return qgraphicsitem_cast<aadlinterface::AADLFunctionGraphicsItem *>(d->items.value(rootEntity->id()));
    return nullptr;
}

void InterfaceDocument::updateItem(QGraphicsItem *item)
{
    Q_ASSERT(item);
    if (!item)
        return;

    switch (item->type()) {
    case aadlinterface::AADLCommentGraphicsItem::Type:
        updateComment(qgraphicsitem_cast<aadlinterface::AADLCommentGraphicsItem *>(item));
        break;
    case aadlinterface::AADLConnectionGraphicsItem::Type:
        updateConnection(qgraphicsitem_cast<aadlinterface::AADLConnectionGraphicsItem *>(item));
        break;
    case aadlinterface::AADLFunctionGraphicsItem::Type:
        updateFunction(qgraphicsitem_cast<aadlinterface::AADLFunctionGraphicsItem *>(item));
        break;
    case aadlinterface::AADLFunctionTypeGraphicsItem::Type:
        updateFunctionType(qgraphicsitem_cast<aadlinterface::AADLFunctionTypeGraphicsItem *>(item));
        break;
    case aadlinterface::AADLInterfaceGraphicsItem::Type:
        updateInterface(qgraphicsitem_cast<aadlinterface::AADLInterfaceGraphicsItem *>(item));
        break;
    default:
        break;
    }

    if (d->mutex->tryLock()) {
        updateSceneRect();
        d->mutex->unlock();
    }
}

void InterfaceDocument::updateComment(aadlinterface::AADLCommentGraphicsItem *comment)
{
    comment->updateFromEntity();
}

void InterfaceDocument::updateInterface(aadlinterface::AADLInterfaceGraphicsItem *iface)
{
    iface->updateFromEntity();
}

void InterfaceDocument::updateFunction(aadlinterface::AADLFunctionGraphicsItem *function)
{
    function->updateFromEntity();
}

void InterfaceDocument::updateFunctionType(aadlinterface::AADLFunctionTypeGraphicsItem *functionType)
{
    functionType->updateFromEntity();
}

void InterfaceDocument::updateConnection(aadlinterface::AADLConnectionGraphicsItem *connection)
{
    connection->updateFromEntity();
}

void InterfaceDocument::clearScene()
{
    if (d->graphicsScene) {
        d->graphicsScene->clear();
    }
    d->items.clear();
}

void InterfaceDocument::changeRootItem(shared::Id id)
{
    if (d->model->rootObjectId() == id)
        return;

    const QVariantList rootEntityParams { QVariant::fromValue(d->model), QVariant::fromValue(id) };
    const auto geometryCmd =
            aadlinterface::cmd::CommandsFactory::create(aadlinterface::cmd::ChangeRootEntity, rootEntityParams);
    aadlinterface::cmd::CommandsStack::current()->push(geometryCmd);
}

void InterfaceDocument::showNIYGUI(const QString &title)
{
    QString header = title.isEmpty() ? "NIY" : title;
    QWidget *mainWindow = qobject_cast<QWidget *>(parent());
    QMessageBox::information(mainWindow, header, "Not implemented yet!");
}

void InterfaceDocument::updateSceneRect()
{
    const QRectF itemsRect = d->graphicsScene->itemsBoundingRect();
    if (itemsRect.isEmpty()) {
        d->graphicsScene->setSceneRect(d->desktopGeometry);
        return;
    }

    if (itemsRect != d->prevItemsRect) {
        const QRectF sceneRect = d->graphicsScene->sceneRect();
        const QRectF updated = sceneRect.united(itemsRect);

        if (sceneRect != updated) {
            d->graphicsView->setUpdatesEnabled(false);
            d->graphicsScene->setSceneRect(updated);
            d->graphicsView->setUpdatesEnabled(true);

            d->prevItemsRect = itemsRect;
        }
    }
}

}
