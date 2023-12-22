#include "connection.h"

#include <QLineF>
#include <QList>

namespace topohelp {
namespace connection {

/*!
 * \brief createConnectionPath
 *
 * Generates a path represented by a PointsList connecting two points within a given ConnectionEnvInfo. The path is
 * determined based on the provided information, considering start and end directions, as well as potential
 * simplification.
 *
 * \param info A constant reference to the ConnectionEnvInfo containing information about the connection environment,
 * including start & end points on the related objects' geometries, and the rest of scene objects rects to be obeyed.
 * \return PointsList Returns a list of QPointF representing the generated
 * connection path. If no valid path is found, an empty list is returned. If the path has fewer than two points after
 * simplification, the start and end points are returned directly.
 *
 * \param info.objFrom A QRectF representing the bounding box of the object from which the connection starts.
 * \param info.pntFrom A QPointF representing the starting point of the connection.
 * \param info.objTo A QRectF representing the bounding box of the object to which the connection leads.
 * \param info.pntTo A QPointF representing the ending point of the connection.
 * \param info.sceneObjects A QList<QRectF> containing bounding boxes the newly created connection should account.
 */
PointsList createConnectionPath(const ConnectionEnvInfo &info)
{
    const QLineF &startDirection = topohelp::geom::ifaceSegment(info.objFrom, info.pntFrom, info.pntTo);
    if (startDirection.isNull()) {
        return {};
    }

    const QLineF &endDirection = topohelp::geom::ifaceSegment(info.objTo, info.pntTo, info.pntFrom);
    if (endDirection.isNull()) {
        return {};
    }

    const PointsList &points = topohelp::geom::path(info.sceneObjects, startDirection, endDirection);
    if (points.size() < 2) {
        // if no path was found, fall back to start/end directly
        return { info.pntFrom, info.pntTo };
    }

    return topohelp::geom::simplifyPoints(points);
}

} // namespace connection

} // namespace topohelp
