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

#pragma once

#include <asn1library/asn1/value.h>
#include <conversion/common/model.h>
#include <conversion/common/modelproperties.h>
#include <conversion/common/modeltype.h>
#include <list>

namespace simulatortrail::model {

class SimulatorTrailModel final : public conversion::Model
{
public:
    SimulatorTrailModel();

    /**
     * @brief   Get this model type
     *
     * @return  Model type
     */
    conversion::ModelType modelType() const override;

    void appendValue(Asn1Acn::ValuePtr value);

    const std::list<Asn1Acn::ValuePtr> &getValues() const;

private:
    std::list<Asn1Acn::ValuePtr> m_values;
};
}

namespace conversion {
/**
 * @brief   Specialization for SimulatorTrail model
 */
template<>
struct ModelProperties<::simulatortrail::model::SimulatorTrailModel> {
    /// @brief  Model type
    static const ModelType type = ModelType::SimulatorTrail;
    /// @brief  Model name
    static inline const QString name = ModelTypeProperties<type>::name;
    /// @brief  Model extension
    static inline const QStringList extensions = ModelTypeProperties<type>::extensions;
};
}
