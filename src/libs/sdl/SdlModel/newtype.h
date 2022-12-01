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

namespace sdl {

class Newtype final : public Node
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name                Newtype name
     * @param   indexingTypeName    Name of the indexing type
     * @param   elementTypeName     Name of the element type
     */
    Newtype(QString name, QString indexingTypeName, QString elementTypeName);
    /**
     * @bref    Deleted copy constructor
     */
    Newtype(const Newtype &) = delete;
    /**
     * @brief   Default move constructor
     */
    Newtype(Newtype &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Newtype &operator=(const Newtype &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Newtype &operator=(Newtype &&) = default;

public:
    /**
     * @brief   Getter for indexing type name
     *
     * @return  Indexing type name
     */
    auto indexingTypeName() const -> const QString &;
    /**
     * @brief   Setter for indexing type name
     *
     * @param   indexingTypeName        Indexing type name
     */
    auto setIndexingTypeName(QString indexingTypeName) -> void;

    /**
     * @brief   Getter for element type name
     *
     * @return  Element type name
     */
    auto elementTypeName() const -> const QString &;
    /**
     * @brief   Setter for element type name
     *
     * @param   elementTypeName        Element type name
     */
    auto setElementTypeName(QString elementTypeName) -> void;

public:
    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    QString m_indexingTypeName;
    QString m_elementTypeName;
};

} // namespace sdl
