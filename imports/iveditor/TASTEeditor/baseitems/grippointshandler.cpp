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
  along with this program. If not, see
  <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "grippointshandler.h"

#include "common/utils.h"

#include <QDebug>
#include <QGraphicsView>
#include <QMetaEnum>
#include <QPainter>
#include <QPropertyAnimation>

namespace aadlinterface {

GripPointsHandler::GripPointsHandler(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , AbstractInteractiveObject()
{
    setFlags(QGraphicsItem::ItemIgnoresTransformations | QGraphicsItem::ItemSendsGeometryChanges);
    hide();
}

GripPoint *GripPointsHandler::createGripPoint(GripPoint::Location location, int idx)
{
    auto grip = new GripPoint(location, this);
    m_usedPoints.insert(grip->location());
    m_gripPoints.insert(idx == -1 ? m_gripPoints.size() : idx, grip);
    return grip;
}

void GripPointsHandler::removeGripPoint(GripPoint *handle)
{
    m_gripPoints.removeOne(handle);
    delete handle;
}

GripPoint::Locations GripPointsHandler::usedPoints() const
{
    return m_usedPoints;
}

QList<GripPoint *> GripPointsHandler::gripPoints() const
{
    return m_gripPoints;
}

void GripPointsHandler::updateLayout()
{
    for (GripPoint *handle : m_gripPoints) {
        const bool used = m_usedPoints.contains(handle->location());
        handle->setIsUsed(used);
        if (used)
            handle->updateLayout();
    }
}

QRectF GripPointsHandler::boundingRect() const
{
    QRectF bounds;
    if (QGraphicsItem *parent = parentItem()) {
        bounds = parent->boundingRect();

        const QPointF &scaleFactor(viewScale());
        bounds = { bounds.topLeft().x() * scaleFactor.x(), bounds.topLeft().y() * scaleFactor.y(),
                   bounds.width() * scaleFactor.x(), bounds.height() * scaleFactor.y() };
    }
    return bounds;
}

void GripPointsHandler::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void GripPointsHandler::handleGripPointPress(GripPoint *handle, const QPointF &at)
{
    Q_EMIT manualGeometryChangeStart(handle, at);
}

void GripPointsHandler::handleGripPointMove(GripPoint *handle, const QPointF &from, const QPointF &to)
{
    Q_EMIT manualGeometryChangeProgress(handle, from, to);
}

void GripPointsHandler::handleGripPointRelease(GripPoint *handle, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_EMIT manualGeometryChangeFinish(handle, pressedAt, releasedAt);
}

void GripPointsHandler::showAnimated()
{
    changeVisibilityAnimated(true);
}

void GripPointsHandler::hideAnimated()
{
    changeVisibilityAnimated(false);
}

void GripPointsHandler::onOpacityAnimationFinished()
{
    if (!m_visible)
        setVisible(false);
}

void GripPointsHandler::changeVisibilityAnimated(bool appear)
{
    m_visible = appear;
    const qreal from = m_visible ? 0. : 1.;
    const qreal to = m_visible ? 1. : 0.;
    const int duration = m_visible ? 100 : 150;
    setVisible(true);

    if (QPropertyAnimation *anim = aadlinterface::createLinearAnimation(this, "opacity", from, to, duration)) {
        connect(anim, &QPropertyAnimation::finished, this, &GripPointsHandler::onOpacityAnimationFinished);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

QSizeF GripPointsHandler::minSize() const
{
    static const qreal SpanBetweenTwoGrips = 2.;
    const qreal oneSide = GripPoint::sideSize() * 3. + SpanBetweenTwoGrips * 2.;
    return { oneSide, oneSide };
}

QPointF GripPointsHandler::viewScale() const
{
    if (QGraphicsItem *parent = parentItem()) {
        if (scene()) {
            const int viewsCount = scene()->views().size();
            if (viewsCount) {
                if (viewsCount > 1) {
                    qWarning() << Q_FUNC_INFO << "The multiple views support is not implemented yet.";
                }

                const QTransform &t = parent->deviceTransform(scene()->views().first()->viewportTransform());
                return { t.m11(), t.m22() };
            }
        }
    }

    return { 1., 1. };
}

void GripPointsHandler::setGripPointPos(GripPoint *grip, const QPointF &pos)
{
    if (grip) {
        const QPointF &currScale(viewScale());
        const QPointF &destination(mapFromScene(pos));
        const QPointF &destinationScaled = { destination.x() * currScale.x(), destination.y() * currScale.y() };
        grip->setPos(destinationScaled);
    }
}

} // namespace taste3
