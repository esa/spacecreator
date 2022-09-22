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

#include <QCoreApplication>
#include <conversion/asn1/Asn1Importer/importer.h>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvXmlImporter/importer.h>
#include <conversion/msc/MscImporter/importer.h>
#include <conversion/msc/MscOptions/options.h>
#include <conversion/sdl/MscToSdlTranslator/translator.h>
#include <cstdlib>
#include <iostream>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <msccore/msclibrary.h>
#include <sdl/SdlExporter/exporter.h>
#include <sdl/SdlOptions/options.h>
#include <shared/common.h>
#include <shared/sharedlibrary.h>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    if (argc != 2) {
        std::cerr << "Usage: ./msctosdlconverter <file>\n";
        return EXIT_FAILURE;
    }

    ivm::initIVLibrary();
    msc::initMscLibrary();
    shared::initSharedLibrary();

    auto dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    dynPropConfig->init(shared::interfaceCustomAttributesFilePath());

    conversion::Options options;
    options.add(conversion::msc::MscOptions::inputFilepath, QString(argv[1]));
    options.add(conversion::asn1::Asn1Options::inputFilepath, "observer.asn");
    options.add(conversion::iv::IvOptions::inputFilepath, "interfaceview.xml");
    options.add(conversion::iv::IvOptions::configFilepath, shared::interfaceCustomAttributesFilePath());

    conversion::msc::importer::MscImporter mscImporter;
    auto mscModel = mscImporter.importModel(options);

    conversion::asn1::importer::Asn1Importer asn1Importer;
    auto asn1Model = asn1Importer.importModel(options);

    conversion::iv::importer::IvXmlImporter ivImporter;
    auto ivModel = ivImporter.importModel(options);

    conversion::sdl::translator::MscToSdlTranslator translator;
    auto outputModels = translator.translateModels({ mscModel.get(), asn1Model.get(), ivModel.get() }, options);
    auto sdlModel = dynamic_cast<sdl::SdlModel *>(outputModels[0].get());

    sdl::exporter::SdlExporter exporter;
    exporter.exportModel(sdlModel, options);

    std::cout << "Conversion finished\n";
}
