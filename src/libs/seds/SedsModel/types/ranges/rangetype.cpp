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
auto enumFromString(const QStringRef &enumStr) -> std::optional<RangeType>
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

} // namespace seds::model
