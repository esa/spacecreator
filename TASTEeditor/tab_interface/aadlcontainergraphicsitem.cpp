#include "aadlcontainergraphicsitem.h"

#include "tab_aadl/aadlobjectcontainer.h"

#include <QApplication>
#include <QPainter>

static const qreal kBorderWidth = 2;
static const QMarginsF kTextMargins = { 20, 20, 20, 20 };

namespace taste3 {
namespace aadl {

AADLContainerGraphicsItem::AADLContainerGraphicsItem(AADLObjectContainer *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
{
    setObjectName(QLatin1String("AADLContainerGraphicsItem"));
    setFlag(QGraphicsItem::ItemIsSelectable);
    QColor brushColor { QLatin1String("#a8a8a8") };
    if (auto parentContainer = qgraphicsitem_cast<AADLContainerGraphicsItem *>(parentItem()))
        brushColor = parentContainer->brush().color().darker();
    setBrush(brushColor);
    setPen(QPen(brushColor.darker(), 2));
    setFont(QFont(qApp->font().family(), 16, QFont::Bold, true));
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
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(
            boundingRect().adjusted(kBorderWidth / 2, kBorderWidth / 2, -kBorderWidth / 2, -kBorderWidth / 2));
    painter->setFont(font());
    painter->drawText(boundingRect().marginsRemoved(kTextMargins), Qt::AlignTop | Qt::AlignLeft, entity()->title());
    painter->restore();

    InteractiveObject::paint(painter, option, widget);
}

} // namespace aadl
} // namespace taste3
