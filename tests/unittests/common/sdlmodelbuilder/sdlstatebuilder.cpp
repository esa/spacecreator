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

#include "sdlstatebuilder.h"

namespace tests::common {

SdlStateBuilder::SdlStateBuilder(QString name)
    : m_state(std::make_unique<State>())
{
    m_state->setName(std::move(name));
}

std::unique_ptr<State> SdlStateBuilder::build()
{
    return std::move(m_state);
}

SdlStateBuilder &SdlStateBuilder::withInput(std::unique_ptr<Input> input)
{
    m_state->addInput(std::move(input));

    return *this;
}

SdlStateBuilder &SdlStateBuilder::withContinuousSignal(std::unique_ptr<ContinuousSignal> continuousSignal)
{
    m_state->addContinuousSignal(std::move(continuousSignal));

    return *this;
}

} // namespace tests::common
