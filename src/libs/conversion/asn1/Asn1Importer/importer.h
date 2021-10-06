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

namespace conversion {
class Model;
class Options;
} // namespace conversion

namespace conversion::asn1::importer {

/**
 * @brief   Importer that reads ASN.1 files
 */
class Asn1Importer final : public conversion::importer::ModelImporter
{
public:
    /**
     * @brief   Reads given ASN.1 file and produces ASN.1 model
     *
     * @param   options     List of options
     *
     * @throws  conversion::importer::ImportException
     *
     * @return  Imported ASN.1 model
     */
    virtual auto importModel(const conversion::Options &options) const -> std::unique_ptr<conversion::Model> override;
};

} // namespace conversion::asn1::importer
