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

#include "simulatortrailmodel.h"

namespace simulatortrail::model {

SimulatorTrailModel::SimulatorTrailModel() {}

conversion::ModelType SimulatorTrailModel::modelType() const
{
    return conversion::ModelProperties<SimulatorTrailModel>::type;
}

void SimulatorTrailModel::appendValue(Asn1Acn::ValuePtr value)
{
    m_values.push_back(std::move(value));
}

const std::list<Asn1Acn::ValuePtr> &SimulatorTrailModel::getValues() const
{
    return m_values;
}
}
