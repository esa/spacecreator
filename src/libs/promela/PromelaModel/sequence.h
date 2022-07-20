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

#include <list>
#include <memory>

namespace promela::model {
class ProctypeElement;
/**
 * @brief Representation of sequence in promela
 *
 * Sequence is a list of steps, where first element might be condition guard
 */
class Sequence final
{
public:
    /**
     * @brief Type of sequence
     */
    enum class Type
    {
        NORMAL,
        ATOMIC,
        D_STEP,
    };

    /**
     * @brief Constructor.
     *
     * @param type Sequence type.
     */
    Sequence(Type type);

    /**
     * @brief Getter for sequence type
     *
     * @return Sequence type
     */
    Type getType() const noexcept;

    /**
     * @brief Getter for content of sequence
     *
     * @return list of sequence elements
     */
    const std::list<std::unique_ptr<ProctypeElement>> &getContent() const noexcept;
    /**
     * @brief Add element to sequence
     *
     * @param element element to add
     */
    void appendElement(std::unique_ptr<ProctypeElement> element);

    /**
     * @brief Construct element and add it to sequence
     *
     * @param args... arguments to create ProctypeElement
     */
    template<typename... Args>
    void appendElement(Args... args)
    {
        m_content.push_back(std::make_unique<ProctypeElement>(std::forward<Args>(args)...));
    }

    Type m_type;
    std::list<std::unique_ptr<ProctypeElement>> m_content;
};
}
