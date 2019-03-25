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

#include "interactiveobject.h"

class QGraphicsLineItem;

namespace msc {
class TextItem;

class CommentItem : public InteractiveObject
{
    Q_OBJECT
public:
    explicit CommentItem(QGraphicsItem *parent = nullptr);

    void setText(const QString &text);
    QString text() const;
    void attachTo(msc::InteractiveObject *iObj);
    InteractiveObject *object() const;
    void setGlobal(bool isGlobal);
    void updateLayout();

    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

Q_SIGNALS:
    void commentChanged(const QString &comment);

protected:
    void targetPositionChanged();
    void prepareHoverMark() override;
    void rebuildLayout() override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override;
    void onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override;

private:
    void textEdited(const QString &text);

private:
    TextItem *m_textItem = nullptr;
    QGraphicsLineItem *m_linkItem = nullptr;
    QPointer<InteractiveObject> m_iObj;
    bool m_inverseLayout = false;
    bool m_isGlobal = true;
};

} // ns msc
