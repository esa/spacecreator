#pragma once

#include "utils.h"

namespace topohelp {
namespace cnct {

struct ConnectionEnvInfo {
    QRectF objFrom;
    QPointF pntFrom;
    QRectF objTo;
    QPointF pntTo;
    QList<QRectF> sceneObjects;
};

PointsList createConnection(const ConnectionEnvInfo &info);

PointsList simplifyPoints(const PointsList &points);

} // namespace cnct
} // namespace topohelp
