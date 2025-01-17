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

#include "trailevent.h"

#include <QString>

namespace spintrail::model {
/**
 * @brief Representation of unhandled input in Spin Trail
 */
class UnhandledInputEvent : public TrailEvent
{
public:
    /**
     * @brief Constructor.
     *
     * @param functionName name of the function
     * @param interfaceName name of the interface
     */
    UnhandledInputEvent(QString functionName, QString interfaceName);

    /**
     * @brief Getter for function name
     *
     * @return name of the function
     */
    const QString &getFunctionName() const;
    /**
     * @brief Getter for name of the timer
     *
     * @return name of the timer.
     */
    const QString &getInterfaceName() const;

    /**
     * @brief Getter for event type
     *
     * @return Type of event
     */
    EventType getEventType() const override;

private:
    QString m_functionName;
    QString m_interfaceName;
};
}
