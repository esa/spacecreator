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

#include "types/encodings/coreencodingandprecision.h"

namespace seds::model {

template<>
auto enumFromString(const QStringRef &enumStr) -> std::optional<CoreEncodingAndPrecision>
{
    if (enumStr.compare(QStringLiteral("ieee754_2008_single"), Qt::CaseInsensitive) == 0) {
        return CoreEncodingAndPrecision::IeeeSingle;
    } else if (enumStr.compare(QStringLiteral("ieee754_2008_double"), Qt::CaseInsensitive) == 0) {
        return CoreEncodingAndPrecision::IeeeDouble;
    } else if (enumStr.compare(QStringLiteral("ieee754_2008_quad"), Qt::CaseInsensitive) == 0) {
        return CoreEncodingAndPrecision::IeeeQuad;
    } else if (enumStr.compare(QStringLiteral("milstd_1750a_simple"), Qt::CaseInsensitive) == 0) {
        return CoreEncodingAndPrecision::MilstdSimple;
    } else if (enumStr.compare(QStringLiteral("milstd_1750a_extended"), Qt::CaseInsensitive) == 0) {
        return CoreEncodingAndPrecision::MilstdExtended;
    } else {
        return std::nullopt;
    }
}

} // namespace seds::model
