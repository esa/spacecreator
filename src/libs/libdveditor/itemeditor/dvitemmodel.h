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

#include "common.h"
#include "dvmodel.h"
#include "interface/veitemmodel.h"

namespace shared {
namespace ui {
class VEInteractiveObject;
}
namespace cmd {
class CommandsStackBase;
}
}

namespace dve {

class DVItemModel : public shared::ui::VEItemModel
{
    Q_OBJECT
public:
    enum class Roles
    {
        IdRole = Qt::UserRole,
    };
    Q_ENUM(Roles);

    explicit DVItemModel(dvm::DVModel *model, shared::cmd::CommandsStackBase *commandsStack, QObject *parent = nullptr);
    ~DVItemModel() override;

    dvm::DVModel *objectsModel() const override;

protected:
    shared::ui::VEInteractiveObject *createItem(shared::Id objectId) override;
};

} // namespace dve
