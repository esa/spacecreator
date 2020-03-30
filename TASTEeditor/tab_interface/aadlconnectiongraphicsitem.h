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

#include "baseitems/interactiveobject.h"
#include "tab_aadl/aadlobject.h"

#include <QGraphicsObject>
#include <QGraphicsRectItem>
#include <QPointer>

namespace taste3 {
namespace aadl {
class AADLObjectConnection;
class AADLInterfaceGraphicsItem;
class AADLFunctionGraphicsItem;

class AADLConnectionGraphicsItem : public InteractiveObject
{
    Q_OBJECT
public:
    enum
    {
        Type = UserType + static_cast<int>(AADLObject::Type::Connection)
    };
    int type() const override { return Type; }

    explicit AADLConnectionGraphicsItem(AADLObjectConnection *connection, AADLInterfaceGraphicsItem *ifaceStart,
                                        AADLInterfaceGraphicsItem *ifaceEnd, QGraphicsItem *parent = nullptr);
    ~AADLConnectionGraphicsItem() override;

    static QVector<QPointF> connectionPath(AADLInterfaceGraphicsItem *ifaceStart, AADLInterfaceGraphicsItem *ifaceEnd);
    static QVector<QPointF> connectionPath(QGraphicsScene *scene, const QPointF &startIfacePos,
                                           const QRectF &sourceRect, const QPointF &endIfacePos,
                                           const QRectF &targetRect);

    void setPoints(const QVector<QPointF> &points);
    QVector<QPointF> points() const;

    QVector<QPointF> graphicsPoints() const;

    AADLObjectConnection *entity() const;
    void updateFromEntity() override;

    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    static QVector<QPointF> simplify(const QVector<QPointF> &points);

    void initGripPoints() override;
    void updateGripPoints() override;

    AADLInterfaceGraphicsItem *endItem() const;
    AADLInterfaceGraphicsItem *startItem() const;

    AADLFunctionGraphicsItem *sourceItem() const;
    AADLFunctionGraphicsItem *targetItem() const;

    QList<QVariantList> prepareChangeCoordinatesCommandParams() const override;

    QString prepareTooltip() const override;

    void updateEdgePoint(const AADLInterfaceGraphicsItem *iface);

    void layOut();

protected:
    void onManualMoveStart(GripPoint *gp, const QPointF &at) override;
    void onManualMoveProgress(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt) override;

    virtual ColorManager::HandledColors handledColorType() const override;

protected Q_SLOTS:
    virtual void applyColorScheme() override;
    void rebuildLayout() override;
    void onSelectionChanged(bool isSelected) override;

private:
    //    void adjustGripPointCount();
    bool removeCollidedGrips(GripPoint *gp);
    void simplify();
    void updateBoundingRect();

    enum class IfaceConnectionReference
    {
        Set = 0,
        Remove
    };
    void updateInterfaceConnectionsReference(IfaceConnectionReference action);

private:
    class GraphicsPathItem : public QGraphicsPathItem
    {
    public:
        explicit GraphicsPathItem(QGraphicsItem *parent = nullptr);
        QPainterPath shape() const override;
    };

    const QPointer<AADLInterfaceGraphicsItem> m_startItem;
    const QPointer<AADLInterfaceGraphicsItem> m_endItem;
    GraphicsPathItem *m_item = nullptr;
    QVector<QPointF> m_points;
    bool m_firstUpdate { true };
};

} // namespace aadl
} // namespace taste3
