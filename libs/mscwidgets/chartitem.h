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

#include <QMargins>

class QGraphicsRectItem;

namespace msc {

class MscChart;
class TextItem;

class ChartItem : public InteractiveObject
{
    Q_OBJECT

public:
    explicit ChartItem(MscChart *chartEntity, QGraphicsItem *parent = nullptr);
    ~ChartItem() override;

    QRectF boundingRect() const override;

    QString chartName() const;
    QString chartNameGuiText() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF box() const;
    void setBox(const QRectF &r);

    static const QMarginsF &chartMargins();

    void applyCif() override;

    bool geometryManagedByCif() const;
    QRectF storedCustomRect() const;

    QPainterPath shape() const override;

Q_SIGNALS:
    void chartBoxChanged();

public Q_SLOTS:
    void setName(const QString &name);

protected:
    void onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void prepareHoverMark() override;

private Q_SLOTS:
    void onNameEdited(const QString &text);
    void updateTitlePos();
    void onManualGeometryChangeFinished(GripPoint::Location, const QPointF &from, const QPointF &to);

private:
    void updateCif() override;
    MscChart *chart() const;

private:
    QGraphicsRectItem *m_rectItem = nullptr;
    TextItem *m_textItemName = nullptr;
    QRectF m_box, m_originalBox;
    bool m_guard = false;
};

} // ns msc
