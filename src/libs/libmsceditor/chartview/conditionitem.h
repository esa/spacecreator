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

#include "baseitems/eventitem.h"

#include <QPointer>

class QGraphicsPolygonItem;

namespace msc {

class MscCondition;
class TextItem;

class ConditionItem : public EventItem
{
    Q_OBJECT

public:
    explicit ConditionItem(
            MscCondition *condition, ChartLayoutManagerBase *chartLayoutManager, QGraphicsItem *parent = nullptr);

    MscCondition *modelItem() const;

    QString name() const;

    QPainterPath shape() const override;

    static ConditionItem *createDefaultItem(
            MscCondition *condition, ChartLayoutManagerBase *chartLayoutManager, const QPointF &pos);

    void applyCif() override;
    void updateCif() override;

public Q_SLOTS:
    void setName(const QString &name);
    void setInstancesRect(const QRectF &instancesRect);
    void buildLayout();

private Q_SLOTS:
    void onNameEdited(const QString &name);
    void rebuildLayout() override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void onManualMoveProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;

    void initGripPoints() override;
    cif::CifLine::CifType mainCifType() const override;

private:
    MscCondition *m_condition = nullptr;

    QGraphicsPolygonItem *m_polygonItem = nullptr;
    TextItem *m_nameItem = nullptr;

    QRectF m_InstancesRect = QRectF();
};

} // namespace msc
