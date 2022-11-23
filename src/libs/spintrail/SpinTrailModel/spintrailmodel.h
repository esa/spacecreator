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

#include "trailevent.h"

#include <conversion/common/model.h>
#include <conversion/common/modelproperties.h>
#include <conversion/common/modeltype.h>
#include <list>
#include <memory>

namespace spintrail::model {
/**
 * @brief Representation of Spin Trail model
 */
class SpinTrailModel final : public conversion::Model
{
public:
    /**
     * @brief Constructor. Constructs empty model
     */
    SpinTrailModel();

    /**
     * @brief   Get this model type
     *
     * @return  Model type
     */
    conversion::ModelType modelType() const override;

    /**
     * @brief Append event to the model
     *
     * @param event event to be added
     */
    void appendEvent(std::unique_ptr<TrailEvent> event);

    /**
     * @brief Getter for all events in the model
     *
     * @return List of all events in model
     */
    const std::list<std::unique_ptr<TrailEvent>> &getEvents() const;

private:
    std::list<std::unique_ptr<TrailEvent>> m_events;
};
}

namespace conversion {
/**
 * @brief   Specialization for SpinTrail model
 */
template<>
struct ModelProperties<::spintrail::model::SpinTrailModel> {
    /// @brief  Model type
    static const ModelType type = ModelType::SpinTrail;
    /// @brief  Model name
    static inline const QString name = ModelTypeProperties<type>::name;
    /// @brief  Model extension
    static inline const QStringList extensions = ModelTypeProperties<type>::extensions;
};
}
