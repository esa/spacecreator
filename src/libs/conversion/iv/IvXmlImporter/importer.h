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
#include <conversion/common/import/modelimporter.h>
#include <memory>

namespace ivm {
class IVPropertyTemplateConfig;
} // namespace ivm

namespace conversion {
class Model;
class Options;
} // namespace conversion

namespace conversion::iv::importer {

/**
 * @brief   Importer that reads IV XML files
 */
class IvXmlImporter final : public conversion::importer::ModelImporter
{
public:
    /**
     * @brief   Reads given IV XML file and produces InterfaceView model
     *
     * @param   options     List of options
     *
     * @throws  conversion::importer::ImportException
     *
     * @return  Imported InterfaceView model
     */
    virtual auto importModel(const conversion::Options &options) const -> std::unique_ptr<conversion::Model> override;

private:
    /**
     * @brief   Initiatlizes config for IV parser
     *
     * @param   options     List of options
     *
     * @return  IV parser config
     */
    auto initConfig(const conversion::Options &options) const -> ivm::IVPropertyTemplateConfig *;
    /**
     * @brief   Use IV parser to parse input xml file
     *
     * @param   options     List of options
     * @param   config      IV parser config
     *
     * @return  IV model
     */
    auto parse(const conversion::Options &options, ivm::IVPropertyTemplateConfig *config) const
            -> std::unique_ptr<conversion::Model>;
};

} // namespace conversion::iv::importer
