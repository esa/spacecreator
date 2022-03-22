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

#include "specialized/patcherfunctionstranslator.h"

#include <conversion/common/overloaded.h>

namespace conversion::asn1::translator {

std::vector<Asn1Acn::PatcherFunction> PatcherFunctionsTranslator::translate(
        const seds::model::ContainerDataType &sedsType)
{
    std::vector<Asn1Acn::PatcherFunction> result;

    for (const auto &entry : sedsType.entries()) {
        // clang-format off
        std::visit(overloaded {
            [&](const seds::model::ErrorControlEntry &errorControlEntry) {
                auto patcherFunction = buildErrorControlEntryPatcherFunction(errorControlEntry);
                result.push_back(std::move(patcherFunction));
            },
            [&](const seds::model::LengthEntry &lengthEntry) {
                auto patcherFunction = buildLengthEntryPatcherFunction(lengthEntry);
                result.push_back(std::move(patcherFunction));
            },
            [&](const auto &e) {
                Q_UNUSED(e)
            }
        }, entry);
        // clang-format on
    }

    return result;
}

Asn1Acn::PatcherFunction PatcherFunctionsTranslator::buildErrorControlEntryPatcherFunction(
        const seds::model::ErrorControlEntry &errorControlEntry)
{
    Q_UNUSED(errorControlEntry);
    return { "encodeError", "decodeError" };
}

Asn1Acn::PatcherFunction PatcherFunctionsTranslator::buildLengthEntryPatcherFunction(
        const seds::model::LengthEntry &lengthEntry)
{
    Q_UNUSED(lengthEntry);
    return { "encodeLength", "decodeLength" };
}

} // namespace conversion::asn1::translator
