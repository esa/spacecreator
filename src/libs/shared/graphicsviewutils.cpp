#include "graphicsviewutils.h"

#include <QPainterPath>

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
 * \brief Helper to detect if the \a line intersects the \a rect.
 * Coordinates of the intersection point stored in \a intersectPos.
 * Returns \c true if \a rect and \a line are not null and intersected.
 */
bool intersects(const QRectF &rect, const QLineF &line, QPointF *intersectPos)
{
    if (rect.isNull() || line.isNull())
        return false;

    const QVector<QLineF> rectLines = {
        { rect.topLeft(), rect.topRight() },
        { rect.topRight(), rect.bottomRight() },
        { rect.bottomRight(), rect.bottomLeft() },
        { rect.bottomLeft(), rect.topLeft() },
    };

    for (const QLineF &rectLine : rectLines) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        if (rectLine.intersects(line, intersectPos) == QLineF::BoundedIntersection)
#else
        if (rectLine.intersect(line, intersectPos) == QLineF::BoundedIntersection)
#endif
            return true;
    }

    return false;
}

/*!
 * \brief Helper to detect if at least one sub segment of the \a polygon intersects the \a rect.
 * Coordinates of the intersection point stored in \a intersctPos.
 * Returns \c true if a sub segment of the \a polygon intersects the \a rect.
 */
/*!
 * Returns coordinates of intersection points.
 */
bool intersects(const QRectF &rect, const QPolygonF &polygon, QPointF *intersectPos)
{
    for (int idx = 1; idx < polygon.size(); ++idx) {
        if (intersects(rect, QLineF(polygon.value(idx - 1), polygon.value(idx)), intersectPos))
            return true;
    }
    return false;
}

/*!
 * \brief Helper to detect if each sub segment of the \a polygon intersects the \a rect.
 * Returns coordinates of intersection points.
 */
QVector<QPointF> intersectionPoints(const QRectF &rect, const QPolygonF &polygon)
{
    if (rect.isNull() || polygon.isEmpty())
        return {};

    const QVector<QLineF> rectLines = {
        { rect.topLeft(), rect.topRight() },
        { rect.topRight(), rect.bottomRight() },
        { rect.bottomRight(), rect.bottomLeft() },
        { rect.bottomLeft(), rect.topLeft() },
    };

    QPointF intersectPos;
    QVector<QPointF> points;
    for (int idx = 1; idx < polygon.size(); ++idx) {
        const QLineF line = { polygon.value(idx - 1), polygon.value(idx) };
        for (const QLineF &rectLine : rectLines) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
            if (rectLine.intersects(line, &intersectPos) == QLineF::BoundedIntersection)
#else
            if (rectLine.intersect(line, &intersectPos) == QLineF::BoundedIntersection)
#endif
                points.append(intersectPos);
        }
    }
    return points;
}

}
}
