#include "resizelimits.h"
#include "grippoint.h"

#include <QtGlobal>
#include <QLine>
#include <QPointF>

namespace {
QString rect2string(QRectF r)
{
    QString s("(%1, %2), (%3, %4)");
    QString rectStr = s.arg(r.x()).arg(r.y()).arg(r.right()).arg(r.bottom());
    return rectStr;
}

QString point2string(QPointF p)
{
    QString s("(%1, %2)");
    return s.arg(p.x()).arg(p.y());
}

}

namespace shared {
namespace ui {

static const qreal epsilon = 0.0;
/**
 * @brief Given the grip the user dragged from one point to another, the constructor
 * calculates which edges of the rectangle was moved and whether each was making the rectangle smaller or bigger.
 * Distance resized for each axis are also calculated and stored.
 * @param grip
 * @param from
 * @param to
 */
ResizeLimits::ResizeLimits(GripPoint *grip, const QPointF &from, const QPointF &to, QRectF rect)
    : m_gripPointLocation(grip->location())
    , m_from(from)
    , m_to(to)
    , m_horizontalDistance(to.y() - from.y())
    , m_verticalDistance(to.x() - from.x())
    , m_rect(rect)
    , m_horizontalEdge(HorizontalEdge::None)
    , m_horizontalResizeType(ResizeType::None)
    , m_horizontalLimitSet(false)
    , m_horizontalLimit(42)
    , m_verticalEdge(VerticalEdge::None)
    , m_verticalResizeType(ResizeType::None)
    , m_verticalLimitSet(false)
    , m_verticalLimit(42)
{
    Q_ASSERT(rect.isValid());

    QPointF direction = to - from;
    bool directionLeft = direction.x() < 0;
    bool directionRight = direction.x() > 0;
    bool directionDown = direction.y() > 0;
    bool directionUp = direction.y() < 0;


    switch (m_gripPointLocation) {
    case GripPoint::Top:
        m_horizontalEdge = HorizontalEdge::TopEdge;
        if (directionUp) {
            m_horizontalResizeType = ResizeType::Max;
        }
        if (directionDown) {
            m_horizontalResizeType = ResizeType::Min;
        }
        break;

    case GripPoint::Right:
        m_verticalEdge = VerticalEdge::RightEdge;
        if (directionRight) {
            m_verticalResizeType = ResizeType::Max;
        }
        if (directionLeft) {
            m_verticalResizeType = ResizeType::Min;
        }
        break;

    case GripPoint::Left:
        m_verticalEdge = VerticalEdge::LeftEdge;
        if (directionLeft) {
            m_verticalResizeType = ResizeType::Max;
        }
        if (directionRight) {
            m_verticalResizeType = ResizeType::Min;
        }
        break;

    case GripPoint::Bottom:
        m_horizontalEdge = HorizontalEdge::BottomEdge;
        if (directionDown) {
            m_horizontalResizeType = ResizeType::Max;
        }
        if (directionUp) {
            m_horizontalResizeType = ResizeType::Min;
        }
        break;

    case GripPoint::TopLeft:
        m_horizontalEdge = HorizontalEdge::TopEdge;
        if (directionUp) {
            m_horizontalResizeType = ResizeType::Max;
        }
        if (directionDown) {
            m_horizontalResizeType = ResizeType::Min;
        }

        m_verticalEdge = VerticalEdge::LeftEdge;
        if (directionLeft) {
            m_verticalResizeType = ResizeType::Max;
        }
        if (directionRight) {
            m_verticalResizeType = ResizeType::Min;
        }
        break;

    case GripPoint::TopRight:
        m_horizontalEdge = HorizontalEdge::TopEdge;
        if (directionUp) {
            m_horizontalResizeType = ResizeType::Max;
        }
        if (directionDown) {
            m_horizontalResizeType = ResizeType::Min;
        }

        m_verticalEdge = VerticalEdge::RightEdge;
        if (directionRight) {
            m_verticalResizeType = ResizeType::Max;
        }
        if (directionLeft) {
            m_verticalResizeType = ResizeType::Min;
        }
        break;

    case GripPoint::BottomLeft:
        m_horizontalEdge = HorizontalEdge::BottomEdge;
        if (directionDown) {
            m_horizontalResizeType = ResizeType::Max;
        }
        if (directionUp) {
            m_horizontalResizeType = ResizeType::Min;
        }

        m_verticalEdge = VerticalEdge::LeftEdge;
        if (directionLeft) {
            m_verticalResizeType = ResizeType::Max;
        }
        if (directionRight) {
            m_verticalResizeType = ResizeType::Min;
        }
        break;

    case GripPoint::BottomRight:
        m_horizontalEdge = HorizontalEdge::BottomEdge;
        if (directionDown) {
            m_horizontalResizeType = ResizeType::Max;
        }
        if (directionUp) {
            m_horizontalResizeType = ResizeType::Min;
        }

        m_verticalEdge = VerticalEdge::RightEdge;
        if (directionRight) {
            m_verticalResizeType = ResizeType::Max;
        }
        if (directionLeft) {
            m_verticalResizeType = ResizeType::Min;
        }
        break;

    default:
        break;
    }

    QRectF unlimitedRect = getUnlimitedRect();
    Q_ASSERT(unlimitedRect.isValid());
}
/**
 * @brief Add the resizing distances to a given rectangle.
 * @param oldRect
 * @return QRectF
 */
QRectF ResizeLimits::getUnlimitedRect()
{
    Q_ASSERT(m_rect.isValid());

    QRectF newRect = m_rect;

    bool horizontalResizeTypeIsMaxOrMin = (m_horizontalResizeType == ResizeType::Max ||
            m_horizontalResizeType == ResizeType::Min);
    if (horizontalResizeTypeIsMaxOrMin) {
        if (m_horizontalEdge == HorizontalEdge::TopEdge) {
            qreal newTop = m_rect.top() + getHorizontalDistance();
            newRect.setTop(newTop);
        }
        Q_ASSERT(newRect.isValid());

        if (m_horizontalEdge == HorizontalEdge::BottomEdge) {
            qreal newBottom = m_rect.bottom() + getHorizontalDistance();
            newRect.setBottom(newBottom);
        }
        Q_ASSERT(newRect.isValid());
    }

    bool verticalResizeTypeIsMaxOrMin = (m_verticalResizeType == ResizeType::Max ||
            m_verticalResizeType == ResizeType::Min);
    if (verticalResizeTypeIsMaxOrMin) {
        if (m_verticalEdge == VerticalEdge::LeftEdge && verticalResizeTypeIsMaxOrMin) {
            qreal newLeft = m_rect.left() + getVerticalDistance();
            newRect.setLeft(newLeft);
        }
        Q_ASSERT(newRect.isValid());

        if (m_verticalEdge == VerticalEdge::RightEdge && verticalResizeTypeIsMaxOrMin) {
            qreal newRight = m_rect.right() + getVerticalDistance();
            newRect.setRight(newRight);
        }
        Q_ASSERT(newRect.isValid());
    }
    return newRect;
}

/**
 * @brief calculates the rectangle, the rectangle given at construction can resize while respecing the
 * limitations set on this object
 * @return QRectF
 */
QRectF ResizeLimits::getLimitedRect()
{
    QRectF newRect = m_rect;

    if (m_horizontalEdge == HorizontalEdge::TopEdge) {
        qreal newTop = m_rect.top();
        qreal horizontalDistance = getHorizontalDistance();
        if (m_horizontalResizeType == ResizeType::Max) {
            // Top edge is moving up
            bool isGripHandleMovingTowardMouse = m_to.y() <= m_rect.top();
            if (isGripHandleMovingTowardMouse) {
                qreal unlimitedNewTop = m_rect.top() + horizontalDistance;
                if (isHorizontalLimitSet()) {
                    newTop = qMax(unlimitedNewTop, getHorizontalLimit());
                } else {
                    newTop = unlimitedNewTop;
                }
            }
        }
        if (m_horizontalResizeType == ResizeType::Min) {
            // Top edge is moving down
            bool isGripHandleMovingTowardMouse = m_to.y() >= m_rect.top();
            // Only move top downwards if mouse is below rect's top
            if (isGripHandleMovingTowardMouse) {
                qreal unlimitedNewTop = m_rect.top() + horizontalDistance;
                if (isHorizontalLimitSet()) {
                    // Don't move past internal horizontal limit
                    newTop = qMin(unlimitedNewTop, getHorizontalLimit());
                } else {
                    newTop = unlimitedNewTop;
                }
            }
        }
        newRect.setTop(newTop);
        Q_ASSERT(newRect.isValid());
    }

    if (m_horizontalEdge == HorizontalEdge::BottomEdge) {
        qreal newBottom = m_rect.bottom();
        if (m_horizontalResizeType == ResizeType::Max) {
            // Bottom edge is moving down
            bool isGripHandleMovingTowardMouse = m_to.y() >= m_rect.bottom();
            // Only move bottom upwards if mouse is above rect's bottom
            if (isGripHandleMovingTowardMouse) {
                qreal unlimitedNewBottom = m_rect.bottom() + getHorizontalDistance();
                if (isHorizontalLimitSet()) {
                    // Don't move past external limit
                    newBottom = qMin(unlimitedNewBottom, getHorizontalLimit());
                } else {
                    newBottom = unlimitedNewBottom;
                }
            }
        }
        if (m_horizontalResizeType == ResizeType::Min) {
            // Bottom edge is moving up
            bool isGripHandleMovingTowardMouse = m_to.y() <= m_rect.bottom();
            if (isGripHandleMovingTowardMouse) {
                qreal unlimitedNewBottom = m_rect.bottom() + getHorizontalDistance();
                if (isHorizontalLimitSet()) {
                    newBottom = qMax(unlimitedNewBottom, getHorizontalLimit());
                } else {
                    newBottom = unlimitedNewBottom;
                }
            }
        }
        newRect.setBottom(newBottom);
        Q_ASSERT(newRect.isValid());
    }

    if (m_verticalEdge == VerticalEdge::LeftEdge) {
        qreal newLeft = m_rect.left();
        if (m_verticalResizeType == ResizeType::Max) {
            // Left edge is moving left
            bool isGripMovingTowardMouse = m_to.x() <= m_rect.left();
            if (isGripMovingTowardMouse){
                qreal unlimitedNewLeft = m_rect.left() + getVerticalDistance();
                newLeft = (isVerticalLimitSet() ? qMax(unlimitedNewLeft, getVerticalLimit()) : unlimitedNewLeft);
            }
        }
        if (m_verticalResizeType == ResizeType::Min) {
            // Left edge is moving right
            bool isGripMovingTowardMouse = m_to.x() >= m_rect.left();
            if (isGripMovingTowardMouse) {
                qreal unlimitedNewLeft = m_rect.left() + getVerticalDistance();
                newLeft = (isVerticalLimitSet() ? qMin(unlimitedNewLeft, getVerticalLimit()) : unlimitedNewLeft);
            }
        }
        newRect.setLeft(newLeft);
        Q_ASSERT(newRect.isValid());
    }

    if (m_verticalEdge == VerticalEdge::RightEdge) {
        qreal newRight = m_rect.right();
        if (m_verticalResizeType == ResizeType::Max) {
            // Right edge is moving right
            bool isGripMovingTowardMouse = m_to.x() >= m_rect.right();
            if (isGripMovingTowardMouse) {
                qreal unlimitedNewRight = m_rect.right() + getVerticalDistance();
                newRight = (isVerticalLimitSet() ? qMin(unlimitedNewRight, getVerticalLimit()) : unlimitedNewRight);
            }
        }
        if (m_verticalResizeType == ResizeType::Min) {
            // Right edge is moving left
            bool isGripMovingTowardMouse = m_to.x() <= m_rect.right();
            if (isGripMovingTowardMouse) {
            qreal unlimitedNewRight = m_rect.right() + getVerticalDistance();
            newRight = (isVerticalLimitSet() ? qMax(unlimitedNewRight, getVerticalLimit()) : unlimitedNewRight);
            }
        }
        newRect.setRight(newRight);
        Q_ASSERT(newRect.isValid());
    }

    return newRect;
}

/**
 * @brief Get the unlimited rect as if the user had only used the right-, or left-grippoint.
 * @return
 */
QRectF ResizeLimits::getUnlimitedVerticalRect()
{
    Q_ASSERT(m_rect.isValid());

    bool verticalResizeTypeIsMaxOrMin = (m_verticalResizeType == ResizeType::Max ||
            m_verticalResizeType == ResizeType::Min);
    if (!verticalResizeTypeIsMaxOrMin) {
        return m_rect;
    }

    QRectF newRect = m_rect;

    if (m_verticalEdge == VerticalEdge::LeftEdge ) {
        qreal unlimitedNewLeft = m_rect.left() + getVerticalDistance();
        newRect.setLeft(unlimitedNewLeft);
    }
    Q_ASSERT(newRect.isValid());

    if (m_verticalEdge == VerticalEdge::RightEdge) {
        qreal unlimitedNewRight = m_rect.right() + getVerticalDistance();
        newRect.setRight(unlimitedNewRight);
    }
    Q_ASSERT(newRect.isValid());

    return newRect;
}

/**
 * @brief Get the unlimited rect as if the user had only used the top-, or bottom-grippoint.
 * @return
 */
QRectF ResizeLimits::getUnlimitedHorizontalRect()
{
    Q_ASSERT(m_rect.isValid());

    bool horizontalResizeTypeIsMaxOrMin = (m_horizontalResizeType == ResizeType::Max ||
            m_horizontalResizeType == ResizeType::Min);
    if (!horizontalResizeTypeIsMaxOrMin) {
        return m_rect;
    }

    QRectF newRect = m_rect;

    if (m_horizontalEdge == HorizontalEdge::TopEdge) {
        qreal unlimitedNewTop = m_rect.top() + getHorizontalDistance();
        newRect.setTop(unlimitedNewTop);
        Q_ASSERT(newRect.isValid());
    }

    if (m_horizontalEdge == HorizontalEdge::BottomEdge) {
        qreal unlimitedNewBottom = m_rect.bottom() + getHorizontalDistance();
        newRect.setBottom(unlimitedNewBottom);
        Q_ASSERT(newRect.isValid());
    }

    return newRect;
}

QRectF ResizeLimits::getUnlimitedTopLeftRect()
{
    bool topLeftRectExists = isTopEdgeMaximizing() && isLeftEdgeMaximizing();
    if (!topLeftRectExists) {
        QString msg("ResizeLimits::getUnilimitedTopLeftRect: TopLeftRect does not exist. %1");
        throw std::runtime_error(msg.arg(*this).toStdString());
    }
    QLineF v = getResizeLine();
    QRectF topLeftRect = QRectF(v.p2(), v.p1());
    Q_ASSERT(topLeftRect.isValid());
    return topLeftRect;
}

QRectF ResizeLimits::getUnlimitedTopRightRect()
{
    bool topRightRectExists = isTopEdgeMaximizing() && isRightEdgeMaximizing();
    if (!topRightRectExists) {
        QString msg("ResizeLimits::getUnlimitedTopRightRect: TopRightRect does not exist. %1");
        throw std::runtime_error(msg.arg(*this).toStdString());
    }
    QLineF v = getResizeLine();
    QPointF topLeft = QPointF(v.p1().x(), v.p2().y());
    QPointF bottomRight = QPointF(v.p2().x(), v.p1().y());
    QRectF topRightRect = QRectF(topLeft, bottomRight);
    return topRightRect;
}

QRectF ResizeLimits::getUnlimitedBottomRightRect()
{
    bool bottomRightRectExists = isBottomEdgeMaximizing() && isRightEdgeMaximizing();
    if (!bottomRightRectExists) {
        QString msg("ResizeLimits::getUnlimitedBottomRightRect: BottomRightRect does not exist. %1");
        throw std::runtime_error(msg.arg(*this).toStdString());
    }
    QLineF v = getResizeLine();
    QRectF bottomRightRect = QRectF(v.p1(), v.p2());
    return bottomRightRect;
}

QRectF ResizeLimits::getUnlimitedBottomLeftRect()
{
    bool bottomLeftRectExists = isBottomEdgeMaximizing() && isLeftEdgeMaximizing();
    if (!bottomLeftRectExists) {
        QString msg("ResizeLimits::getUnlimitedBottomLeftRect: BottomLeftRect does not exists. %1");
        throw std::runtime_error(msg.arg(*this).toStdString());
    }
    QLineF v = getResizeLine();
    QPointF topLeft = QPointF(v.p2().x(), v.p1().y());
    QPointF bottomRight = QPointF(v.p1().x(), v.p2().y());
    QRectF bottomLeftRect = QRectF(topLeft, bottomRight);
    return bottomLeftRect;
}

qreal ResizeLimits::getHorizontalDistance() const
{
    return m_horizontalDistance;
}

qreal ResizeLimits::getVerticalDistance() const
{
    return m_verticalDistance;
}

VerticalEdge ResizeLimits::getVerticalEdge() const
{
    return m_verticalEdge;
}

qreal ResizeLimits::getVerticalLimit()
{
    Q_ASSERT(m_verticalLimitSet);
    return m_verticalLimit;
}

bool isNotBetween(qreal value, qreal min, qreal max)
{
    return value < min || max < value;
}

void ResizeLimits::setVerticalLimit(qreal verticalLimit)
{
    if (isLeftEdgeMaximizing() &&  m_rect.left() < verticalLimit) {
        QString msg(
                "Limit for left-edge maximizing must be left of left-edge. "
                "Left-edge was %1 and verticalLimit was %2");
        //throw std::runtime_error(msg.arg(m_rect.left()).arg(verticalLimit).toStdString());
    }
    if (isRightEdgeMaximizing() && verticalLimit < m_rect.right() ) {
        QString msg("Limit for right-edge maximizing must be right of right-edge. "
                    "Right-edge is %1 and verticalLimit is %2");
        //throw std::runtime_error(msg.arg(m_rect.right()).arg(verticalLimit).toStdString());
    }
    if (isLeftEdgeMinimizing() && isNotBetween(verticalLimit, m_rect.left(), m_rect.right())) {
        QString msg("Limit for left-edge minimizing must be between left and right edge. "
                    "Left edge is %1 right edge is %2 and verticalLimit is %3");
        //throw std::runtime_error(msg.arg(m_rect.left()).arg(m_rect.right()).arg(verticalLimit).toStdString());
    }
    if (isRightEdgeMinimizing() && isNotBetween(verticalLimit, m_rect.left(), m_rect.right())) {
        QString msg("Limit for right-edge minimizing must be between left and right edge. "
                    "Left-edge is %1, right-edge is %2 and verticalLimit is %3");
        //throw std::runtime_error(msg.arg(m_rect.left()).arg(m_rect.right()).arg(verticalLimit).toStdString());
    }
    m_verticalLimitSet = true;
    m_verticalLimit = verticalLimit;
}

bool ResizeLimits::isVerticalLimitSet()
{
    return m_verticalLimitSet;
}

ResizeType ResizeLimits::getVerticalResizeType() const
{
    return m_verticalResizeType;
}

HorizontalEdge ResizeLimits::getHorizontalEdge() const
{
    return m_horizontalEdge;
}

qreal ResizeLimits::getHorizontalLimit()
{
    Q_ASSERT(m_horizontalLimitSet);
    return m_horizontalLimit;
}

void ResizeLimits::setHorizontalLimit(qreal horizontalLimit)
{
    // Sanity check
    if (isTopEdgeMaximizing() && m_rect.top() < horizontalLimit) {
        QString msg("Limit for top-edge maximizing must be located above top-edge (numerically less). "
                    "Top-edge was %1. HorizontalLimit was %2");
        //throw std::runtime_error(msg.arg(m_rect.top()).arg(horizontalLimit).toStdString());
    }
    if (isBottomEdgeMaximizing() && horizontalLimit < m_rect.bottom()) {
        QString msg("Limit for bottom-edge maximizing must be located below bottom-edge (numerically larger)."
                    " Bottom-edge was %1. HorizontalLimit was %2");
        //throw std::runtime_error(msg.arg(m_rect.bottom()).arg(horizontalLimit).toStdString());
    }
    if (isTopEdgeMinimizing() && isNotBetween(horizontalLimit, m_rect.top(), m_rect.bottom())) {
        QString msg("Limt for top-edge minimizing must be located between (or on) top-edge and bottom-edge."
                    " Top edge was %1. horizontalLimit was %2. bottom edge was %3");
        //throw std::runtime_error(msg.arg(m_rect.top()).arg(horizontalLimit).arg(m_rect.bottom()).toStdString());
    }
    if (isBottomEdgeMinimizing() && isNotBetween(horizontalLimit, m_rect.top(), m_rect.bottom())) {
        QString msg("Limit for bottom-edge minimizing must be located between (or on) top-edge and bottom-edge."
                    " Top-edge was %1. horizontalLimit was %2. bottom-edge was %3");
        //throw std::runtime_error(msg.arg(m_rect.top()).arg(horizontalLimit).arg(m_rect.bottom()).toStdString());
    }

    m_horizontalLimitSet = true;
    m_horizontalLimit = horizontalLimit;
}

bool ResizeLimits::isHorizontalLimitSet()
{
    return m_horizontalLimitSet;
}

ResizeType ResizeLimits::getHorizontalResizeType() const
{
    return m_horizontalResizeType;
}

bool ResizeLimits::isLeftEdgeMaximizing() const
{
    return m_verticalResizeType == ResizeType::Max && m_verticalEdge == VerticalEdge::LeftEdge;
}

bool ResizeLimits::isRightEdgeMaximizing() const
{
    return m_verticalResizeType == ResizeType::Max && m_verticalEdge == VerticalEdge::RightEdge;
}

bool ResizeLimits::isTopEdgeMaximizing() const
{
    return m_horizontalResizeType == ResizeType::Max && m_horizontalEdge == HorizontalEdge::TopEdge;
}

bool ResizeLimits::isBottomEdgeMaximizing() const
{
    return m_horizontalResizeType == ResizeType::Max && m_horizontalEdge == HorizontalEdge::BottomEdge;
}

bool ResizeLimits::isLeftEdgeMinimizing() const
{
    return m_verticalResizeType == ResizeType::Min && m_verticalEdge == VerticalEdge::LeftEdge;
}

bool ResizeLimits::isRightEdgeMinimizing() const
{
    return m_verticalResizeType == ResizeType::Min && m_verticalEdge == VerticalEdge::RightEdge;
}

bool ResizeLimits::isTopEdgeMinimizing() const
{
    return m_horizontalResizeType == ResizeType::Min && m_horizontalEdge == HorizontalEdge::TopEdge;
}

bool ResizeLimits::isBottomEdgeMinimizing() const
{
    return m_horizontalResizeType == ResizeType::Min && m_horizontalEdge == HorizontalEdge::BottomEdge;
}

QPair<HorizontalEdge, VerticalEdge> ResizeLimits::getIntersectedEdges(QRectF otherRect)
{
    // The movement
    QLineF resizeLine = getResizeLine();

    // The rect
    QLineF leftEdge(otherRect.topLeft(), otherRect.bottomLeft());
    QLineF topEdge(otherRect.topLeft(), otherRect.topRight());
    QLineF rightEdge(otherRect.topRight(), otherRect.bottomRight());
    QLineF bottomEdge(otherRect.bottomLeft(), otherRect.bottomRight());

    bool leftEdgeIntersects = resizeLine.intersects(leftEdge) == QLineF::IntersectType::BoundedIntersection;
    bool topEdgeIntersects = resizeLine.intersects(topEdge) == QLineF::IntersectType::BoundedIntersection;
    bool rightEdgeIntersects = resizeLine.intersects(rightEdge) == QLineF::IntersectType::BoundedIntersection;
    bool bottomEdgeIntersects = resizeLine.intersects(bottomEdge) == QLineF::IntersectType::BoundedIntersection;

    HorizontalEdge intersectedHorizontalEdge = HorizontalEdge::None;
    VerticalEdge intersectedVerticalEdge = VerticalEdge::None;

    if (leftEdgeIntersects) {
        intersectedVerticalEdge = VerticalEdge::LeftEdge;
    }
    if (topEdgeIntersects) {
        intersectedHorizontalEdge = HorizontalEdge::TopEdge;
    }
    if (rightEdgeIntersects) {
        intersectedVerticalEdge = VerticalEdge::RightEdge;
    }
    if (bottomEdgeIntersects) {
        intersectedHorizontalEdge = HorizontalEdge::BottomEdge;
    }
    return { intersectedHorizontalEdge, intersectedVerticalEdge };
}

QLineF ResizeLimits::getResizeLine() const
{
    QPointF startPoint; // One of the grippoints
    QPointF endPoint;
    if (m_gripPointLocation == GripPoint::TopLeft) {
        startPoint = m_rect.topLeft();
    }
    if (m_gripPointLocation == GripPoint::TopRight) {
        startPoint = m_rect.topRight();
    }
    if (m_gripPointLocation == GripPoint::BottomLeft) {
        startPoint = m_rect.bottomLeft();
    }
    if (m_gripPointLocation == GripPoint::BottomRight) {
        startPoint = m_rect.bottomRight();
    }
    if (m_gripPointLocation == GripPoint::Top) {
        startPoint.setX(m_rect.center().x());
        startPoint.setY(m_rect.top());
    }
    if (m_gripPointLocation == GripPoint::Left) {
        startPoint.setX(m_rect.left());
        startPoint.setY(m_rect.center().y());
    }
    if (m_gripPointLocation == GripPoint::Right) {
        startPoint.setX(m_rect.right());
        startPoint.setY((m_rect.center().y()));
    }
    if (m_gripPointLocation == GripPoint::Bottom) {
        startPoint.setX(m_rect.center().x());
        startPoint.setY(m_rect.bottom());
    }

    QPointF v(m_verticalDistance, m_horizontalDistance);
    endPoint = startPoint + v;
    QLineF resizeLine(startPoint, endPoint);
    return resizeLine;
}


ResizeLimits::operator QString() const
{
    // Original Rect
    QString originalRect = rect2string(m_rect);

    // GripPoint location
    QString gripPointLocation;
    switch (m_gripPointLocation) {
    case GripPoint::Top:
        gripPointLocation = "Top";
        break;
    case GripPoint::Left:
        gripPointLocation = "Left";
        break;
    case GripPoint::Bottom:
        gripPointLocation = "Bottom";
        break;
    case GripPoint::Right:
        gripPointLocation = "Right";
        break;
    case GripPoint::TopLeft:
        gripPointLocation = "TopLeft";
        break;
    case GripPoint::BottomLeft:
        gripPointLocation = "BottomLeft";
        break;
    case GripPoint::TopRight:
        gripPointLocation = "TopRight";
        break;
    case GripPoint::BottomRight:
        gripPointLocation = "BottomRigth";
        break;
    default:
        break;
    }

    QLineF v = getResizeLine();
    // From and To
    QString m("Movement: (start=(%1, %2) stop=(%3, %4))");
    QString movement = m.arg(v.p1().x()).arg(v.p1().y()).arg(v.p2().x()).arg(v.p2().y());

    // Limits
    QString limitsStr;

    // Horizontal Limits
    QString horizontalEdge;
    if (m_horizontalEdge == HorizontalEdge::TopEdge) {
        horizontalEdge = "TopEdge";
    }
    if (m_horizontalEdge == HorizontalEdge::BottomEdge) {
        horizontalEdge = "BottomEdge";
    }

    QString horizontalResizeType;
    switch (m_horizontalResizeType) {
    case ResizeType::Max:
        horizontalResizeType = "Max";
        break;
    case ResizeType::Min:
        horizontalResizeType = "Min";
        break;
    case ResizeType::None:
        horizontalResizeType = "None";
        break;
    }

    if (m_horizontalLimitSet) {
        QString horizontalLimit = QString::number(m_horizontalLimit);
        QString horizontalLimitStr("Horizontal: Edge=%1 Limit=%2");
        limitsStr.append(horizontalLimitStr.arg(horizontalEdge, horizontalLimit));
    } else {
        limitsStr.append(" (No horizontal limit)");
    }

    // Vertical Limits
    QString verticalEdge;
    if (m_verticalEdge == VerticalEdge::LeftEdge) {
        verticalEdge = "LeftEdge";
    }
    if (m_verticalEdge == VerticalEdge::RightEdge) {
        verticalEdge = "RightEdge";
    }

    QString verticalResizeType;
    switch (m_verticalResizeType) {
    case ResizeType::Max:
        verticalResizeType = "Max";
        break;
    case ResizeType::Min:
        verticalResizeType = "Min";
        break;
    case ResizeType::None:
        verticalResizeType = "None";
        break;
    }

    if (m_verticalLimitSet) {
        QString verticalLimit = QString::number(m_verticalLimit);
        QString verticalLimitsStr = QString("Vertical: Edge=%1 Limit=%2");
        limitsStr.append(verticalLimitsStr.arg(verticalEdge, verticalLimit));
    } else {
        limitsStr.append(" (No vertical limit)");
    }

    // ResizeType
    QString resizeTypeStr(" ResizeType: (vertical=%1, horizontal=%2)");
    resizeTypeStr = resizeTypeStr.arg(verticalResizeType, horizontalResizeType);

    // Distance
    QString distanceStr("horizontalDistance=%1 verticalDistance=%2");
    distanceStr = distanceStr.arg(getHorizontalDistance()).arg(getVerticalDistance());

    QString result("ResizeLimits: OriginalRect=%1  GripPoint=%2  %3 %4 %5 %6");
    QString resultStr = result.arg(originalRect, gripPointLocation, resizeTypeStr, movement, limitsStr, distanceStr);

    return resultStr;

}

}
}
