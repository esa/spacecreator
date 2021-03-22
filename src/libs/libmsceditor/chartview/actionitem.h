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

#include "baseitems/eventitem.h"

#include <QPointer>

namespace msc {

class MscAction;
class TextItem;

class ActionItem : public EventItem
{
    Q_OBJECT
public:
    explicit ActionItem(
            msc::MscAction *action, ChartLayoutManager *chartLayoutManager, QGraphicsItem *parent = nullptr);

    MscAction *modelItem() const;

    void applyCif() override;
    void updateCif() override;

public Q_SLOTS:
    void setActionText(const QString &text);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void onManualMoveProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;

    void initGripPoints() override;
    cif::CifLine::CifType mainCifType() const override;

private Q_SLOTS:
    void onTextEdited(const QString &text);
    void rebuildLayout() override;

private:
    QString actionText() const;

    QPointer<msc::MscAction> m_action;
    TextItem *m_textItem = nullptr;
};

} // namespace msc
