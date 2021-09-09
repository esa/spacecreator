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

#include "registrar.h"

#include <conversion/asn1/Asn1Exporter/exporter.h>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
#include <conversion/common/modeltype.h>
#include <memory>

namespace conversion::asn1 {

bool Asn1Registrar::registerCapabilities(conversion::Registry &registry)
{
    bool result = true;
    auto sedsToAsn1Translator = std::make_unique<translator::SedsToAsn1Translator>();
    result &= registry.registerTranslator({ ModelType::Seds }, ModelType::Asn1, std::move(sedsToAsn1Translator));

    if (!result) {
        return false;
    }

    auto asn1exporter = std::make_unique<exporter::Asn1Exporter>();
    result &= registry.registerExporter(ModelType::Asn1, std::move(asn1exporter));

    return result;
}

} // namespace conversion::asn1
