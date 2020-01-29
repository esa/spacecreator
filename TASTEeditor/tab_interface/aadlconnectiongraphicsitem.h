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

#include "baseitems/clicknotifieritem.h"
#include "baseitems/common/abstractinteractiveobject.h"
#include "tab_aadl/aadlobject.h"

#include <QGraphicsObject>
#include <QGraphicsRectItem>
#include <QPointer>

namespace taste3 {
namespace aadl {
class AADLObjectConnection;
class AADLInterfaceGraphicsItem;
class AADLFunctionGraphicsItem;

class AADLConnectionGraphicsItem : public ClickNotifierItem
{
    Q_OBJECT
public:
    explicit AADLConnectionGraphicsItem(AADLObjectConnection *connection, AADLInterfaceGraphicsItem *ifaceStart,
                                        AADLInterfaceGraphicsItem *ifaceEnd, QGraphicsItem *parent = nullptr);
    ~AADLConnectionGraphicsItem() override;

    static QVector<QPointF> connectionPath(AADLInterfaceGraphicsItem *ifaceStart, AADLInterfaceGraphicsItem *ifaceEnd);

    void setPoints(const QVector<QPointF> &points);
    QVector<QPointF> points() const;

    QVector<QPointF> currentPoints() const;

    AADLObjectConnection *entity() const;

    void scheduleLayoutUpdate();
    void instantLayoutUpdate();
    void updateFromEntity();

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

    AADLInterfaceGraphicsItem *endItem() const;
    AADLInterfaceGraphicsItem *startItem() const;

    AADLFunctionGraphicsItem *sourceItem() const;
    AADLFunctionGraphicsItem *targetItem() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;

    void rebuildLayout();

    void handleSelectionChanged(bool isSelected);

    bool handleGripPointPress(QGraphicsRectItem *handle, QGraphicsSceneMouseEvent *event);
    bool handleGripPointMove(QGraphicsRectItem *handle, QGraphicsSceneMouseEvent *event);
    bool handleGripPointRelease(QGraphicsRectItem *handle, QGraphicsSceneMouseEvent *event);

    virtual ColorManager::HandledColors handledColorType() const override;
    virtual AADLObject *aadlObject() const override;

protected Q_SLOTS:
    virtual void colorSchemeUpdated() override;

private:
    void adjustGripPointCount();
    QGraphicsRectItem *createGripPoint();
    void simplify();
    void updateBoundingRect();
    void createCommand();

private:
    class GraphicsPathItem : public QGraphicsPathItem
    {
    public:
        explicit GraphicsPathItem(QGraphicsItem *parent = nullptr);
        QPainterPath shape() const override;
    };

private:
    const QPointer<AADLInterfaceGraphicsItem> m_startItem;
    const QPointer<AADLInterfaceGraphicsItem> m_endItem;
    GraphicsPathItem *m_item = nullptr;
    QRectF m_boundingRect;

    QVector<QPointF> m_points;
    QVector<QPointF> m_tmpPoints;
    QList<QGraphicsRectItem *> m_grips;
    bool m_layoutDirty = false;
};

} // namespace aadl
} // namespace taste3
