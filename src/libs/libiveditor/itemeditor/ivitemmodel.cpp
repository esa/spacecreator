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
                || shared::graphicsviewutils::comparePolygones(connection->points(),
                           shared::graphicsviewutils::polygon(connection->entity()->coordinates())));
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

static const QSet<ivm::IVObject::Type> kRectangularTypes { ivm::IVObject::Type::FunctionType,
    ivm::IVObject::Type::Function, ivm::IVObject::Type::Comment };

static const QSet<ivm::IVObject::Type> kInterfaceTypes { ivm::IVObject::Type::RequiredInterface,
    ivm::IVObject::Type::ProvidedInterface, ivm::IVObject::Type::InterfaceGroup };

static const QSet<ivm::IVObject::Type> kConnectionTypes { ivm::IVObject::Type::Connection,
    ivm::IVObject::Type::ConnectionGroup };

namespace ive {

IVItemModel::IVItemModel(ivm::IVModel *model, cmd::CommandsStack *commandsStack, QObject *parent)
    : shared::ui::VEItemModel(model, commandsStack, parent)
    , m_textUpdate(new shared::DelayedSignal(this))
{
    m_textUpdate->setInterval(10);
    connect(m_textUpdate, &shared::DelayedSignal::triggered, this, &IVItemModel::updateInterfaceTexts);
    connect(model, &ivm::IVModel::rootObjectChanged, this, &IVItemModel::onRootObjectChanged);
}

IVItemModel::~IVItemModel() {}

void IVItemModel::onObjectAdded(shared::Id objectId)
{
    if (!m_graphicsScene || objectId.isNull()) {
        return;
    }

    auto object = objectsModel()->getObject(objectId);
    if (!object) {
        return;
    }

    if (object->type() == ivm::IVObject::Type::InterfaceGroup) {
        return;
    }

    setupGeometry(object);
    if (!isVisible(object)) {
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

static inline QPointF mapPositionFromOrigin(ivm::IVInterface *iface, ivm::meta::Props::Token coordinateToken,
        const QRectF &functionRect, Qt::Alignment *side)
{
    const auto parentFn = iface->parentObject()->as<ivm::IVFunctionType *>();
    const QRectF fnRect = shared::graphicsviewutils::rect(
            ivm::IVObject::coordinatesFromString(
                    parentFn->entityAttributeValue<QString>(ivm::meta::Props::token(coordinateToken))))
                                  .normalized();
    const QString strCoordinates = iface->entityAttributeValue<QString>(ivm::meta::Props::token(coordinateToken));
    QPointF pos = shared::graphicsviewutils::pos(ivm::IVObject::coordinatesFromString(strCoordinates));

    *side = shared::graphicsviewutils::getNearestSide(fnRect, pos);

    pos = shared::graphicsviewutils::getSidePosition(fnRect, pos, *side);
    if (qFuzzyCompare(fnRect.top(), pos.y())) {
        const qreal sf = (pos.x() - fnRect.left()) / (fnRect.right() - fnRect.left());
        pos = QLineF(functionRect.topLeft(), functionRect.topRight()).pointAt(sf);
    } else if (qFuzzyCompare(fnRect.bottom(), pos.y())) {
        const qreal sf = (pos.x() - fnRect.left()) / (fnRect.right() - fnRect.left());
        pos = QLineF(functionRect.bottomLeft(), functionRect.bottomRight()).pointAt(sf);
    } else if (qFuzzyCompare(fnRect.left(), pos.x())) {
        const qreal sf = (pos.y() - fnRect.top()) / (fnRect.bottom() - fnRect.top());
        pos = QLineF(functionRect.topLeft(), functionRect.bottomLeft()).pointAt(sf);
    } else if (qFuzzyCompare(fnRect.right(), pos.x())) {
        const qreal sf = (pos.y() - fnRect.top()) / (fnRect.bottom() - fnRect.top());
        pos = QLineF(functionRect.topRight(), functionRect.bottomRight()).pointAt(sf);
    }
    return pos;
}

void IVItemModel::setupRectangularGeometry(ivm::IVObject *obj)
{
    ivm::IVObject *parentObj = obj->parentObject();
    if (!isVisible(parentObj)) {
        return;
    }

    if (obj->isRootObject()) { // Root Level Function in Inner View
        static const QString tokenStr = ivm::meta::Props::token(ivm::meta::Props::Token::RootCoordinates);
        if (obj->hasEntityAttribute(tokenStr)) {
            return;
        }

        QRectF mappedViewportGeometry;
        if (const QGraphicsView *view = m_graphicsScene->views().value(0)) {
            const QRect viewportGeometry =
                    view->viewport()->geometry().marginsRemoved(shared::graphicsviewutils::kContentMargins.toMargins());
            mappedViewportGeometry = QRectF(view->mapToScene(QPoint(0, 0)),
                    view->mapToScene(QPoint(viewportGeometry.width(), viewportGeometry.height())));
        }

        const QString strRootCoord =
                ivm::IVObject::coordinatesToString(shared::graphicsviewutils::coordinates(mappedViewportGeometry));
        obj->setEntityProperty(tokenStr, strRootCoord);
        obj->setAttributeExportable(tokenStr, true);
    } else {
        static const QString tokenStr = ivm::meta::Props::token(ivm::meta::Props::Token::coordinates);
        const QString coordinatesStr = obj->entityAttributeValue<QString>(tokenStr);
        if (!coordinatesStr.isEmpty()) {
            const QVector<qint32> coordinates = ivm::IVObject::coordinatesFromString(coordinatesStr);
            if (!coordinates.isEmpty()) {
                const QRectF geometry = shared::graphicsviewutils::rect(coordinates);
                if (geometry.isValid()) {
                    return;
                }
            }
        }

        QRectF parentGeometry;
        if (parentObj) {
            static const QString parentTokenStr = ivm::meta::Props::token(ivm::meta::Props::Token::RootCoordinates);
            const QVariant rootCoord = parentObj->entityAttributeValue(parentTokenStr);
            if (rootCoord.isValid()) {
                parentGeometry =
                        shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(rootCoord.toString()));
            } else {
                QRectF mappedViewportGeometry;
                if (const QGraphicsView *view = m_graphicsScene->views().value(0)) {
                    const QRect viewportGeometry = view->viewport()->geometry().marginsRemoved(
                            shared::graphicsviewutils::kContentMargins.toMargins());
                    mappedViewportGeometry = QRectF(view->mapToScene(QPoint(0, 0)),
                            view->mapToScene(QPoint(viewportGeometry.width(), viewportGeometry.height())));
                }
                mappedViewportGeometry.moveCenter(parentGeometry.center());
                parentGeometry |= mappedViewportGeometry;
            }
        }
        QList<QRectF> existingRects;
        QRectF itemsGeometry;
        for (const shared::VEObject *veObj : objectsModel()->objects()) { // Check if visible would be ehough
            if (veObj->id() == obj->id() || veObj->id() == objectsModel()->rootObjectId()
                    || veObj->parentObject() != parentObj) {
                continue;
            }
            auto child = qobject_cast<const ivm::IVObject *>(veObj);
            if (kRectangularTypes.contains(child->type())) {
                const QVariant coordinates = child->entityAttributeValue(tokenStr);
                if (!coordinates.isValid()) {
                    continue;
                }
                const QRectF rect =
                        shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(coordinates.toString()));
                itemsGeometry |= rect;
                existingRects.append(rect);
            }
        }

        QRectF itemGeometry;
        shared::graphicsviewutils::findGeometryForRect(itemGeometry, parentGeometry, existingRects);
        if (parentObj) {
            const QString strRootCoord = ivm::IVObject::coordinatesToString(
                    shared::graphicsviewutils::coordinates(parentGeometry | itemsGeometry | itemGeometry));
            parentObj->setEntityProperty(
                    ivm::meta::Props::token(ivm::meta::Props::Token::RootCoordinates), strRootCoord);
            parentObj->setAttributeExportable(ivm::meta::Props::token(ivm::meta::Props::Token::RootCoordinates), false);
        }

        const QString strCoord =
                ivm::IVObject::coordinatesToString(shared::graphicsviewutils::coordinates(itemGeometry));
        obj->setEntityProperty(tokenStr, strCoord);
        if (parentObj) {
            obj->setAttributeExportable(tokenStr, false);
        }
    }
}

void IVItemModel::setupInterfaceGeometry(ivm::IVObject *obj)
{
    auto parentObj = qobject_cast<ivm::IVFunctionType *>(obj->parentObject());
    if (!parentObj) {
        return;
    }
    if (!isVisible(parentObj->parentObject())) {
        return;
    }

    ivm::meta::Props::Token token { ivm::meta::Props::Token::Unknown };
    if (parentObj->isRootObject()) {
        token = ivm::meta::Props::Token::RootCoordinates;
    } else {
        token = ivm::meta::Props::Token::coordinates;
    }
    if (obj->hasEntityAttribute(ivm::meta::Props::token(token))) {
        return;
    }
    const QString parentStrCoord = parentObj->entityAttributeValue<QString>(ivm::meta::Props::token(token));
    const QRectF parentRect = shared::graphicsviewutils::rect(shared::VEObject::coordinatesFromString(parentStrCoord));
    Qt::Alignment side = Qt::AlignCenter;

    QPointF ifacePos;
    if (obj->hasEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::coordinates))) {
        ifacePos = mapPositionFromOrigin(
                obj->as<ivm::IVInterface *>(), ivm::meta::Props::Token::coordinates, parentRect, &side);
    } else if (obj->hasEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::RootCoordinates))) {
        ifacePos = mapPositionFromOrigin(
                obj->as<ivm::IVInterface *>(), ivm::meta::Props::Token::RootCoordinates, parentRect, &side);
    } else {
        ifacePos = parentRect.topLeft();
    }

    const qreal sideSize = IVInterfaceGraphicsItem::baseLength();
    const QRectF kBaseRect = shared::graphicsviewutils::adjustFromPoint(QPointF(0, 0), sideSize);

    QRectF itemRect = shared::graphicsviewutils::adjustFromPoint(
            ifacePos, sideSize + IVInterfaceGraphicsItem::minSiblingDistance());

    QList<QRectF> siblingsRects;
    for (const ivm::IVInterface *iface : parentObj->allInterfaces()) {
        if (iface->id() == obj->id()) {
            continue;
        }
        const QString ifaceStrCoord = iface->entityAttributeValue<QString>(ivm::meta::Props::token(token));
        if (ifaceStrCoord.isEmpty()) {
            continue;
        }

        const QPointF ifacePoint =
                shared::graphicsviewutils::pos(shared::VEObject::coordinatesFromString(ifaceStrCoord));
        itemRect.moveCenter(ifacePoint);
        siblingsRects.append(itemRect);
    }

    itemRect.moveCenter(ifacePos);
    QPointF innerGeometry = ifacePos;
    QRectF intersectedRect;

    if (((ifacePos.isNull() && siblingsRects.isEmpty())
                || shared::graphicsviewutils::isCollided(siblingsRects, itemRect, &intersectedRect))
            && parentRect.isValid()) {
        QPainterPath pp;
        pp.addRect(kBaseRect);
        const QList<QPair<Qt::Alignment, QPainterPath>> sidePaths {
            { Qt::AlignLeft, pp },
            { Qt::AlignTop, pp },
            { Qt::AlignRight, pp },
            { Qt::AlignBottom, pp },
        };
        shared::PositionLookupHelper helper(sidePaths, parentRect, siblingsRects, ifacePos);
        if (helper.lookup()) {
            innerGeometry = helper.mappedOriginPoint();
        }
    }

    const QString strCoord = ivm::IVObject::coordinatesToString(shared::graphicsviewutils::coordinates(innerGeometry));
    obj->setEntityProperty(ivm::meta::Props::token(token), strCoord);
    if (parentObj->parentObject()) {
        obj->setAttributeExportable(ivm::meta::Props::token(token), false);
    }
}

void IVItemModel::setupConnectionGeometry(ivm::IVObject *obj)
{
    if (!isVisible(obj->parentObject())) {
        return;
    }

    if (obj->hasEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::coordinates))) {
        return;
    }

    ivm::IVConnection *connection = qobject_cast<ivm::IVConnection *>(obj);
    if (!connection) {
        return;
    }
    ivm::IVInterface *startEndPoint = connection->sourceInterface();
    if (!startEndPoint || !startEndPoint->function()) {
        return;
    }
    ivm::IVInterface *endEndPoint = connection->targetInterface();
    if (!endEndPoint || !endEndPoint->function()) {
        return;
    }

    const QPointF startPos = shared::graphicsviewutils::pos(startEndPoint->coordinates());
    const QPointF endPos = shared::graphicsviewutils::pos(endEndPoint->coordinates());
    const QRectF startRect = shared::graphicsviewutils::rect(startEndPoint->function()->coordinates());
    const QRectF endRect = shared::graphicsviewutils::rect(endEndPoint->function()->coordinates());

    QList<QRectF> siblingRects;
    for (auto entity : objectsModel()->objects()) {
        if (entity->parentObject() == connection->parentObject()) {
            ivm::IVObject *iObj = qobject_cast<ivm::IVObject *>(entity);
            if (!iObj)
                continue;

            if (kRectangularTypes.contains(iObj->type())) {
                siblingRects.append(shared::graphicsviewutils::rect(iObj->coordinates()));
            }
        }
    }

    const QVector<QPointF> points =
            shared::graphicsviewutils::createConnectionPath(siblingRects, startPos, startRect, endPos, endRect);
    connection->setCoordinates(shared::graphicsviewutils::coordinates(points));
    if (obj->parentObject()) {
        obj->setAttributeExportable(ivm::meta::Props::token(ivm::meta::Props::Token::coordinates), false);
    }
}

bool IVItemModel::isVisible(ivm::IVObject *obj)
{
    if (!obj) {
        return true;
    }

    const int lowestLevel = gi::nestingLevel(objectsModel()->rootObject()) + 1;
    const int objectLevel = gi::nestingLevel(obj);
    const bool isRootOrRootChild = obj->id() == objectsModel()->rootObjectId()
            || (objectsModel()->rootObject() && obj->parentObject() == objectsModel()->rootObject());
    return !((objectLevel < lowestLevel || objectLevel > (lowestLevel + gi::kNestingVisibilityLevel))
            && !isRootOrRootChild);
}

void IVItemModel::setupGeometry(ivm::IVObject *obj)
{
    if (!obj) {
        return;
    }
    if (kInterfaceTypes.contains(obj->type())) {
        setupInterfaceGeometry(obj);
    } else if (kRectangularTypes.contains(obj->type())) {
        setupRectangularGeometry(obj);
    } else if (kConnectionTypes.contains(obj->type())) {
        setupConnectionGeometry(obj);
    } else {
        qWarning() << "Unhandled entity:" << obj->type();
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

    const bool isClean = m_commandsStack->isClean();
    const auto geometryCmd = new cmd::CmdRootEntityChange(objectsModel(), id);
    m_commandsStack->push(geometryCmd);
    if (isClean)
        m_commandsStack->setClean();
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
