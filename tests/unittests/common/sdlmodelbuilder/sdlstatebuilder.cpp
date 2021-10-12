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
{
    m_stateName = std::move(name);
}

std::unique_ptr<State> SdlStateBuilder::build()
{
    return std::make_unique<State>(std::move(m_stateName), std::move(m_inputs), std::move(m_continuousSignals));
}

SdlStateBuilder &SdlStateBuilder::withInputs(std::vector<std::unique_ptr<Input>> inputs)
{
    m_inputs = std::move(inputs);

    return *this;
}

SdlStateBuilder &SdlStateBuilder::withInput(std::unique_ptr<Input> input)
{
    m_inputs.push_back(std::move(input));

    return *this;
}

SdlStateBuilder &SdlStateBuilder::withContinuousSignals(
        std::vector<std::unique_ptr<ContinuousSignal>> continuousSignals)
{
    m_continuousSignals = std::move(continuousSignals);

    return *this;
}

} // namespace tests::common
