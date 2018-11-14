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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "grippointshandler.h"
#include "common/utils.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QMetaEnum>

namespace msc {

/*!
  \class msc::GripPointsHandler
  \brief Set of handlers (up to 9) for resizing/moving item by mouse.

  \inmodule MscWidgets

*/

const QMap<GripPoint::Location, GripPoint *> createGripPoints(GripPointsHandler *parent)
{
    QMap<GripPoint::Location, GripPoint *> result;

    const QMetaEnum e = QMetaEnum::fromType<msc::GripPoint::Location>();
    for (int i = 0; i < e.keyCount(); ++i)
        result.insert(GripPoint::Location(e.value(i)), new GripPoint(GripPoint::Location(e.value(i)), parent));

    return result;
}

GripPoint::Locations initUsedPoints()
{
    GripPoint::Locations result;

    const QMetaEnum e = QMetaEnum::fromType<msc::GripPoint::Location>();
    for (int i = 0; i < e.keyCount(); ++i)
        result.insert(GripPoint::Location(e.value(i)));

    return result;
}

GripPointsHandler::GripPointsHandler(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , AbstractInteractiveObject()
    , m_gripPoints(createGripPoints(this))
    , m_usedPoints(initUsedPoints())
{
    setFlags(QGraphicsItem::ItemSendsGeometryChanges);
    hide();
}

void GripPointsHandler::setUsedPoints(GripPoint::Locations points)
{
    if (m_usedPoints == points)
        return;

    m_usedPoints = points;
    updateLayout();
}

GripPoint::Locations GripPointsHandler::usedPoints() const
{
    return m_usedPoints;
}

QVector<GripPoint *> GripPointsHandler::gripPoints() const
{
    return m_gripPoints.values().toVector();
}

GripPoint *GripPointsHandler::gripPoint(GripPoint::Location pnt) const
{
    return m_gripPoints.contains(pnt) ? m_gripPoints.value(pnt) : nullptr;
}

void GripPointsHandler::updateLayout()
{
    for (GripPoint *gp : m_gripPoints) {
        const bool used = m_usedPoints.contains(gp->location());
        gp->setIsUsed(used);
        if (used)
            gp->updateLayout();
    }

    const QRectF &bounds = boundingRect();
    m_borderPath = QPainterPath();
    m_borderPath.addRect(bounds);
    const qreal clip = m_highlighter.borderWidth() * 2.;
    const QRectF bodyRect = bounds.adjusted(clip, clip, -clip, -clip);
    m_borderPath.addRect(bodyRect);

    m_bodyPath = QPainterPath();
    m_bodyPath.addRect(bodyRect);
}

QRectF GripPointsHandler::boundingRect() const
{
    return parentItem() ? parentItem()->boundingRect() : QRectF();
}

QVariant GripPointsHandler::itemChange(GraphicsItemChange change,
                                       const QVariant &value)
{
    switch (change) {
    case ItemVisibleChange: {
        if (value.toBool())
            updateLayout();
        break;
    }
    default:
        break;
    }
    return QGraphicsItem::itemChange(change, value);
}

void GripPointsHandler::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *option,
                              QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->fillPath(m_bodyPath, m_bodyBrush);
    painter->fillPath(m_borderPath, m_highlighter.bodyColor());
    painter->strokePath(m_borderPath, m_highlighter.borderColor());
}

void GripPointsHandler::handleGripPointMove(GripPoint *handle,
                                            const QPointF &from, const QPointF &to)
{
    Q_EMIT rectChanged(handle->location(), from, to);
}

void GripPointsHandler::showAnimated()
{
    changeVisibilityAnimated(true);
}

void GripPointsHandler::hideAnimated()
{
    changeVisibilityAnimated(false);
}

void GripPointsHandler::onVisibilityFinished()
{
    if (!m_visible)
        setVisible(false);
}

void GripPointsHandler::changeVisibilityAnimated(bool appear)
{
    m_visible = appear;
    const qreal from = m_visible ? 0. : 1.;
    const qreal to = m_visible ? 1. : 0.;
    const int duration = m_visible ? 250. : 750.;
    setVisible(true);

    if (QPropertyAnimation *anim =
                utils::createLinearAnimation(this, "opacity", from, to, duration)) {
        if (m_visible)
            connect(anim, &QPropertyAnimation::finished, this,
                    &GripPointsHandler::onVisibilityFinished);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

QSizeF GripPointsHandler::minSize() const
{
    static const qreal SpanBetweenTwoGrips = 2.;
    const qreal oneSide = GripPoint::sideSize() * 3. + SpanBetweenTwoGrips * 2.;
    return { oneSide, oneSide };
}

} // namespace msc
