/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2024 N7 Space Sp. z o.o.
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

namespace promela::common {
/**
 * @brief Helper for generation of names of promela entites.
 */
class PromelaNameHelper
{
public:
    /**
     * @brief Create a name of type alias for component of SEQUENCE type.
     * The resulting name uses double underscore separator.
     *
     * @param parentTypeName name of the SEQUENCE type
     * @param componentName name of the component
     * @returns a name of type for component
     */
    static QString createChildTypeName(const QString &parentTypeName, const QString &componentName);

    /**
     * @brief Create a name of type alias for component of SEQUENCE type for use in c_code blocks.
     * The resulting name uses single underscore separator.
     *
     * @param parentTypeName name of the SEQUENCE type
     * @param componentName name of the component
     * @returns a name of type for component
     */
    static QString createChildTypeNameForCCode(const QString &parentTypeName, const QString &componentName);
};
}
