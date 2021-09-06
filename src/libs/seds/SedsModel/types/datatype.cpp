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

#include "types/datatype.h"

#include "types/arraydatatype.h"
#include "types/binarydatatype.h"
#include "types/booleandatatype.h"
#include "types/containerdatatype.h"
#include "types/enumerateddatatype.h"
#include "types/floatdatatype.h"
#include "types/integerdatatype.h"
#include "types/stringdatatype.h"
#include "types/subrangedatatype.h"

namespace seds::model {

const QString &dataTypeNameStr(const DataType &dataType)
{

    switch (dataType.index()) {
    case 0:
        return std::get<ArrayDataType>(dataType).nameStr();
    case 1:
        return std::get<BinaryDataType>(dataType).nameStr();
    case 2:
        return std::get<BooleanDataType>(dataType).nameStr();
    case 3:
        return std::get<ContainerDataType>(dataType).nameStr();
    case 4:
        return std::get<EnumeratedDataType>(dataType).nameStr();
    case 5:
        return std::get<FloatDataType>(dataType).nameStr();
    case 6:
        return std::get<IntegerDataType>(dataType).nameStr();
    case 7:
        return std::get<StringDataType>(dataType).nameStr();
    case 8:
        return std::get<SubRangeDataType>(dataType).nameStr();
    default: {
        static QString emptyStr("");
        return emptyStr;
    }
    }
}

} // namespace seds::model
