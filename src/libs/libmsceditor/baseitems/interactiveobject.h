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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "cif/cifblock.h"
#include "cif/cifline.h"
#include "ui/interactiveobjectbase.h"

#include <QPointer>

namespace msc {

class ChartItem;
class ChartLayoutManager;
class MscEntity;

class InteractiveObject : public shared::ui::InteractiveObjectBase
{
    Q_OBJECT

public:
    explicit InteractiveObject(
            msc::MscEntity *entity, msc::ChartLayoutManager *chartLayoutManager, QGraphicsItem *parent = nullptr);

    msc::MscEntity *modelEntity() const;

    virtual QPair<QPointF, bool> commentPoint() const;

    void postCreatePolishing() override;

    virtual void applyCif();
    virtual void updateCif();
    virtual bool geometryManagedByCif() const;

    virtual void moveSilentlyBy(const QPointF &shift);

    void setChartItem(msc::ChartItem *item);

Q_SIGNALS:
    void cifChanged();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    cif::CifBlockShared cifBlockByType(cif::CifLine::CifType type) const;
    virtual cif::CifLine::CifType mainCifType() const;

    bool cifChangedEnough(const QVector<QPoint> &storedCif, const QVector<QPoint> newCif);

    QRectF getChartContentBox() const;
    QRectF getChartBox() const;

    QPointer<msc::MscEntity> m_entity;
    QPointer<ChartLayoutManager> m_chartLayoutManager;
    mutable QPointer<msc::ChartItem> m_chartItem;

    bool m_hovered = false;
    qreal m_storedZ = 0.;
};

}
