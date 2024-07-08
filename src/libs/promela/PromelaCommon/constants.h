/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2024 N7 Space Sp. z o.o.
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

namespace promela::common {
/**
 * @brief Constants used in promela modules.
 */
class PromelaConstants
{
public:
    /** @brief a common part of printf message for sending */
    inline static const QString channelSendMessage = "channel_send";
    /** @brief a common part of printf message for receiving */
    inline static const QString channelRecvMessage = "channel_recv";
    /** @brief an unit of indent used for promela and c code inside promela */
    inline static const QString baseIndent = "    ";
    /** @brief suffix for type of array element */
    inline static const QString sequenceOfElementTypeNameSuffix = "elem";
};
}
