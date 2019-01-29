/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "chartitem.h"
#include "mscchart.h"
#include "baseitems/textitem.h"

#include <QPointer>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>

#include <QDebug>
#define LOG qDebug()

namespace msc {

ChartItem::ChartItem(MscChart *chart, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_rectItem(new QGraphicsRectItem(this))
    , m_textItem(new TextItem(this))
    , m_chart(chart)
{
    m_rectItem->setPen(QColor(Qt::black));
    m_rectItem->setBrush(Qt::white);

    if (m_chart)
        setName(m_chart->name());

    connect(m_chart, &msc::MscChart::nameChanged, this, &ChartItem::setName);
}

ChartItem::~ChartItem() {}

QRectF ChartItem::boundingRect() const
{
    return m_rectItem->boundingRect();
}

QSizeF ChartItem::textSize(const QFont &font) const
{
    QSizeF size;
    if (m_chart) {
        QFontMetricsF fm(font);
        size = fm.size(0, m_chart->name());
    }
    return size;
}

QString ChartItem::chartName() const
{
    return m_chart ? m_chart->name() : QString();
}

QString ChartItem::chartNameGuiText() const
{
    return m_textItem->toPlainText();
}

void ChartItem::setName(const QString &name)
{
    QString nameValidated(name.trimmed());
    if (nameValidated.isEmpty())
        nameValidated = MscEntity::DefaultName;

    const QString newName = QString("<b>msc</b> %1").arg(nameValidated);

    if (m_textItem->toHtml() != newName || m_chart->name() != nameValidated) {
        m_chart->setName(nameValidated);

        m_textItem->setHtml(newName);
        m_textItem->adjustSize();

        updateBox();
    }
}

void ChartItem::paint(QPainter * /*painter*/, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/) {}

QRectF ChartItem::box() const
{
    return m_box; // m_rectItem->rect();
}

void ChartItem::setBox(const QRectF &r)
{
    if (m_box == r)
        return;
    m_box = r;
    updateBox();
}

void ChartItem::updateBox()
{
    static constexpr qreal paddingHalth = 10.;

    const QRectF txtRect = m_textItem->boundingRect().translated(m_textItem->pos());
    const QRectF newRect =
            m_box.adjusted(-paddingHalth, -(2 * paddingHalth + txtRect.height()), paddingHalth, paddingHalth);

    m_rectItem->setRect(newRect);
    m_textItem->setPos(newRect.topLeft() + QPointF(paddingHalth, paddingHalth));

    update();
}

} // ns msc
