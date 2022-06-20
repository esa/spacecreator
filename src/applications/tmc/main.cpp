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

#include <QCoreApplication>
#include <QString>
#include <ivcore/ivlibrary.h>
#include <shared/sharedlibrary.h>
#include <tmc/TmcVerifier/verifier.h>

const auto separator = QString(":");

static QString extractObserverPath(const QString &info)
{
    const auto elements = info.split(separator, QString::KeepEmptyParts);
    if (elements.size() == 0) {
        qCritical("Malformed observer info: missing path");
    }
    return elements[0];
}

static uint32_t extractObserverPriority(const QString &info)
{
    const auto elements = info.split(separator, QString::KeepEmptyParts);
    bool ok = true;
    const auto priority = elements.size() > 1 ? elements[1].toUInt(&ok) : 1;
    if (!ok) {
        qCritical("Malformed observer info: priority could not be parsed as an integer");
    }
    return priority;
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(asn1_resources);

    shared::initSharedLibrary();
    ivm::initIVLibrary();

    QCoreApplication app(argc, argv);

    app.setOrganizationName(SC_ORGANISATION);
    app.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    app.setApplicationVersion(spaceCreatorVersion);
    app.setApplicationName(QObject::tr("TASTE Model Checker"));

    std::optional<QString> inputIvFilepath;
    std::optional<QString> outputDirectory;
    QStringList stopConditionFiles;
    QStringList observerInfos;
    std::vector<QString> environmentFunctions;
    std::optional<QString> globalInputVectorLengthLimit;

    const QStringList args = app.arguments();

    for (int i = 1; i < args.size(); ++i) {
        const QString &arg = args[i];
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
            if (outputDirectory.has_value()) {
                qCritical("Duplicated -o argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            outputDirectory = args[i];
        } else if (arg == "-scl") {
            ++i;
            stopConditionFiles.append(args[i]);
        } else if (arg == "-os") {
            ++i;
            observerInfos.append(args[i]);
        } else if (arg == "-e" || arg == "--envfunc") {
            ++i;
            environmentFunctions.emplace_back(args[i]);
        } else if (arg == "-ivl") {
            ++i;
            globalInputVectorLengthLimit = args[i];
        } else if (arg == "-h" || arg == "--help") {
            qInfo("tmc: TASTE Model Chcecker");
            qInfo("Usage: tmc [OPTIONS]");
            qInfo("  -iv <filepath>       Use <filepath> as input InterfaceView");
            qInfo("  -o <dir>             Use <dir> as output directory.");
            qInfo("  -scl <filepath>      Use <filepath> as input Stop Condition source file.");
            qInfo("  -os <filepath>[:priority]");
            qInfo("                       Use <filepath> as an Observer source file.");
            qInfo("                       Integer <priority> of the Observer is optional");
            qInfo("  -e, --envfunc <name> Use <name> to specify a SDL function that  should be treated as environment");
            qInfo("  -h, --help           Print this message and exit.");
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
    if (!outputDirectory.has_value()) {
        qCritical("Missing mandatory argument: output directory");
        exit(EXIT_FAILURE);
    }

    tmc::verifier::TmcVerifier verifier(inputIvFilepath.value(), outputDirectory.value());

    verifier.addEnvironmentFunctions(environmentFunctions);
    verifier.setGlobalInputVectorLengthLimit(globalInputVectorLengthLimit);

    if (!verifier.addStopConditionFiles(stopConditionFiles)) {
        return EXIT_FAILURE;
    }
    for (const auto &info : observerInfos) {

        if (!verifier.attachObserver(extractObserverPath(info), extractObserverPriority(info))) {
            return EXIT_FAILURE;
        }
    }

    if (verifier.execute()) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
