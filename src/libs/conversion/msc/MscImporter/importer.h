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

#include <QString>
#include <conversion/common/import/modelimporter.h>
#include <memory>

namespace conversion {
class Model;
class Options;
} // namespace conversion

namespace conversion::msc::importer {

/**
 * @brief   Importer that reads MSC files
 */
class MscImporter final : public conversion::importer::ModelImporter
{
public:
    /**
     * @brief   Reads given MSC file and produces MSC model
     *
     * @param   options     List of options
     *
     * @return  Imported MSC model
     */
    virtual auto importModel(const conversion::Options &options) const -> std::unique_ptr<conversion::Model> override;
};

} // namespace conversion::msc::importer
