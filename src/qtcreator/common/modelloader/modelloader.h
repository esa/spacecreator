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
#include <asn1model.h>
#include <conversion/common/model.h>
#include <csv/CsvModel/csvmodel.h>
#include <ivmodel.h>
#include <memory>

namespace plugincommon {

/**
 * @brief  Class for loading models from text files
 *
 */
class ModelLoader
{
public:
    /**
     * @brief  load InterfaceView model from a file
     *
     * @param  ivConfigFilename  name of InterfaceView configuration file
     * @param  ivFilename        name of the file describing InterfaceView
     *
     * @return InterfaceView model
     */
    static auto loadIvModel(const QString &ivConfigFilename, const QString &ivFilename)
            -> std::unique_ptr<ivm::IVModel>;

    /**
     * @brief  load SOIS Electronic Datasheet model from a file
     *
     * @param  sedsFilename  name of the file with SEDS
     *
     * @return SEDS model
     */
    static auto loadSedsModel(const QString &sedsFilename) -> std::unique_ptr<conversion::Model>;

    /**
     * @brief  load ASN.1 model from a file
     *
     * @param  filename  name of the file with ASN.1 definitions
     *
     * @return ASN.1 model
     */
    static auto loadAsn1Model(const QString &filename) -> std::unique_ptr<Asn1Acn::Asn1Model>;

    /**
     * @brief  load CSV data model from a file
     *
     * @param  filename  name of the file with CSV data
     *
     * @return CSV model
     */
    static auto loadCsvModel(const QString &filename) -> std::unique_ptr<csv::CsvModel>;
};

} // namespace plugincommon
