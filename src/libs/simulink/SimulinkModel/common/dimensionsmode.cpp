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

#include "dimensionsmode.h"

namespace simulink::model {

template<>
auto enumFromString(const StringRef &enumStr) -> std::optional<DimensionsMode>
{
    if (enumStr.compare(QStringLiteral("Fixed"), Qt::CaseInsensitive) == 0) {
        return DimensionsMode::Fixed;
    } else if (enumStr.compare(QStringLiteral("Variable"), Qt::CaseInsensitive) == 0) {
        return DimensionsMode::Variable;
    } else {
        return std::nullopt;
    }
}

template<>
auto stringFromEnum(DimensionsMode enumType) -> const QString &
{
    switch (enumType) {
    case DimensionsMode::Fixed: {
        static const QString strValue = "Fixed";
        return strValue;
    }
    case DimensionsMode::Variable: {
        static const QString strValue = "Variable";
        return strValue;
    }
    default: {
        static const QString unhandled = "unhandledSignalType";
        return unhandled;
    }
    }
}

} // namespace simulink::model
