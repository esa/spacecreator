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

#include "sedsconverter.h"

#include <QCoreApplication>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/seds/SedsOptions/options.h>

using conversion::ConversionException;
using conversion::ModelType;
using conversion::Options;
using conversion::asn1::Asn1Options;
using conversion::exporter::ExportException;
using conversion::importer::ImportException;
using conversion::iv::IvOptions;
using conversion::seds::SedsOptions;
using conversion::translator::TranslationException;

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    if (argc < 2) {
        qFatal("No input seds filename\n");
    }

    sedsconverter::SedsConverter sedsConverter;

    Options options;
    options.add(Asn1Options::inputFilename, "optional.asn");
    options.add(Asn1Options::importAsn1File);
    options.add(IvOptions::configFilename, "config.xml");
    options.add(IvOptions::outputFilename, "output.xml");
    options.add(SedsOptions::inputFilename, argv[1]);
    options.add(SedsOptions::schemaFilename, "seds.xsd");
    options.add(SedsOptions::externalRefFilename, "config.toml");
    options.add(SedsOptions::skipValidation);
    options.add(SedsOptions::keepIntermediateFiles);
    options.add(IvOptions::configFilename, "config.xml");
    options.add(IvOptions::outputFilename, QString("%1.iv").arg(argv[1]));

    try {
        sedsConverter.convert({ ModelType::Seds }, ModelType::Asn1, {}, std::move(options));
    } catch (const ImportException &ex) {
        const auto errorMessage = QString("Import failure: %1").arg(ex.errorMessage());
        qFatal("%s", errorMessage.toLatin1().constData());
    } catch (const TranslationException &ex) {
        const auto errorMessage = QString("Translation failure: %1").arg(ex.errorMessage());
        qFatal("%s", errorMessage.toLatin1().constData());
    } catch (const ExportException &ex) {
        const auto errorMessage = QString("Export failure: %1").arg(ex.errorMessage());
        qFatal("%s", errorMessage.toLatin1().constData());
    } catch (const ConversionException &ex) {
        const auto errorMessage = QString("Conversion failure: %1").arg(ex.errorMessage());
        qFatal("%s", errorMessage.toLatin1().constData());
    }
}
