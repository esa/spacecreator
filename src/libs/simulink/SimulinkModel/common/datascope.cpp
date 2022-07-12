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

#include "datascope.h"

namespace simulink::model {

template<>
auto enumFromString(const StringRef &enumStr) -> std::optional<DataScope>
{
    if (enumStr.compare(QStringLiteral("Auto"), Qt::CaseInsensitive) == 0) {
        return DataScope::Auto;
    } else if (enumStr.compare(QStringLiteral("Exported"), Qt::CaseInsensitive) == 0) {
        return DataScope::Exported;
    } else if (enumStr.compare(QStringLiteral("Imported"), Qt::CaseInsensitive) == 0) {
        return DataScope::Imported;
    } else {
        return std::nullopt;
    }
}

template<>
auto stringFromEnum(DataScope enumType) -> const QString &
{
    switch (enumType) {
    case DataScope::Auto: {
        static const QString strValue = "Auto";
        return strValue;
    }
    case DataScope::Exported: {
        static const QString strValue = "Exported";
        return strValue;
    }
    case DataScope::Imported: {
        static const QString strValue = "Imported";
        return strValue;
    }
    default: {
        static const QString unhandled = "unhandledSignalType";
        return unhandled;
    }
    }
}

} // namespace simulink::model
