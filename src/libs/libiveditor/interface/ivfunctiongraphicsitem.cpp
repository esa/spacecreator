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

#include "ivfunctiongraphicsitem.h"

#include "ivcommentgraphicsitem.h"
#include "ivconnection.h"
#include "ivconnectiongraphicsitem.h"
#include "ivfunction.h"
#include "ivfunctionnamegraphicsitem.h"
#include "ivinterfacegraphicsitem.h"
#include "ivmodel.h"
#include "baseitems/common/ivutils.h"
#include "colors/colormanager.h"
#include "graphicsitemhelpers.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QGraphicsView>
#include <QPainter>
#include <QSvgRenderer>
#include <QTimer>
#include <QtDebug>
#include <cmath>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
#include <QScopeGuard>
#else
template<typename F>
class QScopeGuard;
template<typename F>
QScopeGuard<F> qScopeGuard(F f);

template<typename F>
class QScopeGuard
{
public:
    QScopeGuard(QScopeGuard &&other) Q_DECL_NOEXCEPT : m_func(std::move(other.m_func)), m_invoke(other.m_invoke)
    {
        other.dismiss();
    }

    ~QScopeGuard()
    {
        if (m_invoke)
            m_func();
    }

    void dismiss() Q_DECL_NOEXCEPT { m_invoke = false; }

private:
    explicit QScopeGuard(F f) Q_DECL_NOEXCEPT : m_func(std::move(f)) { }

    Q_DISABLE_COPY(QScopeGuard)

    F m_func;
    bool m_invoke = true;
    friend QScopeGuard qScopeGuard<F>(F);
};

template<typename F>
QScopeGuard<F> qScopeGuard(F f)
{
    return QScopeGuard<F>(std::move(f));
}
#endif

static const qreal kBorderWidth = 2.0;
static const qreal kRadius = 10.0;
static const qreal kOffset = kBorderWidth / 2.0;

namespace ive {

QPointer<QSvgRenderer> IVFunctionGraphicsItem::m_svgRenderer = {};

IVFunctionGraphicsItem::IVFunctionGraphicsItem(ivm::IVFunction *entity, QGraphicsItem *parent)
    : IVFunctionTypeGraphicsItem(entity, parent)
{
    m_textItem->setVisible(!isRootItem());
    m_textItem->setTextAlignment(Qt::AlignCenter);

    if (!m_svgRenderer) // TODO: change icon
        m_svgRenderer = new QSvgRenderer(QLatin1String(":/tab_interface/toolbar/icns/change_root.svg"));
}

void IVFunctionGraphicsItem::init()
{
    IVFunctionTypeGraphicsItem::init();
    connect(entity(), &ivm::IVFunction::childAdded, this, [this]() { update(); });
    connect(entity(), &ivm::IVFunction::childRemoved, this, [this]() { update(); });
}

ivm::IVFunction *IVFunctionGraphicsItem::entity() const
{
    return qobject_cast<ivm::IVFunction *>(m_dataObject);
}

QPainterPath IVFunctionGraphicsItem::shape() const
{
    QPainterPath pp;
    const QRectF br = boundingRect().adjusted(kOffset, kOffset, -kOffset, -kOffset);
    if (isRootItem())
        pp.addRect(br);
    else
        pp.addRoundedRect(br, kRadius, kRadius);
    return pp;
}

void IVFunctionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void IVFunctionGraphicsItem::onManualResizeProgress(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt)
        return;

    const QRectF rect = transformedRect(grip, pressedAt, releasedAt);
    if (gi::isBounded(this, rect)) {
        IVFunctionTypeGraphicsItem::onManualResizeProgress(grip, pressedAt, releasedAt);
        layoutConnectionsOnResize(IVConnectionGraphicsItem::CollisionsPolicy::Ignore);
    }
}

void IVFunctionGraphicsItem::onManualMoveProgress(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (isRootItem())
        return;

    if (pressedAt == releasedAt)
        return;

    const QRectF rect = transformedRect(grip, pressedAt, releasedAt);
    if (gi::isBounded(this, rect)) {
        IVFunctionTypeGraphicsItem::onManualMoveProgress(grip, pressedAt, releasedAt);
        layoutConnectionsOnMove(IVConnectionGraphicsItem::CollisionsPolicy::Ignore);
    }
}

void IVFunctionGraphicsItem::onManualResizeFinish(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(grip)

    if (pressedAt == releasedAt)
        return;

    if (gi::isBounded(this, sceneBoundingRect()) && !gi::isCollided(this, sceneBoundingRect())) {
        layoutConnectionsOnResize(IVConnectionGraphicsItem::CollisionsPolicy::PartialRebuild);
        updateEntity();
    } else { // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
        layoutConnectionsOnResize(IVConnectionGraphicsItem::CollisionsPolicy::Ignore);
    }
}

void IVFunctionGraphicsItem::onManualMoveFinish(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(grip)

    if (isRootItem())
        return;

    if (pressedAt == releasedAt)
        return;

    if (gi::isBounded(this, sceneBoundingRect()) && !gi::isCollided(this, sceneBoundingRect())) {
        layoutConnectionsOnMove(IVConnectionGraphicsItem::CollisionsPolicy::PartialRebuild);
        updateEntity();
    } else { // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
        layoutConnectionsOnMove(IVConnectionGraphicsItem::CollisionsPolicy::Ignore);
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

void IVFunctionGraphicsItem::drawNestedView(QPainter *painter)
{
    if (!entity()->hasNestedChildren()) {
        return;
    }

    painter->save();
    auto cleanup = qScopeGuard([painter] { painter->restore(); });

    const QRectF br = boundingRect();

    const shared::ColorHandler ch =
            shared::ColorManager::instance()->colorsForItem(shared::ColorManager::FunctionScale);
    painter->setBrush(ch.brush());
    painter->setPen(QPen(ch.penColor(), ch.penWidth()));

    QRectF nestedRect;
    const QVector<ivm::IVObject *> childEntities = entity()->children();
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
    for (const ivm::IVObject *child : childEntities) {
        const QString strCoordinates = child->prop(ivm::meta::Props::token(token)).toString();
        if (child->type() == ivm::IVObject::Type::Function
                || child->type() == ivm::IVObject::Type::FunctionType
                || child->type() == ivm::IVObject::Type::Comment) {
            const QRectF itemSceneRect = ive::rect(ivm::IVObject::coordinatesFromString(strCoordinates));
            if (itemSceneRect.isValid()) {
                nestedRect |= itemSceneRect;
                existingRects.insert(child->id(), itemSceneRect);
            } else {
                itemsCountWithoutGeometry.append(child->id());
            }
        } else if (auto connection = qobject_cast<const ivm::IVConnection *>(child)) {
            if (connection->source()->id() != entity()->id() && connection->target()->id() != entity()->id()) {
                const QPolygonF itemScenePoints = ive::polygon(ivm::IVObject::coordinatesFromString(strCoordinates));
                if (!itemScenePoints.isEmpty()) {
                    existingPolygons.insert(child->id(), itemScenePoints);
                }
            } else {
                ivm::IVObject *outerIface = connection->source()->id() == entity()->id()
                        ? connection->sourceInterface()
                        : connection->target()->id() == entity()->id() ? connection->targetInterface() : nullptr;

                ivm::IVObject *innerIface = connection->source()->id() == entity()->id()
                        ? connection->targetInterface()
                        : connection->target()->id() == entity()->id() ? connection->sourceInterface() : nullptr;

                if (!outerIface || !innerIface) {
                    continue;
                }

                const auto items = childItems();
                auto it = std::find_if(
                        items.cbegin(), items.cend(), [ifaceId = outerIface->id()](const QGraphicsItem *item) {
                            if (item->type() != IVInterfaceGraphicsItem::Type) {
                                return false;
                            } else if (auto iface = qgraphicsitem_cast<const IVInterfaceGraphicsItem *>(item)) {
                                return iface->entity()->id() == ifaceId;
                            } else {
                                return false;
                            }
                        });
                if (it == items.cend()) {
                    continue;
                }

                QPointF innerIfacePos;
                if (innerIface->hasProperty(ivm::meta::Props::token(token))) {
                    const QString ifaceStrCoordinates = innerIface->prop(ivm::meta::Props::token(token)).toString();
                    innerIfacePos = ive::pos(ivm::IVObject::coordinatesFromString(ifaceStrCoordinates));
                }
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

        const int count = std::count_if(childEntities.cbegin(), childEntities.cend(), [](const ivm::IVObject *child) {
            return child->type() == ivm::IVObject::Type::Function
                    || child->type() == ivm::IVObject::Type::FunctionType
                    || child->type() == ivm::IVObject::Type::Comment;
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
        QFont painterFont = painter->font();
        painterFont.setItalic(true);
        painterFont.setPointSize(painterFont.pointSize() - 1);
        const QFontMetricsF fm(painterFont);
        for (auto it = existingRects.cbegin(); it != existingRects.cend(); ++it) {
            const QRectF r = it.value();
            const QRectF mappedRect = transform.mapRect(r);
            mappedRects << mappedRect;
            painter->drawRect(mappedRect);

            const QString text = entity()->model()->getObject(it.key())->titleUI();
            const QRectF textRect = fm.boundingRect(
                    mappedRect.adjusted(4, 4, -4, -4), Qt::AlignTop | Qt::AlignLeft | Qt::TextDontClip, text);
            if (mappedRect.contains(textRect)) {
                painter->setFont(painterFont);
                painter->drawText(textRect, Qt::AlignTop | Qt::AlignLeft, text);
            }
        }
        for (const QPolygonF &p : qAsConst(existingPolygons)) {
            painter->drawPolyline(transform.map(p));
        }
        for (const ConnectionData &connectionData : qAsConst(parentConnections)) {
            const QRectF innerRect = transform.mapRect(existingRects.value(connectionData.innerFunctionId));
            const QRectF outerRect = mapRectFromScene(sceneBoundingRect());
            const QPointF outerPos = mapFromScene(connectionData.outerMappedScenePos);
            QPointF innerPos;
            if (connectionData.innerScenePos.isNull()) {
                const QPointF ratio { (outerRect.right() - outerPos.x()) / outerRect.width(),
                    (outerRect.bottom() - outerPos.y()) / outerRect.height() };
                const qreal x = innerRect.left() + innerRect.width() * ratio.x();
                const qreal y = innerRect.top() + innerRect.height() * ratio.y();
                const Qt::Alignment side = ive::getNearestSide(outerRect, outerPos);
                innerPos = ive::getSidePosition(innerRect, QPointF(x, y), side);
            } else {
                innerPos = transform.map(connectionData.innerScenePos);
            }
            painter->drawPolyline(ive::createConnectionPath(mappedRects, outerPos, outerRect, innerPos, innerRect));
        }
    }
}

void IVFunctionGraphicsItem::layoutConnectionsOnResize(IVConnectionGraphicsItem::CollisionsPolicy collisionsPolicy)
{
    /// Changing inner and outer connections bound to current function item
    for (const auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<IVInterfaceGraphicsItem *>(item)) {
            IVConnectionGraphicsItem::layoutInterfaceConnections(
                    iface, IVConnectionGraphicsItem::LayoutPolicy::Scaling, collisionsPolicy, true);
        } else if (auto connection = qgraphicsitem_cast<IVConnectionGraphicsItem *>(item)) {
            if (connection->sourceItem() != this && connection->targetItem() != this)
                connection->layout();
        }
    }
}

void IVFunctionGraphicsItem::layoutConnectionsOnMove(IVConnectionGraphicsItem::CollisionsPolicy collisionsPolicy)
{
    /// Changing outer connections only cause inner stay unchanged as children of current item
    for (const auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<IVInterfaceGraphicsItem *>(item)) {
            IVConnectionGraphicsItem::layoutInterfaceConnections(
                    iface, IVConnectionGraphicsItem::LayoutPolicy::Scaling, collisionsPolicy, false);
        }
    }
}

void IVFunctionGraphicsItem::prepareTextRect(QRectF &textRect, const QRectF &targetTextRect) const
{
    textRect.moveCenter(targetTextRect.center());
}

shared::ColorManager::HandledColors IVFunctionGraphicsItem::handledColorType() const
{
    if (isRootItem())
        return shared::ColorManager::HandledColors::FunctionRoot;

    const QRectF nestedRect = nestedItemsSceneBoundingRect();
    if (nestedRect.isValid() && !sceneBoundingRect().contains(nestedRect.marginsAdded(kContentMargins)))
        return shared::ColorManager::HandledColors::FunctionPartial;

    return shared::ColorManager::HandledColors::FunctionRegular;
}

void IVFunctionGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen p = h.pen();
    QBrush b = h.brush();

    if (auto parentFunction = qgraphicsitem_cast<IVFunctionGraphicsItem *>(parentItem())) {
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
        if (child->type() == IVFunctionGraphicsItem::Type)
            if (auto nestedFunction = qobject_cast<IVFunctionGraphicsItem *>(child->toGraphicsObject()))
                nestedFunction->applyColorScheme();

    update();
}

QString IVFunctionGraphicsItem::prepareTooltip() const
{
    const QString title = uniteNames<ivm::IVFunctionType *>({ entity() }, QString());
    const QString prototype =
            uniteNames<const ivm::IVFunctionType *>({ entity()->instanceOf() }, tr("Instance of: "));
    const QString ris = uniteNames<ivm::IVInterface *>(entity()->ris(), tr("RI: "));
    const QString pis = uniteNames<ivm::IVInterface *>(entity()->pis(), tr("PI: "));

    return joinNonEmpty({ title, prototype, ris, pis }, QStringLiteral("<br>"));
}
}
