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

#include "aadlobjectcomment.h"
#include "aadlobjectconnection.h"
#include "aadlobjectconnectiongroup.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"
#include "aadlobjectifacegroup.h"
#include "baseitems/common/aadlutils.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "commandsstack.h"
#include "interface/aadlcommentgraphicsitem.h"
#include "interface/aadlconnectiongraphicsitem.h"
#include "interface/aadlconnectiongroupgraphicsitem.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlfunctiontypegraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"
#include "interface/aadlinterfacegroupgraphicsitem.h"
#include "interfacetabgraphicsscene.h"

#include <QGuiApplication>
#include <QItemSelectionModel>
#include <QMutex>
#include <QScreen>
#include <QUndoStack>
#include <QtDebug>

#ifdef AADL_ITEM_DUMP
static inline void dumpItem(QObject *obj, bool strict = false)
{
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
#endif

namespace aadlinterface {

AADLItemModel::AADLItemModel(QObject *parent)
    : aadl::AADLObjectsModel(parent)
    , m_itemSelectionModel(new QItemSelectionModel(this))
    , m_graphicsScene(new InterfaceTabGraphicsScene(this))
    , m_mutex(new QMutex(QMutex::NonRecursive))
{
    if (QGuiApplication::primaryScreen()) {
        m_desktopGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    }

    connect(this, &aadl::AADLObjectsModel::modelReset, this, &AADLItemModel::clearScene);
    connect(this, &aadl::AADLObjectsModel::rootObjectChanged, this, &AADLItemModel::onRootObjectChanged,
            Qt::QueuedConnection);
    connect(this, &aadl::AADLObjectsModel::aadlObjectsAdded, this, &AADLItemModel::onAADLObjectsAdded,
            Qt::QueuedConnection);
    connect(this, &aadl::AADLObjectsModel::aadlObjectRemoved, this, &AADLItemModel::onAADLObjectRemoved);

    connect(m_graphicsScene, &QGraphicsScene::selectionChanged, this, &AADLItemModel::onSceneSelectionChanged);
    connect(m_itemSelectionModel, &QItemSelectionModel::selectionChanged, this, &AADLItemModel::onViewSelectionChanged);
}

AADLItemModel::~AADLItemModel()
{
    delete m_mutex;
}

QGraphicsScene *AADLItemModel::scene() const
{
    return m_graphicsScene;
}

QItemSelectionModel *AADLItemModel::selectionModel() const
{
    return m_itemSelectionModel;
}

void AADLItemModel::onAADLObjectAdded(aadl::AADLObject *object)
{
    if (!m_graphicsScene) {
        return;
    }

    auto propertyChanged = [this]() {
        if (auto senderObject = qobject_cast<aadl::AADLObject *>(sender())) {
            if (auto item = m_items.value(senderObject->id()))
                updateItem(item);
        }
    };

    auto item = m_items.value(object->id());
    if (!item) {
        item = createItemForObject(object);
        if (const auto connectionGroup = qobject_cast<aadl::AADLObjectConnectionGroup *>(object)) {
            connect(connectionGroup, &aadl::AADLObjectConnectionGroup::connectionAdded, this,
                    &AADLItemModel::onConnectionAddedToGroup);
            connect(connectionGroup, &aadl::AADLObjectConnectionGroup::connectionRemoved, this,
                    &AADLItemModel::onConnectionRemovedFromGroup);
            for (auto connection : connectionGroup->groupedConnections()) {
                onConnectionAddedToGroup(connection);
            }
            updateSceneRect();
        }
        connect(object, &aadl::AADLObject::coordinatesChanged, this, propertyChanged);
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
                            if (auto function = qobject_cast<aadl::AADLObjectFunction *>(entity)) {
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

void AADLItemModel::onAADLObjectsAdded(const QVector<aadl::AADLObject *> &objects)
{
    QList<aadl::AADLObject *> objectsToAdd(objects.toList());
    aadl::AADLObject::sortObjectList(objectsToAdd);
    for (auto object : objectsToAdd) {
        onAADLObjectAdded(object);
    }

    updateSceneRect();
}

void AADLItemModel::onRootObjectChanged(shared::Id rootId)
{
    Q_UNUSED(rootId)
    clearScene();
    onAADLObjectsAdded(visibleObjects().toVector());
}

void AADLItemModel::onAADLObjectRemoved(aadl::AADLObject *object)
{
    if (!m_graphicsScene) {
        return;
    }

    m_rmQueu.enqueue(object);

    while (!m_rmQueu.isEmpty()) {
        if (m_mutex->tryLock()) {
            aadl::AADLObject *obj = m_rmQueu.dequeue();
            if (const auto connectionGroup = qobject_cast<aadl::AADLObjectConnectionGroup *>(obj)) {
                for (const auto connection : connectionGroup->groupedConnections()) {
                    onConnectionRemovedFromGroup(connection);
                }
            }

            if (auto item = m_items.take(obj->id())) {
                m_graphicsScene->removeItem(item);
                delete item;
                updateSceneRect();
            }
            m_mutex->unlock();
        }
    }
}

void AADLItemModel::onConnectionAddedToGroup(aadl::AADLObjectConnection *connection)
{
    onAADLObjectRemoved(connection);
    onAADLObjectRemoved(connection->sourceInterface());
    onAADLObjectRemoved(connection->targetInterface());
}

void AADLItemModel::onConnectionRemovedFromGroup(aadl::AADLObjectConnection *connection)
{
    onAADLObjectAdded(connection->targetInterface());
    onAADLObjectAdded(connection->sourceInterface());
    onAADLObjectAdded(connection);
}

void AADLItemModel::onSceneSelectionChanged()
{
    const QList<QGraphicsItem *> selectedItems = m_graphicsScene->selectedItems();
    QItemSelection itemSelection;
    for (auto item : selectedItems) {
        if (auto iObj = qobject_cast<aadlinterface::InteractiveObject *>(item->toGraphicsObject())) {
            const QModelIndex idx = indexFromObject(iObj->aadlObject());
            if (itemSelection.isEmpty()) {
                itemSelection.select(idx, idx);
            } else {
                itemSelection.merge(QItemSelection(idx, idx), QItemSelectionModel::SelectCurrent);
            }
        }
    }
    m_itemSelectionModel->select(itemSelection,
            QItemSelectionModel::Rows | QItemSelectionModel::Current | QItemSelectionModel::ClearAndSelect);
}

void AADLItemModel::onViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    auto updateSelection = [this](const QItemSelection &selection, bool value) {
        for (const QModelIndex &idx : selection.indexes()) {
            if (auto const obj = objectFromIndex(idx)) {
                if (auto graphicsItem = m_items.value(obj->id())) {
                    graphicsItem->setSelected(value);
                }
            }
        }
    };

    updateSelection(deselected, false);
    updateSelection(selected, true);
}

AADLFunctionGraphicsItem *AADLItemModel::rootItem() const
{
    return qgraphicsitem_cast<AADLFunctionGraphicsItem *>(m_items.value(rootObjectId()));
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

void AADLItemModel::removeItemForObject(aadl::AADLObject *object)
{
    if (auto connectionItem = m_items.take(object->id())) {
        m_graphicsScene->removeItem(connectionItem);
        delete connectionItem;
    }
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
    if (rootObjectId() == id)
        return;

    const QVariantList rootEntityParams { QVariant::fromValue(this), QVariant::fromValue(id) };
    const auto geometryCmd = cmd::CommandsFactory::create(cmd::ChangeRootEntity, rootEntityParams);
    cmd::CommandsStack::push(geometryCmd);
}

void AADLItemModel::zoomChanged()
{
    for (auto item : m_graphicsScene->selectedItems()) {
        if (auto iObj = qobject_cast<aadlinterface::InteractiveObject *>(item->toGraphicsObject())) {
            iObj->updateGripPoints();
        }
    }
}

QGraphicsItem *AADLItemModel::getItem(const shared::Id id) const
{
    return m_items.value(id);
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

QGraphicsItem *AADLItemModel::createItemForObject(aadl::AADLObject *obj)
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
    case aadl::AADLObject::Type::Comment: {
        auto comment = new AADLCommentGraphicsItem(qobject_cast<aadl::AADLObjectComment *>(obj), parentItem);
        nestedGeomtryConnect(parentItem, comment);
        iObj = comment;
    } break;
    case aadl::AADLObject::Type::InterfaceGroup:
        iObj = new AADLInterfaceGroupGraphicsItem(qobject_cast<aadl::AADLObjectIfaceGroup *>(obj), parentItem);
        break;
    case aadl::AADLObject::Type::RequiredInterface:
    case aadl::AADLObject::Type::ProvidedInterface:
        iObj = new AADLInterfaceGraphicsItem(qobject_cast<aadl::AADLObjectIface *>(obj), parentItem);
        break;
    case aadl::AADLObject::Type::ConnectionGroup:
        if (auto connection = qobject_cast<aadl::AADLObjectConnectionGroup *>(obj)) {
            aadl::AADLObjectIface *ifaceStart = connection->sourceInterface();
            auto startItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                    ifaceStart ? m_items.value(ifaceStart->id()) : nullptr);

            aadl::AADLObjectIface *ifaceEnd = connection->targetInterface();
            auto endItem =
                    qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(ifaceEnd ? m_items.value(ifaceEnd->id()) : nullptr);

            iObj = new AADLConnectionGroupGraphicsItem(connection,
                    qobject_cast<AADLInterfaceGroupGraphicsItem *>(startItem),
                    qobject_cast<AADLInterfaceGroupGraphicsItem *>(endItem), parentItem);
        }
        break;
    case aadl::AADLObject::Type::Connection:
        if (auto connection = qobject_cast<aadl::AADLObjectConnection *>(obj)) {
            aadl::AADLObjectIface *ifaceStart = connection->sourceInterface();
            auto startItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                    ifaceStart ? m_items.value(ifaceStart->id()) : nullptr);

            aadl::AADLObjectIface *ifaceEnd = connection->targetInterface();
            auto endItem =
                    qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(ifaceEnd ? m_items.value(ifaceEnd->id()) : nullptr);

            iObj = new AADLConnectionGraphicsItem(connection, startItem, endItem, parentItem);
        }
        break;
    case aadl::AADLObject::Type::Function: {
        auto function = new AADLFunctionGraphicsItem(qobject_cast<aadl::AADLObjectFunction *>(obj), parentItem);
        nestedGeomtryConnect(parentItem, function);
        iObj = function;
    } break;
    case aadl::AADLObject::Type::FunctionType: {
        auto functionType =
                new AADLFunctionTypeGraphicsItem(qobject_cast<aadl::AADLObjectFunctionType *>(obj), parentItem);
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

} // namespace aadlinterface
