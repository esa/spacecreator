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

namespace sdl {

class Visitor;

/**
 * @brief   Represents an SDL connection between channel and signalroute
 */
class Connection final
{
public:
    /**
     * @brief   Default constructor
     */
    Connection() = default;
    /**
     * @brief   Constructor
     *
     * @param   channelName         Channel name
     * @param   signalRouteName     Signal route name
     */
    Connection(QString channelName, QString signalRouteName);
    /**
     * @brief   Deleted copy constructor
     */
    Connection(const Connection &) = delete;
    /**
     * @brief   Default move constructor
     */
    Connection(Connection &&) = default;

    /**
     * @brief   Default virtual destructor
     */
    virtual ~Connection() = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Connection &operator=(const Connection &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Connection &operator=(Connection &&) = default;

public:
    /**
     * @brief   Getter for channel name
     *
     * @return  Channel name
     */
    auto channelName() const -> const QString &;
    /**
     * @brief   Setter for channel name
     *
     * @param   name    Channel name
     */
    auto setChannelName(QString name) -> void;

    /**
     * @brief   Getter for signal route name
     *
     * @return  Signal route name
     */
    auto signalRouteName() const -> const QString &;
    /**
     * @brief   Setter for signal route name
     *
     * @param   name    Signal route name
     */
    auto setSignalRouteName(QString name) -> void;

public:
    /**
     * @brief   Visitor acceptor (shall call visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void;

private:
    QString m_channelName;
    QString m_signalRouteName;
};

} // namespace sdl
