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

#include "resizelimits.h"
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
    void onManualResizeFinish(GripPoint *grip,
                              const QPointF &pressedAt,
                              const QPointF &releasedAt) override;
    void onManualMoveFinish(GripPoint *grip,
                            const QPointF &pressedAt,
                            const QPointF &releasedAt) override;

    virtual QSizeF minimumSize() const;

    virtual QRectF resizedRect(ResizeLimits resizeLimits);
    ResizeLimits resizeRectForConnectionEndpoints(ResizeLimits resizeLimits);

    QList<VERectGraphicsItem *> findCollidingVERectGraphicsItems(const QRectF &rect, QMarginsF margins) const;

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
    QRectF movedRect(const QPointF &from, const QPointF &to);

    bool m_overridingCursor = false;
    QSet<VEInteractiveObject *> m_collidedItems;
};

} // namespace ui

} // namespace shared
