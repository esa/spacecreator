/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include "baseitems/interactiveobject.h"

class QGraphicsLineItem;
class QGraphicsPolygonItem;
class QGraphicsTextItem;

namespace msc {
class InstanceItem;
class MscMessage;
class ArrowItem;

class MessageItem : public InteractiveObject
{
    Q_OBJECT
public:
    explicit MessageItem(MscMessage *message, QGraphicsItem *parent = nullptr);

    void setSourceInstanceItem(InstanceItem *sourceInstance);
    void setTargetInstanceItem(InstanceItem *targetInstance);

    QString name() const;

    void updateAnchorSource(const QPointF &delta);
    void updateAnchorTarget(const QPointF &delta);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

public Q_SLOTS:
    void updateLayout();
    void setName(const QString &name);

protected:
    virtual void handleGripPointMovement(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    Q_INVOKABLE void updateGripPoints();

private Q_SLOTS:
    void buildLayout();

private:
    msc::MscMessage *m_message = nullptr;
    ArrowItem *m_arrowItem = nullptr;
    InstanceItem *m_sourceInstance = nullptr;
    InstanceItem *m_targetInstance = nullptr;
    bool m_layoutDirty = false;
};

} // namespace mas

#endif // MESSAGEITEM_H
