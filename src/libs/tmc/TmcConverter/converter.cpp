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

#include "converter.h"

#include <QDebug>
#include <QProcess>
#include <QtDebug>
#include <QtGlobal>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/asn1/Asn1Registrar/registrar.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/converter/converter.h>
#include <conversion/converter/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvRegistrar/registrar.h>
#include <conversion/promela/PromelaRegistrar/registrar.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivxmlreader.h>
#include <promela/PromelaOptions/options.h>
#include <tmc/SdlToPromelaConverter/converter.h>

using conversion::ConversionException;
using conversion::Converter;
using conversion::ModelType;
using conversion::Options;
using conversion::RegistrationFailedException;
using conversion::asn1::Asn1Options;
using conversion::asn1::Asn1Registrar;
using conversion::exporter::ExportException;
using conversion::importer::ImportException;
using conversion::iv::IvOptions;
using conversion::iv::IvRegistrar;
using conversion::promela::PromelaOptions;
using conversion::promela::PromelaRegistrar;
using conversion::translator::TranslationException;
using ivm::IVFunction;
using ivm::IVInterface;
using ivm::IVModel;
using shared::InterfaceParameter;
using tmc::converter::TmcConverter;

namespace tmc::converter {
TmcConverter::TmcConverter(const QString &inputIvFilepath, const QString &outputDirectory)
    : m_inputIvFilepath(inputIvFilepath)
    , m_outputDirectoryFilepath(outputDirectory)
    , m_ivBaseDirectory(QFileInfo(m_inputIvFilepath).dir())
    , m_outputDirectory(outputDirectory)
{
    Asn1Registrar asn1Registrar;
    bool result = asn1Registrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::Asn1);
    }
    IvRegistrar ivRegistrar;
    result = ivRegistrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::InterfaceView);
    }
    PromelaRegistrar tmcRegistrar;
    result = tmcRegistrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::Promela);
    }

    m_dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    m_dynPropConfig->init(QLatin1String("default_attributes.xml"));
}

bool TmcConverter::convert()
{
    QList<QFileInfo> allSdlFiles;
    if (!convertSystem(allSdlFiles)) {
        return false;
    }
    if (!convertStopConditions(allSdlFiles)) {
        return false;
    }
    return true;
}

bool TmcConverter::addStopConditionFiles(const QStringList &files)
{
    for (const QString &filepath : files) { // NOLINT(readability-use-anyofallof)
        QFileInfo fileinfo(filepath);
        if (!fileinfo.exists()) {
            return false;
        }
        m_stopConditionsFiles.append(filepath);
    }

    return true;
}

bool TmcConverter::convertModel(const std::set<conversion::ModelType> &sourceModelTypes,
        conversion::ModelType targetModelType, const std::set<conversion::ModelType> &auxilaryModelTypes,
        conversion::Options options) const
{
    try {
        Converter converter(m_registry, std::move(options));
        converter.convert(sourceModelTypes, targetModelType, auxilaryModelTypes);
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

bool TmcConverter::convertSystem(QList<QFileInfo> &allSdlFiles)
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

    qDebug() << "Reading InterfaceView from " << ivFileInfo.absoluteFilePath();

    std::unique_ptr<IVModel> inputIv = readInterfaceView(ivFileInfo.absoluteFilePath());

    if (!inputIv) {
        qCritical() << "Unable to read InterfaceView.";
        return false;
    }

    QStringList modelFunctions;
    QStringList environmentFunctions;
    findFunctionsToConvert(*inputIv, modelFunctions, environmentFunctions);
    QStringList environmentDatatypes;
    findEnvironmentDatatypes(*inputIv, environmentFunctions, environmentDatatypes);

    qDebug() << "Using the following SDL functions: " << modelFunctions.join(", ");
    qDebug() << "Using the following ENV functions: " << environmentFunctions.join(", ");
    qDebug() << "Using the following ENV data types: " << environmentDatatypes.join(", ");

    for (const QString &ivFunction : modelFunctions) {
        const QFileInfo &systemStructure = sdlSystemStructureLocation(ivFunction);
        const QFileInfo &sdlProcess = sdlImplementationLocation(ivFunction);

        QList<QFileInfo> sdlFiles;
        sdlFiles.append(systemStructure);
        sdlFiles.append(sdlProcess);

        allSdlFiles.append(systemStructure);
        allSdlFiles.append(sdlProcess);

        const QFileInfo outputFile = outputFilepath(ivFunction.toLower() + ".pml");

        SdlToPromelaConverter sdl2Promela;

        if (!sdl2Promela.convertSdl(sdlFiles, outputFile)) {
            return false;
        }
    }

    QStringList asn1Files;

    const QFileInfo dataviewUniq = dataViewUniqLocation();
    if (!dataviewUniq.exists()) {
        qCritical() << "File " << dataviewUniq.absoluteFilePath() << " does not exist.";
        return false;
    }

    asn1Files.append(dataviewUniq.absoluteFilePath());

    for (const QString &ivFunction : modelFunctions) {
        const QFileInfo functionDatamodel = sdlFunctionDatamodelLocation(ivFunction);

        if (!functionDatamodel.exists()) {
            qCritical() << "File " << functionDatamodel.absoluteFilePath() << " does not exist.";
            return false;
        }

        asn1Files.append(functionDatamodel.absoluteFilePath());
    }

    const QFileInfo outputDataview = outputFilepath("dataview.pml");

    convertDataview(asn1Files, outputDataview.absoluteFilePath());

    const QFileInfo outputEnv = outputFilepath("env_inlines.pml");

    createEnvGenerationInlines(dataviewUniq, outputEnv, environmentDatatypes);

    const QFileInfo outputSystemFile = outputFilepath("system.pml");

    convertInterfaceview(
            ivFileInfo.absoluteFilePath(), outputSystemFile.absoluteFilePath(), modelFunctions, environmentFunctions);

    return true;
}

bool TmcConverter::convertStopConditions(const QList<QFileInfo> &allSdlFiles)
{
    const QDir outputDirectory = QDir(m_outputDirectory);

    for (const QString &filepath : m_stopConditionsFiles) {
        const QFileInfo input(filepath);
        const QString base = input.baseName();
        const QFileInfo output = outputFilepath(base.toLower() + ".pml");

        SdlToPromelaConverter sdl2Promela;
        if (sdl2Promela.convertStopCondition(input, output, allSdlFiles)) {
            return false;
        }
    }
    return true;
}
bool TmcConverter::convertInterfaceview(const QString &inputFilepath, const QString &outputFilepath,
        const QStringList &modelFunctions, const QStringList &environmentFunctions)
{
    qDebug() << "Converting InterfaceView " << inputFilepath << " to " << outputFilepath;

    Options options;

    options.add(IvOptions::inputFilepath, inputFilepath);
    options.add(IvOptions::configFilepath, IvOptions::defaultConfigFilename);
    options.add(PromelaOptions::outputFilepath, outputFilepath);

    for (const QString &function : modelFunctions) {
        options.add(PromelaOptions::modelFunctionName, function);
    }

    for (const QString &function : environmentFunctions) {
        options.add(PromelaOptions::environmentFunctionName, function);
    }

    for (const QString &filepath : m_stopConditionsFiles) {
        const QFileInfo input(filepath);
        const QString base = input.baseName();

        options.add(PromelaOptions::additionalIncludes, base.toLower() + ".pml");
    }

    ModelType sourceModelType = ModelType::InterfaceView;

    return convertModel({ sourceModelType }, ModelType::Promela, {}, std::move(options));
}

bool TmcConverter::convertDataview(const QList<QString> &inputFilepathList, const QString &outputFilepath)
{
    qDebug() << "Converting ASN.1 files:";
    for (const QString &file : inputFilepathList) {
        qDebug() << "    " << file;
    }
    qDebug() << "  to:";
    qDebug() << "    " << outputFilepath;
    Options options;

    for (const QString &inputFileName : inputFilepathList) {
        options.add(Asn1Options::inputFilepath, inputFileName);
    }

    options.add(PromelaOptions::outputFilepath, outputFilepath);

    ModelType sourceModelType = ModelType::Asn1;

    return convertModel({ sourceModelType }, ModelType::Promela, {}, std::move(options));
}

std::unique_ptr<IVModel> TmcConverter::readInterfaceView(const QString &filepath)
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

void TmcConverter::findFunctionsToConvert(const IVModel &model, QStringList &sdlFunctions, QStringList &envFunctions)
{
    QStringList functionNames;

    QVector<IVFunction *> ivFunctionList = model.allObjectsByType<IVFunction>();
    for (IVFunction *ivFunction : ivFunctionList) {
        if (isSdlFunction(ivFunction)) {
            sdlFunctions.append(ivFunction->property("name").toString());
        } else {
            envFunctions.append(ivFunction->property("name").toString());
        }
    }
}

bool TmcConverter::isSdlFunction(const ivm::IVFunction *function)
{
    const QString defaultImplementation = function->defaultImplementation();
    for (const auto &impl : function->implementations()) { // NOLINT(readability-use-anyofallof)
        if (impl.name() == defaultImplementation && impl.value().type() == QVariant::Type::String
                && impl.value().toString().toLower() == "sdl") {
            return true;
        }
    }
    return false;
}

void TmcConverter::findEnvironmentDatatypes(
        const ivm::IVModel &model, const QStringList &envFunctions, QStringList &envDataTypes)
{
    for (const QString &functionName : envFunctions) {
        IVFunction *function = model.getFunction(functionName, Qt::CaseSensitivity::CaseInsensitive);
        QVector<IVInterface *> requiredInterfaces = function->ris();
        for (const IVInterface *interface : requiredInterfaces) {
            QVector<InterfaceParameter> parameters = interface->params();
            if (parameters.size() > 1) {
                // TODO report an error
            } else if (parameters.size() == 1) {
                envDataTypes.append(parameters.front().paramTypeName());
            }
        }
    }
}

bool TmcConverter::createEnvGenerationInlines(
        const QFileInfo &inputDataView, const QFileInfo &outputFilepath, const QStringList &envDatatypes)
{
    qDebug() << "Converting ASN.1 environment value generators using " << inputDataView.absoluteFilePath();
    Options options;

    options.add(Asn1Options::inputFilepath, inputDataView.absoluteFilePath());
    options.add(PromelaOptions::outputFilepath, outputFilepath.absoluteFilePath());

    options.add(PromelaOptions::asn1ValueGeneration);

    for (const QString &datatype : envDatatypes) {
        options.add(PromelaOptions::asn1ValueGenerationForType, datatype);
    }

    ModelType sourceModelType = ModelType::Asn1;

    try {
        return convertModel({ sourceModelType }, ModelType::Promela, {}, std::move(options));
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

QFileInfo TmcConverter::workDirectory() const
{
    return m_ivBaseDirectory.absolutePath() + QDir::separator() + "work";
}

QFileInfo TmcConverter::dataViewUniqLocation() const
{
    return workDirectory().absoluteFilePath() + QDir::separator() + "dataview" + QDir::separator()
            + "dataview-uniq.asn";
}

QFileInfo TmcConverter::sdlImplementationBaseDirectory(const QString &functionName) const
{
    return workDirectory().absoluteFilePath() + QDir::separator() + functionName.toLower() + QDir::separator() + "SDL";
}

QFileInfo TmcConverter::sdlImplementationLocation(const QString &functionName) const
{
    return sdlImplementationBaseDirectory(functionName).absoluteFilePath() + QDir::separator() + "src"
            + QDir::separator() + functionName.toLower() + ".pr";
}

QFileInfo TmcConverter::sdlSystemStructureLocation(const QString &functionName) const
{
    return sdlImplementationBaseDirectory(functionName).absoluteFilePath() + QDir::separator() + "src"
            + QDir::separator() + "system_structure.pr";
}

QFileInfo TmcConverter::sdlFunctionDatamodelLocation(const QString &functionName) const
{
    return sdlImplementationBaseDirectory(functionName).absoluteFilePath() + QDir::separator() + "code"
            + QDir::separator() + functionName.toLower() + "_datamodel.asn";
}

QFileInfo TmcConverter::outputFilepath(const QString &name)
{
    return m_outputDirectory.absolutePath() + QDir::separator() + name;
}
}
