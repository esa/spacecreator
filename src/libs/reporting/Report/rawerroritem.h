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

namespace reporting {

/**
 * @brief Structure to hold raw error messages for parsing.
 */
struct RawErrorItem {
    /** Path to the file containing spin traces */
    QString spinTraceFile;
    /** Path to scenario.sim associated with the error */
    QString scenarioFile;
    /** Cycle number identified as a error source in the spin trace file,
     * along with the detected observer name */
    std::pair<QString, int> possibleCycleSource;
};

}
