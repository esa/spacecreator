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

#include "complexity.h"

namespace simulink::model {

template<>
auto enumFromString(const StringRef &enumStr) -> std::optional<Complexity>
{
    if (enumStr.compare(QStringLiteral("real"), Qt::CaseInsensitive) == 0) {
        return Complexity::Real;
    } else if (enumStr.compare(QStringLiteral("complex"), Qt::CaseInsensitive) == 0) {
        return Complexity::Complex;
    } else {
        return std::nullopt;
    }
}

template<>
auto stringFromEnum(Complexity enumType) -> const QString &
{
    switch (enumType) {
    case Complexity::Real: {
        static const QString strValue = "Real";
        return strValue;
    }
    case Complexity::Complex: {
        static const QString strValue = "Complex";
        return strValue;
    }
    default: {
        static const QString unhandled = "unhandledSignalType";
        return unhandled;
    }
    }
}

} // namespace simulink::model
