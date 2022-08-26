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

#include "registrar.h"

#include <conversion/common/modeltype.h>
#include <conversion/msc/MscExporter/exporter.h>
#include <conversion/msc/MscImporter/importer.h>
#include <conversion/sdl/MscToSdlTranslator/translator.h>
#include <memory>

namespace conversion::msc {

bool MscRegistrar::registerCapabilities(conversion::Registry &registry)
{
    auto mscImporter = std::make_unique<importer::MscImporter>();
    auto result = registry.registerImporter(ModelType::Msc, std::move(mscImporter));
    if (!result) {
        return false;
    }

    auto mscToSdlTranslator = std::make_unique<sdl::translator::MscToSdlTranslator>();
    result = registry.registerTranslator({ ModelType::Msc, ModelType::Asn1, ModelType::InterfaceView }, ModelType::Sdl, std::move(mscToSdlTranslator));
    if (!result) {
        return false;
    }

    auto mscExporter = std::make_unique<exporter::MscExporter>();
    return registry.registerExporter(ModelType::Msc, std::move(mscExporter));
}

} // namespace conversion::msc
