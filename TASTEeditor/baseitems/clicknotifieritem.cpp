#include "clicknotifieritem.h"

namespace taste3 {

ClickNotifierItem::ClickNotifierItem(QObject *obj, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_dataObject(obj)
{
}

QObject *ClickNotifierItem::dataObject() const
{
    return m_dataObject;
}

void ClickNotifierItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
    QGraphicsObject::mouseReleaseEvent(event);
}

void ClickNotifierItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit doubleClicked();
    QGraphicsObject::mouseDoubleClickEvent(event);
}

} // namespace taste3
