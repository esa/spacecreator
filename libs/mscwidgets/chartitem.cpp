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

#include "chartitem.h"

#include "baseitems/common/coordinatesconverter.h"
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
    , m_textItemName(new NameItem(this))
    , m_chart(chart)
{
    m_rectItem->setPen(QColor(Qt::black));
    m_rectItem->setBrush(Qt::white);

    connect(m_chart, &msc::MscChart::nameChanged, this, &ChartItem::setName);

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
}

ChartItem::~ChartItem() {}

QRectF ChartItem::boundingRect() const
{
    return m_textItemName->boundingRect() | m_rectItem->boundingRect();
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

        updateTitlePos();
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
    if (m_box == r) {
        updateCif();
        return;
    }

    m_box = r;
    m_rectItem->setRect(m_box);

    if (QGraphicsScene *pScene = scene())
        pScene->setSceneRect(boundingRect());

    updateTitlePos();
    updateCif();
}

void ChartItem::updateTitlePos()
{
    m_textItemName->setPos(m_box.topLeft());
}

void ChartItem::applyCif()
{
    if (!m_chart)
        return;

    const QRectF &storedRect = storedCustomRect();
    if (!storedRect.isNull())
        setBox(storedRect);
}

void ChartItem::updateCif()
{
    if (!m_chart)
        return;

    const QRectF &storedRect = storedCustomRect();
    if (storedRect == m_box)
        return;

    const QVector<QPointF> scenePoints { m_box.topLeft(), m_box.bottomRight() };
    bool converted(false);
    const QVector<QPoint> &cifPoints = utils::CoordinatesConverter::sceneToCif(scenePoints, &converted);
    if (!converted) {
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << scenePoints;
        return;
    }

    const QRect cifRect { cifPoints.first(), cifPoints.last() };
    m_chart->setCifRect(cifRect);
}

bool ChartItem::geometryManagedByCif() const
{
    return !m_chart->cifRect().isNull();
}

QRectF ChartItem::storedCustomRect() const
{
    if (!m_chart)
        return {};

    const QRect &cifRect = m_chart->cifRect();
    if (cifRect.isNull())
        return {};

    bool topLeftConverted(false);
    const QPointF &topLeft = utils::CoordinatesConverter::cifToScene(cifRect.topLeft(), &topLeftConverted);
    bool bottomRightConverted(false);
    const QPointF &bottomRight =
            utils::CoordinatesConverter::cifToScene(cifRect.bottomRight() - cifRect.topLeft(), &bottomRightConverted);

    if (!topLeftConverted || !bottomRightConverted) {
        qWarning() << "ChartItem: Coordinates conversion (mm->scene) failed" << cifRect;
        return {};
    }

    return { mapFromScene(topLeft), mapFromScene(bottomRight) };
}

} // ns msc
