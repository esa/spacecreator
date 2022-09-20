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

#include "spintrailtosimulatortrailtranslator.h"

#include <simulatortrail/SimulatorTrailModel/simulatortrailmodel.h>

using namespace simulatortrail::model;
using conversion::Model;
using conversion::ModelType;

namespace simulatortrail::translator {
std::vector<std::unique_ptr<conversion::Model>> SpinTrailToSimulatorTrailTranslator::translateModels(
        std::vector<conversion::Model *> sourceModels, const conversion::Options &options) const
{
    Q_UNUSED(options);
    Q_UNUSED(sourceModels);
    std::vector<std::unique_ptr<Model>> result;

    std::unique_ptr<SimulatorTrailModel> simulatorTrail = std::make_unique<SimulatorTrailModel>();

    result.push_back(std::move(simulatorTrail));

    return result;
}

conversion::ModelType SpinTrailToSimulatorTrailTranslator::getSourceModelType() const
{
    return ModelType::SpinTrail;
}

conversion::ModelType SpinTrailToSimulatorTrailTranslator::getTargetModelType() const
{
    return ModelType::SimulatorTrail;
}

std::set<conversion::ModelType> SpinTrailToSimulatorTrailTranslator::getDependencies() const
{
    return std::set<ModelType> { ModelType::SpinTrail };
}

}
