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

#ifndef TIMERITEM_H
#define TIMERITEM_H

#include "baseitems/interactiveobject.h"

#include <QPointF>
#include <QPointer>
#include <QRectF>

namespace msc {

class InstanceItem;
class MscTimer;
class TextItem;

class TimerItem : public InteractiveObject
{
    Q_OBJECT
public:
    explicit TimerItem(msc::MscTimer *timer, QGraphicsItem *parent = nullptr);

    MscTimer *modelItem() const;

    void connectObjects(InstanceItem *instance, qreal y);
    void setInstance(InstanceItem *instance);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

public Q_SLOTS:
    void setName(const QString &text);

protected:
    void onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void prepareHoverMark() override;

private Q_SLOTS:
    void onTextEdited(const QString &text);
    void rebuildLayout() override;
    void onInstanceMoved(const QPointF &from, const QPointF &to);
    void onManualGeometryChangeFinished(GripPoint::Location pos, const QPointF &from, const QPointF &to);

private:
    void drawStartSymbol(QPainter *painter, const QRectF &rect);
    void drawStopSymbol(QPainter *painter, const QRectF &rect);
    void drawTimeoutArrow(QPainter *painter, const QPointF &pt);

    QPointer<msc::MscTimer> m_timer;
    QPointer<InstanceItem> m_instance;
    TextItem *m_textItem = nullptr;
};

} // namespace msc

#endif // TIMERITEM_H
