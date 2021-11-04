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

namespace promela::model {
/**
 * @brief Representation of a reference to mtype in promela.
 */
class MtypeRef final
{
public:
    /**
     * @brief Constructor.
     *
     * Construct reference to unnamed mtype.
     */
    MtypeRef();
    /**
     * @brief Constructor.
     *
     * Constructs reference to named mtype.
     *
     * @param ref name of mtype.
     */
    MtypeRef(QString ref);

    /**
     * @brief Getter for the name of mtype
     *
     * @return name of mtype
     */
    const QString &getName() const noexcept;
    /**
     * @brief Checks if referenced mtype is named
     *
     * @return true if referenced mtype is named mtype, otherwise false
     */
    bool isNamed() const noexcept;

private:
    bool m_named;
    QString m_ref;
};
}
