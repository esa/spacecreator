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

#include "sdlinputbuilder.h"

#include <memory>

namespace tests::common {

SdlInputBuilder::SdlInputBuilder()
    : m_input(std::make_unique<Input>())
{
}

std::unique_ptr<Input> SdlInputBuilder::build()
{
    return std::move(m_input);
}

SdlInputBuilder &SdlInputBuilder::withName(QString name)
{
    m_input->setName(std::move(name));

    return *this;
}

SdlInputBuilder &SdlInputBuilder::withTransition(Transition *transition)
{
    m_input->setTransition(transition);

    return *this;
}

SdlInputBuilder &SdlInputBuilder::withParameter(std::unique_ptr<Input::Parameter> parameter)
{
    m_input->addParameter(std::move(parameter));

    return *this;
}

} // namespace tests::common
