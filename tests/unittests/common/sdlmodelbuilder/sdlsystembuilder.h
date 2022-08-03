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

#include <sdl/SdlModel/block.h>
#include <sdl/SdlModel/channel.h>
#include <sdl/SdlModel/system.h>

namespace tests::common {

class SdlSystemBuilder final
{
public:
    SdlSystemBuilder(QString systemName);

    auto build() -> sdl::System;

    auto withFreeformText(QString text) -> SdlSystemBuilder &;
    auto withSignal(QString signalName) -> SdlSystemBuilder &;
    auto withInputRename(QString signalName, QString originalSignalName, QString originalFunctionName)
            -> SdlSystemBuilder &;
    auto withOutputRename(QString signalName, QString originalSignalName, QString originalFunctionName)
            -> SdlSystemBuilder &;
    auto withChannel(sdl::Channel channel) -> SdlSystemBuilder &;
    auto withBlock(sdl::Block block) -> SdlSystemBuilder &;

private:
    sdl::System m_system;
};

} // namespace tests::common
