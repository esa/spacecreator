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

#include "baseitems/interactiveobject.h"

class QGraphicsPathItem;

namespace msc {
class TextItem;
class MscComment;
class MscChart;

class CommentItem : public InteractiveObject
{
    Q_OBJECT
public:
    explicit CommentItem(MscChart *chart, ChartLayoutManager *chartLayoutManager, QGraphicsItem *parent = nullptr);
    ~CommentItem() override;

    void setText(const QString &text);
    QString text() const;

    void attachTo(msc::InteractiveObject *iObj);
    InteractiveObject *object() const;

    bool geometryManagedByCif() const override;

    bool isGlobal() const;

    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void applyCif() override;
    void updateCif() override;

    void setGlobalPreview(bool isGlobalPreview);

protected:
    cif::CifLine::CifType mainCifType() const override;
    void initGripPoints() override;
    void rebuildLayout() override;
    void updateLinkPath();
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void onManualMoveProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onManualResizeProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to) override;

private Q_SLOTS:
    void onLinkedItemMoved(const QPointF &from, const QPointF &to);

private:
    void textEdited(const QString &text);
    MscComment *commentEntity() const;

    TextItem *m_textItem = nullptr;
    QGraphicsPathItem *m_linkItem = nullptr;
    QPointer<MscChart> m_chart;
    QPointer<InteractiveObject> m_iObj;
    bool m_inverseLayout = false;
    bool m_isGlobalPreview = false;
};

} // ns msc
