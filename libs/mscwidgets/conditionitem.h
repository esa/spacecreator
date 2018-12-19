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

#ifndef CONDITIONITEM_H
#define CONDITIONITEM_H

#include "baseitems/interactiveobject.h"

class QGraphicsPolygonItem;

namespace msc {

class MscCondition;
class TextItem;

class ConditionItem : public InteractiveObject
{
    Q_OBJECT

public:
    explicit ConditionItem(MscCondition *condition, QGraphicsItem *parent = nullptr);

    MscCondition *modelItem() const;

    QString name() const;

    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setBoundingRect(const QRectF &geometry);

    static ConditionItem *createDefaultItem(MscCondition *condition, const QPointF &pos);

public Q_SLOTS:
    void setName(const QString &name);
    void buildLayout();

private Q_SLOTS:
    void onNameEdited(const QString &name);

protected:
    void onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override;

private:
    MscCondition *m_condition = nullptr;

    QGraphicsPolygonItem *m_polygonItem = nullptr;
    TextItem *m_nameItem = nullptr;
};

} // namespace msc

#endif // CONDITIONITEM_H
