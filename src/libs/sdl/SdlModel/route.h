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
#include <QStringList>

namespace sdl {

class Visitor;

class Route final
{
public:
    /**
     * @brief   Default constructor
     */
    Route() = default;
    /**
     * @brief   Deleted copy constructor
     */
    Route(const Route &system) = default;
    /**
     * @brief   Default move constructor
     */
    Route(Route &&system) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Route &operator=(const Route &) = default;
    /**
     * @brief   Default move assignment operator
     */
    Route &operator=(Route &&) = default;

public:
    /**
     * @brief   Getter for from
     *
     * @return  From
     */
    auto from() const -> const QString &;
    /**
     * @brief   Setter forr from
     *
     * @param   from    From
     */
    auto setFrom(QString from) -> void;

    /**
     * @brief   Getter for to
     *
     * @return  To
     */
    auto to() const -> const QString &;
    /**
     * @brief   Setter for to
     *
     * @param   to      To
     */
    auto setTo(QString to) -> void;

    /**
     * @brief   Getter for with
     *
     * @return  With
     */
    auto with() const -> const QStringList &;
    /**
     * @brief   Add for with signal
     *
     * @param   with    With signal
     */
    auto addWith(QString with) -> void;
    /**
     * @brief   Setter for with signals
     *
     * @param   with    With signals
     */
    auto setWith(QStringList with) -> void;

public:
    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    auto accept(Visitor &visitor) const -> void;

private:
    QString m_from;
    QString m_to;
    QStringList m_with;
};

} // namespace sdl
