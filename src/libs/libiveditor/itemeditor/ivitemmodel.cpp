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

#include "ivitemmodel.h"

#include "commands/cmdrootentitychange.h"
#include "commandsstack.h"
#include "delayedsignal.h"
#include "graphicsitemhelpers.h"
#include "graphicsviewutils.h"
#include "interfacetabgraphicsscene.h"
#include "itemeditor/common/ivutils.h"
#include "ivcomment.h"
#include "ivcommentgraphicsitem.h"
#include "ivconnection.h"
#include "ivconnectiongraphicsitem.h"
#include "ivconnectiongroup.h"
#include "ivconnectiongroupgraphicsitem.h"
#include "ivfunction.h"
#include "ivfunctiongraphicsitem.h"
#include "ivfunctiontypegraphicsitem.h"
#include "ivinterface.h"
#include "ivinterfacegraphicsitem.h"
#include "ivinterfacegroup.h"
#include "ivinterfacegroupgraphicsitem.h"
#include "positionlookuphelper.h"
#include "ui/veconnectiongraphicsitem.h"

#include <QGraphicsView>
#include <QGuiApplication>
#include <QMutex>
#include <QScreen>
#include <QUndoStack>
#include <QtDebug>

#define IV_ITEM_DUMP

#ifdef IV_ITEM_DUMP
static inline void dumpItem(QObject *obj, bool strict = false)
{
    auto item = qobject_cast<shared::ui::VEInteractiveObject *>(obj);
    if (!item)
        return;

    qInfo() << item->metaObject()->className() << ":\n";

    if (auto fn = qobject_cast<ive::IVFunctionGraphicsItem *>(obj)) {
        const bool isInheritFnT = fn->entity()->inheritsFunctionType();
        qDebug() << isInheritFnT;
    }

    for (auto attr : item->entity()->entityAttributes()) {
        qInfo() << attr.name() << ":\t" << attr.value() << "\n";
    }

    if (auto iface = qobject_cast<ive::IVInterfaceGraphicsItem *>(item)) {
        qInfo() << "\nGraphics Iface geometry:\n" << iface->scenePos() << "\n" << iface->sceneBoundingRect() << "\n";
        qInfo() << "\nInternal Iface data:\n"
                << iface->entity()->title() << "\n"
                << shared::graphicsviewutils::pos(iface->entity()->coordinates()) << "\n";
        qInfo() << "\n####:\n" << iface->connectionItems();
        Q_ASSERT(!strict
                || iface->scenePos().toPoint() == shared::graphicsviewutils::pos(iface->entity()->coordinates()));
    } else if (auto connection = qobject_cast<ive::IVConnectionGraphicsItem *>(item)) {
        qInfo() << "\nGraphics Connection geometry:\n"
                << connection->points() << "\n"
                << connection->graphicsPoints() << "\n";
        qInfo() << "\nInternal Connection data:\n"
                << (connection->entity()->title().isEmpty() ? QStringLiteral("Connection: %1 <=> %2")
                                                                      .arg(connection->startItem()->entity()->title(),
                                                                              connection->endItem()->entity()->title())
                                                            : connection->entity()->title())
                << "\n"
                << shared::graphicsviewutils::polygon(connection->entity()->coordinates()) << "\n";
        Q_ASSERT(!strict
                || shared::graphicsviewutils::comparePolygones(connection->graphicsPoints(),
                        shared::graphicsviewutils::polygon(connection->entity()->coordinates())));
        Q_ASSERT(!strict
                || shared::graphicsviewutils::comparePolygones(
                        connection->points(), shared::graphicsviewutils::polygon(connection->entity()->coordinates())));
    } else if (auto rectItem = qobject_cast<shared::ui::VERectGraphicsItem *>(item)) {
        qInfo() << "\nGraphics" << rectItem->metaObject()->className() << "geometry:\n"
                << rectItem->sceneBoundingRect() << "\n";
        qInfo() << "\nInternal Function data:\n"
                << rectItem->entity()->title() << "\n"
                << shared::graphicsviewutils::rect(rectItem->entity()->coordinates()) << "\n";
        Q_ASSERT(!strict
                || rectItem->sceneBoundingRect().toRect()
                        == shared::graphicsviewutils::rect(rectItem->entity()->coordinates()).toRect());
    } else {
        qFatal("Not implemented trace");
    }
}
#endif

namespace ive {

IVItemModel::IVItemModel(ivm::IVModel *model, cmd::CommandsStack *commandsStack, QObject *parent)
    : shared::ui::VEItemModel(model, commandsStack, parent)
    , m_textUpdate(new shared::DelayedSignal(this))
{
    m_textUpdate->setInterval(10);
    connect(m_textUpdate, &shared::DelayedSignal::triggered, this, &IVItemModel::updateInterfaceTexts);
    connect(model, &ivm::IVModel::rootObjectChanged, this, &IVItemModel::onRootObjectChanged);
}

IVItemModel::~IVItemModel() { }

void IVItemModel::onObjectAdded(shared::Id objectId)
{
    if (!m_graphicsScene || objectId.isNull()) {
        return;
    }

    auto object = objectsModel()->getObject(objectId);
    if (!object) {
        return;
    }

    setupInnerGeometry(object);
    if (object->type() == ivm::IVObject::Type::InterfaceGroup) {
        return;
    }

    const int lowestLevel = gi::nestingLevel(objectsModel()->rootObject()) + 1;
    const int objectLevel = gi::nestingLevel(object);
    const bool isRootOrRootChild = objectId == objectsModel()->rootObjectId()
            || (objectsModel()->rootObject() && object->parentObject() == objectsModel()->rootObject());
    if ((objectLevel < lowestLevel || objectLevel > (lowestLevel + gi::kNestingVisibilityLevel))
            && !isRootOrRootChild) {
        return;
    }

    shared::ui::VEItemModel::onObjectAdded(objectId);
}

void IVItemModel::onRootObjectChanged(shared::Id rootId)
{
    Q_UNUSED(rootId)
    clearScene();

    for (auto obj : objectsModel()->visibleObjects()) {
        onObjectAdded(obj->id());
    }
}

void IVItemModel::onObjectRemoved(shared::Id objectId)
{
    shared::ui::VEItemModel::onObjectRemoved(objectId);
    scheduleInterfaceTextUpdate();
}

void IVItemModel::onConnectionAddedToGroup(ivm::IVConnection *connection)
{
    auto connectionGroupObject = qobject_cast<ivm::IVConnectionGroup *>(sender());
    if (!connectionGroupObject) {
        connectionGroupObject =
                qobject_cast<ivm::IVConnectionGroup *>(objectsModel()->getObjectByName(connection->groupName()));
        if (!connectionGroupObject) {
            return;
        }
    }
    updateInterfaceOnConnectionAdd(connection, connectionGroupObject->sourceInterfaceGroup());
    updateInterfaceOnConnectionAdd(connection, connectionGroupObject->targetInterfaceGroup());

    onObjectRemoved(connection->id());
    onObjectRemoved(connection->sourceInterface()->id());
    onObjectRemoved(connection->targetInterface()->id());
}

void IVItemModel::onConnectionRemovedFromGroup(ivm::IVConnection *connection)
{
    if (!connection) {
        return;
    }

    auto connectionGroupObject = qobject_cast<ivm::IVConnectionGroup *>(sender());
    if (!connectionGroupObject) {
        connectionGroupObject =
                qobject_cast<ivm::IVConnectionGroup *>(objectsModel()->getObjectByName(connection->groupName()));
        if (!connectionGroupObject) {
            return;
        }
    }

    if (connection->targetInterface())
        onObjectAdded(connection->targetInterface()->id());
    if (connection->sourceInterface())
        onObjectAdded(connection->sourceInterface()->id());
    onObjectAdded(connection->id());

    updateInterfaceOnConnectionRemove(connection, connectionGroupObject->sourceInterfaceGroup());
    updateInterfaceOnConnectionRemove(connection, connectionGroupObject->targetInterfaceGroup());
}

/*!
   Schedules an update for all interface texts (geometry constraints)
 */
void IVItemModel::scheduleInterfaceTextUpdate()
{
    m_textUpdate->triggerSignal();
}

/*!
   Checks the space for the interfaces to set it to e proper length
 */
void IVItemModel::updateInterfaceTexts()
{
    for (QGraphicsItem *item : m_items) {
        if (auto interfaceItem = dynamic_cast<ive::IVInterfaceGraphicsItem *>(item)) {
            interfaceItem->updateLabel();
        }
    }
}

IVFunctionGraphicsItem *IVItemModel::rootItem() const
{
    return getItem<IVFunctionGraphicsItem *>(objectsModel()->rootObjectId());
}

void IVItemModel::setupInnerGeometry(ivm::IVObject *obj) const
{
    if (!obj) {
        return;
    }

    QVariant innerCoord = obj->entityAttributeValue(ivm::meta::Props::token(ivm::meta::Props::Token::InnerCoordinates));
    if (innerCoord.isValid()) {
        return;
    }
    ivm::IVFunctionType *parentObj = obj->parentObject() ? obj->parentObject()->as<ivm::IVFunctionType *>() : nullptr;
    if (!parentObj) {
        return;
    }

    static const QSet<ivm::IVObject::Type> kRectangularTypes { ivm::IVObject::Type::FunctionType,
        ivm::IVObject::Type::Function, ivm::IVObject::Type::Comment };

    static const QSet<ivm::IVObject::Type> kInterfaceTypes { ivm::IVObject::Type::RequiredInterface,
        ivm::IVObject::Type::ProvidedInterface, ivm::IVObject::Type::InterfaceGroup };

    if (kRectangularTypes.contains(obj->type())) {
        QRectF rootGeometry;
        const QVariant rootCoord =
                parentObj->entityAttributeValue(ivm::meta::Props::token(ivm::meta::Props::Token::RootCoordinates));
        if (rootCoord.isValid()) {
            rootGeometry = shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(rootCoord.toString()));
        }
        QList<QRectF> existingRects;
        QRectF innerItemsGeometry;
        for (const ivm::IVObject *child : parentObj->children()) {
            if (kRectangularTypes.contains(child->type())) {
                innerCoord =
                        child->entityAttributeValue(ivm::meta::Props::token(ivm::meta::Props::Token::InnerCoordinates));
                if (!innerCoord.isValid()) {
                    continue;
                }
                const QRectF innerRect =
                        shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(innerCoord.toString()));
                innerItemsGeometry |= innerRect;
                existingRects.append(innerRect);
            }
        }
        QRectF innerGeometry;
        QRectF newRootGeometry { rootGeometry };
        shared::graphicsviewutils::findGeometryForRect(innerGeometry, newRootGeometry, existingRects);
        if (!rootCoord.isValid()) {
            QRectF mappedViewportGeometry;
            if (const QGraphicsView *view = m_graphicsScene->views().value(0)) {
                const QRect viewportGeometry = view->viewport()->geometry().marginsRemoved(
                        shared::graphicsviewutils::kContentMargins.toMargins());
                mappedViewportGeometry = QRectF(view->mapToScene(QPoint(0, 0)),
                        view->mapToScene(QPoint(viewportGeometry.width(), viewportGeometry.height())));
            }
            mappedViewportGeometry.moveCenter(rootGeometry.center());
            rootGeometry |= mappedViewportGeometry;
        } else {
            rootGeometry |= newRootGeometry;
        }

        const QString strRootCoord =
                ivm::IVObject::coordinatesToString(shared::graphicsviewutils::coordinates(rootGeometry));
        parentObj->setEntityProperty(ivm::meta::Props::token(ivm::meta::Props::Token::RootCoordinates), strRootCoord);

        const QString strCoord =
                ivm::IVObject::coordinatesToString(shared::graphicsviewutils::coordinates(innerGeometry));
        obj->setEntityProperty(ivm::meta::Props::token(ivm::meta::Props::Token::InnerCoordinates), strCoord);
    } else if (kInterfaceTypes.contains(obj->type())) {
        const QString parentStrCoord = parentObj->entityAttributeValue<QString>(
                ivm::meta::Props::token(ivm::meta::Props::Token::InnerCoordinates));
        const QRectF parentRect =
                shared::graphicsviewutils::rect(shared::VEObject::coordinatesFromString(parentStrCoord));
        QList<QRectF> siblingsRects;
        const qreal side = IVInterfaceGraphicsItem::baseLength();
        const QRectF kBaseRect { -side / 2, -side / 2, side, side };
        QRectF itemRect { kBaseRect };
        const QPointF initialOffset = itemRect.topLeft();
        itemRect.setWidth(IVInterfaceGraphicsItem::minSiblingDistance() - itemRect.x());
        for (const ivm::IVInterface *iface : parentObj->allInterfaces()) {
            const QString ifaceStrCoord = iface->entityAttributeValue<QString>(
                    ivm::meta::Props::token(ivm::meta::Props::Token::InnerCoordinates));
            if (ifaceStrCoord.isEmpty())
                continue;

            const QPointF ifacePoint =
                    shared::graphicsviewutils::pos(shared::VEObject::coordinatesFromString(ifaceStrCoord));
            itemRect.moveCenter(ifacePoint);
            siblingsRects.append(itemRect);
        }

        itemRect.translate(parentRect.topLeft());
        QPointF innerGeometry;
        QRectF intersectedRect;
        if (shared::graphicsviewutils::isCollided(siblingsRects, itemRect, &intersectedRect) && parentRect.isValid()) {
            QPainterPath pp;
            pp.addRect(kBaseRect);
            const QList<QPair<Qt::Alignment, QPainterPath>> sidePaths {
                { Qt::AlignLeft, pp },
                { Qt::AlignTop, pp },
                { Qt::AlignRight, pp },
                { Qt::AlignBottom, pp },
            };
            shared::PositionLookupHelper helper(sidePaths, parentRect, siblingsRects, itemRect, initialOffset);
            if (helper.lookup()) {
                innerGeometry = helper.mappedOriginPoint();
            }
        }

        const QString strCoord =
                ivm::IVObject::coordinatesToString(shared::graphicsviewutils::coordinates(innerGeometry));
        obj->setEntityProperty(ivm::meta::Props::token(ivm::meta::Props::Token::InnerCoordinates), strCoord);
    }
}

void IVItemModel::updateInterfaceOnConnectionAdd(
        ivm::IVConnection *connection, ivm::IVInterfaceGroup *connectionGroupEndPoint)
{
    ivm::IVInterface *ifaceObject = connectionGroupEndPoint->function()->id() == connection->source()->id()
            ? connection->sourceInterface()
            : connectionGroupEndPoint->function()->id() == connection->target()->id() ? connection->targetInterface()
                                                                                      : nullptr;
    if (!ifaceObject) {
        return;
    }

    if (auto ifaceItem = getItem<IVInterfaceGraphicsItem *>(ifaceObject->id())) {
        for (auto ifaceConnection : ifaceItem->connectionItems()) {
            if (ifaceConnection->entity()->id() == connection->id()) {
                continue;
            }
            ifaceConnection->replaceInterface(
                    ifaceItem, getItem<IVInterfaceGroupGraphicsItem *>(connectionGroupEndPoint->id()));
        }
    }
}

void IVItemModel::updateInterfaceOnConnectionRemove(
        ivm::IVConnection *connection, ivm::IVInterfaceGroup *connectionGroupEndPoint)
{
    ivm::IVInterface *ifaceObject = connectionGroupEndPoint->function()->id() == connection->source()->id()
            ? connection->sourceInterface()
            : connectionGroupEndPoint->function()->id() == connection->target()->id() ? connection->targetInterface()
                                                                                      : nullptr;
    if (!ifaceObject) {
        return;
    }
    if (auto ifaceItem = getItem<IVInterfaceGraphicsItem *>(ifaceObject->id())) {
        for (auto ifaceConnection : ifaceItem->connectionItems()) {
            const bool currentHandledConnection = ifaceConnection->entity()->id() == connection->id();
            const bool isLinkedIface =
                    ifaceConnection->sourceItem()->entity()->id() == connection->sourceInterface()->id()
                    || ifaceConnection->sourceItem()->entity()->id() == connection->targetInterface()->id()
                    || ifaceConnection->targetItem()->entity()->id() == connection->sourceInterface()->id()
                    || ifaceConnection->targetItem()->entity()->id() == connection->targetInterface()->id();
            if (currentHandledConnection || !isLinkedIface) {
                continue;
            }
            ifaceConnection->replaceInterface(ifaceItem, getItem<IVInterfaceGroupGraphicsItem *>(ifaceObject->id()));
        }
    }
}

void IVItemModel::changeRootItem(shared::Id id)
{
    if (objectsModel()->rootObjectId() == id) {
        return;
    }
    if (!m_commandsStack) {
        qWarning() << Q_FUNC_INFO << "No command stack set in IVItemModel";
        return;
    }

    const auto geometryCmd = new cmd::CmdRootEntityChange(objectsModel(), id);
    m_commandsStack->push(geometryCmd);
}

ivm::IVModel *IVItemModel::objectsModel() const
{
    return qobject_cast<ivm::IVModel *>(m_model);
}

shared::ui::VEInteractiveObject *IVItemModel::createItem(shared::Id objectId)
{
    auto obj = objectsModel()->getObject(objectId);
    Q_ASSERT(obj);
    if (!obj) {
        return nullptr;
    }

    QGraphicsItem *parentItem = obj->parentObject() ? m_items.value(obj->parentObject()->id()) : nullptr;

    switch (obj->type()) {
    case ivm::IVObject::Type::Comment: {
        auto comment = new IVCommentGraphicsItem(qobject_cast<ivm::IVComment *>(obj), parentItem);
        connect(comment, &shared::ui::InteractiveObjectBase::boundingBoxChanged, this,
                &IVItemModel::scheduleInterfaceTextUpdate);
        return comment;
    } break;
    case ivm::IVObject::Type::InterfaceGroup:
        return new IVInterfaceGroupGraphicsItem(qobject_cast<ivm::IVInterfaceGroup *>(obj), parentItem);
    case ivm::IVObject::Type::RequiredInterface:
    case ivm::IVObject::Type::ProvidedInterface: {
        auto ifItem = new IVInterfaceGraphicsItem(qobject_cast<ivm::IVInterface *>(obj), parentItem);
        connect(ifItem, &shared::ui::InteractiveObjectBase::boundingBoxChanged, this,
                &IVItemModel::scheduleInterfaceTextUpdate);
        return ifItem;
    } break;
    case ivm::IVObject::Type::ConnectionGroup:
        if (auto connection = qobject_cast<ivm::IVConnectionGroup *>(obj)) {
            auto ifaceGroupItem = [this](const shared::Id &id) -> ive::IVInterfaceGroupGraphicsItem * {
                const auto it = m_items.constFind(id);
                if (it != m_items.constEnd()) {
                    return qgraphicsitem_cast<ive::IVInterfaceGroupGraphicsItem *>(*it);
                }
                shared::ui::VEItemModel::onObjectAdded(id);
                return getItem<ive::IVInterfaceGroupGraphicsItem *>(id);
            };

            IVInterfaceGroupGraphicsItem *startItem =
                    connection->sourceInterface() ? ifaceGroupItem(connection->sourceInterface()->id()) : nullptr;

            IVInterfaceGroupGraphicsItem *endItem =
                    connection->targetInterface() ? ifaceGroupItem(connection->targetInterface()->id()) : nullptr;

            return new IVConnectionGroupGraphicsItem(connection, startItem, endItem, parentItem);
        }
        break;
    case ivm::IVObject::Type::Connection:
        if (auto connection = qobject_cast<ivm::IVConnection *>(obj)) {
            ivm::IVInterface *ifaceStart = connection->sourceInterface();
            auto startItem = qgraphicsitem_cast<IVInterfaceGraphicsItem *>(
                    ifaceStart ? m_items.value(ifaceStart->id()) : nullptr);

            ivm::IVInterface *ifaceEnd = connection->targetInterface();
            auto endItem =
                    qgraphicsitem_cast<IVInterfaceGraphicsItem *>(ifaceEnd ? m_items.value(ifaceEnd->id()) : nullptr);

            return new IVConnectionGraphicsItem(connection, startItem, endItem, parentItem);
        }
        break;
    case ivm::IVObject::Type::Function: {
        auto function = new IVFunctionGraphicsItem(qobject_cast<ivm::IVFunction *>(obj), parentItem);
        connect(function, &shared::ui::InteractiveObjectBase::boundingBoxChanged, this,
                &IVItemModel::scheduleInterfaceTextUpdate);
        return function;
    } break;
    case ivm::IVObject::Type::FunctionType: {
        auto functionType = new IVFunctionTypeGraphicsItem(qobject_cast<ivm::IVFunctionType *>(obj), parentItem);
        connect(functionType, &shared::ui::InteractiveObjectBase::boundingBoxChanged, this,
                &IVItemModel::scheduleInterfaceTextUpdate);
        return functionType;
    } break;
    default: {
        qCritical() << "Unknown object type:" << obj->type();
        break;
    }
    }

    return {};
}

void IVItemModel::initItem(shared::ui::VEInteractiveObject *item)
{
    shared::ui::VEItemModel::initItem(item);
    shared::VEObject *object = item->entity();
    if (!object) {
        return;
    }

    connect(object, &shared::VEObject::attributeChanged, this, &IVItemModel::scheduleInterfaceTextUpdate);

    if (const auto connectionGroupObject = qobject_cast<ivm::IVConnectionGroup *>(object)) {
        connect(connectionGroupObject, &ivm::IVConnectionGroup::connectionAdded, this,
                &IVItemModel::onConnectionAddedToGroup, Qt::UniqueConnection);
        connect(connectionGroupObject, &ivm::IVConnectionGroup::connectionRemoved, this,
                &IVItemModel::onConnectionRemovedFromGroup, Qt::UniqueConnection);

        for (auto groupedConnectionObject : connectionGroupObject->groupedConnections()) {
            onConnectionAddedToGroup(groupedConnectionObject);
        }
    }
    if (auto ivObject = object->as<ivm::IVObject *>()) {
        connect(ivObject, &ivm::IVObject::visibilityChanged, this, [this, id = ivObject->id()](bool) {
            if (auto item = getItem<shared::ui::VEInteractiveObject *>(id)) {
                item->scheduleLayoutUpdate();
                scheduleInterfaceTextUpdate();
            }
        });

#ifdef IV_ITEM_DUMP
        connect(item, &shared::ui::VEInteractiveObject::clicked, this, [this, item]() { dumpItem(sender()); });
#endif
    }
}

} // namespace ive
