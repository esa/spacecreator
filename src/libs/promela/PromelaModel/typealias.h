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

#include "basictypes.h"
#include "utyperef.h"

#include <QString>
#include <variant>

namespace promela::model {
/**
 * @brief TypeAlias is used to define alias for other type in promela.
 */
class TypeAlias final
{
public:
    /**
     * @brief variant to represent type.
     *
     * The data type can be basic type or reference to other defined type
     */
    using Type = std::variant<BasicType, UtypeRef>;

    /**
     * @brief Constructor.
     *
     * @param name name of the alias
     * @param type type
     */
    TypeAlias(QString name, Type type);

    /**
     * @brief Getter for name of alias
     *
     * @return alias name
     */
    const QString &getName() const noexcept;
    /**
     * @brief Setter for name of alias
     *
     * @param name new name of alias
     */
    void setName(QString name);

    /**
     * @brief Getter for type of alias
     *
     * @return alias type
     */
    const Type &getType() const noexcept;
    /**
     * @brief Setter for type of alias
     *
     * @param type new type of alias
     */
    void setType(Type type);

private:
    QString m_name;
    Type m_type;
};
}
