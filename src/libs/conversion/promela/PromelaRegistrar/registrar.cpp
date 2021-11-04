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

#include <conversion/common/modeltype.h>
#include <promela/Asn1ToPromelaTranslator/translator.h>
#include <promela/PromelaExporter/promelaexporter.h>

using promela::exporter::PromelaExporter;
using promela::translator::Asn1ToPromelaTranslator;

namespace conversion::promela {
bool PromelaRegistrar::registerCapabilities(conversion::Registry &registry)
{
    auto promelaExporter = std::make_unique<PromelaExporter>();
    bool result = registry.registerExporter(ModelType::Promela, std::move(promelaExporter));
    if (!result) {
        return false;
    }

    auto asn1ToPromelaTranslator = std::make_unique<Asn1ToPromelaTranslator>();

    return registry.registerTranslator({ ModelType::Asn1 }, ModelType::Promela, std::move(asn1ToPromelaTranslator));
}
}
