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

#include <QString>

namespace tmc {
class TmcConstants
{
public:
    inline static const QString SETTINGS_TMC_SPIN_EXE_KEY = "SpaceCreator/tmcSpinExe";
    inline static const QString SETTINGS_TMC_SPIN_DEFAULT_OUTPUT_DIRECTORY = "SpaceCreator/tmcSpinOutputDir";
    inline static const QString SETTINGS_TMC_SDL2PROMELA_TIMEOUT = "SpaceCreator/tmcSdl2PromelaTimeout";
    inline static const QString SETTINGS_TMC_CCOMPILER_TIMEOUT = "SpaceCreator/tmcCCompilerTimeout";
    inline static const QString SETTINGS_TMC_EXTERNAL_COMMAND_TIMEOUT = "SpaceCreator/tmcExternalCommandTimeout";
    inline static const QString SETTINGS_TMC_COMPILER_FLAGS = "SpaceCreator/tmcCompilerFlags";
};
}
