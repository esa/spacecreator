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
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/converter/exceptions.h>
#include <ivcore/ivlibrary.h>
#include <msccore/msclibrary.h>
#include <shared/sharedlibrary.h>
#include <tmc/TmcConverter/converter.h>

using tmc::converter::TmcConverter;

const auto separator = QString(":");

static QString extractObserverPath(const QString &info)
{
#if QTC_VERSION == 408
    const auto elements = info.split(separator, QString::KeepEmptyParts);
#endif
#if QTC_VERSION == 800
    const auto elements = info.split(separator, Qt::KeepEmptyParts);
#endif



    if (elements.size() == 0) {
        qCritical("Malformed observer info: missing path");
    }
    return elements[0];
}

static uint32_t extractObserverPriority(const QString &info)
{
#if QTC_VERSION == 408
    const auto elements = info.split(separator, QString::KeepEmptyParts);
#endif
#if QTC_VERSION == 800
    const auto elements = info.split(separator, Qt::KeepEmptyParts);
#endif
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

    ivm::initIVLibrary();
    shared::initSharedLibrary();
    msc::initMscLibrary();

    QCoreApplication app(argc, argv);

    app.setOrganizationName(SC_ORGANISATION);
    app.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    app.setApplicationVersion(spaceCreatorVersion);
    app.setApplicationName(QObject::tr("TASTE Model Checker trace generator"));

    const QStringList args = app.arguments();

    std::optional<QString> inputIvFilepath;
    std::optional<QString> outputDirectory;
    std::optional<QString> inputSpinTrailFilepath;
    std::optional<QString> outputSimulatorTrailFilepath;
    QStringList observerInfos;
    QStringList mscObserverFiles;
    std::vector<QString> environmentFunctions;
    std::vector<QString> keepFunctions;

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
        } else if (arg == "-ot") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -ot");
                exit(EXIT_FAILURE);
            }
            if (outputSimulatorTrailFilepath.has_value()) {
                qCritical("Duplicated -ot argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            outputSimulatorTrailFilepath = args[i];
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
        } else if (arg == "-h" || arg == "--help") {
            qInfo("trailgen: TASTE Model Checker trail generator");
            qInfo("Usage: trailgen [OPTIONS] <input spin trail>");
            qInfo("  -iv <filepath>         Use <filepath> as input InterfaceView");
            qInfo("  -o  <dir>              Use <dir> as output directory.");
            qInfo("  -ot  <filepath>         Use <filepath> as output Simulator Trail");
            qInfo("  -os <filepath>[:priority]");
            qInfo("                         Use <filepath> as an Observer source file.");
            qInfo("                         Integer <priority> of the Observer is optional");
            qInfo("  -mos <filepath>        Use <filepath> as an MSC validation.");
            qInfo("  -e, --envfunc <name>   Use <name> to specify a SDL function that should be treated as "
                  "environment");
            qInfo("  -k, --keepfunc <name>  Use <name> to specify a SDL function that shouldn't be treated as "
                  "environment.");
            qInfo("                         All other functions will be treated as environment");
            qInfo("  -h, --help             Print this message and exit.");
        } else {
            if (inputSpinTrailFilepath.has_value()) {
                qCritical("Duplicated input file argument.");
                qDebug() << inputSpinTrailFilepath.value();
                exit(EXIT_FAILURE);
            }
            inputSpinTrailFilepath = args[i];
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

    try {
        std::unique_ptr<TmcConverter> converter =
                std::make_unique<TmcConverter>(inputIvFilepath.value(), outputDirectory.value());
        converter->setEnvironmentFunctions(environmentFunctions);
        converter->setKeepFunctions(keepFunctions);
        converter->setMscObserverFiles(mscObserverFiles);
        for (const auto &info : observerInfos) {
            if (!converter->attachObserver(extractObserverPath(info), extractObserverPriority(info))) {
                exit(EXIT_FAILURE);
            }
        }
        converter->convertTrace(inputSpinTrailFilepath.value(), outputSimulatorTrailFilepath.value());
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
