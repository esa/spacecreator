/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QPointF>
#include <QPointer>
#include <QRectF>

class QGraphicsLineItem;

class tst_TimerItem;

namespace msc {

class ChartViewModel;
class InstanceItem;
class MscTimer;
class TextItem;

class TimerItem : public InteractiveObject
{
    Q_OBJECT
public:
    explicit TimerItem(msc::MscTimer *timer, ChartViewModel *model, QGraphicsItem *parent = nullptr);

    MscTimer *modelItem() const;

    void connectObjects(InstanceItem *instance, qreal y);
    void setInstance(InstanceItem *instance);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

public Q_SLOTS:
    void setName(const QString &text);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void prepareHoverMark() override;

private Q_SLOTS:
    void onTextEdited(const QString &text);
    void rebuildLayout() override;
    void onInstanceMoved(const QPointF &from, const QPointF &to);
    void onManualGeometryChangeFinished(GripPoint::Location pos, const QPointF &from, const QPointF &to);
    void updateConnectorLineVisibility();

private:
    void drawStartSymbol(QPainter *painter, const QRectF &rect);
    void drawStopSymbol(QPainter *painter, const QRectF &rect);
    void drawTimeoutArrow(QPainter *painter, const QPointF &pt);
    QRectF symbolBox() const;
    bool canConnectTimers(msc::MscTimer *targetTimer, const QPointF &targetPos);

    QPointer<msc::MscTimer> m_timer;
    QPointer<InstanceItem> m_instance;
    QPointer<ChartViewModel> m_model;
    TextItem *m_textItem = nullptr;
    QGraphicsLineItem *m_timerConnector = nullptr;
    msc::TimerItem *m_connectingItem = nullptr;

    friend tst_TimerItem;
};

} // namespace msc
