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

#include "animation.h"

#include <QDebug>
#include <QGraphicsView>
#include <QMetaEnum>
#include <QPainter>
#include <QPropertyAnimation>

namespace shared {
namespace ui {

GripPointsHandler::GripPointsHandler(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    setFlags(QGraphicsItem::ItemIgnoresTransformations | QGraphicsItem::ItemSendsGeometryChanges);
    hide();
}

GripPoint *GripPointsHandler::createGripPoint(GripPoint::Location location, int idx)
{
    auto grip = new GripPoint(location, this);
    m_usedPoints.insert(grip->location());
    m_gripPoints.insert(idx < 0 ? m_gripPoints.size() : idx, grip);
    return grip;
}

void GripPointsHandler::removeGripPoint(GripPoint *handle)
{
    m_gripPoints.removeOne(handle);
    delete handle;
}

QRectF GripPointsHandler::boundingRect() const
{
    if (QGraphicsItem *parent = parentItem()) {
        auto bounds = parent->boundingRect();
        auto scaleFactor = viewScale();
        return QRectF(bounds.topLeft().x() * scaleFactor.x(), bounds.topLeft().y() * scaleFactor.y(),
                bounds.width() * scaleFactor.x(), bounds.height() * scaleFactor.y());
    }
    return QRectF();
}

void GripPointsHandler::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) { }

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

QSizeF GripPointsHandler::minSize() const
{
    const qreal SpanBetweenTwoGrips = 2;
    const qreal oneSide = GripPoint::sideSize() * 3 + SpanBetweenTwoGrips * 2;
    return { oneSide, oneSide };
}

void GripPointsHandler::setUsedPoints(GripPoint::Locations points)
{
    if (m_usedPoints == points)
        return;

    m_usedPoints = points;
    for (auto location : points)
        createGripPoint(location);

    updateLayout();
}

GripPoint::Locations GripPointsHandler::usedPoints() const
{
    return m_usedPoints;
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

void GripPointsHandler::setGripPointPos(GripPoint::Location location, const QPointF &pos)
{
    for (auto l : m_gripPoints) {
        if (l->location() == location) {
            setGripPointPos(l, pos);
            return;
        }
    }
}

GripPoint *GripPointsHandler::gripPoint(GripPoint::Location location)
{
    for (auto gp : m_gripPoints) {
        if (gp->location() == location) {
            return gp;
        }
    }
    return nullptr;
}

void GripPointsHandler::showAnimated()
{
    changeVisibilityAnimated(true);
}

void GripPointsHandler::hideAnimated()
{
    changeVisibilityAnimated(false);
}

void GripPointsHandler::opacityAnimationFinished()
{
    if (!m_visible) {
        // The visible flag needs to be set to false. But setting it directly has the side effect to leave some
        // semi-transparent grip points visible
        QMetaObject::invokeMethod(
                this, [this]() { hide(); }, Qt::QueuedConnection);
    }
}

void GripPointsHandler::changeVisibilityAnimated(bool appear)
{
    if (appear == m_visible) {
        return;
    }

    m_visible = appear;
    const qreal from = m_visible ? 0. : 1.;
    const qreal to = m_visible ? 1. : 0.;
    const int duration = m_visible ? 100 : 150;
    setVisible(true);

    if (QPropertyAnimation *anim = shared::Animation::createLinearAnimation(this, "opacity", from, to, duration)) {
        connect(anim, &QPropertyAnimation::finished, this, &GripPointsHandler::opacityAnimationFinished);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

QPointF GripPointsHandler::viewScale() const
{
    if (QGraphicsItem *parent = parentItem()) {
        if (scene()) {
            const int viewsCount = scene()->views().size();
            if (viewsCount) {
                if (viewsCount > 1) {
                    static bool warningShown = false;
                    if (!warningShown) {
                        qWarning() << Q_FUNC_INFO << "The multiple views support is not implemented yet.";
                        warningShown = true;
                    }
                }

                const QTransform &t = parent->deviceTransform(scene()->views().first()->viewportTransform());
                return { t.m11(), t.m22() };
            }
        }
    }

    return { 1., 1. };
}

}
}
