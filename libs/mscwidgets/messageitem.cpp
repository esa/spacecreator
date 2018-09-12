#include "messageitem.h"

#include <QBrush>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QPolygonF>

namespace msc {

static const double ARROW_HEIGHT = 10.0;
static const double ARROW_WIDTH = 20.0;

MessageItem::MessageItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_line(new QGraphicsLineItem(this))
    , m_arrow(new QGraphicsPolygonItem(this))
    , m_nameItem(new QGraphicsTextItem(this))
{
    m_arrow->setBrush(QBrush(Qt::black));
    QPolygonF polygon;
    polygon.append(QPointF(0.0, 0.0));
    polygon.append(QPointF(ARROW_WIDTH, ARROW_HEIGHT / 2.0));
    polygon.append(QPointF(0.0, ARROW_HEIGHT));
    m_arrow->setPolygon(polygon);
}

void MessageItem::setWidth(double width)
{
    m_arrow->setPos(width - ARROW_WIDTH, m_nameItem->boundingRect().height());
    double y = m_nameItem->boundingRect().height() + ARROW_HEIGHT / 2.0;
    m_line->setLine(0.0, y, width, y);

    centerName();

    prepareGeometryChange();
    update();
}

QRectF MessageItem::boundingRect() const
{
    QRectF rect = childrenBoundingRect();
    rect.moveTo(pos());
    return rect;
}

void MessageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void MessageItem::setName(const QString &name)
{
    m_nameItem->setPlainText(name);
    centerName();

    prepareGeometryChange();
    update();
}

void MessageItem::centerName()
{
    m_nameItem->setX((m_line->boundingRect().width() - m_nameItem->boundingRect().width()) / 2.0);
}

} // namespace mas
