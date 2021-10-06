/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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
 * @brief   Interface for SDL entities that could be visited
 */
class Node
{
protected:
    /**
     * @brief   Constructor
     */
    Node(QString name);

    /**
     * @brief   Deleted copy constructor
     */
    Node(const Node &other) = delete;

    /**
     * @brief   Move constructor
     */
    Node(Node &&) = default;

public:
    virtual ~Node();

    /**
     * @brief   Setter for this model type
     *
     * @param   name name to set
     */
    auto setName(QString name) -> void;

    /**
     * @brief   Getter for this node name
     *
     * @return  node name
     */
    auto name() const -> const QString &;

    /**
     * @brief   Visitor acceptor (shall call visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void = 0;

private:
    QString m_name;
};

} // namespace sdl
