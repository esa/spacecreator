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
#include <conversion/common/model.h>
#include <conversion/common/options.h>
#include <memory>
#include <optional>

namespace conversion::iv::importer {

/**
 * @brief   Importer that reads IV XML files
 */
class IvXmlImporter final : public conversion::importer::ModelImporter
{
public:
    /**
     * @brief   Read given SEDS file and produce SEDS model
     *
     * @param   options     List of options
     *
     * @throws  conversion::importer::FileNotFound
     * @throws  seds::symbolreader::SymbolDefinitionReaderException
     * @throws  seds::preprocessor::XmlPreprocessorException
     * @throws  seds::preprocessor::UndefinedExternalReference
     * @throws  seds::validator::ValidatorException
     *
     * @return  Imported SEDS model
     */
    virtual auto importModel(const conversion::Options &options) const -> std::unique_ptr<conversion::Model> override;
};

} // namespace conversion::iv::importer
