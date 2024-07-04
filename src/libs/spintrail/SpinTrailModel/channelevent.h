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

#include <QList>
#include <QString>

namespace spintrail::model {
/**
 * @brief Representation of channel involved activity in Spin.
 */
class ChannelEvent : public TrailEvent
{
public:
    /**
     * @brief Type of channel operation
     */
    enum class Type
    {
        Send,
        Recv,
    };

    /**
     * @brief Constructor
     *
     * @param type type of channel event
     * @param channelName name of the channel
     * @param parameters list of values send/received from channel
     */
    ChannelEvent(Type type, QString channelName, QList<QString> parameters);

    /**
     * @brief Setter for channel operation event type
     *
     * @param type type of channel operation
     */
    void setType(Type type);
    /**
     * @brief Getter for channel operation event type
     *
     * @return Type of channel operation
     */
    Type getType() const;

    /**
     * @brief Setter for channel name
     *
     * @param channelName name of channel
     */
    void setChannelName(QString channelName);
    /**
     * @brief Getter for channel name
     *
     * @return Name of channel
     */
    const QString &getChannelName() const;

    /**
     * @brief Setter for parameters
     *
     * @param parameters list of parameters
     */
    void setParameters(QList<QString> parameters);
    /**
     * @brief Getter for parameters
     *
     * @return List of parameters
     */
    const QList<QString> &getParameters() const;

    /**
     * @brief Getter for event type
     *
     * @return Type of event
     */
    EventType getEventType() const override;

private:
    Type m_type;
    QString m_channelName;
    QList<QString> m_parameters;
};
}
