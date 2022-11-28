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

#include "node.h"

#include <QString>
#include <vector>

namespace sdl {

class Syntype final : public Node
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name            Syntype name
     * @param   baseTypeName    Name of the base type
     */
    Syntype(QString name, QString baseTypeName);
    /**
     * @bref    Deleted copy constructor
     */
    Syntype(const Syntype &) = delete;
    /**
     * @brief   Default move constructor
     */
    Syntype(Syntype &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Syntype &operator=(const Syntype &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Syntype &operator=(Syntype &&) = default;

public:
    /**
     * @brief   Getter for base type name
     *
     * @return  Base type name
     */
    auto baseTypeName() const -> const QString &;
    /**
     * @brief   Setter for base type name
     *
     * @param   baseTypeName        Base type name to set
     */
    auto setBaseTypeName(QString baseTypeName) -> void;

    /**
     * @brief   Getter for syntype constants
     *
     * @return  Vector of constants
     */
    auto constants() const -> const std::vector<QString> &;
    /**
     * @brief   Add single value constant
     *
     * @param   value       Constant value
     */
    auto addValueConstant(const uint64_t value) -> void;

public:
    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    const static inline QString m_valueConstantTemplate = "%1";

private:
    QString m_baseTypeName;
    std::vector<QString> m_constants;
};

} // namespace sdl;
