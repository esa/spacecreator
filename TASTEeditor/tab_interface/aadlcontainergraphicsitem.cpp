#include "aadlcontainergraphicsitem.h"

#include "tab_aadl/aadlobjectcontainer.h"

#include <QPainter>

static const qreal kBorderWidth = 2;
static const QMarginsF kTextMargins = { 20, 20, 20, 20 };

namespace taste3 {
namespace aadl {

AADLContainerGraphicsItem::AADLContainerGraphicsItem(AADLObjectContainer *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
{
    setObjectName(QLatin1String("AADLContainerGraphicsItem"));
}

AADLObjectContainer *AADLContainerGraphicsItem::entity() const
{
    return qobject_cast<AADLObjectContainer *>(m_entity);
}

void AADLContainerGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QPen pen = painter->pen();
    pen.setWidthF(kBorderWidth);
    painter->setPen(pen);
    painter->setBrush(QColor(0xf9e29c));
    painter->drawRect(
            boundingRect().adjusted(kBorderWidth / 2, kBorderWidth / 2, -kBorderWidth / 2, -kBorderWidth / 2));
    auto font = painter->font();
    font.setPointSizeF(16);
    font.setBold(true);
    font.setItalic(true);
    painter->setFont(font);
    painter->drawText(boundingRect().marginsAdded(kTextMargins), Qt::AlignTop | Qt::AlignLeft, entity()->title());
    painter->restore();

    InteractiveObject::paint(painter, option, widget);
}

} // namespace aadl
} // namespace taste3
