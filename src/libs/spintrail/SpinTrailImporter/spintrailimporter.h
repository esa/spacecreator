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

#include <QRegularExpression>
#include <conversion/common/import/modelimporter.h>
#include <spintrail/SpinTrailModel/spintrailmodel.h>

namespace conversion {
class Model;
class Options;
}

namespace spintrail::importer {
/**
 * @brief Importer to read Spin Trail
 */
class SpinTrailImporter final : public conversion::importer::ModelImporter
{
public:
    /**
     * @brief   Import SpinTrail from file
     *
     * @param   options     List of options
     *
     * @return  Imported SpinTrail model
     */
    virtual std::unique_ptr<conversion::Model> importModel(const conversion::Options &options) const override;

private:
    constexpr static inline size_t CONTINUOUS_SIGNAL_EVENT_FUNCTION_NAME = 1;
    constexpr static inline size_t SET_TIMER_EVENT_FUNCTION_NAME = 1;
    constexpr static inline size_t SET_TIMER_EVENT_INTERFACE_NAME = 2;
    constexpr static inline size_t SET_TIMER_EVENT_INTERVAL = 3;
    constexpr static inline size_t RESET_TIMER_EVENT_FUNCTION_NAME = 1;
    constexpr static inline size_t RESET_TIMER_EVENT_INTERFACE_NAME = 2;
    constexpr static inline size_t CHANNEL_EVENT_CHANNEL_NAME = 1;
    constexpr static inline size_t CHANNEL_EVENT_PARAMETER_LIST = 2;
    constexpr static inline size_t UNHANDLED_INPUT_EVENT_FUNCTION_NAME = 1;
    constexpr static inline size_t UNHANDLED_INPUT_EVENT_INTERFACE_NAME = 2;

private:
    void processLine(spintrail::model::SpinTrailModel &model, const QString &line) const;
};
}
