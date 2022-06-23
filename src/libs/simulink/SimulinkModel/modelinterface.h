/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#pragma once

#include "workspace/workspace.h"
#include "inport/inports.h"
#include "outport/outports.h"

namespace simulink::model {

class ModelInterface final
{
public:
    ModelInterface() = default;
    ModelInterface(ModelInterface &&) = default;
    ModelInterface &operator=(ModelInterface &&) = default;

public:
    auto workspace() const -> const Workspace &;
    auto setWorkspace(Workspace workspace) -> void;

    auto inports() const -> const Inports &;
    auto setInports(Inports inports) -> void;

    auto outports() const -> const Outports &;
    auto setOutports(Outports outports) -> void;

private:
    Workspace m_workspace;
    Inports m_inports;
    Outports m_outports;
};

} // namespace simulink::model
