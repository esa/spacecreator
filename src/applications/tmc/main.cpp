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
#include <msccore/msclibrary.h>
#include <shared/qstringhash.h>
#include <shared/sharedlibrary.h>
#include <tmc/TmcVerifier/verifier.h>
#include <unordered_map>

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
    msc::initMscLibrary();

    QCoreApplication app(argc, argv);

    app.setOrganizationName(SC_ORGANISATION);
    app.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    app.setApplicationVersion(spaceCreatorVersion);
    app.setApplicationName(QObject::tr("TASTE Model Checker"));

    std::optional<QString> inputIvFilepath;
    std::optional<QString> outputDirectory;
    QStringList stopConditionFiles;
    QStringList observerInfos;
    QStringList mscObserverFiles;
    std::vector<QString> environmentFunctions;
    std::vector<QString> keepFunctions;
    std::optional<QString> globalInputVectorLengthLimit;
    std::unordered_map<QString, QString> interfaceInputVectorLengthLimits;
    std::optional<QString> processesBasePriority;
    std::vector<QString> subtypesFilepaths;
    std::optional<float> delta;
    bool isRealTypeEnabled = false;

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
        } else if (arg == "-mos") {
            ++i;
            mscObserverFiles.append(args[i]);
        } else if (arg == "-e" || arg == "--envfunc") {
            ++i;
            environmentFunctions.emplace_back(args[i]);
        } else if (arg == "-k" || arg == "--keep") {
            ++i;
            keepFunctions.emplace_back(args[i]);
        } else if (arg == "-ivl") {
            ++i;

            auto limit = args[i];
            auto limitSemicolonCount = limit.count(':');

            if (limitSemicolonCount == 0) {
                if (globalInputVectorLengthLimit.has_value()) {
                    qCritical("Duplicated global input vector length limit argument");
                    exit(EXIT_FAILURE);
                }

                bool valueOk;
                limit.toInt(&valueOk);

                if (!valueOk) {
                    qCritical("Global input vector length limit argument value is not an integer");
                    exit(EXIT_FAILURE);
                }

                globalInputVectorLengthLimit = limit;
            } else if (limitSemicolonCount == 1) {
                auto interfaceLimit = limit.split(':');

                if (interfaceInputVectorLengthLimits.count(interfaceLimit[0]) != 0) {
                    qCritical("Duplicated input vector length limit argument for interface %s",
                            interfaceLimit[0].toStdString().c_str());
                    exit(EXIT_FAILURE);
                }

                bool valueOk;
                interfaceLimit[1].toInt(&valueOk);

                if (!valueOk) {
                    qCritical("Input vector length limit argument for interface %s is not an integer",
                            interfaceLimit[0].toStdString().c_str());
                    exit(EXIT_FAILURE);
                }

                interfaceInputVectorLengthLimits.insert({ interfaceLimit[0], interfaceLimit[1] });
            } else {
                qCritical("Incorrect input vector length limit argument value");
                exit(EXIT_FAILURE);
            }
        } else if (arg == "-pbp") {
            if (i + 1 == args.size()) {
                qCritical("Missing value after -pbp");
                exit(EXIT_FAILURE);
            }
            if (processesBasePriority.has_value()) {
                qCritical("Duplicated -pbp argument");
                exit(EXIT_FAILURE);
            }
            ++i;

            bool valueOk;
            args[i].toUInt(&valueOk);

            if (!valueOk) {
                qCritical("Processes base priority argument value is not an unsigned integer");
                exit(EXIT_FAILURE);
            }

            processesBasePriority = args[i];
        } else if (arg == "-sub") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -sub");
                exit(EXIT_FAILURE);
            }
            ++i;
            subtypesFilepaths.emplace_back(args[i]);
        } else if (arg == "-delta") {
            if (i + 1 == args.size()) {
                qCritical("Missing value after -delta");
                exit(EXIT_FAILURE);
            }
            if (delta.has_value()) {
                qCritical("Duplicated -delta argument");
                exit(EXIT_FAILURE);
            }
            ++i;

            bool valueOk;
            args[i].toFloat(&valueOk);

            if (!valueOk) {
                qCritical("Delta is not a float");
                exit(EXIT_FAILURE);
            }

            delta = std::abs(args[i].toFloat());
        } else if (arg == "-enable-reals") {
            ++i;
            isRealTypeEnabled = true;
        } else if (arg == "-h" || arg == "--help") {
            qInfo("tmc: TASTE Model Checker");
            qInfo("Usage: tmc [OPTIONS]");
            qInfo("  -iv <filepath>         Use <filepath> as input InterfaceView");
            qInfo("  -o <dir>               Use <dir> as output directory.");
            qInfo("  -scl <filepath>        Use <filepath> as input Stop Condition source file.");
            qInfo("  -os <filepath>[:priority]");
            qInfo("                         Use <filepath> as an Observer source file.");
            qInfo("                         Integer <priority> of the Observer is optional");
            qInfo("  -e, --envfunc <name>   Use <name> to specify a SDL function that should be treated as "
                  "environment");
            qInfo("  -k, --keepfunc <name>  Use <name> to specify a SDL function that shouldn't be treated as "
                  "environment.");
            qInfo("                         All other functions will be treated as environment");
            qInfo("  -ivl <limit>           Use <limit> to specify input vector length limit.");
            qInfo("                         Provide number only to set the global limit");
            qInfo("                         Use <interface_name>:<number> to set limit for a single interface");
            qInfo("                         Interface limit overrides the global limit");
            qInfo("  -pbp <value>           Use <value> to specify base priority for all non-environment processess");
            qInfo("                         Base priority will be added to interfaces priorities specified in ");
            qInfo("                         the Interface View");
            qInfo("  -sub <filepath>        Use <filepath> as an ASN.1 file used for subtyping");
            qInfo("  -enable-reals          Enable support for real type");
            qInfo("  -delta <delta value>   Set delta for reals generation");
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
    if (!outputDirectory.has_value()) {
        qCritical("Missing mandatory argument: output directory");
        exit(EXIT_FAILURE);
    }
    if (!environmentFunctions.empty() && !keepFunctions.empty()) {
        qCritical("-e and -k arguments cannot be combined");
        exit(EXIT_FAILURE);
    }

    tmc::verifier::TmcVerifier verifier(inputIvFilepath.value(), outputDirectory.value());

    verifier.setMscObserverFiles(mscObserverFiles);
    verifier.setEnvironmentFunctions(environmentFunctions);
    verifier.setKeepFunctions(keepFunctions);
    verifier.setGlobalInputVectorLengthLimit(std::move(globalInputVectorLengthLimit));
    verifier.setInterfaceInputVectorLengthLimits(std::move(interfaceInputVectorLengthLimits));
    verifier.setProcessesBasePriority(std::move(processesBasePriority));
    verifier.setSubtypesFilepaths(subtypesFilepaths);
    verifier.setDelta(delta);
    verifier.setRealTypeEnabled(isRealTypeEnabled);

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
