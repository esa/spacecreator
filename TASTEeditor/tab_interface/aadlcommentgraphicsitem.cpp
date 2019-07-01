#include "aadlcommentgraphicsitem.h"

#include "baseitems/textgraphicsitem.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QtDebug>

static const qreal kBorderWidth = 1;
static const qreal kMargins = 6 + kBorderWidth;

namespace taste3 {
namespace aadl {

AADLCommentGraphicsItem::AADLCommentGraphicsItem(QObject *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
    , m_textItem(new TextGraphicsItem(this))
{
    setObjectName(QLatin1String("AADLCommentGraphicsItem"));
    setFlag(QGraphicsItem::ItemIsSelectable);

    m_textItem->setFramed(false);
    m_textItem->setEditable(true);
    m_textItem->setBackgroundColor(Qt::transparent);
    m_textItem->setTextAllignment(Qt::AlignCenter);
    m_textItem->setTextWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    connect(m_textItem, &TextGraphicsItem::edited, this, &AADLCommentGraphicsItem::textEdited);
    connect(m_textItem, &TextGraphicsItem::textChanged, this, [this]() {
        prepareGeometryChange();
        m_boundingRect = m_textItem->boundingRect();
        updateGripPoints();
    });

    setHighlightable(false);
}

void AADLCommentGraphicsItem::onManualResizeProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to)
{
    InteractiveObject::onManualResizeProgress(grip, from, to);
    m_textItem->setExplicitSize(m_boundingRect.size());
}

void AADLCommentGraphicsItem::textEdited(const QString &text)
{
    Q_UNUSED(text);

    rebuildLayout();
    updateGripPoints();

    Q_EMIT needUpdateLayout();
}

void AADLCommentGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    m_textItem->enableEditMode();
    InteractiveObject::mouseDoubleClickEvent(event);
}

void AADLCommentGraphicsItem::setText(const QString &text)
{
    if (m_textItem->toPlainText() == text)
        return;

    m_textItem->setTextWidth(150);
    m_textItem->setPlainText(text);
    m_textItem->setTextWidth(m_textItem->idealWidth());

    instantLayoutUpdate();
}

QString AADLCommentGraphicsItem::text() const
{
    return m_textItem->toPlainText();
}

void AADLCommentGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QPen pen = painter->pen();
    pen.setWidthF(kBorderWidth);
    pen.setCapStyle(Qt::FlatCap);
    painter->setPen(pen);
    const QRectF br = mapRectFromItem(m_textItem, m_textItem->boundingRect());
    painter->setBrush(QColor(0xf9e29c));
    painter->drawPolygon(QVector<QPointF> { br.topRight() - QPointF(kMargins, 0), br.topLeft(), br.bottomLeft(),
                                            br.bottomRight(), br.topRight() + QPointF(0, kMargins),
                                            br.topRight() - QPointF(kMargins, 0) });
    painter->drawPolyline(QVector<QPointF> { br.topRight() + QPointF(0, kMargins),
                                             br.topRight() - QPointF(kMargins, -kMargins),
                                             br.topRight() - QPointF(kMargins, 0) });
    painter->restore();

    InteractiveObject::paint(painter, option, widget);
}

void AADLCommentGraphicsItem::rebuildLayout()
{
    const QRectF br = m_textItem->sceneBoundingRect();
    setPos(br.topLeft());
    m_textItem->setExplicitSize(br.size());

    prepareGeometryChange();
    m_boundingRect = m_textItem->boundingRect();
}

} // namespace aadl
} // namespace taste3
