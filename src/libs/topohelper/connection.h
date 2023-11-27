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

PointsList createConnectionPath(const ConnectionEnvInfo &info);

} // namespace cnct
} // namespace topohelp
