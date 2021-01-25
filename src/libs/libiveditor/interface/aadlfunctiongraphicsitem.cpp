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
#include "aadlconnectiongraphicsitem.h"
#include "aadlfunctionnamegraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "baseitems/common/aadlutils.h"
#include "colors/colormanager.h"
#include "interface/commands/commandids.h"
#include "interface/commands/commandsfactory.h"

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
static const QList<int> kNestedTypes { ive::AADLFunctionGraphicsItem::Type,
    ive::AADLFunctionTypeGraphicsItem::Type, ive::AADLCommentGraphicsItem::Type };

namespace ive {

QPointer<QSvgRenderer> AADLFunctionGraphicsItem::m_svgRenderer = {};

AADLFunctionGraphicsItem::AADLFunctionGraphicsItem(ivm::AADLObjectFunction *entity, QGraphicsItem *parent)
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
    connect(entity(), &ivm::AADLObjectFunction::childAdded, this, [this]() { update(); });
    connect(entity(), &ivm::AADLObjectFunction::childRemoved, this, [this]() { update(); });
}

ivm::AADLObjectFunction *AADLFunctionGraphicsItem::entity() const
{
    return qobject_cast<ivm::AADLObjectFunction *>(aadlObject());
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

        if (!sceneBoundingRect().contains(nestedItemsSceneBoundingRect())) {
            drawInnerFunctions(painter);
        }
    }

    painter->restore();
}

void AADLFunctionGraphicsItem::onManualResizeProgress(
        shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to)
{
    AADLFunctionTypeGraphicsItem::onManualResizeProgress(grip, from, to);
    layoutConnectionsOnResize(ConnectionLayoutPolicy::IgnoreCollisions);
}

void AADLFunctionGraphicsItem::onManualResizeFinish(
        shared::ui::GripPoint *, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt)
        return;

    if (allowGeometryChange(pressedAt, releasedAt)) {
        layoutConnectionsOnResize(ConnectionLayoutPolicy::PartialRebuildOnCollision);
        updateEntity();
    } else { // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
        layoutConnectionsOnResize(ConnectionLayoutPolicy::IgnoreCollisions);
    }
}

void AADLFunctionGraphicsItem::onManualMoveProgress(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to)
{
    if (isRootItem())
        return;

    AADLFunctionTypeGraphicsItem::onManualMoveProgress(grip, from, to);
    layoutConnectionsOnMove(ConnectionLayoutPolicy::IgnoreCollisions);
}

void AADLFunctionGraphicsItem::onManualMoveFinish(
        shared::ui::GripPoint *, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (isRootItem())
        return;

    if (pressedAt == releasedAt)
        return;

    if (allowGeometryChange(pressedAt, releasedAt)) {
        layoutConnectionsOnMove(ConnectionLayoutPolicy::PartialRebuildOnCollision);
        updateEntity();
    } else { // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
        layoutConnectionsOnMove(ConnectionLayoutPolicy::IgnoreCollisions);
    }
}

void AADLFunctionGraphicsItem::layoutConnectionsOnResize(ConnectionLayoutPolicy layoutPolicy)
{
    /// Changing inner and outer connections bound to current function item
    for (auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(item)) {
            layoutConnection(iface, layoutPolicy, true);
        } else if (auto connection = qgraphicsitem_cast<AADLConnectionGraphicsItem *>(item)) {
            if (connection->sourceItem() != this && connection->targetItem() != this)
                connection->layout();
        }
    }
}

void AADLFunctionGraphicsItem::layoutConnection(
        AADLInterfaceGraphicsItem *ifaceItem, ConnectionLayoutPolicy layoutPolicy, bool includingNested)
{
    for (AADLConnectionGraphicsItem *connection : ifaceItem->connectionItems()) {
        Q_ASSERT(connection->startItem() && connection->endItem());
        if (includingNested || connection->parentItem() != this) {
            if (ConnectionLayoutPolicy::IgnoreCollisions == layoutPolicy) {
                connection->updateEndPoint(ifaceItem);
            } else if (ConnectionLayoutPolicy::PartialRebuildOnCollision == layoutPolicy) {
                connection->updateEndPoint(ifaceItem);
                connection->updateOverlappedSections();
            } else {
                const auto collidingItems = scene()->collidingItems(connection);
                if (std::any_of(
                            collidingItems.begin(), collidingItems.end(), [this](const QGraphicsItem *collidingItem) {
                                return kNestedTypes.contains(collidingItem->type()) && collidingItem != this;
                            })) {
                    connection->layout();
                }
            }
        }
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

void AADLFunctionGraphicsItem::drawInnerFunctions(QPainter *painter)
{
    if (!entity()->hasNestedChildren()) {
        return;
    }

    const QRectF br = boundingRect();

    const ColorHandler ch = ColorManager::instance()->colorsForItem(ColorManager::FunctionScale);
    painter->setBrush(ch.brush());
    painter->setPen(QPen(ch.penColor(), ch.penWidth()));

    QRectF nestedRect;
    const QVector<ivm::AADLObject *> childEntities = entity()->children();
    static const ivm::meta::Props::Token token = ivm::meta::Props::Token::InnerCoordinates;
    int itemsCountWithoutGeometry = 0;
    QList<QRectF> existingRects;
    QList<QPolygonF> existingPolygons;
    for (const ivm::AADLObject *child : childEntities) {
        const QString strCoordinates = child->prop(ivm::meta::Props::token(token)).toString();
        if (child->aadlType() == ivm::AADLObject::Type::Function
                || child->aadlType() == ivm::AADLObject::Type::FunctionType
                || child->aadlType() == ivm::AADLObject::Type::Comment) {
            const QRectF itemSceneRect = ive::rect(ivm::AADLObject::coordinatesFromString(strCoordinates));
            if (itemSceneRect.isValid()) {
                nestedRect |= itemSceneRect;
                existingRects << itemSceneRect;
            } else {
                itemsCountWithoutGeometry += 1;
            }
        } else if (auto connection = qobject_cast<const ivm::AADLObjectConnection *>(child)) {
            if (connection->source()->id() != entity()->id() && connection->target()->id() != entity()->id()) {
                const QPolygonF itemScenePoints =
                        ive::polygon(ivm::AADLObject::coordinatesFromString(strCoordinates));
                if (!itemScenePoints.isEmpty()) {
                    existingPolygons << itemScenePoints;
                }
            }
        }
    }
    while (itemsCountWithoutGeometry-- > 0) {
        QRectF itemRect { QPointF(), DefaultGraphicsItemSize };
        findGeometryForRect(itemRect, nestedRect, existingRects);
        existingRects << itemRect;
    }

    if (!nestedRect.isValid()) {
        auto view = scene()->views().value(0);
        if (!view)
            return;

        const int count =
                std::count_if(childEntities.cbegin(), childEntities.cend(), [](const ivm::AADLObject *child) {
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

        for (const QRectF &r : qAsConst(existingRects)) {
            painter->drawRect(transform.mapRect(r));
        }
        for (const QPolygonF &p : qAsConst(existingPolygons)) {
            painter->drawPolyline(transform.map(p));
        }
    }
}

void AADLFunctionGraphicsItem::layoutConnectionsOnMove(ConnectionLayoutPolicy layoutPolicy)
{
    /// Changing outer connections only cause inner stay unchanged as children of current item
    for (auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(item)) {
            layoutConnection(iface, layoutPolicy, false);
        }
    }
}

void AADLFunctionGraphicsItem::prepareTextRect(QRectF &textRect, const QRectF &targetTextRect) const
{
    textRect.moveCenter(targetTextRect.center());
}

ColorManager::HandledColors AADLFunctionGraphicsItem::handledColorType() const
{
    if (isRootItem())
        return ColorManager::HandledColors::FunctionRoot;

    const QRectF nestedRect = nestedItemsSceneBoundingRect();
    if (nestedRect.isValid() && !sceneBoundingRect().contains(nestedRect.marginsAdded(kContentMargins)))
        return ColorManager::HandledColors::FunctionPartial;

    return ColorManager::HandledColors::FunctionRegular;
}

void AADLFunctionGraphicsItem::applyColorScheme()
{
    const ColorHandler &h = colorHandler();
    QPen p = h.pen();
    QBrush b = h.brush();

    if (auto parentFunction = qgraphicsitem_cast<AADLFunctionGraphicsItem *>(parentItem())) {
        if (!parentFunction->entity()->props().contains("color") && !entity()->props().contains("color")
                && parentFunction->handledColorType() == ColorManager::HandledColors::FunctionRegular) { // [Hm...]
            b.setColor(parentFunction->brush().color().darker(125));
            p.setColor(parentFunction->pen().color().darker(125));
        }
    }

    if (pen() == p && brush() == b)
        return;

    setPen(p);
    setBrush(b);

    // During undo, a child can be updated before its parent,
    // so on the step marked as [Hm...] above, the parent is still of type FunctionPartial and not the FunctionRegular.
    // Thus, the child gets the "default" colour, instead of "parent.darker".
    // For now, I can't see a better way but just to update children colours manually:
    for (auto child : childItems())
        if (child->type() == AADLFunctionGraphicsItem::Type)
            if (auto nestedFunction = qobject_cast<AADLFunctionGraphicsItem *>(child->toGraphicsObject()))
                nestedFunction->applyColorScheme();

    update();
}

QString AADLFunctionGraphicsItem::prepareTooltip() const
{
    const QString title = uniteNames<ivm::AADLObjectFunctionType *>({ entity() }, QString());
    const QString prototype =
            uniteNames<const ivm::AADLObjectFunctionType *>({ entity()->instanceOf() }, tr("Instance of: "));
    const QString ris = uniteNames<ivm::AADLObjectIface *>(entity()->ris(), tr("RI: "));
    const QString pis = uniteNames<ivm::AADLObjectIface *>(entity()->pis(), tr("PI: "));

    return joinNonEmpty({ title, prototype, ris, pis }, QStringLiteral("<br>"));
}

}
