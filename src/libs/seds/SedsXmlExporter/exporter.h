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

#pragma once

#include <conversion/common/export/modelexporter.h>

namespace seds::exporter {

class SedsXmlExporter final : public conversion::exporter::ModelExporter
{
public:
    /**
     * @brief   Default constructor
     */
    SedsXmlExporter() = default;
    /**
     * @brief   Deleted copy constructor
     */
    SedsXmlExporter(const SedsXmlExporter &) = delete;
    /**
     * @brief   Default move constructor
     */
    SedsXmlExporter(SedsXmlExporter &&) = default;
    /**
     * @brief   Deleted copy assignment operator
     */
    SedsXmlExporter &operator=(const SedsXmlExporter &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    SedsXmlExporter &operator=(SedsXmlExporter &&) = default;

    /**
     * @brief   Exports SEDS model to a file
     *
     * @param   model       Model to export
     * @param   options     Options for export configuration
     */
    virtual auto exportModel(const conversion::Model *model, const conversion::Options &options) const -> void override;
};

} // namespace seds::exporter
