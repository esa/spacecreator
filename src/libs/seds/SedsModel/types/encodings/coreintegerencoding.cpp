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

#include "types/encodings/coreintegerencoding.h"

namespace seds::model {

template<>
auto enumFromString(const QStringRef &enumStr) -> std::optional<CoreIntegerEncoding>
{
    if (enumStr.compare(QStringLiteral("unsigned"), Qt::CaseInsensitive) == 0) {
        return CoreIntegerEncoding::Unsigned;
    } else if (enumStr.compare(QStringLiteral("signmagnitude"), Qt::CaseInsensitive) == 0) {
        return CoreIntegerEncoding::SignMagnitude;
    } else if (enumStr.compare(QStringLiteral("twoscomplement"), Qt::CaseInsensitive) == 0) {
        return CoreIntegerEncoding::TwosComplement;
    } else if (enumStr.compare(QStringLiteral("onescomplement"), Qt::CaseInsensitive) == 0) {
        return CoreIntegerEncoding::OnesComplement;
    } else if (enumStr.compare(QStringLiteral("bcd"), Qt::CaseInsensitive) == 0) {
        return CoreIntegerEncoding::Bcd;
    } else if (enumStr.compare(QStringLiteral("packedbcd"), Qt::CaseInsensitive) == 0) {
        return CoreIntegerEncoding::PackedBcd;
    } else {
        return std::nullopt;
    }
}

} // namespace seds::model
