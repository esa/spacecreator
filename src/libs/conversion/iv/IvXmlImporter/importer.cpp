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

#include "importer.h"

#include <QFileInfo>
#include <QString>
#include <conversion/common/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/model.h>
#include <conversion/common/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <ivcore/ivxmlreader.h>

using conversion::Options;
using conversion::importer::ImportException;
using conversion::iv::IvOptions;
using ivm::IVModel;
using ivm::IVPropertyTemplateConfig;
using ivm::IVXMLReader;

namespace conversion::iv::importer {

std::unique_ptr<conversion::Model> IvXmlImporter::importModel(const Options &options) const
{
    auto *config = initConfig(options);

    return parse(options, config);
}

IVPropertyTemplateConfig *IvXmlImporter::initConfig(const Options &options) const
{
    const auto configFilepath = options.value(IvOptions::configFilepath);
    if (!configFilepath) {
        throw ImportException("InterfaceView configuration file wasn't specified");
    }

    QFileInfo configFile(*configFilepath);
    if (!configFile.exists()) {
        auto errorMsg = QString("InterfaceView configuration file '%1' doesn't exist").arg(*configFilepath);
        throw ImportException(std::move(errorMsg));
    }

    auto *config = IVPropertyTemplateConfig::instance();
    config->init(*configFilepath);

    return config;
}

std::unique_ptr<conversion::Model> IvXmlImporter::parse(const Options &options, IVPropertyTemplateConfig *config) const
{
    const auto inputFilepath = options.value(IvOptions::inputFilepath);
    if (!inputFilepath) {
        throw ImportException("Interface View file to import wasn't specified");
    }

    IVXMLReader parser;

    if (!parser.readFile(*inputFilepath)) {
        throw ImportException(parser.errorString());
    }

    auto model = std::make_unique<IVModel>(config);
    model->initFromObjects(parser.parsedObjects(), parser.externalAttributes());

    return model;
}

} // namespace conversion::iv::importer
