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

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/asn1/Asn1Registrar/registrar.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/options.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/converter/converter.h>
#include <conversion/converter/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvRegistrar/registrar.h>
#include <conversion/registry/registry.h>
#include <conversion/simulatortrail/SimulatorTrailRegistrar/registrar.h>
#include <conversion/spintrail/SpinTrailRegistrar/registrar.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <shared/common.h>
#include <shared/sharedlibrary.h>
#include <simulatortrail/SimulatorTrailOptions/options.h>
#include <spintrail/SpinTrailOptions/options.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const QStringList args = app.arguments();

    std::optional<QString> inputIvFilepath;
    std::optional<QString> inputSpinTrailFilepath;
    std::optional<QString> outputSimulatorTrailFilepath;

    for (int i = 1; i < args.size(); ++i) {
        const QString &arg = args[1];
        if (arg == "-iv") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -iv");
                exit(EXIT_FAILURE);
            }
            if (inputIvFilepath.has_value()) {
                qCritical("Duplicated -iv argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            inputIvFilepath = args[i];
        } else if (arg == "-o") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -o");
                exit(EXIT_FAILURE);
            }
            if (outputSimulatorTrailFilepath.has_value()) {
                qCritical("Duplicated -o argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            outputSimulatorTrailFilepath = args[i];
        } else if (arg == "-h" || arg == "--help") {
            qInfo("trailgen: TASTE Model Checker trail generator");
            qInfo("Usage: trailgen [OPTIONS] <input spin trail>");
            qInfo("  -iv <filepath>         Use <filepath> as input InterfaceView");
            qInfo("  -o  <filepath>         Use <filepath> as output Simulator Trail");
            qInfo("  -h, --help             Print this message and exit.");
        } else {
            if (inputSpinTrailFilepath.has_value()) {
                qCritical("Duplicated input file argument.");
                exit(EXIT_FAILURE);
            }
            inputSpinTrailFilepath = args[i];
        }
    }

    if (!inputIvFilepath.has_value()) {
        qCritical("Missing mandatory argument: input InterfaceView");
        exit(EXIT_FAILURE);
    }
    if (!outputSimulatorTrailFilepath.has_value()) {
        qCritical("Missing mandatory argument: output SimulatorTrail filepath");
        exit(EXIT_FAILURE);
    }
    if (!inputSpinTrailFilepath.has_value()) {
        qCritical("Missing mandatory argument: input SpinTrail filepath");
        exit(EXIT_FAILURE);
    }

    const QFileInfo ivFileInfo(inputIvFilepath.value());
    const QDir baseDirectory = ivFileInfo.absoluteDir();
    const QString observerAsn1Filepath = baseDirectory.absolutePath() + QDir::separator() + "work" + QDir::separator()
            + "simulation" + QDir::separator() + "observers" + QDir::separator() + "observer.asn";

    ivm::initIVLibrary();
    shared::initSharedLibrary();

    auto dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    dynPropConfig->init(shared::interfaceCustomAttributesFilePath());

    conversion::Registry registry;

    {
        conversion::spintrail::SpinTrailRegistrar spinTrailRegistrar;
        if (!spinTrailRegistrar.registerCapabilities(registry)) {
            exit(EXIT_FAILURE);
        }
    }
    {
        conversion::simulatortrail::SimulatorTrailRegistrar simulatorTrailRegistrar;
        if (!simulatorTrailRegistrar.registerCapabilities(registry)) {
            exit(EXIT_FAILURE);
        }
    }
    {
        conversion::asn1::Asn1Registrar asn1Registrar;
        if (!asn1Registrar.registerCapabilities(registry)) {
            exit(EXIT_FAILURE);
        }
    }
    {
        conversion::iv::IvRegistrar ivRegistrar;
        if (!ivRegistrar.registerCapabilities(registry)) {
            exit(EXIT_FAILURE);
        }
    }

    try {
        std::set<conversion::ModelType> sourceModels;
        sourceModels.insert(conversion::ModelType::Asn1);
        sourceModels.insert(conversion::ModelType::InterfaceView);
        sourceModels.insert(conversion::ModelType::SpinTrail);

        conversion::Options options;
        options.add(conversion::simulatortrail::SimulatorTrailOptions::outputFilepath,
                outputSimulatorTrailFilepath.value());
        options.add(conversion::spintrail::SpinTrailOptions::inputFilepath, inputSpinTrailFilepath.value());
        options.add(conversion::asn1::Asn1Options::inputFilepath, observerAsn1Filepath);
        options.add(conversion::iv::IvOptions::inputFilepath, inputIvFilepath.value());
        options.add(conversion::iv::IvOptions::configFilepath, shared::interfaceCustomAttributesFilePath());

        conversion::Converter converter(registry, std::move(options));
        converter.convert(sourceModels, conversion::ModelType::SimulatorTrail, {});
    } catch (const conversion::importer::ImportException &ex) {
        const auto errorMessage = QString("Import failure: %1").arg(ex.errorMessage());
        qFatal("%s", errorMessage.toLatin1().constData());
        exit(EXIT_FAILURE);
    } catch (const conversion::translator::TranslationException &ex) {
        const auto errorMessage = QString("Translation failure: %1").arg(ex.errorMessage());
        qFatal("%s", errorMessage.toLatin1().constData());
        exit(EXIT_FAILURE);
    } catch (const conversion::exporter::ExportException &ex) {
        const auto errorMessage = QString("Export failure: %1").arg(ex.errorMessage());
        qFatal("%s", errorMessage.toLatin1().constData());
        exit(EXIT_FAILURE);
    } catch (const conversion::ConversionException &ex) {
        const auto errorMessage = QString("Conversion failure: %1").arg(ex.errorMessage());
        qFatal("%s", errorMessage.toLatin1().constData());
        exit(EXIT_FAILURE);
    }

    qInfo() << "Generation finished.";

    exit(EXIT_SUCCESS);
}
