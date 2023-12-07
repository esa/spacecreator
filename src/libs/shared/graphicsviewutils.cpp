#include "graphicsviewutils.h"

#include "positionlookuphelper.h"
#include "topohelper/geometry.h"
#include "ui/graphicsviewbase.h"
#include "ui/verectgraphicsitem.h"

#include <QFontMetricsF>
#include <QPainter>
#include <QPainterPath>
#include <QTextLayout>
#include <QtDebug>
#include <QtMath>

namespace shared {
namespace graphicsviewutils {

QPainterPath lineShape(const QLineF &line, qreal span)
{
    QPainterPath result;

    QLineF normalSpan(line.normalVector());
    normalSpan.setLength(span);

    const QPointF delta = line.p1() - normalSpan.p2();
    const QLineF dec(line.translated(delta));
    const QLineF inc(line.translated(-delta));

    result.moveTo(dec.p1());
    result.lineTo(dec.p2());
    result.lineTo(inc.p2());
    result.lineTo(inc.p1());

    return result;
}

/*!
   Constructs a rectangular octagon. \p rect gives the size and \p cut is the length (in x and y) of the cut.
 */
QPainterPath edgeCuttedRectShape(const QRectF &rect, qreal cut)
{
    QPainterPath result;

    const QPointF hcut(cut, 0.);
    const QPointF vcut(0., cut);

    result.moveTo(rect.topLeft() + hcut);
    result.lineTo(rect.topRight() - hcut);
    result.lineTo(rect.topRight() + vcut);
    result.lineTo(rect.bottomRight() - vcut);
    result.lineTo(rect.bottomRight() - hcut);
    result.lineTo(rect.bottomLeft() + hcut);
    result.lineTo(rect.bottomLeft() - vcut);
    result.lineTo(rect.topLeft() + vcut);
    result.lineTo(rect.topLeft() + hcut);

    return result;
}

/*!
 * \brief Iterates over all \a scene items in the specified \a pos and returns the first one which QGraphicsItem::type
 * found in \a acceptableTypes list.
 */
QGraphicsItem *nearestItem(const QGraphicsScene *scene, const QPointF &pos, const QList<int> &acceptableTypes)
{
    if (scene)
        for (QGraphicsItem *item : scene->items(pos)) {
            if (acceptableTypes.contains(item->type()))
                return item;
        }
    return nullptr;
}

/*!
 * \brief Iterates over all \a scene items in the specified \a area and returns the first one which
 * QGraphicsItem::type found in \a acceptableTypes list.
 */
QGraphicsItem *nearestItem(const QGraphicsScene *scene, const QRectF &area, const QList<int> &acceptableTypes)
{
    const QList<QGraphicsItem *> areaItems = scene->items(area);
    if (areaItems.isEmpty())
        return nullptr;

    const QPointF point = area.center();
    if (areaItems.size() == 1) {
        auto item = areaItems.value(0);
        if (item && item->contains(point) && acceptableTypes.contains(item->type()))
            return item;
    }

    qreal distance = std::numeric_limits<int>::max();
    QGraphicsItem *nearestToCenter = nullptr;
    for (QGraphicsItem *item : areaItems) {
        if (!acceptableTypes.isEmpty() && !acceptableTypes.contains(item->type()))
            continue;

        const QRectF itemRect = item->sceneBoundingRect();
        qreal itemDistance = qAbs(itemRect.right() - point.x());
        itemDistance = std::min(itemDistance, qAbs(itemRect.left() - point.x()));
        itemDistance = std::min(itemDistance, qAbs(itemRect.top() - point.y()));
        itemDistance = std::min(itemDistance, qAbs(itemRect.bottom() - point.y()));
        if (itemDistance < distance) {
            nearestToCenter = item;
            distance = itemDistance;
        }
    }
    return nearestToCenter;
}

/*!
 * \brief Iterates over all \a scene items in the square with center in \a center and the size of \a offset and returns
 * the first one which QGraphicsItem::type found in \a acceptableTypes list.
 */
QGraphicsItem *nearestItem(
        const QGraphicsScene *scene, const QPointF &center, qreal offset, const QList<int> &acceptableTypes)
{
    const QRectF area { center - QPointF(offset / 2, offset / 2), center + QPointF(offset / 2, offset / 2) };
    return nearestItem(scene, area, acceptableTypes);
}

/*!
 * Gets geometries of rectangular items with the same level
 * as \a item in scene coordinates with \a margins added
 */
QList<QRectF> siblingItemsRects(const QGraphicsItem *item)
{
    if (!item) {
        return {};
    }

    const QGraphicsScene *scene = item->scene();
    if (!scene) {
        return {};
    }

    QList<QRectF> rects;
    const QList<QGraphicsItem *> siblingItems = item->parentItem() ? item->parentItem()->childItems() : scene->items();
    for (auto siblingItem : siblingItems) {
        if (item != siblingItem && qobject_cast<ui::VERectGraphicsItem *>(siblingItem->toGraphicsObject()) != nullptr
                && siblingItem->parentItem() == item->parentItem()) {
            rects.append(siblingItem->sceneBoundingRect());
        }
    }
    return rects;
}

bool isCollided(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect)
{
    if (!upcomingItem || !upcomingItemRect.isValid()) {
        return false;
    }

    const QList<QRectF> siblingRects = siblingItemsRects(upcomingItem);
    return topohelp::geom::isCollided(siblingRects, upcomingItemRect.marginsAdded(topohelp::kContentMargins));
}

bool isBounded(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect)
{
    if (!upcomingItem || !upcomingItemRect.isValid()) {
        return false;
    }

    using topohelp::kContentMargins;

    if (auto rectItem = qobject_cast<const ui::VERectGraphicsItem *>(upcomingItem->toGraphicsObject())) {
        // Is this item contained by its parent
        if (auto parentObj = rectItem->parentItem()) {
            const QRectF outerRect = parentObj->sceneBoundingRect().marginsRemoved(kContentMargins);
            return topohelp::geom::isRectBounded(outerRect, upcomingItemRect);
        }
        // Are all my children contained in me
        return topohelp::geom::isRectBounded(
                upcomingItemRect.marginsRemoved(kContentMargins), rectItem->nestedItemsSceneBoundingRect());
    }
    return true;
}

void drawText(QPainter *painter, const QRectF &rect, const QString &text, qreal margin)
{
    qreal y = margin;
    const qreal lineWidth = rect.width() - 2 * margin;
    const qreal maxY = rect.height() - margin;
    const QFontMetricsF fm(painter->font());
    bool complete = false;
    for (const auto &line : text.split(QLatin1Char('\n'))) {
        QTextLayout textLayout(line);
        textLayout.setFont(painter->font());
        textLayout.beginLayout();
        while (true) {
            QTextLine textLine = textLayout.createLine();
            if (!textLine.isValid()) {
                break;
            }

            textLine.setLineWidth(lineWidth);
            if (maxY < y + textLine.height()) {
                const QString lastLine = line.mid(textLine.textStart());
                const QString elidedLastLine = fm.elidedText(lastLine, Qt::ElideRight, lineWidth);
                painter->drawText(QPointF(margin, y + fm.ascent()) + rect.topLeft(), elidedLastLine);
                complete = true;
                break;
            }

            textLine.draw(painter, QPointF(margin, y) + rect.topLeft());
            y += textLine.height();
        }
        textLayout.endLayout();
        if (complete)
            break;
    }
}

void findGeometryForPoint(
        QPointF &ifacePos, const QRectF &boundedRect, const QList<QRectF> &existingRects, const QMarginsF &margins)
{
    using topohelp::kInterfaceBaseLength;
    using namespace topohelp::geom;

    const QRectF kBaseRect = adjustFromPoint(QPointF(0, 0), kInterfaceBaseLength);
    QRectF itemRect = adjustFromPoint(ifacePos, kInterfaceBaseLength + topohelp::kSiblingMinDistance);

    itemRect.moveCenter(ifacePos);
    QRectF intersectedRect;

    if (((ifacePos.isNull() && existingRects.isEmpty())
                || topohelp::geom::isCollided(existingRects, itemRect, &intersectedRect))
            && boundedRect.isValid()) {
        QPainterPath pp;
        pp.addRect(kBaseRect);
        const QList<QPair<Qt::Alignment, QPainterPath>> sidePaths {
            { Qt::AlignLeft, pp },
            { Qt::AlignTop, pp },
            { Qt::AlignRight, pp },
            { Qt::AlignBottom, pp },
        };
        shared::PositionLookupHelper helper(sidePaths, boundedRect, existingRects, ifacePos);
        if (helper.lookup()) {
            ifacePos = helper.mappedOriginPoint();
        }
    }
}

QGraphicsLineItem *horizontalHelper(qreal y, QGraphicsItem *parent)
{
    const QPen linePen(Qt::gray, 1, Qt::DashLine);
    auto helper = new QGraphicsLineItem(parent);
    helper->setPen(linePen);
    helper->setLine(QLineF(-9999, y, 9999, y));
    return helper;
}

QGraphicsLineItem *verticalHelper(qreal x, QGraphicsItem *parent)
{
    using topohelp::kHelperLineLength;

    const QPen linePen(Qt::gray, 1, Qt::DashLine);
    auto helper = new QGraphicsLineItem(parent);
    helper->setPen(linePen);
    helper->setLine(QLineF(x, -kHelperLineLength, x, kHelperLineLength));
    return helper;
}

QPointF snappedPoint(QGraphicsScene *scene, const QPointF &pos)
{
    if (!scene || scene->views().empty()) {
        return pos;
    }

    if (auto view = dynamic_cast<ui::GraphicsViewBase *>(scene->views()[0])) {
        return view->snappedPoint(pos);
    }

    return pos;
}

QRectF snappedRect(QGraphicsScene *scene, const QRectF &rect)
{
    return QRectF(snappedPoint(scene, rect.topLeft()), snappedPoint(scene, rect.bottomRight()));
}

}
}
