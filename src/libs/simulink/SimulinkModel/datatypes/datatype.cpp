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

#include <QString>

#include "datatype.h"
#include "enumdatatype.h"
#include "aliasdatatype.h"
#include "busdatatype.h"

namespace simulink::model {

const QString &dataTypeNameStr(const DataType &dataType)
{
    const QString *dataTypeName = nullptr;

    std::visit([&dataTypeName](auto &&type) { dataTypeName = &type.name(); }, dataType);

    if (dataTypeName) {
        return *dataTypeName;
    } else {
        static const QString emptyStr = "";
        return emptyStr;
    }
}

} // namespace simulink::model
