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
#include "baseitems/common/utils.h"
#include "baseitems/interactiveobject.h"
#include "chartviewmodel.h"

#include <QMargins>

class QGraphicsRectItem;

namespace msc {

class MscChart;
class TextItem;

class ChartItem : public InteractiveObject
{
    Q_OBJECT

public:
    explicit ChartItem(MscChart *chartEntity, ChartViewModel *chartViewModel = nullptr,
                       QGraphicsItem *parent = nullptr);
    ~ChartItem() override = default;

    QRectF boundingRect() const override;

    QString chartName() const;
    QString chartNameGuiText() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF contentRect() const;
    QPointF setContentRect(const QRectF &r, utils::CifUpdatePolicy cifUpdate = utils::CifUpdatePolicy::UpdateIfExists);

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
    void onManualMoveProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onManualResizeProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void prepareHoverMark() override;

private Q_SLOTS:
    void onNameEdited(const QString &text);
    void updateTitlePos();
    void onManualGeometryChangeFinished(shared::ui::GripPoint::Location, const QPointF &from, const QPointF &to);
    void onChartCifRectChanged();

private:
    MscChart *chart() const;

private:
    QPointer<ChartViewModel> m_chartViewModel;
    QGraphicsRectItem *m_rectItem = nullptr;
    QGraphicsRectItem *m_contentArea = nullptr;
    TextItem *m_textItemName = nullptr;
    QRectF m_prevContentRect;
    bool m_guard = false;

    static QPointF m_margin;
    void updateCifIfNecessary(utils::CifUpdatePolicy cause);
};

} // ns msc
