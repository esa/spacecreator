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
#include <conversion/seds/IvToSedsTranslator/translator.h>
#include <ivcore/ivlibrary.h>
#include <libiveditor/iveditor.h>
#include <memory>
#include <seds/SedsXmlExporter/exporter.h>
#include <seds/SedsXmlImporter/importer.h>

using seds::exporter::SedsXmlExporter;
using seds::importer::SedsXmlImporter;

namespace conversion::seds {

using translator::IvToSedsTranslator;

bool SedsRegistrar::registerCapabilities(conversion::Registry &registry)
{
    ivm::initIVLibrary();
    ive::initIVEditor();

    auto sedsImporter = std::make_unique<SedsXmlImporter>();
    auto result = registry.registerImporter(ModelType::Seds, std::move(sedsImporter));
    if (!result) {
        return false;
    }

    auto ivToSedsTranslator = std::make_unique<IvToSedsTranslator>();
    result = registry.registerTranslator(
            { ModelType::InterfaceView, ModelType::Asn1 }, ModelType::Seds, std::move(ivToSedsTranslator));
    if (!result) {
        return false;
    }

    auto sedsExporter = std::make_unique<SedsXmlExporter>();
    return registry.registerExporter(ModelType::Seds, std::move(sedsExporter));
}

} // namespace conversion::seds
