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

#include "creatortool.h"

#include "aadlcommentgraphicsitem.h"
#include "aadlconnectiongraphicsitem.h"
#include "aadlfunctiongraphicsitem.h"
#include "aadlfunctiontypegraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "app/context/action/actionsmanager.h"
#include "commands/cmdcommentitemcreate.h"
#include "commands/cmdfunctionitemcreate.h"
#include "commands/cmdfunctiontypeitemcreate.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "connectioncreationvalidator.h"
#include "graphicsitemhelpers.h"

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMenu>
#include <QMouseEvent>
#include <QtDebug>
#include <QtMath>
#include <app/commandsstack.h>
#include <baseitems/common/utils.h>
#include <baseitems/graphicsview.h>
#include <limits>
#include <tab_aadl/aadlobjectcomment.h>
#include <tab_aadl/aadlobjectfunction.h>
#include <tab_aadl/aadlobjectfunctiontype.h>
#include <tab_aadl/aadlobjectiface.h>
#include <tab_aadl/aadlobjectsmodel.h>

static const qreal kContextMenuItemTolerance = 10.;
static const QMarginsF kMargins { 50., 50., 50., 50. };
static const QList<int> kFunctionTypes = { taste3::aadl::AADLFunctionGraphicsItem::Type,
                                           taste3::aadl::AADLFunctionTypeGraphicsItem::Type };

namespace taste3 {
namespace aadl {

CreatorTool::CreatorTool(QGraphicsView *view, AADLObjectsModel *model, QObject *parent)
    : QObject(parent)
    , m_view(view)
    , m_model(model)
{
    Q_ASSERT(view);
    Q_ASSERT(model);

    if (m_view && m_view->viewport()) {
        m_view->installEventFilter(this);
        m_view->viewport()->installEventFilter(this);
    }
}

CreatorTool::ToolType CreatorTool::toolType() const
{
    return m_toolType;
}

void CreatorTool::setCurrentToolType(CreatorTool::ToolType type)
{
    if (m_toolType == type)
        return;

    m_toolType = type;

    clearPreviewItem();

    if (m_view)
        m_view->setFocus();
}

bool CreatorTool::eventFilter(QObject *watched, QEvent *event)
{
    if (m_view && watched == m_view->viewport()) {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            return onMousePress(static_cast<QMouseEvent *>(event));
        case QEvent::MouseButtonRelease:
            return onMouseRelease(static_cast<QMouseEvent *>(event));
        case QEvent::MouseMove:
            return onMouseMove(static_cast<QMouseEvent *>(event));
        default:
            break;
        }
    }
    if (m_view == watched && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key()) {
        case Qt::Key_Delete: {
            removeSelectedItems();
        } break;
        case Qt::Key_Escape: {
            if (toolType() == ToolType::Pointer) {
                if (auto scene = m_view->scene())
                    scene->clearSelection();
            } else {
                clearPreviewItem();
                emit created();
            }
        } break;
        }
    }

    return false;
}

bool CreatorTool::onMousePress(QMouseEvent *e)
{
    if (!m_view)
        return false;

    auto scene = m_view->scene();
    if (!scene)
        return false;

    if (e->modifiers() & Qt::ControlModifier) {
        if (e->button() & Qt::MouseButton::LeftButton)
            m_toolType = ToolType::DirectConnection;
        else
            return false;
    } else if (!(e->button() & Qt::RightButton) && m_toolType == ToolType::Pointer) {
        return false;
    }

    const QPointF scenePos = cursorInScene(e->globalPos());
    if (m_toolType == ToolType::DirectConnection) {
        if (!utils::nearestItem(scene, scenePos,
                                { AADLFunctionTypeGraphicsItem::Type, AADLFunctionGraphicsItem::Type }))
            if (!utils::nearestItem(scene, scenePos, ConnectionCreationValidator::kInterfaceTolerance / 2,
                                    { AADLFunctionGraphicsItem::Type }))
                return false;

        if (m_previewConnectionItem) {
            m_connectionPoints.clear();
        } else {
            m_previewConnectionItem = new QGraphicsPathItem;
            m_previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
            m_previewConnectionItem->setZValue(1);
            scene->addItem(m_previewConnectionItem);
        }
        m_connectionPoints.append(scenePos);
        return true;
    } else if (m_toolType == ToolType::MultiPointConnection) {
        QGraphicsItem *item = utils::nearestItem(scene, scenePos, ConnectionCreationValidator::kConnectionTolerance,
                                                 { AADLInterfaceGraphicsItem::Type });
        if (!m_previewConnectionItem) {
            if (!item)
                return false;

            const QPointF startPoint = item->mapToScene(QPointF(0, 0));
            m_previewConnectionItem = new QGraphicsPathItem;
            m_previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
            m_previewConnectionItem->setZValue(1);
            scene->addItem(m_previewConnectionItem);
            m_connectionPoints.append(startPoint);
            return true;
        }
        return !m_connectionPoints.contains(scenePos);
    } else if (m_toolType != ToolType::RequiredInterface && m_toolType != ToolType::ProvidedInterface) {
        if (!m_previewItem) {
            auto findParent = [](QGraphicsItem *baseItem) -> QGraphicsItem * {
                const QList<int> types = { AADLFunctionGraphicsItem::Type };
                if (!baseItem || types.contains(baseItem->type()))
                    return baseItem;

                while (auto parentItem = baseItem->parentItem()) {
                    if (types.contains(parentItem->type()))
                        return parentItem;

                    baseItem = parentItem;
                }
                return nullptr;
            };

            QGraphicsItem *parentItem = findParent(m_view->itemAt(e->localPos().toPoint()));
            m_previewItem = new QGraphicsRectItem(parentItem);
            m_previewItem->setPen(QPen(Qt::blue, 2, Qt::SolidLine));
            m_previewItem->setBrush(QBrush(QColor(30, 144, 255, 90)));
            m_previewItem->setZValue(1);
            m_clickScenePos = scenePos;

            if (!parentItem)
                scene->addItem(m_previewItem);
        }
        const QPointF mappedScenePos = m_previewItem->mapFromScene(scenePos);
        m_previewItem->setRect({ mappedScenePos, mappedScenePos });
        return true;
    }

    return true;
}

bool CreatorTool::onMouseRelease(QMouseEvent *e)
{
    if (!m_view)
        return false;

    const QPointF scenePos = cursorInScene(e->globalPos());

    if (m_toolType == ToolType::Pointer) {
        if ((e->button() & Qt::RightButton) && e->modifiers() == Qt::NoModifier) {
            if (QMenu *menu = populateContextMenu(scenePos)) {
                connect(menu, &QMenu::aboutToHide, this, [this]() {
                    if (m_previewItem)
                        m_previewItem->setVisible(false);
                });
                menu->exec(e->globalPos());
                clearPreviewItem();
                return true;
            }
        }
    } else {
        const bool hasPreview = m_previewItem || m_previewConnectionItem;
        const bool isIface = m_toolType == ToolType::ProvidedInterface || m_toolType == ToolType::RequiredInterface;
        if (hasPreview || isIface) {
            handleToolType(m_toolType, scenePos);
            return true;
        }
    }
    return false;
}

bool CreatorTool::onMouseMove(QMouseEvent *e)
{
    if (!m_view)
        return false;

    const QPointF &scenePos = cursorInScene(e->globalPos());
    if (m_previewItem && m_previewItem->isVisible()) {
        const QPointF eventPos = m_previewItem->mapFromScene(scenePos);
        const QRectF newGeometry = QRectF(m_previewItem->mapFromScene(m_clickScenePos), eventPos).normalized();
        if (m_previewItem->parentItem()) {
            if (!m_previewItem->parentItem()->boundingRect().contains(newGeometry))
                return false;
        } else if (auto scene = m_view->scene()) {
            const QList<QGraphicsItem *> collidedItems = scene->items(newGeometry.marginsAdded(kMargins));
            auto it = std::find_if(
                    collidedItems.constBegin(), collidedItems.constEnd(),
                    [this](const QGraphicsItem *item) { return item != m_previewItem && !item->parentItem(); });
            if (it != collidedItems.constEnd())
                return false;
        }
        m_previewItem->setRect(newGeometry);
        return true;
    } else if (m_previewConnectionItem && m_previewConnectionItem->isVisible() && !m_connectionPoints.isEmpty()) {
        if (m_view->scene()) {
            QPainterPath pp;
            pp.addPolygon(m_connectionPoints);
            pp.lineTo(scenePos);
            m_previewConnectionItem->setPath(pp);
            warnConnectionPreview(scenePos);
            return true;
        }
    }

    return false;
}

QPointF CreatorTool::cursorInScene() const
{
    return cursorInScene(QCursor::pos()); // TODO: add current screen detection
}

QPointF CreatorTool::cursorInScene(const QPoint &globalPos) const
{
    QPointF sceneCoordinates;
    if (m_view) {
        const QPoint viewCoordinates = m_view->viewport()->mapFromGlobal(globalPos);
        sceneCoordinates = m_view->mapToScene(viewCoordinates);
    }
    return sceneCoordinates;
}

template<typename ItemType>
ItemType *itemAt(const QGraphicsScene *scene, const QPointF &point)
{
    QList<QGraphicsItem *> items = scene->items(point);
    if (items.isEmpty())
        return nullptr;
    auto it = std::find_if(items.constBegin(), items.constEnd(),
                           [](const QGraphicsItem *item) { return item->type() == ItemType::Type; });
    if (it == items.constEnd())
        return nullptr;

    return qgraphicsitem_cast<ItemType *>(*it);
}

static inline bool alignToSidePoint(const QGraphicsScene *scene, const QPointF &point, QPointF &pointToAlign)
{
    auto ifaceItem = itemAt<AADLInterfaceGraphicsItem>(scene, point);
    if (!ifaceItem)
        return false;

    QGraphicsItem *targetItem = ifaceItem->targetItem();
    if (!targetItem)
        return false;

    switch (utils::getNearestSide(targetItem->sceneBoundingRect(), point)) {
    case Qt::AlignTop:
    case Qt::AlignBottom:
        pointToAlign.setX(point.x());
        break;
    case Qt::AlignLeft:
    case Qt::AlignRight:
        pointToAlign.setY(point.y());
        break;
    default:
        break;
    }
    return true;
}

static inline QRectF adjustToSize(const QRectF &rect, const QSizeF &minSize)
{
    QRectF itemRect = rect;
    if (itemRect.width() < minSize.width())
        itemRect.setWidth(minSize.width());
    if (itemRect.height() < minSize.height())
        itemRect.setHeight(minSize.height());
    return itemRect;
};

void CreatorTool::handleConnection(const QVector<QPointF> &connectionPoints) const
{
    ConnectionInfo info = ConnectionCreationValidator::validate(m_view ? m_view->scene() : nullptr, connectionPoints);
    if (info.failed())
        return;

    const AADLFunctionGraphicsItem *parentForConnection = nullptr;
    QPointF startInterfacePoint { info.startPointAdjusted };
    QPointF endInterfacePoint { info.endPointAdjusted };
    taste3::cmd::CommandsStack::current()->beginMacro(QObject::tr("Create connection"));
    AADLObjectIface::CreationInfo ifaceCommons;

    bool startRequired = true;
    if (info.startIface && !info.endIface) {
        startRequired = info.startIface->isRequired();

        ifaceCommons = AADLObjectIface::CreationInfo::fromIface(info.startIface);
        ifaceCommons.function = info.endObject;
        ifaceCommons.position = info.endPointAdjusted;
        ifaceCommons.type = (startRequired && info.isToOrFromNested) || (!startRequired && !info.isToOrFromNested)
                ? AADLObjectIface::IfaceType::Required
                : AADLObjectIface::IfaceType::Provided;
        ifaceCommons.id = info.endIfaceId;
        ifaceCommons.resetKind();

        taste3::cmd::CommandsStack::current()->push(
                cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, ifaceCommons.toVarList()));
    } else if (info.endIface && !info.startIface) {
        startRequired = (info.endIface->isRequired() && info.isToOrFromNested)
                || (info.endIface->isProvided() && !info.isToOrFromNested);

        ifaceCommons = AADLObjectIface::CreationInfo::fromIface(info.endIface);
        ifaceCommons.function = info.startObject;
        ifaceCommons.position = info.startPointAdjusted;
        ifaceCommons.type = startRequired ? AADLObjectIface::IfaceType::Required : AADLObjectIface::IfaceType::Provided;
        ifaceCommons.id = info.startIfaceId;
        ifaceCommons.resetKind();

        taste3::cmd::CommandsStack::current()->push(
                cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, ifaceCommons.toVarList()));
    } else if (!info.startIface && !info.endIface) {
        ifaceCommons.model = m_model.data();

        ifaceCommons.function = info.startObject;
        ifaceCommons.position = info.startPointAdjusted;
        ifaceCommons.id = info.startIfaceId;

        const bool isToNested = info.isToOrFromNested && info.functionAtEndPos->isAncestorOf(info.functionAtStartPos);
        if (info.isToOrFromNested)
            ifaceCommons.type =
                    (isToNested) ? AADLObjectIface::IfaceType::Required : AADLObjectIface::IfaceType::Provided;
        else
            ifaceCommons.type =
                    (startRequired ? AADLObjectIface::IfaceType::Required : AADLObjectIface::IfaceType::Provided);
        ifaceCommons.resetKind();

        taste3::cmd::CommandsStack::current()->push(
                cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, ifaceCommons.toVarList()));

        ifaceCommons.function = info.endObject;
        ifaceCommons.position = info.endPointAdjusted;
        ifaceCommons.id = info.endIfaceId;

        if (info.isToOrFromNested)
            ifaceCommons.type =
                    (isToNested) ? AADLObjectIface::IfaceType::Required : AADLObjectIface::IfaceType::Provided;
        else
            ifaceCommons.type =
                    startRequired ? AADLObjectIface::IfaceType::Provided : AADLObjectIface::IfaceType::Required;
        ifaceCommons.resetKind();

        taste3::cmd::CommandsStack::current()->push(
                cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, ifaceCommons.toVarList()));
    } else {
        AADLObjectIface *pi =
                AADLObjectConnection::selectIface<AADLObjectIfaceProvided *>(info.startIface, info.endIface);
        if (!pi)
            pi = info.startIface;
        ifaceCommons = AADLObjectIface::CreationInfo::fromIface(pi);
        ifaceCommons.resetKind();
        ifaceCommons.name.clear();
    }

    AADLFunctionGraphicsItem *prevStartItem =
            qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(info.functionAtStartPos);
    QPointF firstExcludedPoint = *std::next(connectionPoints.constBegin());
    common::Id prevStartIfaceId = info.startIfaceId;
    /// TODO: check creating connection from nested function as a start function
    while (auto item = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(prevStartItem->parentItem())) {
        const QVector<QPointF> intersectionPoints =
                utils::intersectionPoints(item->sceneBoundingRect(), connectionPoints);
        if (intersectionPoints.isEmpty() || intersectionPoints.size() % 2 == 0) {
            parentForConnection = item;
            break;
        }

        auto beginIt = std::find(connectionPoints.constBegin(), connectionPoints.constEnd(), firstExcludedPoint);
        auto endIt = std::find_if(
                connectionPoints.constBegin(), connectionPoints.constEnd(),
                [rect = item->sceneBoundingRect()](const QPointF &point) { return !rect.contains(point); });
        QVector<QPointF> points { startInterfacePoint };
        std::copy(beginIt, endIt, std::back_inserter(points));
        points.append(intersectionPoints.last());

        ifaceCommons.type = startRequired ? AADLObjectIface::IfaceType::Required : AADLObjectIface::IfaceType::Provided;

        if (item == info.functionAtEndPos) {
            ifaceCommons.id = info.endIfaceId;
        } else {
            ifaceCommons.function = item->entity();
            ifaceCommons.position = intersectionPoints.last();
            ifaceCommons.id = common::createId();

            taste3::cmd::CommandsStack::current()->push(
                    cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, ifaceCommons.toVarList()));
        }

        const QVariantList params = { QVariant::fromValue(m_model.data()), QVariant::fromValue(item->entity()),
                                      prevStartIfaceId, ifaceCommons.id, QVariant::fromValue(points) };
        taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateConnectionEntity, params));

        firstExcludedPoint = endIt != connectionPoints.constEnd() ? *endIt : QPointF();
        startInterfacePoint = intersectionPoints.last();
        prevStartItem = item;
        prevStartIfaceId = ifaceCommons.id;
    }

    QPointF lastExcludedPoint = *std::next(connectionPoints.crbegin());
    AADLFunctionGraphicsItem *prevEndItem = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(info.functionAtEndPos);
    common::Id prevEndIfaceId = info.endIfaceId;
    /// TODO: check creating connection from parent item as a start function
    while (auto item = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(prevEndItem->parentItem())) {
        const QVector<QPointF> intersectionPoints =
                utils::intersectionPoints(item->sceneBoundingRect(), connectionPoints);
        if (intersectionPoints.isEmpty() || intersectionPoints.size() % 2 == 0) {
            Q_ASSERT(parentForConnection == item || parentForConnection == nullptr);
            parentForConnection = item;
            break;
        }
        auto beginIt = std::find(connectionPoints.crbegin(), connectionPoints.crend(), lastExcludedPoint);
        auto endIt = std::find_if(
                connectionPoints.crbegin(), connectionPoints.crend(),
                [rect = item->sceneBoundingRect()](const QPointF &point) { return !rect.contains(point); });
        QVector<QPointF> points { endInterfacePoint };
        std::copy(beginIt, endIt, std::back_inserter(points));
        points.append(intersectionPoints.last());

        if (item == info.functionAtStartPos) {
            ifaceCommons.id = info.startIfaceId;
        } else {
            ifaceCommons.id = common::createId();

            ifaceCommons.function = item->entity();
            ifaceCommons.position = intersectionPoints.last();
            ifaceCommons.type =
                    startRequired ? AADLObjectIface::IfaceType::Provided : AADLObjectIface::IfaceType::Required;
            ifaceCommons.id = ifaceCommons.id;

            taste3::cmd::CommandsStack::current()->push(
                    cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, ifaceCommons.toVarList()));
        }
        const QVariantList params = { QVariant::fromValue(m_model.data()), QVariant::fromValue(item->entity()),
                                      prevEndIfaceId, ifaceCommons.id, QVariant::fromValue(points) };
        taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateConnectionEntity, params));

        lastExcludedPoint = endIt != connectionPoints.crend() ? *endIt : QPointF();
        endInterfacePoint = intersectionPoints.last();
        prevEndItem = item;
        prevEndIfaceId = ifaceCommons.id;
    }

    auto beginIt = std::find(connectionPoints.constBegin(), connectionPoints.constEnd(), firstExcludedPoint);
    auto endIt = std::find(beginIt, connectionPoints.constEnd(), lastExcludedPoint);
    QVector<QPointF> points { startInterfacePoint };
    if (endIt != connectionPoints.constEnd()) {
        std::advance(endIt, 1);
        std::copy(beginIt, endIt, std::back_inserter(points));
    }
    points.append(endInterfacePoint);
    Q_ASSERT(points.size() == 2);
    if (points.first() != points.last()) {
        const QVariantList params = { QVariant::fromValue(m_model.data()),
                                      QVariant::fromValue(parentForConnection ? parentForConnection->entity()
                                                                              : nullptr),
                                      prevStartIfaceId, prevEndIfaceId, QVariant::fromValue(points) };
        taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateConnectionEntity, params));
    }

    taste3::cmd::CommandsStack::current()->endMacro();
}

void CreatorTool::handleToolType(CreatorTool::ToolType type, const QPointF &pos)
{
    if (!m_view)
        return;

    if (auto scene = m_view->scene()) {
        switch (type) {
        case ToolType::Comment:
            handleComment(scene, pos);
            break;
        case ToolType::FunctionType:
            handleFunctionType(scene, pos);
            break;
        case ToolType::Function:
            handleFunction(scene, pos);
            break;
        case ToolType::ProvidedInterface:
            handleInterface(scene, AADLObjectIface::IfaceType::Provided, pos);
            break;
        case ToolType::RequiredInterface:
            handleInterface(scene, AADLObjectIface::IfaceType::Required, pos);
            break;
        case ToolType::MultiPointConnection:
            if (!handleConnectionCreate(pos))
                return;
            handleConnection(m_connectionPoints);
            break;
        case ToolType::DirectConnection:
            handleDirectConnection(pos);
            break;
        default:
            break;
        }
        clearPreviewItem();
    }

    emit created();
}

bool CreatorTool::handleConnectionCreate(const QPointF &pos)
{
    QGraphicsScene *scene = m_view ? m_view->scene() : nullptr;
    if (!scene)
        return false;

    if (!m_previewConnectionItem)
        return false;

    QPointF alignedPos { pos };
    if (m_connectionPoints.size() >= 2) {
        if (QGraphicsItem *itemUnderCursor =
                    utils::nearestItem(scene, pos, ConnectionCreationValidator::kConnectionTolerance,
                                       { AADLInterfaceGraphicsItem::Type })) {

            const QPointF finishPoint = itemUnderCursor->mapToScene(QPointF(0, 0));
            if (!alignToSidePoint(scene, finishPoint, m_connectionPoints.last()))
                return false;

            m_connectionPoints.append(finishPoint);
            return true;
        }
    } else if (!alignToSidePoint(scene, m_connectionPoints.value(0), alignedPos))
        return false;

    m_connectionPoints.append(alignedPos);

    QPainterPath pp;
    pp.addPolygon(m_connectionPoints);
    pp.lineTo(pos);
    m_previewConnectionItem->setPath(pp);
    return false;
}

void CreatorTool::handleComment(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene)
    Q_UNUSED(pos)

    if (m_previewItem) {
        AADLObjectFunctionType *parentObject = gi::functionObject(m_previewItem->parentItem());
        if (!parentObject)
            parentObject = gi::functionTypeObject(m_previewItem->parentItem());

        const QRectF itemSceneRect =
                adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()), utils::DefaultGraphicsItemSize);
        const QVariantList params = { QVariant::fromValue(m_model.data()), QVariant::fromValue(parentObject),
                                      itemSceneRect };
        taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateCommentEntity, params));
    }
}

void CreatorTool::handleFunctionType(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene)
    Q_UNUSED(pos)

    if (m_previewItem) {
        const QRectF itemSceneRect =
                adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()), utils::DefaultGraphicsItemSize);
        AADLObjectFunction *parentObject = gi::functionObject(m_previewItem->parentItem());

        const QVariantList params = { QVariant::fromValue(m_model.data()), QVariant::fromValue(parentObject),
                                      itemSceneRect };
        taste3::cmd::CommandsStack::current()->push(
                cmd::CommandsFactory::create(cmd::CreateFunctionTypeEntity, params));
    }
}

void CreatorTool::handleFunction(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene)
    Q_UNUSED(pos)

    if (m_previewItem) {
        const QRectF itemSceneRect =
                adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()), utils::DefaultGraphicsItemSize);
        AADLObjectFunction *parentObject = gi::functionObject(m_previewItem->parentItem());
        const QVariantList params = { QVariant::fromValue(m_model.data()), QVariant::fromValue(parentObject),
                                      itemSceneRect };

        taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateFunctionEntity, params));
    }
}

void CreatorTool::handleInterface(QGraphicsScene *scene, AADLObjectIface::IfaceType type, const QPointF &pos)
{
    if (QGraphicsItem *parentItem = utils::nearestItem(
                scene, utils::adjustFromPoint(pos, ConnectionCreationValidator::kInterfaceTolerance), kFunctionTypes)) {
        AADLObjectFunctionType *parentObject = gi::functionTypeObject(parentItem);

        if (parentObject->isFunction()) {
            if (auto fn = parentObject->as<const AADLObjectFunction *>()) {
                if (const AADLObjectFunctionType *fnType = fn->instanceOf()) {
                    const QString message = tr("Can't add interface directly in <b>%1</b>.<br>"
                                               "Please edit the related <b>%2</b> instead.")
                                                    .arg(fn->title(), fnType->title());
                    emit informUser(tr("Interface adding"), message);
                    return;
                }
            }
        }

        AADLObjectIface::CreationInfo ifaceDescr(m_model.data(), parentObject, pos, type, common::InvalidId);
        ifaceDescr.resetKind();
        taste3::cmd::CommandsStack::current()->push(
                cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, ifaceDescr.toVarList()));
    }
}

void CreatorTool::handleDirectConnection(const QPointF &pos)
{
    if (m_connectionPoints.size() < 1)
        return;

    m_connectionPoints.append(pos);

    handleConnection(m_connectionPoints);
}

void CreatorTool::removeSelectedItems()
{
    if (!m_view)
        return;

    if (auto scene = m_view->scene()) {
        QStringList clonedIfaces;
        taste3::cmd::CommandsStack::current()->beginMacro(tr("Remove selected item(s)"));
        while (!scene->selectedItems().isEmpty()) {
            clearPreviewItem();

            QGraphicsItem *item = scene->selectedItems().first();
            item->setSelected(false);

            if (auto iObj = qobject_cast<InteractiveObject *>(item->toGraphicsObject())) {
                if (auto entity = iObj->aadlObject()) {
                    if (entity->isInterface()) {
                        if (auto iface = entity->as<const AADLObjectIface *>()) {
                            if (iface->isClone()) {
                                clonedIfaces.append(iface->title());
                                continue;
                            }
                        }
                    }
                    const QVariantList params = { QVariant::fromValue(entity), QVariant::fromValue(m_model.data()) };
                    if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::RemoveEntity, params))
                        taste3::cmd::CommandsStack::current()->push(cmdRm);
                }
            }
        }
        taste3::cmd::CommandsStack::current()->endMacro();

        if (!clonedIfaces.isEmpty()) {
            const QString names = clonedIfaces.join(QStringLiteral("\n"));
            const QString msg = tr("The following interfaces can not be removed directly:\n"
                                   "%1\n"
                                   "Please edit its related FunctionType.")
                                        .arg(names);
            emit informUser(tr("Interface removal"), msg);
        }
    }
}

void CreatorTool::clearPreviewItem()
{
    m_clickScenePos = QPointF();

    m_connectionPoints.clear();
    if (m_previewConnectionItem) {
        m_previewConnectionItem->scene()->removeItem(m_previewConnectionItem);
        delete m_previewConnectionItem;
        m_previewConnectionItem = nullptr;
    }

    if (m_previewItem) {
        m_previewItem->scene()->removeItem(m_previewItem);
        delete m_previewItem;
        m_previewItem = nullptr;
    }
    //    if (m_view && m_view->sceneRect() != m_view->scene()->sceneRect())
    //        m_view->setSceneRect({});
}

QMenu *CreatorTool::populateContextMenu(const QPointF &scenePos)
{
    QMenu *menu = new QMenu(m_view);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    populateContextMenu_commonCreate(menu, scenePos);
    populateContextMenu_propertiesDialog(menu, scenePos);
    populateContextMenu_user(menu, scenePos);

    if (menu->isEmpty()) {
        delete menu;
        menu = nullptr;
    }

    return menu;
}

void CreatorTool::populateContextMenu_commonCreate(QMenu *menu, const QPointF &scenePos)
{
    if (m_previewItem) {
        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/function_type.svg")), tr("Function Type"),
                        this, [this, scenePos]() { handleToolType(ToolType::FunctionType, scenePos); });

        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/function.svg")), tr("Function"), this,
                        [this, scenePos]() { handleToolType(ToolType::Function, scenePos); });

        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/comment.svg")), tr("Comment"), this,
                        [this, scenePos]() { handleToolType(ToolType::Comment, scenePos); });

        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/ri.svg")), tr("Required Interface"), this,
                        [this, scenePos]() { handleToolType(ToolType::RequiredInterface, scenePos); });

        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/pi.svg")), tr("Provided Interface"), this,
                        [this, scenePos]() { handleToolType(ToolType::ProvidedInterface, scenePos); });
    }
}

void CreatorTool::populateContextMenu_propertiesDialog(QMenu *menu, const QPointF & /*scenePos*/)
{
    QGraphicsScene *scene = m_view->scene();
    if (!scene)
        return;

    AADLObject *aadlObj { nullptr };
    if (QGraphicsItem *gi = scene->selectedItems().isEmpty() ? nullptr : scene->selectedItems().first()) {
        switch (gi->type()) {
        case AADLFunctionTypeGraphicsItem::Type: {
            aadlObj = gi::functionTypeObject(gi);
            break;
        }
        case AADLFunctionGraphicsItem::Type: {
            aadlObj = gi::functionObject(gi);
            break;
        }
        case AADLInterfaceGraphicsItem::Type: {
            aadlObj = gi::interfaceObject(gi);
            break;
        }
        default:
            break;
        }
    }

    menu->addSeparator();
    QAction *action = menu->addAction(tr("Properties"));
    action->setEnabled(aadlObj);

    if (aadlObj) {
        connect(action, &QAction::triggered, [this, aadlObj]() { emit propertyEditorRequest(aadlObj); });
        common::registerAction(Q_FUNC_INFO, menu->actions().last(), "Properties", "Show AADL object properties editor");
    }
}

void CreatorTool::populateContextMenu_user(QMenu *menu, const QPointF &scenePos)
{
    QGraphicsScene *scene = m_view->scene();
    if (!scene)
        return;

    static const QList<int> showProps { AADLInterfaceGraphicsItem::Type, AADLFunctionTypeGraphicsItem::Type,
                                        AADLFunctionGraphicsItem::Type, AADLCommentGraphicsItem::Type,
                                        AADLConnectionGraphicsItem::Type };

    AADLObject *aadlObj { nullptr };
    if (QGraphicsItem *gi = scene->selectedItems().size() == 1
                ? scene->selectedItems().first()
                : utils::nearestItem(scene, scenePos, kContextMenuItemTolerance, showProps)) {

        switch (gi->type()) {
        case AADLFunctionTypeGraphicsItem::Type: {
            aadlObj = gi::functionTypeObject(gi);
            break;
        }
        case AADLFunctionGraphicsItem::Type: {
            aadlObj = gi::functionObject(gi);
            break;
        }
        case AADLInterfaceGraphicsItem::Type: {
            aadlObj = gi::interfaceObject(gi);
            break;
        }
        case AADLCommentGraphicsItem::Type: {
            aadlObj = gi::commentObject(gi);
            break;
        }
        case AADLConnectionGraphicsItem::Type: {
            aadlObj = gi::connectionObject(gi);
            break;
        }
        default:
            return;
        }
    }

    ctx::ActionsManager::populateMenu(menu, aadlObj);
}

bool CreatorTool::warnConnectionPreview(const QPointF &pos)
{
    QVector<QPointF> connectionPoints = m_connectionPoints;
    if (connectionPoints.size() > 1)
        connectionPoints.replace(connectionPoints.size() - 1, pos);
    else
        connectionPoints.append(pos);

    const ConnectionInfo &info =
            ConnectionCreationValidator::validate(m_view ? m_view->scene() : nullptr, connectionPoints);
    const bool warn = info.failed();

    if (m_previewConnectionItem) {
        QPen p = m_previewConnectionItem->pen();
        p.setColor(warn ? Qt::red : Qt::black);
        m_previewConnectionItem->setPen(p);
    }

    return warn;
}

} // namespace aadl
} // namespace taste3
