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
#include "tab_aadl/aadlobject.h"

namespace taste3 {
namespace aadl {
class AADLObjectIface;

class AADLInterfaceGraphicsItem : public InteractiveObject
{
    Q_OBJECT
public:
    explicit AADLInterfaceGraphicsItem(AADLObjectIface *entity, QGraphicsItem *parent = nullptr);
    enum
    {
        Type = UserType + static_cast<int>(AADLObject::AADLObjectType::AADLIface)
    };

    AADLObjectIface *entity() const;

    int type() const override { return Type; }

    QGraphicsItem *targetItem() const { return m_item; }
    void setTargetItem(QGraphicsItem *item, const QPointF &pos);

protected:
    void rebuildLayout() override;
    void prepareHoverMark() override;

private:
    QGraphicsItem *m_item = nullptr;
    QGraphicsPathItem *m_iface = nullptr;
    QGraphicsTextItem *m_text = nullptr;
};

} // namespace aadl
} // namespace taste3
