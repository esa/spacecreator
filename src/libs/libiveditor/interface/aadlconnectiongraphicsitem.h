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

#pragma once

#include "aadlobject.h"
#include "baseitems/interactiveobject.h"

#include <QPointer>

namespace ivm {
class AADLConnection;
}

namespace ive {

class AADLInterfaceGraphicsItem;
class AADLFunctionGraphicsItem;

class AADLConnectionGraphicsItem : public InteractiveObject
{
    Q_OBJECT
public:
    enum
    {
        Type = UserType + static_cast<int>(ivm::AADLObject::Type::Connection)
    };
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

    int type() const override { return Type; }

    explicit AADLConnectionGraphicsItem(ivm::AADLConnection *connection, AADLInterfaceGraphicsItem *ifaceStart,
            AADLInterfaceGraphicsItem *ifaceEnd, QGraphicsItem *parent = nullptr);
    ~AADLConnectionGraphicsItem() override;

    void setPoints(const QVector<QPointF> &points);
    QVector<QPointF> points() const;

    QVector<QPointF> graphicsPoints() const;

    ivm::AADLConnection *entity() const;
    void updateFromEntity() override;

    void init() override;

    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void initGripPoints() override;
    void updateGripPoints() override;

    AADLInterfaceGraphicsItem *endItem() const;
    AADLInterfaceGraphicsItem *startItem() const;

    AADLFunctionGraphicsItem *sourceItem() const;
    AADLFunctionGraphicsItem *targetItem() const;

    QList<QPair<ivm::AADLObject *, QVector<QPointF>>> prepareChangeCoordinatesCommandParams() const override;

    QString prepareTooltip() const override;

    void layout();

    bool replaceInterface(AADLInterfaceGraphicsItem *ifaceToBeReplaced, AADLInterfaceGraphicsItem *newIface);

    static void layoutInterfaceConnections(AADLInterfaceGraphicsItem *ifaceItem, LayoutPolicy layoutPolicy,
            CollisionsPolicy collisionsPolicy, bool includingNested);
    void layoutConnection(
            AADLInterfaceGraphicsItem *ifaceItem, LayoutPolicy layoutPolicy, CollisionsPolicy collisionsPolicy);

    void updateOverlappedSections();

protected:
    void onManualMoveStart(shared::ui::GripPoint *gp, const QPointF &at) override;
    void onManualMoveProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(shared::ui::GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt) override;

    shared::ColorManager::HandledColors handledColorType() const override;
    virtual void updateBoundingRect();

protected Q_SLOTS:
    void applyColorScheme() override;
    void rebuildLayout() override;
    void onSelectionChanged(bool isSelected) override;

private:
    bool removeCollidedGrips(shared::ui::GripPoint *gp);
    void simplify();

    void transformToEndPoint(const AADLInterfaceGraphicsItem *iface);
    void updateLastSegment(const AADLInterfaceGraphicsItem *iface);
    void updateEndPoint(const AADLInterfaceGraphicsItem *iface);

    enum class IfaceConnectionReference
    {
        Set = 0,
        Remove
    };
    void updateInterfaceConnectionsReference(IfaceConnectionReference action);

    shared::ui::GripPoint *gripPointByPos(const QList<shared::ui::GripPoint *> &grips, const QPointF &pos) const;

private:
    class GraphicsPathItem : public QGraphicsPathItem
    {
    public:
        explicit GraphicsPathItem(QGraphicsItem *parent = nullptr);
        QPainterPath shape() const override;
    };

    QPointer<AADLInterfaceGraphicsItem> m_startItem;
    QPointer<AADLInterfaceGraphicsItem> m_endItem;
    GraphicsPathItem *m_item = nullptr;
    QVector<QPointF> m_points;
    bool m_firstUpdate { true };
};

}
