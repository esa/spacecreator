/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "geometry.h"
#include "topohelper/geometry.h"
#include "veinteractiveobject.h"

#include <QPointer>

namespace shared {
class VEObject;
namespace ui {
class VEConnectionGraphicsItem;
class VERectGraphicsItem;

class VEConnectionEndPointGraphicsItem : public VEInteractiveObject
{
    Q_OBJECT
public:
    explicit VEConnectionEndPointGraphicsItem(VEObject *object, QGraphicsItem *parent = nullptr);
    ~VEConnectionEndPointGraphicsItem() override;

    void addConnection(VEConnectionGraphicsItem *item);
    void removeConnection(VEConnectionGraphicsItem *item);
    QList<QPointer<VEConnectionGraphicsItem>> connectionItems() const;

    VERectGraphicsItem *targetItem() const;

    void updateFromEntity() override;
    QList<QPair<shared::VEObject *, QVector<QPointF>>> prepareChangeCoordinatesCommandParams() const override;

    void adjustItem();

    bool doLayout() override;

    // Gets the alignment of this connection endpoint.
    Qt::Alignment alignment() const;

    // Sets the alignment of this connection endpoint. Used for testing.
    void setAlignment(Qt::Alignment alignment);

public:
    // Methods for children to override
    virtual QPointF connectionEndPoint(const bool innerConnection) const;
    virtual QPointF connectionEndPoint(VEConnectionGraphicsItem *connection = nullptr) const;
    virtual QPainterPath ifaceShape() const;

protected:
    void rebuildLayout() override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void onManualMoveProgress(GripPoint *grip, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(GripPoint *grip, const QPointF &from, const QPointF &to) override;

    virtual void updateInternalItems(Qt::Alignment alignment);
    virtual QPainterPath itemPath(Qt::Alignment alignment) const;

    virtual QList<QPair<Qt::Alignment, QPainterPath>> sidePaths() const;
    virtual topohelp::LookupDirection lookupType() const;

    void updateTextPosition() override;

    void showHelperLines(bool show) override;

protected:
    QList<QPointer<VEConnectionGraphicsItem>> m_connections;
    topohelp::LookupDirection m_adjustDirection = topohelp::LookupDirection::Bidirectional;
    Qt::Alignment m_alignment;
    QGraphicsLineItem *m_horizontalHelper = nullptr;
    QGraphicsLineItem *m_verticalHelper = nullptr;
};

} // namespace ui
} // namespace shared
