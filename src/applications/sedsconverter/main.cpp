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

#include "scversion.h"
#include "sedsconverter.h"
#include "sedsconvertercli.h"

#include <QCoreApplication>
#include <QVector>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <seds/SedsOptions/options.h>

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
    Options options;
    sedsconverter::SedsConverter sedsConverter;
    sedsconverter::SedsConverterCLI cli;

    a.setOrganizationName(SC_ORGANISATION);
    a.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    a.setApplicationVersion(spaceCreatorVersion);
    a.setApplicationName(QObject::tr("SedsConverter"));

    const auto usage = QString("Usage: %1 --in <file> <options> [--out <file>]").arg(argv[0]);
    try {
        cli.parseArguments(a.arguments());
        cli.processOptions(options);
        sedsConverter.convert(
                cli.getSourceModelTypes(), cli.getTargetModelType(), cli.getAuxModelTypes(), std::move(options));
    } catch (const ImportException &ex) {
        const auto errorMessage = QString("Import failure: %1\n%2").arg(ex.errorMessage(), usage);
        qCritical("%s", errorMessage.toLatin1().constData());
        return EXIT_FAILURE;
    } catch (const TranslationException &ex) {
        const auto errorMessage = QString("Translation failure: %1\n%2").arg(ex.errorMessage(), usage);
        qCritical("%s", errorMessage.toLatin1().constData());
        return EXIT_FAILURE;
    } catch (const ExportException &ex) {
        const auto errorMessage = QString("Export failure: %1\n%2").arg(ex.errorMessage(), usage);
        qCritical("%s", errorMessage.toLatin1().constData());
        return EXIT_FAILURE;
    } catch (const ConversionException &ex) {
        const auto errorMessage = QString("Conversion failure: %1\n%2").arg(ex.errorMessage(), usage);
        qCritical("%s", errorMessage.toLatin1().constData());
        return EXIT_FAILURE;
    } catch (const std::exception &ex) {
        const auto errorMessage = QString("Runtime exception: %1\n%2").arg(ex.what(), usage);
        qCritical("%s", errorMessage.toLatin1().constData());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
