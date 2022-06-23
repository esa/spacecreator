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

#include "modelinterface.h"

namespace simulink::model {

const Workspace &ModelInterface::workspace() const
{
    return m_workspace;
}

void ModelInterface::setWorkspace(Workspace workspace)
{
    m_workspace = std::move(workspace);
}

const Inports &ModelInterface::inports() const
{
    return m_inports;
}

void ModelInterface::setInports(Inports inports)
{
    m_inports = std::move(inports);
}

const Outports &ModelInterface::outports() const
{
    return m_outports;
}

void ModelInterface::setOutports(Outports outports)
{
    m_outports = std::move(outports);
}

} // namespace simulink::model
