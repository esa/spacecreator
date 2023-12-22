#pragma once

#include "geometry.h"

namespace topohelp {
namespace connection {

struct ConnectionEnvInfo {
    QRectF objFrom;
    QPointF pntFrom;
    QRectF objTo;
    QPointF pntTo;
    QList<QRectF> sceneObjects;
};

PointsList createConnectionPath(const ConnectionEnvInfo &info);

} // namespace connection
} // namespace topohelp
