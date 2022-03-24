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

#include "modelloader.h"

#include "../messagestrings.h"

#include <asn1library/asn1/asn1model.h>
#include <conversion/asn1/Asn1Importer/importer.h>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvXmlImporter/importer.h>
#include <csv/CsvImporter/csvimporter.h>
#include <csv/CsvOptions/options.h>
#include <memory>
#include <modeltype.h>
#include <seds/SedsModel/sedsmodel.h>
#include <seds/SedsOptions/options.h>
#include <seds/SedsXmlImporter/importer.h>

using conversion::ModelType;

namespace plugincommon {

auto ModelLoader::loadIvModel(const QString &ivConfigFilename, const QString &ivFilename)
        -> std::unique_ptr<ivm::IVModel>
{
    std::unique_ptr<conversion::Model> model;

    conversion::Options options;
    options.add(conversion::iv::IvOptions::inputFilepath, ivFilename);
    options.add(conversion::iv::IvOptions::configFilepath, ivConfigFilename);

    conversion::iv::importer::IvXmlImporter ivImporter;
    model = ivImporter.importModel(options);

    std::unique_ptr<ivm::IVModel> output;
    output.reset(static_cast<ivm::IVModel *>(model.get()));
    model.release();

    return output;
}

auto ModelLoader::loadSedsModel(const QString &sedsFilename) -> std::unique_ptr<conversion::Model>
{
    std::unique_ptr<conversion::Model> model;

    conversion::Options options;
    options.add(conversion::seds::SedsOptions::inputFilepath, sedsFilename);

    seds::importer::SedsXmlImporter sedsImporter;
    model = sedsImporter.importModel(options);

    return model;
}

auto ModelLoader::loadAsn1Model(const QString &filename) -> std::unique_ptr<Asn1Acn::Asn1Model>
{
    std::unique_ptr<conversion::Model> model;

    conversion::Options options;
    options.add(conversion::asn1::Asn1Options::inputFilepath, filename);

    conversion::asn1::importer::Asn1Importer importer;
    try {
        model = importer.importModel(options);
    } catch (const std::exception &ex) {
        return nullptr;
    }

    std::unique_ptr<Asn1Acn::Asn1Model> output;
    output.reset(static_cast<Asn1Acn::Asn1Model *>(model.get()));
    model.release();

    return output;
}

auto ModelLoader::loadCsvModel(const QString &filename) -> std::unique_ptr<csv::CsvModel>
{
    csv::importer::CsvImporter importer;
    csv::importer::Options options;
    options.add(csv::importer::CsvOptions::inputFilepath, filename);

    auto csvModel = importer.importModel(options);
    if (csvModel == nullptr) {
        throw "CSV file could not be read";
    }

    return csvModel;
}

} // namespace plugincommon
