#pragma once

#include "grippoint.h"
#include "qpoint.h"

namespace shared {
namespace ui {

enum class ResizeType
{
    Min, /** The user made the rectangle smaller on one side*/
    Max, /** The user made the rectangle bigger on one side*/
    None /** The user made no change to one side */
};
enum class HorizontalEdge
{
    TopEdge,
    BottomEdge,
    None,
};
enum class VerticalEdge
{
    RightEdge,
    LeftEdge,
    None,
};

/**
 * @brief The ResizeLimits class represents the resizinglimits of a VERectGraphicsItem.
 * When the user drags a GripPoint from one point (from) to another (to), the VERectGraphicsItem is resized
 * on one or two sides. This resizing-action is subject to limitations. When the VERectGraphicsItem
 * becomes bigger it can collide with other elements in the scene. When becoming smaller it can be limited
 * by the presence of interface connection points, text labes etc. Since the VERectGraphicsItem is made to
 * be extended, any number of limits can be impossed on a resize-action by its children.
 * An instance of this class can be handed to a number of limiter-methods in turn, and each limiter-method
 * will decide if the limits expressed needs to be even stricter than they already are.
 *
 * Each axis is represented seperatly since one side can become smaller or bigger without
 * influencing the other side.
 *
 * An example of an instance of this class could be from the user moving the top-left grip-point up and
 * to the right, making the VERectGraphicsItem taller and narrower. This would create an instance of
 * ResizeLimits like this:
 *
 * ResizeLimits:
 * horizontalEdge = Qt::Edge::TopEdge
 * horizontalResizeType = ResizeType::Max
 * horizontalDistance = <how far up the user dragged up>
 * horizontalLimit = <calculated by limiter-methods>
 * verticalEdge = Qt::Edge::LeftEdge
 * verticalResizeType = ResizeType::Min
 * verticalDistance = <how far the user dragged right>
 * verticalLimit = <calculated by limiter-methods>
 *
 * Each limiter-method can look at this instance and see if it wants to limit this resize-action further. If not, it
 * can just ignore it.
 */
class ResizeLimits
{
public:
    ResizeLimits(GripPoint *grip, const QPointF &from, const QPointF &to, QRectF rect);
    QRectF getLimitedRect();
    QRectF getUnlimitedRect();
    QRectF getUnlimitedVerticalRect();
    QRectF getUnlimitedHorizontalRect();
    QRectF getUnlimitedTopLeftRect();
    QRectF getUnlimitedTopRightRect();
    QRectF getUnlimitedBottomRightRect();
    QRectF getUnlimitedBottomLeftRect();

    VerticalEdge getVerticalEdge() const;
    qreal getVerticalDistance() const;
    qreal getVerticalLimit();
    void setVerticalLimit(qreal verticalLimit);
    bool isVerticalLimitSet();
    ResizeType getVerticalResizeType() const;

    HorizontalEdge getHorizontalEdge() const;
    qreal getHorizontalDistance() const;
    qreal getHorizontalLimit();
    void setHorizontalLimit(qreal horizontalLimit);
    bool isHorizontalLimitSet();
    ResizeType getHorizontalResizeType() const;

    bool isLeftEdgeMaximizing() const;
    bool isRightEdgeMaximizing() const;
    bool isTopEdgeMaximizing() const;
    bool isBottomEdgeMaximizing() const;

    bool isLeftEdgeMinimizing() const;
    bool isRightEdgeMinimizing() const;
    bool isTopEdgeMinimizing() const;
    bool isBottomEdgeMinimizing() const;

    QPair<HorizontalEdge, VerticalEdge> getIntersectedEdges(QRectF rect);

    operator QString() const;

private:
    GripPoint::Location m_gripPointLocation;
    QPointF m_from;
    QPointF m_to;
    qreal m_horizontalDistance;
    qreal m_verticalDistance;
    QRectF m_rect;


    // Horizontal
    HorizontalEdge m_horizontalEdge;
    ResizeType m_horizontalResizeType = ResizeType::None;
    bool m_horizontalLimitSet;
    qreal m_horizontalLimit;

    // Vertical
    VerticalEdge m_verticalEdge;
    ResizeType m_verticalResizeType;
    bool m_verticalLimitSet;
    qreal m_verticalLimit;

    QLineF getResizeLine() const;
};

}
}
