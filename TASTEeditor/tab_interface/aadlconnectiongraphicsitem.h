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

#include "baseitems/common/abstractinteractiveobject.h"
#include "tab_aadl/aadlobject.h"

#include <QGraphicsObject>
#include <QGraphicsRectItem>
#include <QPointer>

namespace taste3 {
namespace aadl {
class AADLObjectConnection;
class AADLInterfaceGraphicsItem;

class AADLConnectionGraphicsItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit AADLConnectionGraphicsItem(AADLObjectConnection *connection, QGraphicsItem *parent = nullptr);
    ~AADLConnectionGraphicsItem() override;

    static AADLConnectionGraphicsItem *createConnection(QGraphicsScene *scene, const QPointF &startPoint,
                                                        const QPointF &endPoint);
    void setEndPoints(AADLInterfaceGraphicsItem *pi, AADLInterfaceGraphicsItem *ri);
    void setPoints(const QVector<QPointF> &points);
    QVector<QPointF> points() const;

    AADLObjectConnection *entity() const;

    void scheduleLayoutUpdate();
    void instantLayoutUpdate();

    enum
    {
        Type = UserType + static_cast<int>(AADLObject::AADLObjectType::AADLConnection)
    };

    int type() const override { return Type; }

    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    static QVector<QPointF> simplify(const QVector<QPointF> &points);

    void clear();
    void updateGripPoints(bool forceVisible = false);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;

    void rebuildLayout();

    void updateProvidedInterface(AADLInterfaceGraphicsItem *pi);
    void updateRequiredInterface(AADLInterfaceGraphicsItem *ri);

    void handleSelectionChanged(bool isSelected);

    bool handleGripPointPress(QGraphicsRectItem *handle, QGraphicsSceneMouseEvent *event);
    bool handleGripPointMove(QGraphicsRectItem *handle, QGraphicsSceneMouseEvent *event);
    bool handleGripPointRelease(QGraphicsRectItem *handle, QGraphicsSceneMouseEvent *event);

private:
    void adjustGripPointCount();
    QGraphicsRectItem *createGripPoint();
    void simplify();
    void updateBoundingRect();
    void createCommand();

private:
    QPointer<AADLInterfaceGraphicsItem> m_startItem;
    QPointer<AADLInterfaceGraphicsItem> m_endItem;
    QGraphicsPathItem *m_item = nullptr;
    AADLObjectConnection *m_connection = nullptr;
    QRectF m_boundingRect;

    QVector<QPointF> m_points;
    QVector<QPointF> m_tmpPoints;
    QList<QGraphicsRectItem *> m_grips;
    bool m_layoutDirty = false;
};

} // namespace aadl
} // namespace taste3
