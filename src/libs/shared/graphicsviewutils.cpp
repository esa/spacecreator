#include "graphicsviewutils.h"

#include "connection.h"
#include "positionlookuphelper.h"
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

static const qreal kMinSegmentLength = 10.0;
static const qreal kMinSegmentAngle = 5.0;
static const qreal kConnectionMargin = 16.0;

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

bool intersects(const QRectF &rect, const QLineF &line, QPointF *intersectPos)
{
    return topohelp::utils::intersects(rect, line, intersectPos);
}
bool intersects(const QRectF &rect, const QPolygonF &polygon, QPointF *intersectPos)
{
    return topohelp::utils::intersects(rect, polygon, intersectPos);
}

QRectF getNearestIntersectedRect(
        const QList<QRectF> &existingRects, const QVector<QPointF> &points, topohelp::IntersectionType intersectionType)
{
    return topohelp::utils::getNearestIntersectedRect(existingRects, points, intersectionType);
}

QVector<QPointF> intersectionPoints(const QRectF &rect, const QPolygonF &polygon)
{
    return topohelp::utils::intersectionPoints(rect, polygon);
}

/*!
 * \brief Adjusts appropriate coordinate of \a pos to the \a boundingArea side specified by \a side.
 * Returns the adjusted \a pos.
 */
QPointF getSidePosition(const QRectF &boundingArea, const QPointF &pos, Qt::Alignment side)
{
    switch (side) {
    case Qt::AlignLeft:
        return QPointF(boundingArea.left(), qBound(boundingArea.top(), pos.y(), boundingArea.bottom()));
    case Qt::AlignRight:
        return QPointF(boundingArea.right(), qBound(boundingArea.top(), pos.y(), boundingArea.bottom()));
    case Qt::AlignTop:
        return QPointF(qBound(boundingArea.left(), pos.x(), boundingArea.right()), boundingArea.top());
    case Qt::AlignBottom:
        return QPointF(qBound(boundingArea.left(), pos.x(), boundingArea.right()), boundingArea.bottom());
    }

    return boundingArea.center();
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
 * \fn ive::bool alignedLine(QLineF &line, int angleTolerance)
 * \brief  Adjusts the angle of the \a line to the nearest x90 degrees ccw with \a angleTolerance.
 * Returns true if the \a line's angle adjusted.
 */
bool alignedLine(QLineF &line, int angleTolerance)
{
    if (line.isNull())
        return false;

    static const int kStep = 90;

    auto isBounded = [](int straightAngle, int tolerance, int angle) {
        return straightAngle - tolerance < angle && angle < straightAngle + tolerance;
    };

    for (int angle = 360; angle >= 0; angle -= kStep) {
        if (isBounded(angle, angleTolerance, std::ceil(line.angle()))) {
            line.setAngle(angle);
            return true;
        }
    }
    return false;
}

qreal distanceLine(const QPointF &pnt1, const QPointF &pnt2)
{
    return topohelp::utils::distanceLine(pnt1, pnt2);
}
/*!
 * Converts the IV[AADL/XML] coordinates pair to the QPointF.
 * If \a coordinates is empty or its size != 2 an empty point returned.
 */
QPointF pos(const QVector<qint32> &coordinates)
{
    if (coordinates.isEmpty())
        return {};

    Q_ASSERT(coordinates.size() == 2);
    if (coordinates.size() != 2)
        return {};

    return QPointF(coordinates.first(), coordinates.last());
}

/*!
 * Converts four coordinate numbers from IV[AADL/XML] to the QRectF.
 * If \a coordinates is empty or its size != 4 returns an empty rect.
 */
QRectF rect(const QVector<qint32> &coordinates)
{
    if (coordinates.isEmpty())
        return {};

    Q_ASSERT(coordinates.size() == 4);
    if (coordinates.size() != 4)
        return {};

    QList<QPointF> points;
    for (int idx = 0; idx + 1 < coordinates.size(); idx += 2)
        points.append(QPointF(coordinates.value(idx), coordinates.value(idx + 1)));

    return { points.first(), points.last() };
}

/*!
 * Converts coordinate numbers from IV[AADL/XML] to vector of QPointF.
 * If \a coordinates is empty or its size is odd returns an empty vector.
 */

QVector<QPointF> polygon(const QVector<qint32> &coordinates)
{
    if (coordinates.isEmpty())
        return {};

    Q_ASSERT(coordinates.size() % 2 == 0);
    if (coordinates.size() % 2 != 0)
        return {};

    QVector<QPointF> points;
    for (int idx = 0; idx + 1 < coordinates.size(); idx += 2)
        points.append(QPointF(coordinates.value(idx), coordinates.value(idx + 1)));

    return points;
}

QVector<qint32> coordinates(const QPointF &point)
{
    return topohelp::utils::coordinates(point);
}

QVector<qint32> coordinates(const QRectF &rect)
{
    return topohelp::utils::coordinates(rect);
}

QVector<qint32> coordinates(const QVector<QPointF> &points)
{
    return topohelp::utils::coordinates(points);
}

QRectF adjustFromPoint(const QPointF &pos, const qreal &adjustment)
{
    return topohelp::utils::adjustFromPoint(pos, adjustment);
}

/*!
 * Returns a \a itemRect aligned inside of \a boundingRect but keeping the offset of \a originPointOffset
 */

QRectF alignRectToSide(const QRectF &boundingRect, const QRectF &itemRect, Qt::Alignment side,
        const QPointF &originPointOffset, const QMarginsF &margins)
{
    if (!boundingRect.isValid() || !itemRect.isValid())
        return {};

    QRectF rect { itemRect };
    auto adjustVertically = [=](QRectF &rect) {
        rect.moveTop(qBound(boundingRect.top() + originPointOffset.y() + margins.top(), rect.top(),
                boundingRect.bottom() + originPointOffset.y() - margins.bottom()));
    };
    auto adjustHorizontally = [=](QRectF &rect) {
        rect.moveLeft(qBound(boundingRect.left() + originPointOffset.x() + margins.left(), rect.left(),
                boundingRect.right() + originPointOffset.x() - margins.right()));
    };

    switch (side) {
    case Qt::AlignLeft:
        rect.moveLeft(boundingRect.left() + originPointOffset.x());
        adjustVertically(rect);
        break;
    case Qt::AlignTop:
        rect.moveTop(boundingRect.top() + originPointOffset.y());
        adjustHorizontally(rect);
        break;
    case Qt::AlignRight:
        rect.moveLeft(boundingRect.right() + originPointOffset.x());
        adjustVertically(rect);
        break;
    case Qt::AlignBottom:
        rect.moveTop(boundingRect.bottom() + originPointOffset.y());
        adjustHorizontally(rect);
        break;
    default:
        return {};
    }
    return rect;
}

/*!
 * Moves \a itemRect on the \a side from \a intersectedItemRect
 */

QRectF adjustedRect(const QRectF &itemRect, const QRectF &intersectedItemRect, const Qt::Alignment side,
        const topohelp::LookupDirection direction)
{
    return topohelp::utils::adjustedRect(itemRect, intersectedItemRect, side, direction);
}
/*!
 * Checks intersection \a itemRect with \a itemRects
 * and assign first intersection to \a collidingRect
 */

bool isCollided(const QList<QRectF> &itemRects, const QRectF &itemRect, QRectF *collidingRect)
{
    return topohelp::utils::isCollided(itemRects, itemRect, collidingRect);
}

QList<QVector<QPointF>> generateSegments(const QPointF &startPoint, const QPointF &endPoint)
{
    return topohelp::utils::generateSegments(startPoint, endPoint);
}

QVector<QPointF> generateSegments(const QLineF &startDirection, const QLineF &endDirection)
{
    return topohelp::utils::generateSegments(startDirection, endDirection);
}

QLineF ifaceSegment(const QRectF &sceneRect, const QPointF &firstEndPoint, const QPointF &lastEndPoint)
{
    return topohelp::utils::ifaceSegment(sceneRect, firstEndPoint, lastEndPoint);
}

QVector<QPointF> path(const QList<QRectF> &existingRects, const QPointF &startPoint, const QPointF &endPoint)
{
    return topohelp::utils::path(existingRects, startPoint, endPoint);
}

QVector<QPointF> path(const QList<QRectF> &existingRects, const QLineF &startDirection, const QLineF &endDirection)
{
    return topohelp::utils::path(existingRects, startDirection, endDirection);
}

QVector<QPointF> createConnectionPath(const QList<QRectF> &existingRects, const QPointF &startIfacePos,
        const QRectF &sourceRect, const QPointF &endIfacePos, const QRectF &targetRect)
{
    using namespace topohelp::cnct;
    const ConnectionEnvInfo connectionInfo {
        sourceRect,
        startIfacePos,
        targetRect,
        endIfacePos,
        existingRects,
    };

    return createConnectionPath(connectionInfo);
}

QVector<QPointF> simplifyPoints(const QVector<QPointF> &points)
{
    return topohelp::utils::simplifyPoints(points);
}

Qt::Alignment getNearestSide(const QRectF &boundingArea, const QPointF &pos)
{
    return topohelp::utils::getNearestSide(boundingArea, pos);
}

bool comparePolygones(const QVector<QPointF> &v1, const QVector<QPointF> &v2)
{
    if (v1.size() != v2.size())
        return false;

    for (int idx = 0; idx < v1.size(); ++idx) {
        if (v1.at(idx).toPoint() != v2.at(idx).toPoint())
            return false;
    }
    return true;
}

bool isOnVerticalSide(const QRectF &rect, const QPointF &point)
{
    return (qFuzzyCompare(rect.left(), point.x()) || qFuzzyCompare(rect.right(), point.x()))
            && ((rect.top() < point.y() && rect.bottom() > point.y()) || qFuzzyCompare(rect.top(), point.y())
                    || qFuzzyCompare(rect.bottom(), point.y()));
}

bool isOnHorizontalSide(const QRectF &rect, const QPointF &point)
{
    return (qFuzzyCompare(rect.top(), point.y()) || qFuzzyCompare(rect.bottom(), point.y()))
            && ((rect.left() < point.x() && rect.right() > point.x()) || qFuzzyCompare(rect.left(), point.x())
                    || qFuzzyCompare(rect.right(), point.x()));
}

bool rectContainsPoint(const QRectF &rect, const QPointF &point, bool proper)
{
    if (!rect.contains(point)) {
        return false;
    }
    return !proper || (!isOnHorizontalSide(rect, point) && !isOnVerticalSide(rect, point));
}

/*!
 * Searches rect from \a existingRects that intersects with \a rect
 */
QRectF collidingRect(const QRectF &rect, const QList<QRectF> &existingRects)
{
    return topohelp::utils::collidingRect(rect, existingRects);
};

/*!
 * Searches location for \a itemRect within \a boundedRect that could be expanded to include mentioned rect
 * without overlapping with \a existingRects
 */
void findGeometryForRect(
        QRectF &itemRect, QRectF &boundedRect, const QList<QRectF> &existingRects, const QMarginsF &margins)
{
    return topohelp::utils::findGeometryForRect(itemRect, boundedRect, existingRects, margins);
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

bool isRectBounded(const QRectF &outerRect, const QRectF &innerRect)
{
    return topohelp::utils::isRectBounded(outerRect, innerRect);
}

bool isCollided(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect)
{
    if (!upcomingItem || !upcomingItemRect.isValid()) {
        return false;
    }

    const QList<QRectF> siblingRects = siblingItemsRects(upcomingItem);
    return isCollided(siblingRects, upcomingItemRect.marginsAdded(topohelp::kContentMargins));
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
            return isRectBounded(outerRect, upcomingItemRect);
        }
        // Are all my children contained in me
        return isRectBounded(
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

    const QRectF kBaseRect = adjustFromPoint(QPointF(0, 0), kInterfaceBaseLength);
    QRectF itemRect = adjustFromPoint(ifacePos, kInterfaceBaseLength + topohelp::kSiblingMinDistance);

    itemRect.moveCenter(ifacePos);
    QRectF intersectedRect;

    if (((ifacePos.isNull() && existingRects.isEmpty()) || isCollided(existingRects, itemRect, &intersectedRect))
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
