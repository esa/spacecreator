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

namespace spintrail::model {
/**
 * @brief Base class for all Spin Trail events
 */
class TrailEvent
{
public:
    /**
     * @brief Type of Spin Trail event
     */
    enum class EventType
    {
        CHANNEL_EVENT,
        CONTINUOUS_SIGNAL,
        SET_TIMER_EVENT,
        RESET_TIMER_EVENT,
        UNHANDLED_INPUT_EVENT,
    };

    /**
     * @brief default destructor
     */
    virtual ~TrailEvent() = default;

    /**
     * @brief Getter for event type
     *
     * @return Type of event
     */
    virtual EventType getEventType() const = 0;
};
}
