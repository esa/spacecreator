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

#include "verifier.h"

#include <QProcess>
#include <QtDebug>
#include <QtGlobal>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivxmlreader.h>
#include <promela/PromelaOptions/options.h>

using conversion::ConversionException;
using conversion::ModelType;
using conversion::Options;
using conversion::asn1::Asn1Options;
using conversion::exporter::ExportException;
using conversion::importer::ImportException;
using conversion::iv::IvOptions;
using conversion::promela::PromelaOptions;
using conversion::translator::TranslationException;
using ivm::IVFunction;
using ivm::IVModel;
using tmc::converter::TmcConverter;

namespace tmc::verifier {
TmcVerifier::TmcVerifier(const QString &inputIvFilepath, const QString &outputDirectory)
    : m_inputIvFilepath(inputIvFilepath)
    , m_outputDirectory(outputDirectory)
{
    m_converter = std::make_unique<TmcConverter>();
    m_dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    m_dynPropConfig->init(QLatin1String("default_attributes.xml"));

    m_sdl2PromelaCommand = "sdl2promela";
    m_externalCommandTimeout = 10000;
}

bool TmcVerifier::addStopConditionFiles(const QStringList &files)
{
    for (const QString &filepath : files) {
        QFileInfo fileinfo(filepath);
        if (!fileinfo.exists()) {
            return false;
        }
        m_stopConditionsFiles.append(filepath);
    }
    return true;
}

bool TmcVerifier::execute()
{
    if (!convertSystem()) {
        return false;
    }
    if (!convertStopConditions()) {
        return false;
    }
    if (!buildVerifier()) {
        return false;
    }
    if (!executeVerifier()) {
        return false;
    }
    return true;
}

bool TmcVerifier::convertSystem()
{
    const QFileInfo ivFileInfo(m_inputIvFilepath);

    if (!ivFileInfo.exists()) {
        qCritical() << "File " << ivFileInfo.absoluteFilePath() << " does not exist.";
        return false;
    }

    const QDir outputDirectory = QDir(m_outputDirectory);
    if (!outputDirectory.exists()) {
        qCritical() << "Directory " << outputDirectory << " does not exists.";
        return false;
    }

    const QDir ivBaseDirectory = ivFileInfo.dir();

    qDebug() << "Reding InterfaceView from " << ivFileInfo.absoluteFilePath();

    std::unique_ptr<IVModel> inputIv = readInterfaceView(ivFileInfo.absoluteFilePath());

    if (!inputIv) {
        qCritical() << "Unable to read InterfaceView.";
        return false;
    }

    const QStringList ivFunctionNames = findIvFunctions(*inputIv);

    for (const QString &ivFunction : ivFunctionNames) {
        const QFileInfo systemStructure = ivBaseDirectory.absolutePath() + QDir::separator() + "work"
                + QDir::separator() + ivFunction.toLower() + QDir::separator() + "SDL" + QDir::separator() + "src"
                + QDir::separator() + "system_structure.pr";
        const QFileInfo functionFile = ivBaseDirectory.absolutePath() + QDir::separator() + "work" + QDir::separator()
                + ivFunction.toLower() + QDir::separator() + "SDL" + QDir::separator() + "src" + QDir::separator()
                + ivFunction.toLower() + ".pr";
        const QFileInfo outputFile = outputDirectory.absolutePath() + QDir::separator() + ivFunction.toLower() + ".pml";

        if (!runSdl2Promela(systemStructure, functionFile, outputFile)) {
            return false;
        }
    }

    QStringList asn1Files;

    const QFileInfo dataviewUniq = ivBaseDirectory.absolutePath() + QDir::separator() + "work" + QDir::separator()
            + "dataview" + QDir::separator() + "dataview-uniq.asn";
    if (!dataviewUniq.exists()) {
        qCritical() << "File " << dataviewUniq.absoluteFilePath() << " does not exist.";
        return false;
    }

    asn1Files.append(dataviewUniq.absoluteFilePath());

    for (const QString &ivFunction : ivFunctionNames) {
        const QFileInfo functionDatamodel = ivBaseDirectory.absolutePath() + QDir::separator() + "work"
                + QDir::separator() + ivFunction.toLower() + QDir::separator() + "SDL" + QDir::separator() + "code"
                + QDir::separator() + ivFunction.toLower() + "_datamodel.asn";

        if (!functionDatamodel.exists()) {
            qCritical() << "File " << functionDatamodel.absoluteFilePath() << " does not exist.";
            return false;
        }

        asn1Files.append(functionDatamodel.absoluteFilePath());
    }

    const QFileInfo outputDataview = outputDirectory.absolutePath() + QDir::separator() + "dataview.pml";

    convertDataview(asn1Files, outputDataview.absoluteFilePath());

    const QFileInfo outputSystemFile = outputDirectory.absolutePath() + QDir::separator() + "system.pml";

    convertInterfaceview(ivFileInfo.absoluteFilePath(), outputSystemFile.absoluteFilePath());

    return true;
}

bool TmcVerifier::convertStopConditions()
{
    const QDir outputDirectory = QDir(m_outputDirectory);

    for (const QString &filepath : m_stopConditionsFiles) {
        const QFileInfo input(filepath);
        const QString base = input.baseName();
        const QFileInfo output = outputDirectory.absolutePath() + QDir::separator() + base.toLower() + ".pml";
        if (runSdl2PromelaForScl(input, output)) {
            return false;
        }
    }
    return true;
}

bool TmcVerifier::buildVerifier()
{
    return true;
}

bool TmcVerifier::executeVerifier()
{
    return true;
}

bool TmcVerifier::convertInterfaceview(const QString &inputFilepath, const QString &outputFilepath)
{
    qDebug() << "Converting InterfaceView " << inputFilepath << " to " << outputFilepath;

    Options options;

    options.add(IvOptions::inputFilepath, inputFilepath);
    options.add(IvOptions::configFilepath, IvOptions::defaultConfigFilename);
    options.add(PromelaOptions::outputFilepath, outputFilepath);

    for (const QString &filepath : m_stopConditionsFiles) {
        const QFileInfo input(filepath);
        const QString base = input.baseName();

        options.add(PromelaOptions::additionalIncludes, base.toLower() + ".pml");
    }

    ModelType sourceModelType = ModelType::InterfaceView;

    try {
        m_converter->convert({ sourceModelType }, ModelType::Promela, {}, std::move(options));
        return true;
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
    return false;
}

bool TmcVerifier::convertDataview(const QList<QString> &inputFilepathList, const QString &outputFilepath)
{
    qDebug() << "Converting ASN.1 files:";
    for (const QString &file : inputFilepathList) {
        qDebug() << "    " << file;
    }
    qDebug() << "  to:";
    qDebug() << "    " << outputFilepath;
    Options options;

    for (const QString inputFileName : inputFilepathList) {
        options.add(Asn1Options::inputFilepath, inputFileName);
    }

    options.add(PromelaOptions::outputFilepath, outputFilepath);

    ModelType sourceModelType = ModelType::Asn1;

    try {
        m_converter->convert({ sourceModelType }, ModelType::Promela, {}, std::move(options));
        return true;
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
    return false;
}

std::unique_ptr<IVModel> TmcVerifier::readInterfaceView(const QString &filepath)
{
    ivm::IVXMLReader reader;

    const bool ok = reader.readFile(filepath);
    if (ok) {
        auto model = std::make_unique<IVModel>(m_dynPropConfig);
        model->initFromObjects(reader.parsedObjects());
        return model;
    } else {
        return {};
    }

    return {};
}

QStringList TmcVerifier::findIvFunctions(const IVModel &model)
{
    Q_UNUSED(model);
    QStringList functionNames;

    QVector<IVFunction *> ivFunctionList = model.allObjectsByType<IVFunction>();
    for (IVFunction *ivFunction : ivFunctionList) {
        for (const auto &impl : ivFunction->implementations()) {
            if (impl.value().type() == QVariant::Type::String && impl.value().toString().toLower() == "sdl") {
                const QString functionName = ivFunction->property("name").toString();
                functionNames.append(functionName);
                break;
            }
        }
    }

    return functionNames;
}

bool TmcVerifier::runSdl2Promela(
        const QFileInfo &systemStructure, const QFileInfo &functionFile, const QFileInfo &outputFile)
{
    if (!systemStructure.exists()) {
        qCritical() << "File " << systemStructure.absoluteFilePath() << " does not exist.";
        return false;
    }
    if (!functionFile.exists()) {
        qCritical() << "File " << functionFile.absoluteFilePath() << " does not exist.";
        return false;
    }
    qDebug() << "Converting SDL files:";
    qDebug() << "    " << systemStructure.absoluteFilePath();
    qDebug() << "    " << functionFile.absoluteFilePath();
    qDebug() << "  to:";
    qDebug() << "    " << outputFile.absoluteFilePath();
    QStringList arguments = QStringList() << systemStructure.absoluteFilePath() << functionFile.absoluteFilePath()
                                          << "-o" << outputFile.absoluteFilePath();

    arguments.append(m_sdl2PromelaArgs);

    QProcess process = QProcess();

    qDebug() << "Executing: " << m_sdl2PromelaCommand << " with args:";
    for (const QString &arg : arguments) {
        qDebug() << "    " << arg;
    }
    process.start(m_sdl2PromelaCommand, arguments);

    if (!process.waitForStarted()) {
        qCritical("Cannot start process.");
        QByteArray standardError = process.readAllStandardError();
        QByteArray standardOutput = process.readAllStandardOutput();

        QString str = QString(standardError);
        qCritical() << "Stderr: " << str;
        str = QString(standardOutput);
        qCritical() << "Stdout: " << str;

        process.terminate();
        return false;
    }

    if (!process.waitForFinished(m_externalCommandTimeout)) {
        qCritical() << "Timeout.";
        process.terminate();
        return false;
    }

    if (process.exitCode() != EXIT_SUCCESS) {
        qCritical() << "Proces finished with code: " << process.exitCode();
        return false;
    }

    qDebug() << "Process finished";

    return true;
}

bool TmcVerifier::runSdl2PromelaForScl(const QFileInfo &inputFile, const QFileInfo &outputFile)
{
    qDebug() << "Converting Stop Conditions file: " << inputFile.absoluteFilePath() << " to "
             << outputFile.absoluteFilePath();
    QStringList arguments = QStringList()
            << "--scl" << inputFile.absoluteFilePath() << "-o" << outputFile.absoluteFilePath();

    arguments.append(m_sdl2PromelaArgs);

    QProcess process = QProcess();

    qDebug() << "Executing: " << m_sdl2PromelaCommand << " with args:";
    for (const QString &arg : arguments) {
        qDebug() << "    " << arg;
    }
    process.start(m_sdl2PromelaCommand, arguments);

    if (!process.waitForFinished(m_externalCommandTimeout)) {
        process.terminate();
        return false;
    }

    if (process.exitCode() != EXIT_SUCCESS) {
        return false;
    }

    return true;
}
}
