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

#include "sdlsystembuilder.h"

#include <sdl/SdlModel/rename.h>
#include <sdl/SdlModel/signal.h>

using sdl::Block;
using sdl::Rename;
using sdl::Signal;
using sdl::System;

namespace tests::common {

SdlSystemBuilder::SdlSystemBuilder(QString systemName)
{
    m_system.setName(std::move(systemName));
}

System SdlSystemBuilder::build()
{
    return std::move(m_system);
}

SdlSystemBuilder &SdlSystemBuilder::withFreeformText(QString text)
{
    m_system.addFreeformText(std::move(text));

    return *this;
}

SdlSystemBuilder &SdlSystemBuilder::withSignal(QString signalName)
{
    auto signal = std::make_unique<Signal>(signalName);
    m_system.addSignal(std::move(signal));

    return *this;
}

SdlSystemBuilder &SdlSystemBuilder::withInputRename(
        QString signalName, QString originalSignalName, QString originalFunctionName)
{
    auto rename =
            std::make_unique<Rename>(signalName, Rename::Direction::Input, originalSignalName, originalFunctionName);
    m_system.addSignal(std::move(rename));

    return *this;
}

SdlSystemBuilder &SdlSystemBuilder::withOutputRename(
        QString signalName, QString originalSignalName, QString originalFunctionName)
{
    auto rename =
            std::make_unique<Rename>(signalName, Rename::Direction::Output, originalSignalName, originalFunctionName);
    m_system.addSignal(std::move(rename));

    return *this;
}

SdlSystemBuilder &SdlSystemBuilder::withBlock(Block block)
{
    m_system.setBlock(std::move(block));

    return *this;
}

} // namespace tests::common
