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

#include "baseitems/textitem.h"
#include "commands/common/commandsstack.h"
#include "mscchart.h"

#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPointer>

namespace msc {

ChartItem::ChartItem(MscChart *chart, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_rectItem(new QGraphicsRectItem(this))
    , m_textItemMarker(new NameItem(this))
    , m_textItemName(new NameItem(this))
    , m_chart(chart)
{
    m_rectItem->setPen(QColor(Qt::black));
    m_rectItem->setBrush(Qt::white);

    connect(m_chart, &msc::MscChart::nameChanged, this, &ChartItem::setName);

    m_textItemMarker->setHtml("<b>msc</b>");

    m_textItemName->setEditable(true);
    m_textItemName->setTextWrapMode(QTextOption::NoWrap);
    connect(m_textItemName, &TextItem::edited, this, &ChartItem::onNameEdited);

    if (m_chart)
        setName(m_chart->name());
}

void ChartItem::onNameEdited(const QString &text)
{
    if (!m_chart)
        return;

    cmd::CommandsStack::push(cmd::RenameEntity, { QVariant::fromValue(m_chart), text });
    QMetaObject::invokeMethod(this, "updateBox", Qt::QueuedConnection);
}

ChartItem::~ChartItem() {}

QRectF ChartItem::boundingRect() const
{
    return m_rectItem->boundingRect();
}

QString ChartItem::chartName() const
{
    return m_chart ? m_chart->name() : QString();
}

QString ChartItem::chartNameGuiText() const
{
    return m_textItemName->toPlainText();
}

void ChartItem::setName(const QString &name)
{
    QString nameValidated(name.trimmed());
    if (nameValidated.isEmpty())
        nameValidated = MscEntity::DefaultName;

    if (chartNameGuiText() != nameValidated || m_chart->name() != nameValidated) {
        m_chart->setName(nameValidated);

        m_textItemName->setPlainText(nameValidated);
        m_textItemName->adjustSize();

        updateBox();
    }
}

void ChartItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF ChartItem::box() const
{
    return m_box;
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
    static constexpr qreal paddingHalf = 10.;

    const QRectF txtRect = m_textItemMarker->boundingRect().translated(m_textItemMarker->pos());
    const QRectF newRect = m_box.adjusted(-paddingHalf, -txtRect.height(), paddingHalf, paddingHalf);

    m_textItemMarker->setPos(newRect.topLeft());
    m_textItemName->setPos(m_textItemMarker->boundingRect().translated(m_textItemMarker->pos()).topRight());

    QRectF updatedBox = m_textItemMarker->boundingRect().translated(m_textItemMarker->pos());
    updatedBox = updatedBox.united(m_textItemName->boundingRect().translated(m_textItemName->pos()));
    updatedBox = updatedBox.united(newRect);
    m_rectItem->setRect(updatedBox);
    update();
}

} // ns msc
