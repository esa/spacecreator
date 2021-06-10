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

#include "commands/cmdentitygeometrychange.h"
#include "common.h"

#include <QPointer>

namespace dvm {
class DVModel;
class DVNode;
class DVDevice;
} // namespace dvm

namespace dve {
namespace cmd {

class CmdDeviceEntityCreate : public shared::cmd::CmdEntityGeometryChange
{
public:
    explicit CmdDeviceEntityCreate(
            dvm::DVModel *model, dvm::DVNode *parent, const QPointF &pos, const shared::Id &id = shared::InvalidId);
    ~CmdDeviceEntityCreate() override;

    void redo() override;
    void undo() override;
    int id() const override;

private:
    QPointer<dvm::DVModel> m_model;
    QPointer<dvm::DVNode> m_parent;
    QPointer<dvm::DVDevice> m_entity;
};

} // namespace cmd
} // namespace dve
