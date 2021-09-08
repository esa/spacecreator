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

#include <QString>
#include <conversion/common/export/modelexporter.h>
#include <memory>

namespace conversion {
class Model;
class Options;
} // namespace conversion

namespace conversion::iv::exporter {

/**
 * @brief   Exporter that outputs IV XML files
 */
class IvXmlExporter final : public conversion::exporter::ModelExporter
{
public:
    /**
     * @brief   Creates an IV XML file from the passed model
     *
     * @param   model       Model to export
     * @param   options     List of options
     *
     * @throws  conversion::exporter::ExportException
     */
    virtual auto exportModel(const conversion::Model *model, const conversion::Options &options) const -> void override;
};

} // namespace conversion::iv::exporter
