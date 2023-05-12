#include "minimizelimits.h"

namespace shared::ui {

QRectF MinimizeLimits::applyTo(QRectF rect, const Limits &limits, const GripPoint::Location &gripPointLocation, const QPointF &from, const QPointF &to)
{
    // Apply the movement that shrinks rect described by 'to' while respecting the calculated limits. Movement that does not shrink
    // rect is ignored.
    QPointF direction = to - from;
    bool directionLeft = direction.x() < 0;
    bool directionRight = direction.x() > 0;
    bool directionDown = direction.y() > 0;
    bool directionUp = direction.y() < 0;
    switch (gripPointLocation)
    {
        case GripPoint::Left:
        {
            bool leftGripIsMinimizing = directionRight;
            if (leftGripIsMinimizing)
            {
                rect.setLeft(qMin(to.x(), limits.leftGripsRightMostLimit));
            }
            break;
        }
        case GripPoint::Right:
        {
            bool rightGripIsMinimizing = directionLeft;
            if (rightGripIsMinimizing)
            {
                rect.setRight(qMax(to.x(), limits.rightGripsLeftMostLimit));
            }
            break;
        }
        case GripPoint::Top:
        {
            bool topGripIsMinimizing = directionDown;
            if (topGripIsMinimizing)
            {
                rect.setTop(qMin(to.y(), limits.topGripsBottomMostLimit));
            }
            break;
        }
        case GripPoint::Bottom:
        {
            bool bottomGripIsMinimizing = directionUp;
            if (bottomGripIsMinimizing)
            {
                rect.setBottom(qMax(to.y(), limits.bottomGripsTopMostLimit));
            }
            break;
        }
        case GripPoint::TopLeft:
        {
            bool topGripIsMinimizing = directionDown;
            if (topGripIsMinimizing)
            {
                rect.setTop(qMin(to.y(), limits.topGripsBottomMostLimit));
            }
            bool leftGripIsMinimizing = directionRight;
            if (leftGripIsMinimizing)
            {
                rect.setLeft(qMin(to.x(), limits.leftGripsRightMostLimit));
            }
            break;
        }
        case GripPoint::TopRight:
        {
            bool topGripIsMinimizing = directionDown;
            if (topGripIsMinimizing)
            {
                rect.setTop(qMin(to.y(), limits.topGripsBottomMostLimit));
            }
            bool rightGripIsMinimizing = directionLeft;
            if (rightGripIsMinimizing)
            {
                rect.setRight(qMax(to.x(), limits.rightGripsLeftMostLimit));
            }
            break;
        }
        case GripPoint::BottomLeft:
        {
            bool bottomGripIsMinimizing = directionUp;
            if (bottomGripIsMinimizing)
            {
                rect.setBottom(qMax(to.y(), limits.bottomGripsTopMostLimit));
            }
            bool leftGripIsMinimizing = directionRight;
            if (leftGripIsMinimizing)
            {
                rect.setLeft(qMin(to.x(), limits.leftGripsRightMostLimit));
            }
            break;
        }
        case GripPoint::BottomRight:
        {
            bool bottomGripIsMinimizing = directionUp;
            if (bottomGripIsMinimizing)
            {
                rect.setBottom(qMax(to.y(), limits.bottomGripsTopMostLimit));
            }
            bool rightGripIsMinimizing = directionLeft;
            if (rightGripIsMinimizing)
            {
                rect.setRight(qMax(to.x(), limits.rightGripsLeftMostLimit));
            }
            break;
        }
        default:
            qWarning() << "ResizingLimits: Update grip point handling";
            break;
    }
    return rect;

}

}
