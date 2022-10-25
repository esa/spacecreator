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

#include "export/modelexporter.h"

#include <QTextStream>
#include <simulatortrail/SimulatorTrailModel/simulatortrailmodel.h>

namespace simulatortrail::exporter {
/**
 * @brief SimulatorTrailExporter exports SimulatorTrailModel to textual representation.
 */
class SimulatorTrailExporter final : public ::conversion::exporter::ModelExporter
{
public:
    /**
     * @brief   Exports SimulatorTrailModel to file
     *
     * @param   model       Model to export
     * @param   options     Options for export configuration
     */
    void exportModel(const conversion::Model *model, const conversion::Options &options) const override;

private:
    void doExport(const simulatortrail::model::SimulatorTrailModel &model, QTextStream &stream) const;
};

}
