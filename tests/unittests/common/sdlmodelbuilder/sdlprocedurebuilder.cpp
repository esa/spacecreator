/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "sdlprocedurebuilder.h"

#include <memory>

namespace tests::common {

SdlProcedureBuilder::SdlProcedureBuilder()
    : m_procedure(std::make_unique<Procedure>())
{
}

std::unique_ptr<Procedure> SdlProcedureBuilder::build()
{
    return std::move(m_procedure);
}

SdlProcedureBuilder &SdlProcedureBuilder::withName(QString name)
{
    m_procedure->setName(std::move(name));

    return *this;
}

SdlProcedureBuilder &SdlProcedureBuilder::withTransition(std::unique_ptr<Transition> transition)
{
    m_procedure->setTransition(std::move(transition));

    return *this;
}

SdlProcedureBuilder &SdlProcedureBuilder::withParameter(std::unique_ptr<ProcedureParameter> parameter)
{
    m_procedure->addParameter(std::move(parameter));

    return *this;
}

} // namespace tests::common
