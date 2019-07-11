/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/


#include "aadlcommentgraphicsitem.h"

#include "baseitems/textgraphicsitem.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QPainter>
#include <QtDebug>

static const qreal kBorderWidth = 2;
static const qreal kMargins = 14 + kBorderWidth;

namespace taste3 {
namespace aadl {

AADLCommentGraphicsItem::AADLCommentGraphicsItem(AADLObject *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
    , m_textItem(new TextGraphicsItem(this))
{
    setObjectName(QLatin1String("AADLCommentGraphicsItem"));
    setFlag(QGraphicsItem::ItemIsSelectable);

    m_textItem->setPlainText(entity->title());
    m_textItem->setFramed(false);
    m_textItem->setEditable(true);
    m_textItem->setBackgroundColor(Qt::transparent);
    m_textItem->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_textItem->setTextWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    connect(m_textItem, &TextGraphicsItem::edited, this, &AADLCommentGraphicsItem::textEdited);
    connect(m_textItem, &TextGraphicsItem::textChanged, this, [this]() {
        prepareGeometryChange();
        m_boundingRect = m_textItem->boundingRect();
        updateGripPoints();
    });

    const QColor brushColor { 0xf9e29c };
    setBrush(brushColor);
    setPen(QPen(brushColor.darker(), kBorderWidth, Qt::SolidLine, Qt::FlatCap));
    setFont(QFont(qApp->font()));
    m_textItem->setFont(font());

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
    painter->setPen(pen());
    painter->setBrush(brush());

    const QRectF br = mapRectFromItem(m_textItem, m_textItem->boundingRect());
    auto preparePolygon = [](const QRectF &rect) {
        return QVector<QPointF> { rect.topRight() - QPointF(kMargins, 0),
                                  rect.topLeft(),
                                  rect.bottomLeft(),
                                  rect.bottomRight(),
                                  rect.topRight() + QPointF(0, kMargins),
                                  rect.topRight() - QPointF(kMargins, 0) };
    };
    painter->drawPolygon(preparePolygon(br));

    auto preparePolyline = [](const QRectF &rect) {
        return QVector<QPointF> { rect.topRight() + QPointF(0, kMargins),
                                  rect.topRight() - QPointF(kMargins, -kMargins),
                                  rect.topRight() - QPointF(kMargins, 0) };
    };
    painter->drawPolyline(preparePolyline(br));

    painter->restore();
    InteractiveObject::paint(painter, option, widget);
}

void AADLCommentGraphicsItem::rebuildLayout()
{
    m_textItem->setExplicitSize(m_boundingRect.size());
}

} // namespace aadl
} // namespace taste3
