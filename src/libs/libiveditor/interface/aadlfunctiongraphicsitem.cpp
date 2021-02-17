/*
  Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlfunctiongraphicsitem.h"

#include "aadlcommentgraphicsitem.h"
#include "aadlconnection.h"
#include "aadlconnectiongraphicsitem.h"
#include "aadlfunction.h"
#include "aadlfunctionnamegraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "aadlmodel.h"
#include "baseitems/common/aadlutils.h"
#include "colors/colormanager.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QGraphicsView>
#include <QPainter>
#include <QSvgRenderer>
#include <QTimer>
#include <QtDebug>
#include <cmath>

static const qreal kBorderWidth = 2.0;
static const qreal kRadius = 10.0;
static const qreal kOffset = kBorderWidth / 2.0;

namespace ive {

QPointer<QSvgRenderer> AADLFunctionGraphicsItem::m_svgRenderer = {};

AADLFunctionGraphicsItem::AADLFunctionGraphicsItem(ivm::AADLFunction *entity, QGraphicsItem *parent)
    : AADLFunctionTypeGraphicsItem(entity, parent)
{
    m_textItem->setVisible(!isRootItem());
    m_textItem->setTextAlignment(Qt::AlignCenter);

    if (!m_svgRenderer) // TODO: change icon
        m_svgRenderer = new QSvgRenderer(QLatin1String(":/tab_interface/toolbar/icns/change_root.svg"));
}

void AADLFunctionGraphicsItem::init()
{
    AADLFunctionTypeGraphicsItem::init();
    connect(entity(), &ivm::AADLFunction::childAdded, this, [this]() { update(); });
    connect(entity(), &ivm::AADLFunction::childRemoved, this, [this]() { update(); });
}

ivm::AADLFunction *AADLFunctionGraphicsItem::entity() const
{
    return qobject_cast<ivm::AADLFunction *>(aadlObject());
}

QPainterPath AADLFunctionGraphicsItem::shape() const
{
    QPainterPath pp;
    const QRectF br = boundingRect().adjusted(kOffset, kOffset, -kOffset, -kOffset);
    if (isRootItem())
        pp.addRect(br);
    else
        pp.addRoundedRect(br, kRadius, kRadius);
    return pp;
}

void AADLFunctionGraphicsItem::rebuildLayout()
{
    auto view = scene()->views().value(0);
    if (!view)
        return;

    if (isRootItem()) {
        QRectF nestedItemsInternalRect = nestedItemsSceneBoundingRect();

        const QRect viewportGeometry = view->viewport()->geometry().marginsRemoved(kContentMargins.toMargins());
        const QRectF mappedViewportGeometry =
                QRectF(view->mapToScene(viewportGeometry.topLeft()), view->mapToScene(viewportGeometry.bottomRight()));

        QRectF itemRect = sceneBoundingRect();
        if (!nestedItemsInternalRect.isValid() || !itemRect.isValid()) {
            itemRect = mappedViewportGeometry;

            if (nestedItemsInternalRect.isValid()) {
                itemRect.moveCenter(nestedItemsInternalRect.center());
                itemRect |= nestedItemsInternalRect.marginsAdded(kRootMargins);
            }
        } else {
            itemRect |= nestedItemsInternalRect.marginsAdded(kRootMargins);
        }

        if (setGeometry(itemRect))
            mergeGeometry();
    }

    AADLFunctionTypeGraphicsItem::rebuildLayout();
}

void AADLFunctionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(isSelected() ? selectedPen() : m_pen);
    painter->setBrush(brush());

    const QRectF br = boundingRect().adjusted(kOffset, kOffset, -kOffset, -kOffset);
    if (isRootItem())
        painter->drawRect(br);
    else
        painter->drawRoundedRect(br, kRadius, kRadius);

    if (!isRootItem() && entity() && entity()->hasNestedChildren()) {
        QRectF iconRect { QPointF(0, 0), m_svgRenderer->defaultSize() };
        iconRect.moveTopRight(br.adjusted(kRadius, kRadius, -kRadius, -kRadius).topRight());
        m_svgRenderer->render(painter, iconRect);

        drawNestedView(painter);
    }

    painter->restore();
}

void AADLFunctionGraphicsItem::onManualResizeProgress(
        shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to)
{
    AADLFunctionTypeGraphicsItem::onManualResizeProgress(grip, from, to);
    layoutConnectionsOnResize(AADLConnectionGraphicsItem::CollisionsPolicy::Ignore);
}

void AADLFunctionGraphicsItem::onManualResizeFinish(
        shared::ui::GripPoint *, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt)
        return;

    if (allowGeometryChange(pressedAt, releasedAt)) {
        layoutConnectionsOnResize(AADLConnectionGraphicsItem::CollisionsPolicy::PartialRebuild);
        updateEntity();
    } else { // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
        layoutConnectionsOnResize(AADLConnectionGraphicsItem::CollisionsPolicy::Ignore);
    }
}

void AADLFunctionGraphicsItem::onManualMoveProgress(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to)
{
    if (isRootItem())
        return;

    AADLFunctionTypeGraphicsItem::onManualMoveProgress(grip, from, to);
    layoutConnectionsOnMove(AADLConnectionGraphicsItem::CollisionsPolicy::Ignore);
}

void AADLFunctionGraphicsItem::onManualMoveFinish(
        shared::ui::GripPoint *, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (isRootItem())
        return;

    if (pressedAt == releasedAt)
        return;

    if (allowGeometryChange(pressedAt, releasedAt)) {
        layoutConnectionsOnMove(AADLConnectionGraphicsItem::CollisionsPolicy::PartialRebuild);
        updateEntity();
    } else { // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
        layoutConnectionsOnMove(AADLConnectionGraphicsItem::CollisionsPolicy::Ignore);
    }
}

static inline void drawItems(
        const QRectF &boundingRect, const QList<QRectF> &existingRects, int count, const qreal sf, QPainter *painter)
{
    QRectF childRect { QPointF(), DefaultGraphicsItemSize * sf };
    const qreal yOffset = sf * (kContentMargins.top() + kContentMargins.bottom());
    const qreal xOffset = sf * (kContentMargins.left() + kContentMargins.right());
    const int column = boundingRect.width() / (childRect.width() + xOffset);
    const int row = boundingRect.height() / (childRect.height() + yOffset);
    if (!count || !column || !row) {
        return;
    }
    for (int idx = 0; idx < count && idx < column * row;) {
        const int currentRow = idx / column;
        const int currentColumn = idx % column;
        childRect.moveTopLeft(boundingRect.topLeft()
                + QPointF((childRect.width() + xOffset) * currentColumn, (childRect.height() + yOffset) * currentRow));
        auto it = std::find_if(existingRects.cbegin(), existingRects.cend(),
                [childRect](const QRectF &r) { return r.intersects(childRect); });
        if (it == existingRects.cend()) {
            painter->drawRect(childRect);
            ++idx;
        }
    }
};

void AADLFunctionGraphicsItem::drawNestedView(QPainter *painter)
{
    if (!entity()->hasNestedChildren()) {
        return;
    }

    const QRectF br = boundingRect();

    const shared::ColorHandler ch =
            shared::ColorManager::instance()->colorsForItem(shared::ColorManager::FunctionScale);
    painter->setBrush(ch.brush());
    painter->setPen(QPen(ch.penColor(), ch.penWidth()));

    QRectF nestedRect;
    const QVector<ivm::AADLObject *> childEntities = entity()->children();
    static const ivm::meta::Props::Token token = ivm::meta::Props::Token::InnerCoordinates;
    QList<shared::Id> itemsCountWithoutGeometry;
    QHash<shared::Id, QRectF> existingRects;
    QHash<shared::Id, QPolygonF> existingPolygons;
    struct ConnectionData {
        QPointF outerMappedScenePos;
        QPointF innerScenePos;
        shared::Id innerFunctionId;
    };
    QList<ConnectionData> parentConnections;
    for (const ivm::AADLObject *child : childEntities) {
        const QString strCoordinates = child->prop(ivm::meta::Props::token(token)).toString();
        if (child->aadlType() == ivm::AADLObject::Type::Function
                || child->aadlType() == ivm::AADLObject::Type::FunctionType
                || child->aadlType() == ivm::AADLObject::Type::Comment) {
            const QRectF itemSceneRect = ive::rect(ivm::AADLObject::coordinatesFromString(strCoordinates));
            if (itemSceneRect.isValid()) {
                nestedRect |= itemSceneRect;
                existingRects.insert(child->id(), itemSceneRect);
            } else {
                itemsCountWithoutGeometry.append(child->id());
            }
        } else if (auto connection = qobject_cast<const ivm::AADLConnection *>(child)) {
            if (connection->source()->id() != entity()->id() && connection->target()->id() != entity()->id()) {
                const QPolygonF itemScenePoints = ive::polygon(ivm::AADLObject::coordinatesFromString(strCoordinates));
                if (!itemScenePoints.isEmpty()) {
                    existingPolygons.insert(child->id(), itemScenePoints);
                }
            } else {
                ivm::AADLObject *outerIface = connection->source()->id() == entity()->id()
                        ? connection->sourceInterface()
                        : connection->target()->id() == entity()->id() ? connection->targetInterface() : nullptr;

                ivm::AADLObject *innerIface = connection->source()->id() == entity()->id()
                        ? connection->targetInterface()
                        : connection->target()->id() == entity()->id() ? connection->sourceInterface() : nullptr;

                if (!outerIface || !innerIface) {
                    continue;
                }

                const auto items = childItems();
                auto it = std::find_if(
                        items.cbegin(), items.cend(), [ifaceId = outerIface->id()](const QGraphicsItem *item) {
                            if (item->type() != AADLInterfaceGraphicsItem::Type) {
                                return false;
                            } else if (auto iface = qgraphicsitem_cast<const AADLInterfaceGraphicsItem *>(item)) {
                                return iface->entity()->id() == ifaceId;
                            } else {
                                return false;
                            }
                        });
                if (it == items.cend()) {
                    continue;
                }

                const QString ifaceStrCoordinates = innerIface->prop(ivm::meta::Props::token(token)).toString();
                const QPointF innerIfacePos = ive::pos(ivm::AADLObject::coordinatesFromString(ifaceStrCoordinates));
                ConnectionData cd { (*it)->scenePos(), innerIfacePos, innerIface->parentObject()->id() };
                /// TODO: generate path between outer and inner ifaces
                /// templorary using straight line to present it
                parentConnections << cd;
            }
        }
    }
    while (!itemsCountWithoutGeometry.isEmpty()) {
        const shared::Id id = itemsCountWithoutGeometry.takeLast();
        QRectF itemRect { QPointF(), DefaultGraphicsItemSize };
        findGeometryForRect(itemRect, nestedRect, existingRects.values());
        existingRects.insert(id, itemRect);
    }

    if (!nestedRect.isValid()) {
        auto view = scene()->views().value(0);
        if (!view)
            return;

        const int count = std::count_if(childEntities.cbegin(), childEntities.cend(), [](const ivm::AADLObject *child) {
            return child->aadlType() == ivm::AADLObject::Type::Function
                    || child->aadlType() == ivm::AADLObject::Type::FunctionType
                    || child->aadlType() == ivm::AADLObject::Type::Comment;
        });

        const QRect viewportGeometry = view->viewport()->geometry().marginsRemoved(kContentMargins.toMargins());
        const QRectF mappedViewportGeometry =
                QRectF(view->mapToScene(viewportGeometry.topLeft()), view->mapToScene(viewportGeometry.bottomRight()));

        const qreal sf =
                qMin(br.width() / mappedViewportGeometry.width(), br.height() / mappedViewportGeometry.height());
        drawItems(br, {}, count, sf, painter);
    } else {
        const QRectF contentRect { nestedRect.marginsAdded(kRootMargins) };
        const qreal sf = qMin(br.width() / contentRect.width(), br.height() / contentRect.height());
        const QTransform transform = QTransform::fromScale(sf, sf).translate(-contentRect.x(), -contentRect.y());

        QList<QRectF> mappedRects;
        for (const QRectF &r : qAsConst(existingRects)) {
            const QRectF mappedRect = transform.mapRect(r);
            mappedRects << mappedRect;
            painter->drawRect(mappedRect);
        }
        for (const QPolygonF &p : qAsConst(existingPolygons)) {
            painter->drawPolyline(transform.map(p));
        }
        for (const ConnectionData &connectionData : qAsConst(parentConnections)) {
            const QRectF innerRect = transform.mapRect(existingRects.value(connectionData.innerFunctionId));
            const QPointF innerPos = transform.map(connectionData.innerScenePos);
            const QRectF outerRect = mapRectFromScene(sceneBoundingRect());
            const QPointF outerPos = mapFromScene(connectionData.outerMappedScenePos);
            painter->drawPolyline(ive::createConnectionPath(mappedRects, outerPos, outerRect, innerPos, innerRect));
        }
    }
}

void AADLFunctionGraphicsItem::layoutConnectionsOnResize(AADLConnectionGraphicsItem::CollisionsPolicy collisionsPolicy)
{
    /// Changing inner and outer connections bound to current function item
    for (const auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(item)) {
            AADLConnectionGraphicsItem::layoutInterfaceConnections(
                    iface, AADLConnectionGraphicsItem::LayoutPolicy::Scaling, collisionsPolicy, true);
        } else if (auto connection = qgraphicsitem_cast<AADLConnectionGraphicsItem *>(item)) {
            if (connection->sourceItem() != this && connection->targetItem() != this)
                connection->layout();
        }
    }
}

void AADLFunctionGraphicsItem::layoutConnectionsOnMove(AADLConnectionGraphicsItem::CollisionsPolicy collisionsPolicy)
{
    /// Changing outer connections only cause inner stay unchanged as children of current item
    for (const auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(item)) {
            AADLConnectionGraphicsItem::layoutInterfaceConnections(
                    iface, AADLConnectionGraphicsItem::LayoutPolicy::Scaling, collisionsPolicy, false);
        }
    }
}

void AADLFunctionGraphicsItem::prepareTextRect(QRectF &textRect, const QRectF &targetTextRect) const
{
    textRect.moveCenter(targetTextRect.center());
}

shared::ColorManager::HandledColors AADLFunctionGraphicsItem::handledColorType() const
{
    if (isRootItem())
        return shared::ColorManager::HandledColors::FunctionRoot;

    const QRectF nestedRect = nestedItemsSceneBoundingRect();
    if (nestedRect.isValid() && !sceneBoundingRect().contains(nestedRect.marginsAdded(kContentMargins)))
        return shared::ColorManager::HandledColors::FunctionPartial;

    return shared::ColorManager::HandledColors::FunctionRegular;
}

void AADLFunctionGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen p = h.pen();
    QBrush b = h.brush();

    if (auto parentFunction = qgraphicsitem_cast<AADLFunctionGraphicsItem *>(parentItem())) {
        if (!parentFunction->entity()->hasProperty(QLatin1String("color"))
                && !entity()->hasProperty(QLatin1String("color"))
                && parentFunction->handledColorType()
                        == shared::ColorManager::HandledColors::FunctionRegular) { // [Hm...]
            b.setColor(parentFunction->brush().color().darker(125));
            p.setColor(parentFunction->pen().color().darker(125));
        }
    }

    if (pen() == p && brush() == b)
        return;

    setPen(p);
    setBrush(b);

    // During undo, a child can be updated before its parent,
    // so on the step marked as [Hm...] above, the parent is still of type FunctionPartial and not the
    // FunctionRegular. Thus, the child gets the "default" colour, instead of "parent.darker". For now, I can't see
    // a better way but just to update children colours manually:
    for (auto child : childItems())
        if (child->type() == AADLFunctionGraphicsItem::Type)
            if (auto nestedFunction = qobject_cast<AADLFunctionGraphicsItem *>(child->toGraphicsObject()))
                nestedFunction->applyColorScheme();

    update();
}

QString AADLFunctionGraphicsItem::prepareTooltip() const
{
    const QString title = uniteNames<ivm::AADLFunctionType *>({ entity() }, QString());
    const QString prototype =
            uniteNames<const ivm::AADLFunctionType *>({ entity()->instanceOf() }, tr("Instance of: "));
    const QString ris = uniteNames<ivm::AADLIface *>(entity()->ris(), tr("RI: "));
    const QString pis = uniteNames<ivm::AADLIface *>(entity()->pis(), tr("PI: "));

    return joinNonEmpty({ title, prototype, ris, pis }, QStringLiteral("<br>"));
}
}
