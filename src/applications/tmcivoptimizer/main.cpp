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

#include "scversion.h"

#include <QBuffer>
#include <QCoreApplication>
#include <QSaveFile>
#include <QString>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <ivcore/ivxmlreader.h>
#include <ivcore/ivxmlwriter.h>
#include <optional>
#include <shared/sharedlibrary.h>
#include <templating/templatinglibrary.h>
#include <tmc/TmcInterfaceViewOptimizer/interfaceviewoptimizer.h>

using tmc::InterfaceViewOptimizer;

std::unique_ptr<ivm::IVModel> importIvModel(const QString &filepath)
{
    ivm::IVXMLReader reader;

    const bool ok = reader.readFile(filepath);
    if (!ok) {
        return nullptr;
    }

    auto dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    dynPropConfig->init(shared::interfaceCustomAttributesFilePath());

    auto model = std::make_unique<ivm::IVModel>(dynPropConfig);
    model->initFromObjects(reader.parsedObjects(), reader.externalAttributes());
    return model;
}

void saveOptimizedInterfaceView(const ivm::IVModel *ivModel, const QString &outputFilePath)
{
    ivm::IVXMLWriter exporter;
    exporter.exportObjectsSilently(ivModel->ivobjects().values(), outputFilePath);
}

int main(int argc, char *argv[])
{
    shared::initSharedLibrary();
    ivm::initIVLibrary();
    templating::initTemplatingLibrary();

    QCoreApplication app(argc, argv);

    app.setOrganizationName(SC_ORGANISATION);
    app.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    app.setApplicationVersion(spaceCreatorVersion);
    app.setApplicationName(QObject::tr("TASTE Model Checker InterfaceView optimizer"));

    std::optional<QString> inputIvFilepath;
    std::optional<QString> outputIvFilepath;
    std::vector<QString> environmentFunctions;
    std::vector<QString> keepFunctions;

    const QStringList args = app.arguments();

    for (int i = 1; i < args.size(); ++i) {
        const QString &arg = args[i];
        if (arg == "-i") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -i");
                exit(EXIT_FAILURE);
            }
            if (inputIvFilepath.has_value()) {
                qCritical("Duplicated -i argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            inputIvFilepath = args[i];
        } else if (arg == "-o") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -o");
                exit(EXIT_FAILURE);
            }
            if (outputIvFilepath.has_value()) {
                qCritical("Duplicated -o argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            outputIvFilepath = args[i];
        } else if (arg == "-e" || arg == "--envfunc") {
            ++i;
            environmentFunctions.emplace_back(args[i]);
        } else if (arg == "-k" || arg == "--keep") {
            ++i;
            keepFunctions.emplace_back(args[i]);
        } else if (arg == "-h" || arg == "--help") {
            qInfo("TASTE Model Chcecker InterfaceView optimizer");
            qInfo("Usage: tmcivoptimizer [OPTIONS]");
            qInfo("  -i <filepath>          Use <filepath> as input InterfaceView");
            qInfo("  -o <filepath>          Use <filepath> as output InterfaceView.");
            qInfo("  -e, --envfunc <name>   Use <name> to specify a SDL function that  should be treated as "
                  "environment");
            qInfo("  -k, --keepfunc <name>  Use <name> to specify a SDL function that shouldn't be treated as "
                  "environment.");
            qInfo("                         All other functions will be treated as environment");
            qInfo("  -h, --help             Print this message and exit.");
            exit(EXIT_SUCCESS);
        } else {
            qCritical("Unrecognized command line option");
            exit(EXIT_FAILURE);
        }
    }

    if (!inputIvFilepath.has_value()) {
        qCritical("Missing mandatory argument: input InterfaceView");
        exit(EXIT_FAILURE);
    }
    if (!outputIvFilepath.has_value()) {
        qCritical("Missing mandatory argument: output InterfaceVie");
        exit(EXIT_FAILURE);
    }
    if (!environmentFunctions.empty() && !keepFunctions.empty()) {
        qCritical("-e and -k arguments cannot be combined");
        exit(EXIT_FAILURE);
    }

    auto ivModel = importIvModel(*inputIvFilepath);

    if (!environmentFunctions.empty()) {
        InterfaceViewOptimizer::optimizeModel(
                ivModel.get(), environmentFunctions, InterfaceViewOptimizer::Mode::Environment);
    } else if (!keepFunctions.empty()) {
        InterfaceViewOptimizer::optimizeModel(ivModel.get(), keepFunctions, InterfaceViewOptimizer::Mode::Keep);
    } else {
        InterfaceViewOptimizer::optimizeModel(ivModel.get(), {}, InterfaceViewOptimizer::Mode::None);
    }

    saveOptimizedInterfaceView(ivModel.get(), *outputIvFilepath);

    return EXIT_SUCCESS;
}
