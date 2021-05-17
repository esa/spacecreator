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
    explicit VERectGraphicsItem(shared::VEObject *entity, QGraphicsItem *parentGraphicsItem = nullptr);
    virtual QSizeF minimalSize() const;

    void setRect(const QRectF &geometry);

    void updateFromEntity() override;
    QList<QPair<shared::VEObject *, QVector<QPointF>>> prepareChangeCoordinatesCommandParams() const override;

    /// PUBLIC FOR TESTING PURPOSE, MOVE TO PRIVATE
    QRectF nestedItemsSceneBoundingRect() const;
    bool itemNeedsToBeRelayout() const;
    void layout();

protected:
    void rebuildLayout() override;
    void initGripPoints() override;
    void onManualMoveProgress(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to) override;
    void onManualResizeProgress(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to) override;
    void onManualResizeFinish(
            shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt) override;
    void onManualMoveFinish(shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt) override;

    QRectF transformedRect(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to);

    bool setGeometry(const QRectF &sceneGeometry);

    void handleGeometryChanged(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &releasedAt);
    void handleGeometryChanging(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &releasedAt);

private Q_SLOTS:
    void onGeometryChanged();

private:
    QSet<VEInteractiveObject *> m_collidedItems;
};

}

}
