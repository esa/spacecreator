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
#include <QUndoStack>

#define WARN_NOT_IMPLEMENTED qWarning() << Q_FUNC_INFO << "Not implemented yet."

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

/*!
\class aadlinterface::InterfaceTabDocument
\brief aadlinterface::InterfaceTabDocument is the document for graphical AADL data (loaded from the XML).

\sa aadlinterface::AbstractTabDocument
*/

InterfaceTabDocument::InterfaceTabDocument(QObject *parent)
    : AbstractTabDocument(parent)
    , m_model(new aadl::AADLObjectsModel(this))
    , m_mutex(new QMutex(QMutex::NonRecursive))
    , m_desktopGeometry(QGuiApplication::primaryScreen()->availableGeometry()) // TODO: use dynamic screen detection
{
    connect(m_model, &aadl::AADLObjectsModel::modelReset, this, &InterfaceTabDocument::clearScene);
    connect(m_model, &aadl::AADLObjectsModel::rootObjectChanged, this, &InterfaceTabDocument::onRootObjectChanged,
            Qt::QueuedConnection);
    connect(m_model, &aadl::AADLObjectsModel::aadlObjectAdded, this, &InterfaceTabDocument::onAADLObjectAdded);
    connect(m_model, &aadl::AADLObjectsModel::aadlObjectRemoved, this, &InterfaceTabDocument::onAADLObjectRemoved);
}

InterfaceTabDocument::~InterfaceTabDocument()
{
    if (m_graphicsView && !m_graphicsView->parent())
        delete m_graphicsView;

    delete m_mutex;
}

QWidget *InterfaceTabDocument::createView()
{
    if (!m_graphicsView) {
        m_graphicsView = new aadlinterface::GraphicsView;
        connect(m_graphicsView, &aadlinterface::GraphicsView::zoomChanged, this, [this](qreal percent) {
            for (auto item : m_graphicsScene->selectedItems()) {
                if (auto iObj = qobject_cast<aadlinterface::InteractiveObject *>(item->toGraphicsObject())) {
                    iObj->updateGripPoints();
                }
            }
            m_actZoomIn->setEnabled(!qFuzzyCompare(percent, m_graphicsView->maxZoomPercent()));
            m_actZoomOut->setEnabled(!qFuzzyCompare(percent, m_graphicsView->minZoomPercent()));
        });

        auto sc = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), qobject_cast<QWidget *>(m_graphicsView->window()));
        sc->setContext(Qt::ApplicationShortcut);
        connect(sc, &QShortcut::activated, this, [this]() {
            for (auto item : m_graphicsScene->items()) {
                dumpItem(item->toGraphicsObject(), true);
            }
        });
    }
    m_graphicsView->setScene(m_graphicsScene);
    updateSceneRect();
    return m_graphicsView;
}

QGraphicsScene *InterfaceTabDocument::createScene()
{
    if (!m_graphicsScene) {
        m_graphicsScene = new aadlinterface::InterfaceTabGraphicsScene(this);
        connect(m_graphicsScene, &QGraphicsScene::selectionChanged, [this]() {
            const QList<QGraphicsItem *> selectedItems = m_graphicsScene->selectedItems();
            auto it = std::find_if(selectedItems.cbegin(), selectedItems.cend(), [](QGraphicsItem *item) {
                return qobject_cast<aadlinterface::InteractiveObject *>(item->toGraphicsObject()) != nullptr;
            });
            m_actRemove->setEnabled(it != selectedItems.cend());
        });
    }
    return m_graphicsScene;
}

QMenu *InterfaceTabDocument::customMenu() const
{
    QMenu *root = new QMenu(title());
    QAction *actCommonProps = root->addAction(tr("Common Properties"));
    connect(actCommonProps, &QAction::triggered, this, &InterfaceTabDocument::onAttributesManagerRequested);
    QAction *actDataTypes = root->addAction(tr("Data types"));
    connect(actDataTypes, &QAction::triggered, this, &InterfaceTabDocument::onDataTypesMenuInvoked);
    QAction *actColorScheme = root->addAction(tr("Color Scheme"));
    connect(actColorScheme, &QAction::triggered, this, &InterfaceTabDocument::onColorSchemeMenuInvoked);
    QAction *actDynContext = root->addAction(tr("Context Actions"));
    connect(actDynContext, &QAction::triggered, this, &InterfaceTabDocument::onDynContextEditorMenuInvoked);

    ActionsManager::registerAction(
            Q_FUNC_INFO, actCommonProps, "Edit Properties", "Show editor for common Properties");
    ActionsManager::registerAction(
            Q_FUNC_INFO, actDataTypes, "Edit Datatypes", "Show editor for common Datatypes");
    ActionsManager::registerAction(
            Q_FUNC_INFO, actColorScheme, "Edit Color scheme", "Show editor for common Color schemes");
    ActionsManager::registerAction(
            Q_FUNC_INFO, actDynContext, "Context actions", "Show editor for common custom context menu actions");

    return root;
}

const QHash<shared::Id, aadl::AADLObject *> &InterfaceTabDocument::objects() const
{
    return m_model->objects();
}

/*!
   \brief InterfaceTabDocument::setObjects
 */
void InterfaceTabDocument::setObjects(const QVector<aadl::AADLObject *> &objects)
{
    if (m_model->initFromObjects(objects)) {
        m_model->setRootObject({});
    }
}

bool InterfaceTabDocument::createImpl(const QString &path)
{
    if (!path.isEmpty())
        return loadImpl(path);

    return true;
}

bool InterfaceTabDocument::loadImpl(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        qWarning() << Q_FUNC_INFO << "Invalid path";
        return false;
    }

    aadl::AADLXMLReader parser;
    connect(&parser, &aadl::AADLXMLReader::objectsParsed, this, &InterfaceTabDocument::setObjects);
    connect(&parser, &aadl::AADLXMLReader::error, [](const QString &msg) { qWarning() << msg; });

    return parser.readFile(path);
}

bool InterfaceTabDocument::saveImpl(const QString & /*path*/)
{
    return false;
}

void InterfaceTabDocument::closeImpl()
{
    m_model->clear();
    updateSceneRect();
}

QVector<QAction *> InterfaceTabDocument::initActions()
{
    if (!m_tool) {
        m_tool = new aadlinterface::CreatorTool(m_graphicsView, m_model, this);
        connect(m_tool, &aadlinterface::CreatorTool::created, this, [this]() {
            if (QAction *currentAction = m_actionGroup->checkedAction())
                currentAction->setChecked(false);
            m_tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::Pointer);
        });
        connect(m_tool, &aadlinterface::CreatorTool::propertyEditorRequest, this,
                &InterfaceTabDocument::showPropertyEditor);
        connect(m_tool, &aadlinterface::CreatorTool::informUser, this, &InterfaceTabDocument::showInfoMessage);
    }

    if (!m_actionGroup) {
        m_actionGroup = new QActionGroup(this);
        m_actionGroup->setExclusive(true);
    }

    if (!m_actCreateFunctionType) {
        m_actCreateFunctionType = new QAction(tr("Function Type"));
        ActionsManager::registerAction(
                Q_FUNC_INFO, m_actCreateFunctionType, "Function Type", "Create FunctionType object");

        m_actCreateFunctionType->setCheckable(true);
        m_actCreateFunctionType->setActionGroup(m_actionGroup);
        connect(m_actCreateFunctionType, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateFunctionType);
        m_actCreateFunctionType->setIcon(QIcon(":/tab_interface/toolbar/icns/function_type.svg"));
    }

    if (!m_actCreateFunction) {
        m_actCreateFunction = new QAction(tr("Function"));
        ActionsManager::registerAction(
                Q_FUNC_INFO, m_actCreateFunction, "Function", "Create Function object");

        m_actCreateFunction->setCheckable(true);
        m_actCreateFunction->setActionGroup(m_actionGroup);
        connect(m_actCreateFunction, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateFunction);

        m_actCreateFunction->setIcon(QIcon(":/tab_interface/toolbar/icns/function.svg"));
    }

    if (!m_actCreateProvidedInterface) {
        m_actCreateProvidedInterface = new QAction(tr("Provided Interface"));
        ActionsManager::registerAction(
                Q_FUNC_INFO, m_actCreateProvidedInterface, "Provided Interface", "Create Provided Interface object");

        m_actCreateProvidedInterface->setCheckable(true);
        m_actCreateProvidedInterface->setActionGroup(m_actionGroup);
        connect(m_actCreateProvidedInterface, &QAction::triggered, this,
                &InterfaceTabDocument::onActionCreateProvidedInterface);
        m_actCreateProvidedInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/pi.svg"));
    }

    if (!m_actCreateRequiredInterface) {
        m_actCreateRequiredInterface = new QAction(tr("Required Interface"));
        ActionsManager::registerAction(
                Q_FUNC_INFO, m_actCreateRequiredInterface, "Required Interface", "Create Required Interface object");

        m_actCreateRequiredInterface->setCheckable(true);
        m_actCreateRequiredInterface->setActionGroup(m_actionGroup);
        connect(m_actCreateRequiredInterface, &QAction::triggered, this,
                &InterfaceTabDocument::onActionCreateRequiredInterface);
        m_actCreateRequiredInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/ri.svg"));
    }

    if (!m_actCreateComment) {
        m_actCreateComment = new QAction(tr("Comment"));
        ActionsManager::registerAction(
                Q_FUNC_INFO, m_actCreateComment, "Comment", "Create Comment object");

        m_actCreateComment->setCheckable(true);
        m_actCreateComment->setActionGroup(m_actionGroup);
        connect(m_actCreateComment, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateComment);
        m_actCreateComment->setIcon(QIcon(":/tab_interface/toolbar/icns/comment.svg"));
    }

    //    if (!m_actGroupConnections) {
    //        m_actGroupConnections = new QAction(tr("Group"));
    //        m_actGroupConnections->setCheckable(true);
    //        m_actGroupConnections->setActionGroup(m_actionGroup);
    //        connect(m_actGroupConnections, &QAction::triggered, this,
    //        &InterfaceTabDocument::onActionGroupConnections);
    //        m_actGroupConnections->setIcon(QIcon(":/tab_interface/toolbar/icns/group.svg"));
    //    }

    if (!m_actCreateConnection) {
        m_actCreateConnection = new QAction(tr("Connection"));
        ActionsManager::registerAction(
                Q_FUNC_INFO, m_actCreateConnection, "Connection", "Create Connection object");

        m_actCreateConnection->setCheckable(true);
        m_actCreateConnection->setActionGroup(m_actionGroup);
        connect(m_actCreateConnection, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateConnection);
        m_actCreateConnection->setIcon(QIcon(":/tab_interface/toolbar/icns/connection.svg"));
    }

    if (!m_actRemove) {
        m_actRemove = new QAction(tr("Remove"));
        ActionsManager::registerAction(Q_FUNC_INFO, m_actRemove, "Remove", "Remove selected object");

        m_actRemove->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/remove.svg")));
        m_actRemove->setEnabled(false);
        m_actRemove->setShortcut(QKeySequence::Delete);
        connect(m_actRemove, &QAction::triggered, this, &InterfaceTabDocument::onActionRemoveItem);
    }

    if (!m_actZoomIn) {
        m_actZoomIn = new QAction(tr("Zoom In"));
        ActionsManager::registerAction(Q_FUNC_INFO, m_actZoomIn, "Zoom In", "Scale up the current scene");

        m_actZoomIn->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/zoom_in.svg")));
        m_actZoomIn->setShortcut(QKeySequence::ZoomIn);
        connect(m_actZoomIn, &QAction::triggered, this, &InterfaceTabDocument::onActionZoomIn);
    }

    if (!m_actZoomOut) {
        m_actZoomOut = new QAction(tr("Zoom Out"));
        ActionsManager::registerAction(
                Q_FUNC_INFO, m_actZoomOut, "Zoom Out", "Scale down the current scene");

        m_actZoomOut->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/zoom_out.svg")));
        m_actZoomOut->setShortcut(QKeySequence::ZoomOut);
        connect(m_actZoomOut, &QAction::triggered, this, &InterfaceTabDocument::onActionZoomOut);
    }

    if (!m_actExitToRoot) {
        m_actExitToRoot = new QAction(tr("Exit to root funtion"));
        m_actExitToRoot->setActionGroup(m_actionGroup);
        m_actExitToRoot->setEnabled(false);
        connect(m_actExitToRoot, &QAction::triggered, this, &InterfaceTabDocument::onActionExitToRootFunction);
        m_actExitToRoot->setIcon(QIcon(":/tab_interface/toolbar/icns/exit.svg"));
    }

    if (!m_actExitToParent) {
        m_actExitToParent = new QAction(tr("Exit to parent function"));
        m_actExitToParent->setActionGroup(m_actionGroup);
        m_actExitToParent->setEnabled(false);
        connect(m_actExitToParent, &QAction::triggered, this, &InterfaceTabDocument::onActionExitToParentFunction);
        m_actExitToParent->setIcon(QIcon(":/tab_interface/toolbar/icns/exit_parent.svg"));
    }

    return { m_actCreateFunctionType, m_actCreateFunction, m_actCreateProvidedInterface, m_actCreateRequiredInterface,
        m_actCreateComment, m_actCreateConnection, m_actRemove, m_actZoomIn, m_actZoomOut, m_actExitToRoot,
        m_actExitToParent };
}

void InterfaceTabDocument::onActionCreateFunctionType()
{
    m_tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::FunctionType);
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateFunction()
{
    m_tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::Function);
}

void InterfaceTabDocument::onActionCreateProvidedInterface()
{
    m_tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::ProvidedInterface);
}

void InterfaceTabDocument::onActionCreateRequiredInterface()
{
    m_tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::RequiredInterface);
}

void InterfaceTabDocument::onActionCreateComment()
{
    m_tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::Comment);
}

void InterfaceTabDocument::onActionGroupConnections()
{
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateConnection()
{
    m_tool->setCurrentToolType(aadlinterface::CreatorTool::ToolType::MultiPointConnection);
}

void InterfaceTabDocument::onActionRemoveItem()
{
    m_tool->removeSelectedItems();
}

void InterfaceTabDocument::onActionZoomIn()
{
    m_graphicsView->setZoom(m_graphicsView->zoom() + m_graphicsView->zoomStepPercent());
}

void InterfaceTabDocument::onActionZoomOut()
{
    m_graphicsView->setZoom(m_graphicsView->zoom() - m_graphicsView->zoomStepPercent());
}

void InterfaceTabDocument::onActionExitToRootFunction()
{
    changeRootItem({});
}

void InterfaceTabDocument::onActionExitToParentFunction()
{
    aadl::AADLObject *parentObject = m_model->rootObject() ? m_model->rootObject()->parentObject() : nullptr;
    changeRootItem(parentObject ? parentObject->id() : shared::InvalidId);
}

void InterfaceTabDocument::updateItem(QGraphicsItem *item)
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

    if (m_mutex->tryLock()) {
        updateSceneRect();
        m_mutex->unlock();
    }
}

void InterfaceTabDocument::updateComment(aadlinterface::AADLCommentGraphicsItem *comment)
{
    comment->updateFromEntity();
}

void InterfaceTabDocument::updateInterface(aadlinterface::AADLInterfaceGraphicsItem *iface)
{
    iface->updateFromEntity();
}

void InterfaceTabDocument::updateFunction(aadlinterface::AADLFunctionGraphicsItem *function)
{
    function->updateFromEntity();
}

void InterfaceTabDocument::updateFunctionType(aadlinterface::AADLFunctionTypeGraphicsItem *functionType)
{
    functionType->updateFromEntity();
}

void InterfaceTabDocument::updateConnection(aadlinterface::AADLConnectionGraphicsItem *connection)
{
    connection->updateFromEntity();
}

QGraphicsItem *InterfaceTabDocument::createItemForObject(aadl::AADLObject *obj)
{
    Q_ASSERT(obj);
    if (!obj)
        return nullptr;

    QGraphicsItem *parentItem = obj->parentObject() ? m_items.value(obj->parentObject()->id()) : nullptr;
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
                    ifaceStart ? m_items.value(ifaceStart->id()) : nullptr);

            aadl::AADLObjectIface *ifaceEnd = connection->targetInterface();
            auto endItem = qgraphicsitem_cast<aadlinterface::AADLInterfaceGraphicsItem *>(
                    ifaceEnd ? m_items.value(ifaceEnd->id()) : nullptr);

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

aadlinterface::AADLFunctionGraphicsItem *InterfaceTabDocument::rootItem() const
{
    if (auto rootEntity = m_model->rootObject())
        return qgraphicsitem_cast<aadlinterface::AADLFunctionGraphicsItem *>(m_items.value(rootEntity->id()));
    return nullptr;
}

void InterfaceTabDocument::onAADLObjectAdded(aadl::AADLObject *object)
{
    auto propertyChanged = [this]() {
        if (auto object = qobject_cast<aadl::AADLObject *>(sender())) {
            if (auto item = m_items.value(object->id()))
                updateItem(item);
        }
    };

    auto item = m_items.value(object->id());
    if (!item) {
        item = createItemForObject(object);
        connect(object, &aadl::AADLObject::coordinatesChanged, this, propertyChanged, Qt::QueuedConnection);
        if (auto clickable = qobject_cast<aadlinterface::InteractiveObject *>(item->toGraphicsObject())) {
            connect(clickable, &aadlinterface::InteractiveObject::clicked, this, &InterfaceTabDocument::onItemClicked);
            connect(clickable, &aadlinterface::InteractiveObject::doubleClicked, this,
                    &InterfaceTabDocument::onItemDoubleClicked, Qt::QueuedConnection);
        }
        m_items.insert(object->id(), item);
        if (m_graphicsScene != item->scene())
            m_graphicsScene->addItem(item);
    }
    updateItem(item);
}

void InterfaceTabDocument::onAADLObjectRemoved(aadl::AADLObject *object)
{
    m_rmQueu.enqueue(object);

    while (m_rmQueu.size()) {
        if (m_mutex->tryLock()) {
            aadl::AADLObject *obj = m_rmQueu.dequeue();
            if (auto item = m_items.take(obj->id())) {
                m_graphicsScene->removeItem(item);
                delete item;
                updateSceneRect();
            }
            m_mutex->unlock();
        }
    }
}

void InterfaceTabDocument::onItemClicked()
{
    dumpItem(sender());
}

void InterfaceTabDocument::onItemDoubleClicked()
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

void InterfaceTabDocument::onRootObjectChanged(shared::Id rootId)
{
    Q_UNUSED(rootId)

    QMutexLocker lockme(m_mutex);

    m_actExitToRoot->setEnabled(nullptr != m_model->rootObject());
    m_actExitToParent->setEnabled(nullptr != m_model->rootObject());

    QList<aadl::AADLObject *> objects = m_model->visibleObjects();
    std::stable_sort(objects.begin(), objects.end(),
            [](aadl::AADLObject *obj1, aadl::AADLObject *obj2) { return obj1->aadlType() < obj2->aadlType(); });

    for (auto it = objects.cbegin(); it != objects.cend(); ++it)
        onAADLObjectAdded(*it);

    updateSceneRect();
}

void InterfaceTabDocument::showPropertyEditor(aadl::AADLObject *obj)
{
    aadlinterface::PropertiesDialog *dialog =
            new aadlinterface::PropertiesDialog(obj, qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceTabDocument::showInfoMessage(const QString &title, const QString &message)
{
    // TODO: extend the AbstractTabDocument API for using QMessageBox in mainwindow, not here.
    // Something like:
    //
    // MessageBoxHandler mb(title, message);
    // mb.buttons = QMessageBox::Ok;
    // mb.callback = &someCallbackToHandle(StandardButton);
    // Q_EMIT requestMessageBox(mb)

    QMessageBox::information(qobject_cast<QWidget *>(parent()), title, message);
}

void InterfaceTabDocument::clearScene()
{
    if (m_graphicsScene) {
        m_graphicsScene->clear();
    }
    m_items.clear();
}

void InterfaceTabDocument::changeRootItem(shared::Id id)
{
    if (m_model->rootObjectId() == id)
        return;

    const QVariantList rootEntityParams { QVariant::fromValue(m_model), QVariant::fromValue(id) };
    const auto geometryCmd =
            aadlinterface::cmd::CommandsFactory::create(aadlinterface::cmd::ChangeRootEntity, rootEntityParams);
    aadlinterface::cmd::CommandsStack::current()->push(geometryCmd);
}

void InterfaceTabDocument::onAttributesManagerRequested()
{
    auto dialog = new aadlinterface::DynamicPropertyManager(qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceTabDocument::onDataTypesMenuInvoked()
{
    if (auto act = qobject_cast<QAction *>(sender()))
        showNIYGUI(act->text());
}

void InterfaceTabDocument::onColorSchemeMenuInvoked()
{
    aadlinterface::ColorManagerDialog *dialog =
            new aadlinterface::ColorManagerDialog(qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceTabDocument::onDynContextEditorMenuInvoked()
{
    auto dialog = new DynActionEditor(qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    dialog->open();
    if (!dialog->init())
        dialog->close();
}

void InterfaceTabDocument::showNIYGUI(const QString &title)
{
    QString header = title.isEmpty() ? "NIY" : title;
    QWidget *mainWindow = qobject_cast<QWidget *>(parent());
    QMessageBox::information(mainWindow, header, "Not implemented yet!");
}

QString InterfaceTabDocument::supportedFileExtensions() const
{
    return QStringLiteral("*.xml");
}

void InterfaceTabDocument::updateSceneRect()
{
    const QRectF itemsRect = m_graphicsScene->itemsBoundingRect();
    if (itemsRect.isEmpty()) {
        m_graphicsScene->setSceneRect(m_desktopGeometry);
        return;
    }

    if (itemsRect != m_prevItemsRect) {
        const QRectF sceneRect = m_graphicsScene->sceneRect();
        const QRectF updated = sceneRect.united(itemsRect);

        if (sceneRect != updated) {
            m_graphicsView->setUpdatesEnabled(false);
            m_graphicsScene->setSceneRect(updated);
            m_graphicsView->setUpdatesEnabled(true);

            m_prevItemsRect = itemsRect;
        }
    }
}

}
