#include "connection.h"

#include <QLineF>
#include <QList>

namespace topohelp {
namespace cnct {

static const qreal kMinSegmentLength = 10.0;
static const qreal kMinSegmentAngle = 5.0;

PointsList simplifyPoints(const PointsList &points)
{
    if (points.size() <= 2) {
        return points;
    }

    auto checkLines = [](const QLineF &line1, const QLineF &line2) -> bool {
        if (line1.length() < kMinSegmentLength) {
            return true;
        }
        const int simplifiedAngle = qAbs(qRound(line1.angleTo(line2)) % 180);
        const bool angleOk = simplifiedAngle < kMinSegmentAngle || 180 - simplifiedAngle < kMinSegmentAngle;
        return angleOk;
    };

    PointsList simplifiedPoints(points);
    for (int idx = 1; idx < simplifiedPoints.size() - 1;) {
        const QLineF currentLine { simplifiedPoints.value(idx), simplifiedPoints.value(idx - 1) };
        const QLineF nextLine { simplifiedPoints.value(idx), simplifiedPoints.value(idx + 1) };
        if (checkLines(currentLine, nextLine)) {
            simplifiedPoints.removeAt(idx);
            continue;
        }
        ++idx;
    }
    return simplifiedPoints;
}

PointsList createConnection(const ConnectionEnvInfo &info)
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

    return simplifyPoints(points);
}

} // namespace cnct

} // namespace topohelp
