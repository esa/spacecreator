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

#include "variabledeclaration.h"

#include <QString>
#include <memory>

namespace sdl {

/**
 * @brief   Represents an SDL variable reference
 */
class VariableReference final
{
public:
    /**
     * @brief   Default constructor
     *
     * @param   declaration   referenced variable declaration
     */
    VariableReference(
            const std::shared_ptr<VariableDeclaration> &declaration = std::make_shared<VariableDeclaration>());

    /**
     * @brief   Deleted copy constructor
     */
    VariableReference(const VariableReference &) = delete;

    /**
     * @brief   Default move constructor
     */
    VariableReference(VariableReference &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    VariableReference &operator=(const VariableReference &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    VariableReference &operator=(VariableReference &&) = default;

    /**
     * @brief    Getter for the referenced variable declaration
     *
     * @return   const reference to pointer to variable declaration
     */
    auto declaration() const -> const VariableDeclaration *;

    /**
     * @brief    Setter for the referenced variable declaration
     *
     * @param    declaration const reference to pointer to variable declaration
     */
    auto setDeclaration(const std::shared_ptr<VariableDeclaration> &declaration) -> void;

private:
    std::shared_ptr<VariableDeclaration> m_declaration;
};

} // namespace sdl
