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

#include "executor.h"

#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <iostream>
#include <shared/qstringhash.h>
#include <tmc/TmcConverter/converter.h>
#include <unordered_map>

using tmc::converter::TmcConverter;
using tmc::verifier::TmcVerifier;

namespace {
const auto separator = QString(":");

QString extractObserverPath(const QString &info)
{
    const auto elements = info.split(separator, Qt::KeepEmptyParts);
    if (elements.size() == 0) {
        qCritical("Malformed observer info: missing path");
    }
    return elements[0];
}

uint32_t extractObserverPriority(const QString &info)
{
    const auto elements = info.split(separator, Qt::KeepEmptyParts);
    bool ok = true;
    const auto priority = elements.size() > 1 ? elements[1].toUInt(&ok) : 1;
    if (!ok) {
        qCritical("Malformed observer info: priority could not be parsed as an integer");
    }
    return priority;
}
}

namespace tmc {
TmcExecutor::TmcExecutor(QObject *parent)
    : QObject(parent)
    , m_verifier(nullptr)
{
}

void TmcExecutor::execute()
{
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
    std::optional<QString> delta;
    bool isRealTypeEnabled = false;
    bool isMulticastEnabled = false;
    bool verify = false;
    std::optional<int> sdl2promelaTimeout;
    std::optional<QString> dataviewOutputPath;
    QList<QString> inputAsn1Files;

    const QStringList args = QCoreApplication::arguments();

    for (int i = 1; i < args.size(); ++i) {
        const QString &arg = args[i];
        if (arg == "-iv") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -iv");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
            }
            if (inputIvFilepath.has_value()) {
                qCritical("Duplicated -iv argument");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
            }
            ++i;
            inputIvFilepath = args[i];
        } else if (arg == "-o") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -o");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
            }
            if (outputDirectory.has_value()) {
                qCritical("Duplicated -o argument");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
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
        } else if (arg == "-verify") {
            verify = true;
        } else if (arg == "-ivl") {
            ++i;

            auto limit = args[i];
            auto limitSemicolonCount = limit.count(':');

            if (limitSemicolonCount == 0) {
                if (globalInputVectorLengthLimit.has_value()) {
                    qCritical("Duplicated global input vector length limit argument");
                    QCoreApplication::exit(EXIT_FAILURE);
                    return;
                }

                bool valueOk;
                limit.toInt(&valueOk);

                if (!valueOk) {
                    qCritical("Global input vector length limit argument value is not an integer");
                    QCoreApplication::exit(EXIT_FAILURE);
                    return;
                }

                globalInputVectorLengthLimit = limit;
            } else if (limitSemicolonCount == 1) {
                auto interfaceLimit = limit.split(':');

                if (interfaceInputVectorLengthLimits.count(interfaceLimit[0]) != 0) {
                    qCritical("Duplicated input vector length limit argument for interface %s",
                            interfaceLimit[0].toStdString().c_str());
                    QCoreApplication::exit(EXIT_FAILURE);
                    return;
                }

                bool valueOk;
                interfaceLimit[1].toInt(&valueOk);

                if (!valueOk) {
                    qCritical("Input vector length limit argument for interface %s is not an integer",
                            interfaceLimit[0].toStdString().c_str());
                    QCoreApplication::exit(EXIT_FAILURE);
                    return;
                }

                interfaceInputVectorLengthLimits.insert({ interfaceLimit[0], interfaceLimit[1] });
            } else {
                qCritical("Incorrect input vector length limit argument value");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
            }
        } else if (arg == "-pbp") {
            if (i + 1 == args.size()) {
                qCritical("Missing value after -pbp");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
            }
            if (processesBasePriority.has_value()) {
                qCritical("Duplicated -pbp argument");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
            }
            ++i;

            bool valueOk;
            args[i].toUInt(&valueOk);

            if (!valueOk) {
                qCritical("Processes base priority argument value is not an unsigned integer");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
            }

            processesBasePriority = args[i];
        } else if (arg == "-sub") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -sub");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
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

            delta = args[i];
        } else if (arg == "-enable-reals") {
            isRealTypeEnabled = true;
        } else if (arg == "-multicast") {
            isMulticastEnabled = true;
        } else if (arg == "-sdl2promela-timeout") {
            if (i + 1 == args.size()) {
                qCritical("Missing value after -sdl2promela-timeout");
                exit(EXIT_FAILURE);
            }
            if (sdl2promelaTimeout.has_value()) {
                qCritical("Duplicated -sdl2promela-timeout argument");
                exit(EXIT_FAILURE);
            }
            ++i;

            bool valueOk;
            uint value = args[i].toUInt(&valueOk);

            if (!valueOk) {
                qCritical("sdl2promela timeout is not an integer");
                exit(EXIT_FAILURE);
            }

            sdl2promelaTimeout = value;
        } else if (arg == "-dataview") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -dataview");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
            }
            if (dataviewOutputPath.has_value()) {
                qCritical("Duplicated -dataview argument");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
            }
            ++i;
            dataviewOutputPath = args[i];
        } else if (arg == "-asn") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -asn");
                QCoreApplication::exit(EXIT_FAILURE);
                return;
            }
            ++i;
            inputAsn1Files.push_back(args[i]);
        } else if (arg == "-h" || arg == "--help") {
            qInfo("tmc: TASTE Model Checker");
            qInfo("Usage: tmc [OPTIONS]");
            qInfo("  -iv <filepath>         Use <filepath> as input InterfaceView");
            qInfo("  -o <dir>               Use <dir> as output directory.");
            qInfo("  -scl <filepath>        Use <filepath> as input Stop Condition source file.");
            qInfo("  -os <filepath>[:priority]");
            qInfo("                         Use <filepath> as an Observer source file.");
            qInfo("                         Integer <priority> of the Observer is optional");
            qInfo("  -mos <filepath>        Use <filepath> as an MSC for verification.");
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
            qInfo("  -multicast             Enable support multicast connections in InterfaceView");
            qInfo("  -verify                Run whole verification process");
            qInfo("  -dataview <name>       Translate only ASN.1 files, specified by -asn and save result to <name>");
            qInfo("  -asn <name>            Use <name> as input file to translate ASN.1 to promela");
            qInfo("  -h, --help             Print this message and exit.");
            QCoreApplication::exit(EXIT_SUCCESS);
            return;
        } else {
            qCritical("Unrecognized command line option");
            QCoreApplication::exit(EXIT_FAILURE);
            return;
        }
    }

    if (!inputIvFilepath.has_value() && !dataviewOutputPath.has_value()) {
        qCritical("Missing mandatory argument: input InterfaceView or output Promela dataview");
        QCoreApplication::exit(EXIT_FAILURE);
        return;
    }
    if (inputIvFilepath.has_value() && dataviewOutputPath.has_value()) {
        qCritical("The arguments input InterfaceView and output Promela dataview cannot be used simultaneously");
        QCoreApplication::exit(EXIT_FAILURE);
        return;
    }

    if (inputIvFilepath.has_value()) {
        if (!outputDirectory.has_value()) {
            qCritical("Missing mandatory argument: output directory");
            QCoreApplication::exit(EXIT_FAILURE);
            return;
        }
        if (!environmentFunctions.empty() && !keepFunctions.empty()) {
            qCritical("-e and -k arguments cannot be combined");
            QCoreApplication::exit(EXIT_FAILURE);
            return;
        }

        m_verifier = std::make_unique<TmcVerifier>(inputIvFilepath.value(), outputDirectory.value());

        connect(m_verifier.get(), SIGNAL(finished(bool)), this, SLOT(finished(bool)));
        connect(m_verifier.get(), SIGNAL(verifierMessage(QString)), this, SLOT(verifierMessage(QString)));

        m_verifier->setMscObserverFiles(mscObserverFiles);
        m_verifier->setEnvironmentFunctions(environmentFunctions);
        m_verifier->setKeepFunctions(keepFunctions);
        m_verifier->setGlobalInputVectorLengthLimit(std::move(globalInputVectorLengthLimit));
        m_verifier->setInterfaceInputVectorLengthLimits(std::move(interfaceInputVectorLengthLimits));
        m_verifier->setProcessesBasePriority(std::move(processesBasePriority));
        m_verifier->setSubtypesFilepaths(subtypesFilepaths);
        m_verifier->setDelta(delta);
        m_verifier->setRealTypeEnabled(isRealTypeEnabled);
        m_verifier->setMulticastEnabled(isMulticastEnabled);

        if (!m_verifier->addStopConditionFiles(stopConditionFiles)) {
            qCritical() << "Cannot attach stop condition file";
            QCoreApplication::exit(EXIT_FAILURE);
            return;
        }
        for (const auto &info : observerInfos) {
            if (!m_verifier->attachObserver(extractObserverPath(info), extractObserverPriority(info))) {
                qCritical() << "Cannot attach observer " << info;
                QCoreApplication::exit(EXIT_FAILURE);
                return;
            }
        }
        if (sdl2promelaTimeout.has_value()) {
            m_verifier->setSdl2PromelaTimeout(sdl2promelaTimeout.value());
        }

        if (m_verifier->execute(
                    verify ? TmcVerifier::ExecuteMode::ConvertAndVerify : TmcVerifier::ExecuteMode::ConvertOnly)) {
            return;
        } else {
            qCritical() << "Cannot start verifier";
            QCoreApplication::exit(EXIT_FAILURE);
            return;
        }
    } else {
        if (inputAsn1Files.empty()) {
            qCritical("Missing input ASN.1 files");
            QCoreApplication::exit(EXIT_FAILURE);
            return;
        }

        auto converter = std::make_unique<TmcConverter>("", "");

        connect(converter.get(), SIGNAL(message(QString)), this, SLOT(verifierMessage(QString)));
        if (converter->convertDataview(inputAsn1Files, dataviewOutputPath.value())) {
            QCoreApplication::exit(EXIT_FAILURE);
        }
        QCoreApplication::exit(EXIT_SUCCESS);
    }
}

void TmcExecutor::verifierMessage(QString text)
{
    std::cout << text.toUtf8().constData();
}

void TmcExecutor::finished(bool success)
{
    if (success) {
        QCoreApplication::exit(EXIT_SUCCESS);
    } else {
        QCoreApplication::exit(EXIT_FAILURE);
    }
}
}
