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
#include "baseitems/common/mscutils.h"
#include "baseitems/interactiveobject.h"
#include "ui/grippoint.h"

class QMargins;
class QGraphicsRectItem;

namespace shared {
namespace ui {
class GripPoint;
}
}

namespace msc {

class ChartLayoutManager;
class MscChart;
class TextItem;

class ChartItem : public InteractiveObject
{
    Q_OBJECT

public:
    explicit ChartItem(
            MscChart *chartEntity, ChartLayoutManagerBase *chartLayoutManager, QGraphicsItem *parent = nullptr);
    ~ChartItem() override = default;

    QRectF boundingRect() const override;

    QString chartName() const;
    QString chartNameGuiText() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF contentRect() const;
    QPointF setContentRect(const QRectF &r, CifUpdatePolicy cifUpdate = CifUpdatePolicy::UpdateIfExists);

    static QPointF chartMargin();
    static QMarginsF chartMargins();

    void applyCif() override;

    bool geometryManagedByCif() const override;
    QRectF storedCustomRect() const;

    QPainterPath shape() const override;
    void updateCif() override;

Q_SIGNALS:
    void contentRectChanged(const QRectF &contentArea);

public Q_SLOTS:
    void setName(const QString &name);

protected:
    void onManualResizeProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void initGripPoints() override;

private Q_SLOTS:
    void onNameEdited(const QString &text);
    void updateTitlePos();
    void onManualGeometryChangeFinished(shared::ui::GripPoint *, const QPointF &from, const QPointF &to);
    void onChartCifRectChanged();

private:
    MscChart *chart() const;

private:
    QGraphicsRectItem *m_rectItem = nullptr;
    QGraphicsRectItem *m_contentArea = nullptr;
    TextItem *m_textItemName = nullptr;
    QRectF m_prevContentRect;
    bool m_guard = false;

    static QPointF m_margin;
    void updateCifIfNecessary(CifUpdatePolicy cause);
};

} // ns msc
