/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "colors/colormanager.h"
#include "dvconnection.h"
#include "ui/veconnectiongraphicsitem.h"

class QGraphicsItem;

namespace dve {
class DVDeviceGraphicsItem;

class DVConnectionGraphicsItem : public shared::ui::VEConnectionGraphicsItem
{
    Q_OBJECT
public:
    explicit DVConnectionGraphicsItem(dvm::DVConnection *connection, DVDeviceGraphicsItem *startItem,
            DVDeviceGraphicsItem *endItem, QGraphicsItem *parent = nullptr);
    ~DVConnectionGraphicsItem() override;
    enum
    {
        Type = UserType + static_cast<int>(dvm::DVObject::Type::Connection)
    };

    dvm::DVConnection *entity() const override;

    int type() const override { return Type; }
    int itemLevel(bool isSelected) const override;

protected Q_SLOTS:
    void applyColorScheme() override;

protected:
    shared::ColorManager::HandledColors handledColorType() const override;
};

} // namespace dve
