#include "connection.h"

#include <QLineF>
#include <QList>

namespace topohelp {
namespace cnct {

PointsList createConnectionPath(const ConnectionEnvInfo &info)
{
    const QLineF startDirection = topohelp::utils::ifaceSegment(info.objFrom, info.pntFrom, info.pntTo);
    if (startDirection.isNull()) {
        return {};
    }

    const QLineF endDirection = topohelp::utils::ifaceSegment(info.objTo, info.pntTo, info.pntFrom);
    if (endDirection.isNull()) {
        return {};
    }

    PointsList points = topohelp::utils::path(info.sceneObjects, startDirection, endDirection);
    if (points.size() < 2) {
        // if no path was found, fall back to start/end directly
        return { info.pntFrom, info.pntTo };
    }

    return topohelp::utils::simplifyPoints(points);
}

} // namespace cnct

} // namespace topohelp
