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

namespace conversion::iv {

/**
 * @brief   Possible options for SimulinkToIV conversion
 */
class SimulinkToIvOptions
{
public:
    /** @brief  IVFunction block name */
    inline static const QString ivFunctionName = "SimulinkToIv_IVFunction_Name";
    /** @brief  IVFunction block implementation */
    inline static const QString ivFunctionImplementation = "SimulinkToIv_IVFunction_Implementation";
};

} // namespace conversion::iv
