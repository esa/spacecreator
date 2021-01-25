/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlitemmodel.h"

#include "aadlcomment.h"
#include "aadlconnection.h"
#include "aadlconnectiongroup.h"
#include "aadlfunction.h"
#include "aadliface.h"
#include "aadlifacegroup.h"
#include "baseitems/common/aadlutils.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "commandsstack.h"
#include "delayedsignal.h"
#include "interface/aadlcommentgraphicsitem.h"
#include "interface/aadlconnectiongraphicsitem.h"
#include "interface/aadlconnectiongroupgraphicsitem.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlfunctiontypegraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"
#include "interface/aadlinterfacegroupgraphicsitem.h"
#include "interfacetabgraphicsscene.h"

#include <QGraphicsView>
#include <QGuiApplication>
#include <QMutex>
#include <QScreen>
#include <QUndoStack>
#include <QtDebug>

#ifdef AADL_ITEM_DUMP
static inline void dumpItem(QObject *obj, bool strict = false)
{
    auto item = qobject_cast<ive::InteractiveObject *>(obj);
    if (!item)
        return;

    qDebug() << item->metaObject()->className() << "\n"
             << item->aadlObject()->props() << "\n"
             << item->aadlObject()->attrs();

    if (auto iface = qobject_cast<ive::AADLInterfaceGraphicsItem *>(item)) {
        qDebug() << "\nGraphics Iface geometry:\n" << iface->scenePos() << "\n" << iface->sceneBoundingRect() << "\n";
        qDebug() << "\nInternal Iface data:\n"
                 << iface->entity()->title() << "\n"
                 << ive::pos(iface->entity()->coordinates()) << "\n";
        qDebug() << "\n####:\n" << iface->connectionItems();
        Q_ASSERT(!strict || iface->scenePos().toPoint() == ive::pos(iface->entity()->coordinates()));
    } else if (auto connection = qobject_cast<ive::AADLConnectionGraphicsItem *>(item)) {
        qDebug() << "\nGraphics Connection geometry:\n"
                 << connection->points() << "\n"
                 << connection->graphicsPoints() << "\n";
        qDebug() << "\nInternal Connection data:\n"
                 << (connection->entity()->title().isEmpty() ? QStringLiteral("Connection: %1 <=> %2")
                                                                       .arg(connection->startItem()->entity()->title(),
                                                                               connection->endItem()->entity()->title())
                                                             : connection->entity()->title())
                 << "\n"
                 << ive::polygon(connection->entity()->coordinates()) << "\n";
        Q_ASSERT(!strict
                || ive::comparePolygones(
                        connection->graphicsPoints(), ive::polygon(connection->entity()->coordinates())));
        Q_ASSERT(!strict
                || ive::comparePolygones(
                        connection->points(), ive::polygon(connection->entity()->coordinates())));
    } else if (auto rectItem = qobject_cast<ive::AADLRectGraphicsItem *>(item)) {
        qDebug() << "\nGraphics" << rectItem->metaObject()->className() << "geometry:\n"
                 << rectItem->sceneBoundingRect() << "\n";
        qDebug() << "\nInternal Function data:\n"
                 << rectItem->aadlObject()->title() << "\n"
                 << ive::rect(rectItem->aadlObject()->coordinates()) << "\n";
        Q_ASSERT(!strict
                || rectItem->sceneBoundingRect().toRect()
                        == ive::rect(rectItem->aadlObject()->coordinates()).toRect());
    } else {
        qFatal("Not implemented trace");
    }
}
#endif

namespace ive {

AADLItemModel::AADLItemModel(ivm::AADLModel *model, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_graphicsScene(new InterfaceTabGraphicsScene(this))
    , m_mutex(new QMutex(QMutex::NonRecursive))
    , m_textUpdate(new shared::DelayedSignal(this))
{
    if (QGuiApplication::primaryScreen()) {
        m_desktopGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    }

    connect(m_model, &ivm::AADLModel::modelReset, this, &AADLItemModel::clearScene);
    connect(m_model, &ivm::AADLModel::rootObjectChanged, this, &AADLItemModel::onRootObjectChanged);
    connect(m_model, &ivm::AADLModel::aadlObjectsAdded, this, &AADLItemModel::onAADLObjectsAdded);
    connect(m_model, &ivm::AADLModel::aadlObjectRemoved, this, &AADLItemModel::onAADLObjectRemoved);

    connect(m_graphicsScene, &QGraphicsScene::selectionChanged, this, &AADLItemModel::onSceneSelectionChanged);

    m_textUpdate->setInterval(10);
    connect(m_textUpdate, &shared::DelayedSignal::triggered, this, &AADLItemModel::updateInterfaceTexts);
}

AADLItemModel::~AADLItemModel()
{
    delete m_mutex;
}

QGraphicsScene *AADLItemModel::scene() const
{
    return m_graphicsScene;
}

void AADLItemModel::onAADLObjectAdded(ivm::AADLObject *object)
{
    if (!m_graphicsScene || !object) {
        return;
    }

    setupInnerGeometry(object);

    auto propertyChanged = [this]() {
        if (auto senderObject = qobject_cast<ivm::AADLObject *>(sender())) {
            if (auto item = m_items.value(senderObject->id())) {
                updateItem(item);
            }
            scheduleInterfaceTextUpdate();
        }
    };

    auto item = m_items.value(object->id());
    if (!item) {
        item = createItemForObject(object);
        if (const auto connectionGroupObject = qobject_cast<ivm::AADLConnectionGroup *>(object)) {
            connect(connectionGroupObject, &ivm::AADLConnectionGroup::connectionAdded, this,
                    &AADLItemModel::onConnectionAddedToGroup, Qt::UniqueConnection);
            connect(connectionGroupObject, &ivm::AADLConnectionGroup::connectionRemoved, this,
                    &AADLItemModel::onConnectionRemovedFromGroup, Qt::UniqueConnection);

            for (auto groupedConnectionObject : connectionGroupObject->groupedConnections()) {
                onConnectionAddedToGroup(groupedConnectionObject);
            }
        }
        connect(object, &ivm::AADLObject::visibilityChanged, this, [this, id = object->id()](bool isVisible) {
            if (auto item = dynamic_cast<InteractiveObject *>(m_items.value(id))) {
                item->scheduleLayoutUpdate();
                scheduleInterfaceTextUpdate();
            }
        });
        connect(object, &ivm::AADLObject::coordinatesChanged, this, propertyChanged);
        if (auto clickable = qobject_cast<InteractiveObject *>(item->toGraphicsObject())) {
            connect(
                    clickable, &InteractiveObject::clicked, this,
                    [this, clickable]() {
#ifdef AADL_ITEM_DUMP
                        dumpItem(sender());
#endif
                        if (auto entity = clickable->aadlObject()) {
                            Q_EMIT itemClicked(entity->id());
                        }
                    },
                    Qt::QueuedConnection);
            connect(
                    clickable, &InteractiveObject::doubleClicked, this,
                    [this, clickable]() {
                        if (auto entity = clickable->aadlObject()) {
                            if (auto function = qobject_cast<ivm::AADLFunction *>(entity)) {
                                if (function->hasNestedChildren() && !function->isRootObject()) {
                                    changeRootItem(function->id());
                                    return;
                                }
                            }

                            Q_EMIT itemDoubleClicked(entity->id());
                        }
                    },
                    Qt::QueuedConnection);
        }

        m_items.insert(object->id(), item);
        if (m_graphicsScene != item->scene()) {
            m_graphicsScene->addItem(item);
        }
    }
    updateItem(item);
}

void AADLItemModel::onAADLObjectsAdded(const QVector<ivm::AADLObject *> &objects)
{
    QList<ivm::AADLObject *> objectsToAdd(objects.toList());
    ivm::AADLObject::sortObjectList(objectsToAdd);
    for (auto object : objectsToAdd) {
        onAADLObjectAdded(object);
    }

    updateSceneRect();
}

void AADLItemModel::onRootObjectChanged(shared::Id rootId)
{
    Q_UNUSED(rootId)
    clearScene();
    onAADLObjectsAdded(m_model->visibleObjects().toVector());
}

void AADLItemModel::onAADLObjectRemoved(ivm::AADLObject *object)
{
    if (!m_graphicsScene) {
        return;
    }

    m_rmQueu.enqueue(object);

    while (!m_rmQueu.isEmpty()) {
        if (m_mutex->tryLock()) {
            removeItemForObject(m_rmQueu.dequeue());
            m_mutex->unlock();
        }
    }

    scheduleInterfaceTextUpdate();
}

void AADLItemModel::onConnectionAddedToGroup(ivm::AADLConnection *connection)
{
    auto connectionGroupObject = qobject_cast<ivm::AADLConnectionGroup *>(sender());
    if (!connectionGroupObject) {
        connectionGroupObject =
                qobject_cast<ivm::AADLConnectionGroup *>(m_model->getObjectByName(connection->groupName()));
        if (!connectionGroupObject) {
            return;
        }
    }

    auto handleIface = [this](ivm::AADLConnection *connection,
                               ivm::AADLIfaceGroup *connectionGroupEndPoint) {
        auto ifaceObject = connectionGroupEndPoint->function()->id() == connection->source()->id()
                ? connection->sourceInterface()
                : connectionGroupEndPoint->function()->id() == connection->target()->id()
                        ? connection->targetInterface()
                        : nullptr;
        if (auto ifaceItem = getItem<AADLInterfaceGraphicsItem *>(ifaceObject->id())) {
            for (auto ifaceConnection : ifaceItem->connectionItems()) {
                if (ifaceConnection->entity()->id() == connection->id()) {
                    continue;
                }
                ifaceConnection->replaceInterface(
                        ifaceItem, getItem<AADLInterfaceGroupGraphicsItem *>(connectionGroupEndPoint->id()));
            }
        }
    };

    handleIface(connection, connectionGroupObject->sourceInterfaceGroup());
    handleIface(connection, connectionGroupObject->targetInterfaceGroup());

    onAADLObjectRemoved(connection);
    onAADLObjectRemoved(connection->sourceInterface());
    onAADLObjectRemoved(connection->targetInterface());
}

void AADLItemModel::onConnectionRemovedFromGroup(ivm::AADLConnection *connection)
{
    auto connectionGroupObject = qobject_cast<ivm::AADLConnectionGroup *>(sender());
    if (!connectionGroupObject) {
        connectionGroupObject =
                qobject_cast<ivm::AADLConnectionGroup *>(m_model->getObjectByName(connection->groupName()));
        if (!connectionGroupObject) {
            return;
        }
    }

    onAADLObjectAdded(connection->targetInterface());
    onAADLObjectAdded(connection->sourceInterface());
    onAADLObjectAdded(connection);

    auto handleIface = [this](ivm::AADLConnection *connection,
                               ivm::AADLIfaceGroup *connectionGroupEndPoint) {
        auto ifaceObject = connectionGroupEndPoint->function()->id() == connection->source()->id()
                ? connection->sourceInterface()
                : connectionGroupEndPoint->function()->id() == connection->target()->id()
                        ? connection->targetInterface()
                        : nullptr;
        if (auto ifaceItem = getItem<AADLInterfaceGraphicsItem *>(connectionGroupEndPoint->id())) {
            for (auto ifaceConnection : ifaceItem->connectionItems()) {
                const bool currentHandledConnection = ifaceConnection->entity()->id() == connection->id();
                const bool isLinkedIface =
                        ifaceConnection->entity()->sourceInterface()->id() == connection->sourceInterface()->id()
                        || ifaceConnection->entity()->sourceInterface()->id() == connection->targetInterface()->id()
                        || ifaceConnection->entity()->targetInterface()->id() == connection->sourceInterface()->id()
                        || ifaceConnection->entity()->targetInterface()->id() == connection->targetInterface()->id();
                if (currentHandledConnection || !isLinkedIface) {
                    continue;
                }
                ifaceConnection->replaceInterface(
                        ifaceItem, getItem<AADLInterfaceGroupGraphicsItem *>(ifaceObject->id()));
            }
        }
    };

    handleIface(connection, connectionGroupObject->sourceInterfaceGroup());
    handleIface(connection, connectionGroupObject->targetInterfaceGroup());
}

void AADLItemModel::onSceneSelectionChanged()
{
    QList<shared::Id> ids;
    for (auto item : m_graphicsScene->selectedItems()) {
        if (auto iObj = qobject_cast<InteractiveObject *>(item->toGraphicsObject())) {
            ids.append(iObj->aadlObject()->id());
        }
    }
    Q_EMIT itemsSelected(ids);
}

/*!
   Schedules an update for all interface texts (geometry constraints)
 */
void AADLItemModel::scheduleInterfaceTextUpdate()
{
    m_textUpdate->triggerSignal();
}

/*!
   Checks the space for the interfaces to set it to e proper length
 */
void AADLItemModel::updateInterfaceTexts()
{
    for (QGraphicsItem *item : m_items) {
        if (auto interfaceItem = dynamic_cast<ive::AADLInterfaceGraphicsItem *>(item)) {
            interfaceItem->updateLabel();
        }
    }
}

AADLFunctionGraphicsItem *AADLItemModel::rootItem() const
{
    return qgraphicsitem_cast<AADLFunctionGraphicsItem *>(m_items.value(m_model->rootObjectId()));
}

void AADLItemModel::updateItem(QGraphicsItem *item)
{
    Q_ASSERT(item);
    if (!item)
        return;

    if (auto iObj = qobject_cast<InteractiveObject *>(item->toGraphicsObject())) {
        iObj->updateFromEntity();
    }
    if (m_mutex->tryLock()) {
        updateSceneRect();
        m_mutex->unlock();
    }
}

void AADLItemModel::removeItemForObject(ivm::AADLObject *object)
{
    if (auto item = m_items.take(object->id())) {
        m_graphicsScene->removeItem(item);
        delete item;
        updateSceneRect();
    }
}

void AADLItemModel::setupInnerGeometry(ivm::AADLObject *obj) const
{
    if (!obj
            || !(obj->aadlType() == ivm::AADLObject::Type::Comment
                    || obj->aadlType() == ivm::AADLObject::Type::Function
                    || obj->aadlType() == ivm::AADLObject::Type::FunctionType)) {
        return;
    }
    QVariant innerCoord = obj->prop(ivm::meta::Props::token(ivm::meta::Props::Token::InnerCoordinates));
    if (innerCoord.isValid()) {
        return;
    }
    ivm::AADLFunctionType *parentObj =
            obj->parentObject() ? obj->parentObject()->as<ivm::AADLFunctionType *>() : nullptr;
    if (!parentObj) {
        return;
    }
    QRectF rootGeometry;
    const QVariant rootCoord = parentObj->prop(ivm::meta::Props::token(ivm::meta::Props::Token::RootCoordinates));
    if (rootCoord.isValid()) {
        rootGeometry = ive::rect(ivm::AADLObject::coordinatesFromString(rootCoord.toString()));
    } else if (const QGraphicsView *view = m_graphicsScene->views().value(0)) {
        const QRect viewportGeometry = view->viewport()->geometry().marginsRemoved(kContentMargins.toMargins());
        rootGeometry = QRectF(view->mapToScene(QPoint(0, 0)),
                view->mapToScene(QPoint(viewportGeometry.width(), viewportGeometry.height())));
    }
    QList<QRectF> existingRects;
    for (const auto child : parentObj->children()) {
        if (child->aadlType() == ivm::AADLObject::Type::Comment
                || child->aadlType() == ivm::AADLObject::Type::Function
                || child->aadlType() == ivm::AADLObject::Type::FunctionType) {
            innerCoord = child->prop(ivm::meta::Props::token(ivm::meta::Props::Token::InnerCoordinates));
            if (!innerCoord.isValid()) {
                continue;
            }
            const QRectF innerRect =
                    ive::rect(ivm::AADLObject::coordinatesFromString(innerCoord.toString()));
            rootGeometry |= innerRect;
            existingRects.append(innerRect);
        }
    }
    QRectF innerGeometry;
    findGeometryForRect(innerGeometry, rootGeometry, existingRects);

    const QString strRootCoord = ivm::AADLObject::coordinatesToString(ive::coordinates(rootGeometry));
    parentObj->setProp(ivm::meta::Props::token(ivm::meta::Props::Token::RootCoordinates), strRootCoord);

    const QString strCoord = ivm::AADLObject::coordinatesToString(ive::coordinates(innerGeometry));
    obj->setProp(ivm::meta::Props::token(ivm::meta::Props::Token::InnerCoordinates), strCoord);
}

void AADLItemModel::clearScene()
{
    if (m_graphicsScene) {
        m_graphicsScene->clear();
    }
    m_items.clear();
    updateSceneRect();
}

void AADLItemModel::changeRootItem(shared::Id id)
{
    if (m_model->rootObjectId() == id)
        return;

    const QVariantList rootEntityParams { QVariant::fromValue(m_model), QVariant::fromValue(id) };
    const auto geometryCmd = cmd::CommandsFactory::create(cmd::ChangeRootEntity, rootEntityParams);
    cmd::CommandsStack::push(geometryCmd);
}

void AADLItemModel::zoomChanged()
{
    for (auto item : m_graphicsScene->selectedItems()) {
        if (auto iObj = qobject_cast<ive::InteractiveObject *>(item->toGraphicsObject())) {
            iObj->updateGripPoints();
        }
    }
}

QGraphicsItem *AADLItemModel::getItem(const shared::Id id) const
{
    if (id.isNull()) {
        return nullptr;
    }

    return m_items.value(id);
}

template<typename T>
T AADLItemModel::getItem(const shared::Id id) const
{
    if (id.isNull()) {
        return nullptr;
    }

    return qgraphicsitem_cast<T>(m_items.value(id));
}

ivm::AADLModel *AADLItemModel::objectsModel() const
{
    return m_model;
}

void AADLItemModel::updateSceneRect()
{
    if (!m_graphicsScene) {
        return;
    }

    const QRectF itemsRect = m_graphicsScene->itemsBoundingRect();
    if (itemsRect.isEmpty()) {
        m_prevItemsRect = {};
        m_graphicsScene->setSceneRect(m_desktopGeometry);
        return;
    }

    if (itemsRect != m_prevItemsRect) {
        const QRectF sceneRect = m_graphicsScene->sceneRect();
        const QRectF updated = sceneRect.united(itemsRect);

        if (sceneRect != updated) {
            m_graphicsScene->setSceneRect(updated);
            m_prevItemsRect = itemsRect;
        }
    }
}

QGraphicsItem *AADLItemModel::createItemForObject(ivm::AADLObject *obj)
{
    Q_ASSERT(obj);
    if (!obj)
        return nullptr;

    QGraphicsItem *parentItem = obj->parentObject() ? m_items.value(obj->parentObject()->id()) : nullptr;
    auto nestedGeomtryConnect = [this](QGraphicsItem *parentItem, InteractiveObject *child) {
        if (parentItem) {
            if (auto iObjParent = qobject_cast<InteractiveObject *>(parentItem->toGraphicsObject()))
                this->connect(child, &InteractiveObject::boundingBoxChanged, iObjParent,
                        &InteractiveObject::scheduleLayoutUpdate, Qt::QueuedConnection);
        }
    };

    InteractiveObject *iObj = nullptr;
    switch (obj->aadlType()) {
    case ivm::AADLObject::Type::Comment: {
        auto comment = new AADLCommentGraphicsItem(qobject_cast<ivm::AADLComment *>(obj), parentItem);
        connect(comment, &shared::ui::InteractiveObjectBase::boundingBoxChanged, this,
                &AADLItemModel::scheduleInterfaceTextUpdate);
        nestedGeomtryConnect(parentItem, comment);
        iObj = comment;
    } break;
    case ivm::AADLObject::Type::InterfaceGroup:
        iObj = new AADLInterfaceGroupGraphicsItem(qobject_cast<ivm::AADLIfaceGroup *>(obj), parentItem);
        break;
    case ivm::AADLObject::Type::RequiredInterface:
    case ivm::AADLObject::Type::ProvidedInterface: {
        auto ifItem = new AADLInterfaceGraphicsItem(qobject_cast<ivm::AADLIface *>(obj), parentItem);
        connect(ifItem, &shared::ui::InteractiveObjectBase::boundingBoxChanged, this,
                &AADLItemModel::scheduleInterfaceTextUpdate);
        iObj = ifItem;
        break;
    }
    case ivm::AADLObject::Type::ConnectionGroup:
        if (auto connection = qobject_cast<ivm::AADLConnectionGroup *>(obj)) {
            ivm::AADLIface *ifaceStart = connection->sourceInterface();
            auto startItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                    ifaceStart ? m_items.value(ifaceStart->id()) : nullptr);

            ivm::AADLIface *ifaceEnd = connection->targetInterface();
            auto endItem =
                    qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(ifaceEnd ? m_items.value(ifaceEnd->id()) : nullptr);

            iObj = new AADLConnectionGroupGraphicsItem(connection,
                    qobject_cast<AADLInterfaceGroupGraphicsItem *>(startItem),
                    qobject_cast<AADLInterfaceGroupGraphicsItem *>(endItem), parentItem);
        }
        break;
    case ivm::AADLObject::Type::Connection:
        if (auto connection = qobject_cast<ivm::AADLConnection *>(obj)) {
            ivm::AADLIface *ifaceStart = connection->sourceInterface();
            auto startItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                    ifaceStart ? m_items.value(ifaceStart->id()) : nullptr);

            ivm::AADLIface *ifaceEnd = connection->targetInterface();
            auto endItem =
                    qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(ifaceEnd ? m_items.value(ifaceEnd->id()) : nullptr);

            iObj = new AADLConnectionGraphicsItem(connection, startItem, endItem, parentItem);
        }
        break;
    case ivm::AADLObject::Type::Function: {
        auto function = new AADLFunctionGraphicsItem(qobject_cast<ivm::AADLFunction *>(obj), parentItem);
        connect(function, &shared::ui::InteractiveObjectBase::boundingBoxChanged, this,
                &AADLItemModel::scheduleInterfaceTextUpdate);
        nestedGeomtryConnect(parentItem, function);
        iObj = function;
    } break;
    case ivm::AADLObject::Type::FunctionType: {
        auto functionType =
                new AADLFunctionTypeGraphicsItem(qobject_cast<ivm::AADLFunctionType *>(obj), parentItem);
        connect(functionType, &shared::ui::InteractiveObjectBase::boundingBoxChanged, this,
                &AADLItemModel::scheduleInterfaceTextUpdate);
        nestedGeomtryConnect(parentItem, functionType);
        iObj = functionType;
    } break;
    default: {
        qCritical() << "Unknown object type:" << obj->aadlType();
        break;
    }
    }

    if (iObj) {
        iObj->init();
    }
    return iObj;
}

} // namespace ive
