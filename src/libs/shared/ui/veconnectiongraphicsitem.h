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

#include "ui/veinteractiveobject.h"

namespace shared {
namespace ui {
class VEConnectionEndPointGraphicsItem;
class VERectGraphicsItem;

class VEConnectionGraphicsItem : public VEInteractiveObject
{
    Q_OBJECT
public:
    enum CollisionsPolicy
    {
        Ignore = 0,
        Rebuild,
        PartialRebuild,
    };
    enum LayoutPolicy
    {
        Default,
        LastSegment,
        Scaling,
    };

    explicit VEConnectionGraphicsItem(VEObject *connection, VEConnectionEndPointGraphicsItem *startItem,
            VEConnectionEndPointGraphicsItem *endItem, QGraphicsItem *parent = nullptr);

    ~VEConnectionGraphicsItem() override;

    void setPoints(const QVector<QPointF> &points);
    QVector<QPointF> points() const;
    QVector<QPointF> graphicsPoints() const;

    void updateFromEntity() override;
    void init() override;

    QPainterPath shape() const override;
    QRectF boundingRect() const override;

    void initGripPoints() override;
    void updateGripPoints() override;

    VEConnectionEndPointGraphicsItem *endItem() const;
    VEConnectionEndPointGraphicsItem *startItem() const;

    VERectGraphicsItem *sourceItem() const;
    VERectGraphicsItem *targetItem() const;

    QList<QPair<shared::VEObject *, QVector<QPointF>>> prepareChangeCoordinatesCommandParams() const override;

    bool doLayout() override;

    bool replaceInterface(
            VEConnectionEndPointGraphicsItem *ifaceToBeReplaced, VEConnectionEndPointGraphicsItem *newIface);

    static void layoutInterfaceConnections(VEConnectionEndPointGraphicsItem *ifaceItem, LayoutPolicy layoutPolicy,
            CollisionsPolicy collisionsPolicy, bool includingNested);
    void layoutConnection(
            VEConnectionEndPointGraphicsItem *ifaceItem, LayoutPolicy layoutPolicy, CollisionsPolicy collisionsPolicy);

    void updateOverlappedSections();

protected:
    void onManualMoveStart(GripPoint *gp, const QPointF &at) override;
    void onManualMoveProgress(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt) override;

    virtual void updateBoundingRect();

protected Q_SLOTS:
    void rebuildLayout() override;
    void onSelectionChanged(bool isSelected) override;

private:
    bool removeCollidedGrips(GripPoint *gp);
    void simplify();

    void transformToEndPoint(const VEConnectionEndPointGraphicsItem *iface);
    void updateEndPoint(const VEConnectionEndPointGraphicsItem *iface);

    enum class ConnectionEndPointReference
    {
        Set = 0,
        Remove
    };
    void updateInterfaceConnectionsReference(ConnectionEndPointReference action);

    GripPoint *gripPointByPos(const QList<GripPoint *> &grips, const QPointF &pos) const;

protected:
    class GraphicsPathItem : public QGraphicsPathItem
    {
    public:
        explicit GraphicsPathItem(QGraphicsItem *parent = nullptr);
        QPainterPath shape() const override;
    };

    QVector<QPointF> m_points;
    QPointer<VEConnectionEndPointGraphicsItem> m_startItem;
    QPointer<VEConnectionEndPointGraphicsItem> m_endItem;
    GraphicsPathItem *m_item { nullptr };
    bool m_firstUpdate { true };
};

} // namespace ui
} // namespace shared
