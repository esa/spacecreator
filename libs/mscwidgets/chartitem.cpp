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

static const QSizeF minChartSize { 200.0, 200.0 };

ChartItem::ChartItem(MscChart *chart, QGraphicsItem *parent)
    : InteractiveObject(chart, parent)
    , m_rectItem(new QGraphicsRectItem(this))
    , m_textItemName(new NameItem(this))
{
    m_rectItem->setPen(QColor(Qt::black));
    m_rectItem->setBrush(Qt::white);

    m_textItemName->setEditable(true);
    m_textItemName->setTextWrapMode(QTextOption::NoWrap);
    connect(m_textItemName, &TextItem::edited, this, &ChartItem::onNameEdited);

    if (chart) {
        connect(chart, &msc::MscChart::nameChanged, this, &ChartItem::setName);
        connect(chart, &msc::MscChart::rectChanged, this, &ChartItem::cifChanged);

        setName(chart->name());
    }
}

ChartItem::~ChartItem() {}

void ChartItem::onNameEdited(const QString &text)
{
    if (!m_entity)
        return;

    cmd::CommandsStack::push(cmd::RenameEntity, { QVariant::fromValue(m_entity.data()), text });
}

QRectF ChartItem::boundingRect() const
{
    return m_textItemName->sceneBoundingRect() | m_rectItem->boundingRect();
}

QString ChartItem::chartName() const
{
    return m_entity ? m_entity->name() : QString();
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

    if (chartNameGuiText() != nameValidated || m_entity->name() != nameValidated) {
        m_entity->setName(nameValidated);

        m_textItemName->setPlainText(nameValidated);
        m_textItemName->adjustSize();

        updateTitlePos();
    }
}

void ChartItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

bool sceneToCif(const QRectF sceneRect, QRect &cifRect)
{
    const QVector<QPointF> scenePoints { sceneRect.topLeft(), sceneRect.bottomRight() };
    bool converted(false);
    const QVector<QPoint> cifPoints = utils::CoordinatesConverter::sceneToCif(scenePoints, &converted);
    if (converted)
        cifRect = { cifPoints.first(), cifPoints.last() };
    return converted;
}

bool cifToScene(const QRect &cifRect, QRectF &sceneRect)
{
    const QVector<QPoint> cifPoints { cifRect.topLeft(), cifRect.bottomRight() };
    bool converted(false);
    const QVector<QPointF> scenePoints = utils::CoordinatesConverter::cifToScene(cifPoints, &converted);
    if (converted)
        sceneRect = { scenePoints.first(), scenePoints.last() };
    return converted;
}

void ChartItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    const QPoint shift = QPointF(to - from).toPoint();
    QRect rect = m_rectItem->rect().toRect();
    switch (gp->location()) {
    case GripPoint::Left:
        rect.setLeft(rect.left() + shift.x());
        break;
    case GripPoint::Top:
        rect.setTop(rect.top() + shift.y());
        break;
    case GripPoint::Right:
        rect.setRight(rect.right() + shift.x());
        break;
    case GripPoint::Bottom:
        rect.setBottom(rect.bottom() + shift.y());
        break;
    case GripPoint::TopLeft:
        rect.setTopLeft(rect.topLeft() + shift);
        break;
    case GripPoint::TopRight:
        rect.setTopRight(rect.topRight() + shift);
        break;
    case GripPoint::BottomLeft:
        rect.setBottomLeft(rect.bottomLeft() + shift);
        break;
    case GripPoint::BottomRight:
        rect.setBottomRight(rect.bottomRight() + shift);
        break;
    default:
        qWarning() << "Update grip point handling";
        break;
    }

    QRect oldRect;
    if (geometryManagedByCif()) {
        oldRect = chart()->cifRect();
    } else if (!sceneToCif(m_box, oldRect)) {
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << oldRect;
        return;
    }

    QRect newRect;
    if (sceneToCif(rect, newRect)) {
        msc::cmd::CommandsStack::push(msc::cmd::ChangeChartGeometry,
                                      { oldRect, newRect, QVariant::fromValue<MscChart *>(chart()) });
        updateGripPoints();
        updateCif();
    } else {
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << rect;
    }
}

void ChartItem::prepareHoverMark()
{
    InteractiveObject::prepareHoverMark();
    m_gripPoints->setUsedPoints(GripPoint::Locations { GripPoint::Left, GripPoint::Top, GripPoint::Right,
                                                       GripPoint::Bottom, GripPoint::TopLeft, GripPoint::BottomLeft,
                                                       GripPoint::TopRight, GripPoint::BottomRight });
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
    QRectF newBox = r;
    const QSizeF s = r.size().expandedTo(minChartSize).expandedTo(m_textItemName->boundingRect().size());
    newBox.setSize(s);

    if (m_box == newBox)
        return;

    if (m_guard)
        return;
    m_guard = true;

    m_box = newBox;
    m_rectItem->setRect(m_box);

    if (QGraphicsScene *pScene = scene())
        pScene->setSceneRect(sceneBoundingRect());
    m_guard = false;

    updateTitlePos();

    Q_EMIT chartBoxChanged();
}

const QMarginsF &ChartItem::chartMargins()
{
    static constexpr qreal margin { 20.0 };
    static constexpr QMarginsF margins { margin, margin, margin, margin };
    return margins;
}

void ChartItem::updateTitlePos()
{
    m_textItemName->setPos(m_box.topLeft());
}

void ChartItem::applyCif()
{
    if (!m_entity)
        return;

    const QRectF &storedRect = storedCustomRect();
    if (!storedRect.isNull())
        setBox(storedRect);
}

void ChartItem::updateCif()
{
    if (!m_entity)
        return;

    const QRectF &storedRect = storedCustomRect();
    if (storedRect == m_box)
        return;

    QRect cifRect;
    if (!sceneToCif(m_box, cifRect))
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << cifRect;

    chart()->setCifRect(cifRect);
}

MscChart *ChartItem::chart() const
{
    return qobject_cast<MscChart *>(m_entity);
}

bool ChartItem::geometryManagedByCif() const
{
    return chart() && !chart()->cifRect().isNull();
}

QRectF ChartItem::storedCustomRect() const
{
    if (!m_entity)
        return {};

    const QRect &cifRect = chart()->cifRect();
    if (cifRect.isNull())
        return {};

    QRectF rect;
    if (!cifToScene(cifRect, rect))
        qWarning() << "ChartItem: Coordinates conversion (mm->scene) failed" << cifRect;

    return rect;
}

} // ns msc
