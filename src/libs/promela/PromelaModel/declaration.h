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

#include "channelinit.h"
#include "datatype.h"
#include "expression.h"

#include <QString>
#include <optional>

namespace promela::model {
/**
 * @brief Representation of declaration in promela.
 *
 * The declaration can be used to declare a variable or a field inside user defined data type.
 */
class Declaration final
{
public:
    /**
     * @brief Declaration visibility.
     */
    enum class Visibility
    {
        NORMAL,
        SHOW,
        HIDDEN
    };

    using InitExpression = std::variant<ChannelInit, Expression>;

public:
    /**
     * @brief Constructor.
     *
     * @param type type of declared variable or field
     * @param name name of declared variable or field
     * @param visibility visibility of declared variable or field
     */
    Declaration(DataType type, QString name, Visibility visibility = Visibility::NORMAL);

    /**
     * @brief Getter for declaration data type.
     *
     * @return declaration data type
     */
    const DataType &getType() const noexcept;
    /**
     * @brief Getter for declaration name.
     *
     * @return name of the declared variable or field
     */
    const QString &getName() const noexcept;
    /**
     * @brief Getter for declaration visibility.
     *
     * @return declaration visibility.
     */
    Visibility getVisibility() const noexcept;
    /**
     * @brief Checks if declaration contains an initialization expression
     *
     * @return true if the declaration has initialization expression, otherwise false
     */
    bool hasInit() const noexcept;
    /**
     * @brief Getter for initialization expression.
     *
     * @return intialization expression or empty.
     */
    const std::optional<InitExpression> &getInit() const noexcept;
    /**
     * @brief Setter for initialization expression.
     *
     * @param initExpression InitExpression to set as initialization expression for declaration.
     */
    void setInit(const InitExpression &initExpression);

private:
    DataType m_type;
    QString m_name;
    Visibility m_visibility;
    std::optional<InitExpression> m_init;
};
}
