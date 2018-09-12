#include "instanceitem.h"

#include <QBrush>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

namespace msc {

static const double END_SYMBOL_HEIGHT = 10.0;

InstanceItem::InstanceItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_headSymbol(new QGraphicsRectItem(this))
    , m_nameItem(new QGraphicsTextItem(this))
    , m_axisSymbol(new QGraphicsLineItem(this))
    , m_endSymbol(new QGraphicsRectItem(this))
{
    m_endSymbol->setBrush(QBrush(Qt::black));
}

QRectF InstanceItem::boundingRect() const
{
    QRectF rect = childrenBoundingRect();
    rect.moveTo(pos());
    return rect;
}

void InstanceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

double InstanceItem::horizontalCenter() const
{
    return x() + m_axisSymbol->line().x1();
}

void InstanceItem::setName(const QString &name)
{
    m_nameItem->setPlainText(name);

    QRectF nameRect = m_nameItem->boundingRect();
    m_headSymbol->setRect(nameRect);

    double x = nameRect.center().x();
    double y = nameRect.bottom();
    m_axisSymbol->setLine(x, y, x, y + 120.0);

    x = nameRect.left();
    y = m_axisSymbol->boundingRect().bottom();
    m_endSymbol->setRect(x, y, nameRect.width(), END_SYMBOL_HEIGHT);

    emit horizontalCenterChanged();
    prepareGeometryChange();
    update();
}

QVariant InstanceItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        emit horizontalCenterChanged();
    }

    return QGraphicsObject::itemChange(change, value);
}

} // namespace msc
