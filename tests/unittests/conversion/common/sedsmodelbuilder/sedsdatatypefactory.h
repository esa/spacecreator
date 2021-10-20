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

#include <QStringList>
#include <seds/SedsModel/types/datatype.h>

namespace tests::conversion::common {

/**
 * @brief   Factory for creating SEDS data types
 */
class SedsDataTypeFactory final
{
public:
    /**
     * @brief   Create array data type
     *
     * @param   name            Name for the created type
     * @param   itemTypeName    Name of the array item type
     *
     * @return  Created data type
     */
    static auto createArray(QString name, QString itemTypeName) -> seds::model::ArrayDataType;
    /**
     * @brief   Create binary data type
     *
     * @param   name    Name for the created type
     *
     * @return  Created data type
     */
    static auto createBinary(QString name) -> seds::model::BinaryDataType;
    /**
     * @brief   Create boolean data type
     *
     * @param   name    Name for the created type
     *
     * @return  Created data type
     */
    static auto createBoolean(QString name) -> seds::model::BooleanDataType;
    /**
     * @brief   Create container data type
     *
     * @param   name    Name for the created type
     *
     * @return  Created data type
     */
    static auto createContainer(QString name) -> seds::model::ContainerDataType;
    /**
     * @brief   Create enumerated data type
     *
     * @param   name    Name for the created type
     * @param   elems   List of enum items names
     *
     * @return  Created data type
     */
    static auto createEnumerated(QString name, QStringList elems) -> seds::model::EnumeratedDataType;
    /**
     * @brief   Create float data type
     *
     * @param   name    Name for the created type
     *
     * @return  Created data type
     */
    static auto createFloat(QString name) -> seds::model::FloatDataType;
    /**
     * @brief   Create integer data type
     *
     * @param   name    Name for the created type
     *
     * @return  Created data type
     */
    static auto createInteger(QString name) -> seds::model::IntegerDataType;
    /**
     * @brief   Create string data type
     *
     * @param   name    Name for the created type
     *
     * @return  Created data type
     */
    static auto createString(QString name) -> seds::model::StringDataType;
};

} // namespace tests::conversion::common
