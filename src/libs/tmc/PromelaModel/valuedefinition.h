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
#include <cstdint>

namespace tmc::promela::model {
/**
 * @brief ValueDefinition is used to give names to integer values.
 */
class ValueDefinition final
{
public:
    /**
     *@brief Constructor.
     *
     * @param name name of the defintion
     * @param value value of the definition
     */
    ValueDefinition(QString name, int32_t value);

    /**
     * @brief Getter for the name of definition
     *
     * @return Name of the definition
     */
    const QString &getName() const noexcept;
    /**
     * @brief Setter for name of definition
     *
     * @param name new name of definition
     */
    void setName(QString name);
    /**
     * @brief Getter for value of definition
     *
     * @return Value of definition
     */
    int32_t getValue() const noexcept;
    /**
     * @brief Setter for value of definition
     *
     * @param value new value of definition
     */
    void setValue(int32_t value);

private:
    QString m_name;
    int32_t m_value;
};
}
