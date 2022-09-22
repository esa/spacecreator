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

#include <simulatortrail/SimulatorTrailExporter/simulatortrailexporter.h>
#include <simulatortrail/SpinTrailToSimulatorTrailTranslator/spintrailtosimulatortrailtranslator.h>

using simulatortrail::exporter::SimulatorTrailExporter;
using simulatortrail::translator::SpinTrailToSimulatorTrailTranslator;

namespace conversion::simulatortrail {
bool SimulatorTrailRegistrar::registerCapabilities(conversion::Registry &registry)
{
    std::unique_ptr<SimulatorTrailExporter> exporter = std::make_unique<SimulatorTrailExporter>();
    bool result = registry.registerExporter(ModelType::SimulatorTrail, std::move(exporter));

    if (!result) {
        return false;
    }

    std::unique_ptr<SpinTrailToSimulatorTrailTranslator> translator =
            std::make_unique<SpinTrailToSimulatorTrailTranslator>();

    result = registry.registerTranslator({ ModelType::SpinTrail, ModelType::InterfaceView, ModelType::Asn1 },
            ModelType::SimulatorTrail, std::move(translator));

    if (!result) {
        return false;
    }

    return true;
}
}
