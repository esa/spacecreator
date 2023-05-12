#pragma once

#include <QRectF>
#include "grippoint.h"

namespace shared::ui {

class MinimizeLimits
{
public:
    struct Limits
    {
        qreal leftGripsRightMostLimit;
        qreal rightGripsLeftMostLimit;
        qreal topGripsBottomMostLimit;
        qreal bottomGripsTopMostLimit;
    };
    static QRectF applyTo(QRectF rect, const Limits &limits, const GripPoint::Location &gripPointLocation, const QPointF &from, const QPointF &to);
};

} // ui
