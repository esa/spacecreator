/*
 Copyright (C) 2018-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include "veconnectiongraphicsitem.h"
#include "veinteractiveobject.h"

#include <QPointer>
#include <QSet>

namespace shared {
class VEObject;

namespace ui {

class VERectGraphicsItem : public VEInteractiveObject
{
    Q_OBJECT
public:
    explicit VERectGraphicsItem(VEObject *entity, QGraphicsItem *parentGraphicsItem = nullptr);

    void setRect(const QRectF &geometry);

    /**
     * @brief updateFromEntity
     * Updates this instance's geometry with data from the model entity
     */
    void updateFromEntity() override;


    QList<QPair<VEObject *, QVector<QPointF>>> prepareChangeCoordinatesCommandParams() const override;

    QRectF nestedItemsSceneBoundingRect() const;
    bool layoutShouldBeChanged() const;
    bool doLayout() override;

    void layoutConnectionsOnResize(VEConnectionGraphicsItem::CollisionsPolicy collisionsPolicy);
    void layoutConnectionsOnMove(VEConnectionGraphicsItem::CollisionsPolicy collisionsPolicy);

protected:
    void initGripPoints() override;
    void onManualMoveProgress(GripPoint *grip, const QPointF &from, const QPointF &to) override;
    void onManualResizeProgress(GripPoint *grip, const QPointF &from, const QPointF &to) override;
    void onManualResizeFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt) override;
    void onManualMoveFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt) override;

    /**
     * @brief minimumSize
     * Children classes will know how to calculate their minimum size.
     */
    virtual QSizeF minimumSize() const;

    /**
     * @brief resizedRect takes a GripPoint sitting on this VERectGraphicsItem and two
     * QPointFs representing the movement of the GripPoint and returns
     * a QRectF representing the new sceneBoundingRect of this VERectGraphicsItem based on
     * the allowed movement of that particular handle.
     *
     * A corner GripPoint can move freely. A GripPoint on a horizontal line can only move
     * up or down. A GripPoint on a vertical line can only move left or right.
     *
     * If a transformation is illegal, a QRectF is calculated that performs as much as possible of the
     * transformation as is legal.
     *
     * Limits of movement: VERectGraphicsItem has interfaces (depicted by small triangles) on the edges of the
     * rectangle. A VERectGraphicsItem can not be shrunk to a size smaller than what the location of the interfaces allow.
     * A child class overloading this method will need to call this implementation of resizedRect to calculate the smallest
     * valid rectangle allowed by interfaces and calculate any restrictions of its own.
     *
     * @param grip a GripPoint
     * @param from a QPointF the GripPoint was moved from
     * @param to a QPointF the GripPoint was moved to
     * @return a QRect representing the new sceneBoundingRect of this VERectGraphicsItem
     */
    virtual QRectF resizedRect(GripPoint *grip, const QPointF &from, const QPointF &to);

    bool setGeometry(const QRectF &sceneGeometry);

    /**
     * @brief layoutInterfaces updates the position of each connection point on
     * this VERectGraphicsItem instance
     */
    void layoutInterfaces();

    virtual void updateTextPosition() override;

private Q_SLOTS:
    void onGeometryChanged();

private:
    QRectF checkMinimumSize(GripPoint *grip, const QPointF &to, const QRectF &sceneBoundingRect);
    QRectF checkConnectionEndpoints(GripPoint *grip, const QPointF &to, const QRectF &sceneBoundingRect);
    QRectF movedRect(const QPointF &from, const QPointF &to);

    bool m_overridingCursor = false;
    QSet<VEInteractiveObject *> m_collidedItems;
};

} // namespace ui

} // namespace shared
