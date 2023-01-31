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

#include "types/ranges/rangetype.h"

namespace seds::model {

template<>
auto enumFromString(const QStringView &enumStr) -> std::optional<RangeType>
{
    if (enumStr.compare(QStringLiteral("exclusiveminexclusivemax"), Qt::CaseInsensitive) == 0) {
        return RangeType::ExclusiveMinExclusiveMax;
    } else if (enumStr.compare(QStringLiteral("inclusivemininclusivemax"), Qt::CaseInsensitive) == 0) {
        return RangeType::InclusiveMinInclusiveMax;
    } else if (enumStr.compare(QStringLiteral("inclusiveminexclusivemax"), Qt::CaseInsensitive) == 0) {
        return RangeType::InclusiveMinExclusiveMax;
    } else if (enumStr.compare(QStringLiteral("exclusivemininclusivemax"), Qt::CaseInsensitive) == 0) {
        return RangeType::ExclusiveMinInclusiveMax;
    } else if (enumStr.compare(QStringLiteral("greaterthan"), Qt::CaseInsensitive) == 0) {
        return RangeType::GreaterThan;
    } else if (enumStr.compare(QStringLiteral("lessthan"), Qt::CaseInsensitive) == 0) {
        return RangeType::LessThan;
    } else if (enumStr.compare(QStringLiteral("atleast"), Qt::CaseInsensitive) == 0) {
        return RangeType::AtLeast;
    } else if (enumStr.compare(QStringLiteral("atmost"), Qt::CaseInsensitive) == 0) {
        return RangeType::AtMost;
    } else {
        return std::nullopt;
    }
}

template<>
auto stringFromEnum(RangeType enumType) -> const QString &
{
    switch (enumType) {
    case RangeType::ExclusiveMinExclusiveMax: {
        static const QString strValue = "ExclusiveMinExclusiveMax";
        return strValue;
    }
    case RangeType::InclusiveMinInclusiveMax: {
        static const QString strValue = "InclusiveMinInclusiveMax";
        return strValue;
    }
    case RangeType::InclusiveMinExclusiveMax: {
        static const QString strValue = "InclusiveMinExclusiveMax";
        return strValue;
    }
    case RangeType::ExclusiveMinInclusiveMax: {
        static const QString strValue = "ExclusiveMinInclusiveMax";
        return strValue;
    }
    case RangeType::GreaterThan: {
        static const QString strValue = "GreaterThan";
        return strValue;
    }
    case RangeType::LessThan: {
        static const QString strValue = "LessThan";
        return strValue;
    }
    case RangeType::AtLeast: {
        static const QString strValue = "AtLeast";
        return strValue;
    }
    case RangeType::AtMost: {
        static const QString strValue = "AtMost";
        return strValue;
    }
    default: {
        static const QString unhandled = "unhandledRangeType";
        return unhandled;
    }
    }
}

} // namespace seds::model
