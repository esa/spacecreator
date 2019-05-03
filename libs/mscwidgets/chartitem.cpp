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
#include "baseitems/common/utils.h"
#include "baseitems/textitem.h"
#include "commands/common/commandsstack.h"
#include "mscchart.h"

#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPointer>

namespace msc {

QPointF ChartItem::m_margin = { 20., 20. };

ChartItem::ChartItem(MscChart *chart, ChartViewModel *chartViewModel, QGraphicsItem *parent)
    : InteractiveObject(chart, parent)
    , m_chartViewModel(chartViewModel)
    , m_rectItem(new QGraphicsRectItem(this))
    , m_contentArea(new QGraphicsRectItem(this))
    , m_textItemName(new NameItem(this))
{
    QPen framePen = m_rectItem->pen();
    framePen.setColor(Qt::black);
    framePen.setWidthF(1.);
    m_rectItem->setPen(framePen);
    m_rectItem->setBrush(Qt::white);

    m_contentArea->setPen(QColor(Qt::white));
    m_contentArea->setBrush(Qt::white);

    m_textItemName->setEditable(true);
    m_textItemName->setTextWrapMode(QTextOption::NoWrap);
    m_textItemName->setBackgroundColor(Qt::transparent);
    connect(m_textItemName, &TextItem::edited, this, &ChartItem::onNameEdited);

    if (chart) {
        connect(chart, &msc::MscChart::nameChanged, this, &ChartItem::setName);
        connect(chart, &msc::MscChart::rectChanged, this, &ChartItem::onChartCifRectChanged);
        connect(chart, &msc::MscChart::dataChanged, this, &ChartItem::cifChanged);

        setName(chart->name());
    }
}

void ChartItem::onNameEdited(const QString &text)
{
    if (!m_entity)
        return;

    cmd::CommandsStack::push(cmd::RenameEntity, { QVariant::fromValue(m_entity.data()), text });
}

QRectF ChartItem::boundingRect() const
{
    return utils::framedRect(m_rectItem->boundingRect(), m_rectItem->pen().widthF());
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

void ChartItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (m_prevContentRect.isNull())
        m_prevContentRect = contentRect();

    if (from == to)
        return;

    const QPointF shift = QPointF(to - from);
    QRectF rect = contentRect();
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

    if (m_chartViewModel) {
        const QRectF &minContentRect = m_chartViewModel->minimalContentRect();
        rect.setWidth(qMax(minContentRect.width(), rect.width()));
        rect.setHeight(qMax(minContentRect.height(), rect.height()));
    }

    QSignalBlocker suppressChartBoxChanged(this);
    setContentRect(rect, utils::CifUpdatePolicy::DontChange);
}

void ChartItem::onManualGeometryChangeFinished(GripPoint::Location, const QPointF &, const QPointF &)
{
    QRect cifRectPrev;
    if (!utils::CoordinatesConverter::sceneToCif(m_prevContentRect, cifRectPrev))
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << m_prevContentRect;

    const QRectF &chartBox = contentRect();
    QRect cifRectCurr;
    if (!utils::CoordinatesConverter::sceneToCif(chartBox, cifRectCurr))
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << chartBox;

    const QVariantList params { cifRectPrev, cifRectCurr, QVariant::fromValue<MscChart *>(chart()) };
    msc::cmd::CommandsStack::push(msc::cmd::ChangeChartGeometry, params);
    m_prevContentRect = QRectF();
}

void ChartItem::prepareHoverMark()
{
    InteractiveObject::prepareHoverMark();
    m_gripPoints->setUsedPoints(GripPoint::Locations { GripPoint::Left, GripPoint::Top, GripPoint::Right,
                                                       GripPoint::Bottom, GripPoint::TopLeft, GripPoint::BottomLeft,
                                                       GripPoint::TopRight, GripPoint::BottomRight });
    connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeFinish, this,
            &ChartItem::onManualGeometryChangeFinished, Qt::UniqueConnection);
}

void ChartItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF ChartItem::contentRect() const
{
    return m_contentArea->rect();
}

QPointF ChartItem::setContentRect(const QRectF &r, utils::CifUpdatePolicy cifUpdate)
{
    const QRectF &currContentRect = contentRect();
    const QPointF &shift = -r.topLeft();
    const QRectF &newContentRect = r.translated(shift);
    if (currContentRect == newContentRect)
        return QPointF();

    if (m_guard)
        return QPointF();

    m_guard = true;

    prepareGeometryChange();

    m_contentArea->setRect(newContentRect);
    m_rectItem->setRect(newContentRect.marginsAdded(chartMargins()));

    if (QGraphicsScene *pScene = scene())
        pScene->setSceneRect(sceneBoundingRect().marginsAdded(chartMargins()));

    updateTitlePos();

    updateGripPoints();

    updateCifIfNecessary(cifUpdate);

    m_guard = false;

    Q_EMIT contentRectChanged(contentRect());
    return shift;
}

QPointF ChartItem::chartMargin()
{
    return m_margin;
}

QMarginsF ChartItem::chartMargins()
{
    return { m_margin.x(), m_margin.y(), m_margin.x(), m_margin.y() };
}

void ChartItem::updateTitlePos()
{
    QRectF txtRect(m_textItemName->sceneBoundingRect());
    txtRect.moveTopLeft(m_rectItem->boundingRect().topLeft());
    m_textItemName->setPos(txtRect.topLeft());
    m_margin.ry() = txtRect.height();
}

void ChartItem::applyCif()
{
    if (!m_entity)
        return;

    const QRectF &storedRect = storedCustomRect();
    if (!storedRect.isNull())
        setContentRect(storedRect);
}

void ChartItem::updateCif()
{
    if (!m_entity)
        return;

    const QRectF &storedRect = storedCustomRect();
    const QRectF &currentContentRect = contentRect();
    if (storedRect == currentContentRect)
        return;

    QRect cifRect;
    if (!utils::CoordinatesConverter::sceneToCif(currentContentRect, cifRect))
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
    if (!utils::CoordinatesConverter::cifToScene(cifRect, rect))
        qWarning() << "ChartItem: Coordinates conversion (mm->scene) failed" << cifRect;

    return rect;
}

QPainterPath ChartItem::shape() const
{
    const QRectF &rect = boundingRect();
    const QVector<QLineF> lines { { rect.topLeft(), rect.topRight() },
                                  { rect.topRight(), rect.bottomRight() },
                                  { rect.bottomRight(), rect.bottomLeft() },
                                  { rect.bottomLeft(), rect.topLeft() } };
    QPainterPath result;
    for (const QLineF &line : lines)
        result.addPath(utils::lineShape(line, utils::LineHoverTolerance));
    return result;
}

void ChartItem::updateCifIfNecessary(utils::CifUpdatePolicy cause)
{
    bool writeCif(false);
    switch (cause) {
    case utils::CifUpdatePolicy::DontChange: {
        break;
    }
    case utils::CifUpdatePolicy::UpdateIfExists: {
        writeCif = geometryManagedByCif();
        break;
    }
    case utils::CifUpdatePolicy::ForceCreate: {
        writeCif = true;
        break;
    }
    }
    if (writeCif)
        updateCif();
}

void ChartItem::onChartCifRectChanged()
{
    applyCif();
    Q_EMIT cifChanged();

    // if it's a result of manual resizing, the following signal wouldn't be fired
    // within applyCif->setContentRect chain because the rect is the same
    Q_EMIT contentRectChanged(contentRect());
}

} // ns msc
