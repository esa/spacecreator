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
#include "tab_aadl/aadlobject.h"

namespace taste3 {
class TextGraphicsItem;

namespace aadl {

/// TODO: create base comment item for all libraries
class AADLCommentGraphicsItem : public InteractiveObject
{
    Q_OBJECT
public:
    explicit AADLCommentGraphicsItem(QObject *entity, QGraphicsItem *parent = nullptr);
    enum
    {
        Type = UserType
    };

    int type() const override { return Type; }

    void setText(const QString &text);
    QString text() const;

protected:
    void rebuildLayout() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void onManualResizeProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to) override;

private Q_SLOTS:
    void textEdited(const QString &newText);

private:
    TextGraphicsItem *m_textItem = nullptr;
};

} // namespace aadl
} // namespace taste3
