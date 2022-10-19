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

#include <QBuffer>
#include <QDebug>
#include <QTemporaryFile>
#include <QtDebug>
#include <QtGlobal>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/asn1/Asn1Registrar/registrar.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/converter/converter.h>
#include <conversion/converter/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvRegistrar/registrar.h>
#include <conversion/msc/MscOptions/options.h>
#include <conversion/msc/MscRegistrar/registrar.h>
#include <conversion/promela/PromelaRegistrar/registrar.h>
#include <conversion/sdl/SdlRegistrar/registrar.h>
#include <iostream>
#include <ivcore/ivfunction.h>
#include <ivcore/ivxmlreader.h>
#include <libiveditor/ivexporter.h>
#include <promela/PromelaOptions/options.h>
#include <sdl/SdlOptions/options.h>
#include <tmc/SdlToPromelaConverter/converter.h>
#include <tmc/TmcInterfaceViewOptimizer/interfaceviewoptimizer.h>

using conversion::ConversionException;
using conversion::Converter;
using conversion::Escaper;
using conversion::ModelType;
using conversion::Options;
using conversion::RegistrationFailedException;
using conversion::asn1::Asn1Options;
using conversion::asn1::Asn1Registrar;
using conversion::exporter::ExportException;
using conversion::importer::ImportException;
using conversion::iv::IvOptions;
using conversion::iv::IvRegistrar;
using conversion::msc::MscOptions;
using conversion::msc::MscRegistrar;
using conversion::promela::PromelaOptions;
using conversion::promela::PromelaRegistrar;
using conversion::sdl::SdlOptions;
using conversion::sdl::SdlRegistrar;
using conversion::translator::TranslationException;
using ive::IVExporter;
using ivm::IVFunction;
using ivm::IVFunctionType;
using ivm::IVInterface;
using ivm::IVModel;
using ivm::IVObject;
using shared::InterfaceParameter;
using tmc::converter::TmcConverter;

namespace tmc::converter {

TmcConverter::ObserverInfo::ObserverInfo(const QString path, const uint32_t priority)
{
    m_path = path;
    m_priority = priority;
}

const QString &TmcConverter::ObserverInfo::path() const
{
    return m_path;
}
uint32_t TmcConverter::ObserverInfo::priority() const
{
    return m_priority;
}

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
    MscRegistrar mscRegistrar;
    result = mscRegistrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::Msc);
    }
    PromelaRegistrar tmcRegistrar;
    result = tmcRegistrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::Promela);
    }
    SdlRegistrar sdlRegistrar;
    result = sdlRegistrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::Sdl);
    }

    m_dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    m_dynPropConfig->init(shared::interfaceCustomAttributesFilePath());
}

bool TmcConverter::convert()
{
    std::map<QString, ProcessMetadata> allSdlFiles;
    if (!convertSystem(allSdlFiles)) {
        return false;
    }
    if (!convertStopConditions(allSdlFiles)) {
        return false;
    }
    return true;
}

void TmcConverter::setMscObserverFiles(const QStringList &mscObserverFiles)
{
    m_mscObserverFiles = mscObserverFiles;
}

void TmcConverter::setEnvironmentFunctions(const std::vector<QString> &environmentFunctions)
{
    m_environmentFunctions = environmentFunctions;
}

void TmcConverter::setKeepFunctions(const std::vector<QString> &keepFunctions)
{
    m_keepFunctions = keepFunctions;
}

void TmcConverter::setGlobalInputVectorLengthLimit(std::optional<QString> limit)
{
    m_globalInputVectorLengthLimit = std::move(limit);
}

void TmcConverter::setInterfaceInputVectorLengthLimits(std::unordered_map<QString, QString> limits)
{
    m_interfaceInputVectorLengthLimits = std::move(limits);
}

void TmcConverter::setProcessesBasePriority(std::optional<QString> value)
{
    m_processesBasePriority = std::move(value);
}

void TmcConverter::setDelta(std::optional<int> value)
{
    m_delta = std::move(value);
}

void TmcConverter::setRealTypeEnabled(bool isRealTypeEnabled)
{
    m_isRealTypeEnabled = isRealTypeEnabled;
}

void TmcConverter::setSubtypesFilepaths(const std::vector<QString> &filepaths)
{
    m_subtypesFilepaths = filepaths;
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

bool TmcConverter::attachObserver(const QString &observerPath, const uint32_t priority)
{
    m_observerInfos.emplace_back(ObserverInfo(observerPath, priority));
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

auto TmcConverter::integrateObserver(const ObserverInfo &info, QStringList &observerNames, QStringList &asn1Files,
        std::map<QString, ProcessMetadata> &allSdlFiles, QStringList &attachmentInfos)
{
    const auto process = QFileInfo(info.path());
    const auto processName = process.baseName();
    const auto directory = process.absoluteDir();
    const auto datamodel =
            QFileInfo(directory.absolutePath() + QDir::separator() + processName.toLower() + "_datamodel.asn");

    ProcessMetadata meta(Escaper::escapePromelaName(processName), std::nullopt, process, datamodel, QList<QFileInfo>());
    SdlToPromelaConverter sdl2Promela;

    const auto promelaFilename = Escaper::escapePromelaIV(processName) + ".pml";
    const auto infoPath = outputFilepath(Escaper::escapePromelaIV(processName) + ".info");
    observerNames.append(Escaper::escapePromelaIV(processName));
    sdl2Promela.convertObserverSdl(meta, outputFilepath(promelaFilename), infoPath);
    QFile infoFile(infoPath.absoluteFilePath());
    if (infoFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&infoFile);
        while (!in.atEnd()) {
            attachmentInfos.append(in.readLine() + ":" + QString::number(info.priority()));
            qDebug() << "Appended observer specification " << attachmentInfos.last();
        }
        infoFile.close();
    } else {
        const auto message = QString("Could not open observer info file %1").arg(infoPath.absoluteFilePath());
        throw TranslationException(message);
    }

    allSdlFiles.emplace(processName, meta);
    asn1Files.append(datamodel.absoluteFilePath());
}

bool TmcConverter::convertSystem(std::map<QString, ProcessMetadata> &allSdlFiles)
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

    if (!m_environmentFunctions.empty() && !m_keepFunctions.empty()) {
        qCritical() << "Environment and keep functions shouldn't be specified at once";
        return false;
    }

    if (!m_environmentFunctions.empty()) {
        InterfaceViewOptimizer::optimizeModel(
                inputIv.get(), m_environmentFunctions, InterfaceViewOptimizer::Mode::Environment);
    } else if (!m_keepFunctions.empty()) {
        InterfaceViewOptimizer::optimizeModel(inputIv.get(), m_keepFunctions, InterfaceViewOptimizer::Mode::Keep);
    } else {
        InterfaceViewOptimizer::optimizeModel(inputIv.get(), {}, InterfaceViewOptimizer::Mode::None);
    }

    QTemporaryFile outputOptimizedIvFile;
    if (!outputOptimizedIvFile.open()) {
        qCritical() << "Unable to create temp file for optimized IV model";
        return false;
    }
    outputOptimizedIvFile.close();
    const auto &outputOptimizedIvFileName = outputOptimizedIvFile.fileName();

    saveOptimizedInterfaceView(inputIv.get(), outputOptimizedIvFileName);

    QStringList modelFunctions;
    QStringList environmentFunctions;

    findFunctionsToConvert(*inputIv, modelFunctions, allSdlFiles, environmentFunctions);
    QStringList environmentDatatypes;
    findEnvironmentDatatypes(*inputIv, environmentFunctions, environmentDatatypes);

    qDebug() << "Using the following SDL functions: " << modelFunctions.join(", ");
    qDebug() << "Using the following ENV functions: " << environmentFunctions.join(", ");
    qDebug() << "Using the following ENV data types: " << environmentDatatypes.join(", ");

    QMap<QString, QString> uniqueAsn1Files;
    for (const QString &ivFunction : modelFunctions) {
        const ProcessMetadata &processMetadata = allSdlFiles.at(ivFunction);
        const QFileInfo outputFile = outputFilepath(processMetadata.getName().toLower() + ".pml");

        SdlToPromelaConverter sdl2Promela;

        if (!sdl2Promela.convertSdl(processMetadata, outputFile)) {
            return false;
        }
    }

    if (!convertMscObservers(outputOptimizedIvFileName)) {
        return false;
    }

    QStringList asn1Files;

    for (const auto &info : m_observerInfos) {
        integrateObserver(info, m_observerNames, asn1Files, allSdlFiles, m_observerAttachmentInfos);
    }

    const QFileInfo simuDataView = simuDataViewLocation();
    if (!simuDataView.exists()) {
        qCritical() << "File " << simuDataView.absoluteFilePath() << " does not exist.";
        return false;
    }

    asn1Files.append(simuDataView.absoluteFilePath());

    for (const QString &datamodel : uniqueAsn1Files) {
        if (!QFileInfo(datamodel).exists()) {
            qCritical() << "File " << datamodel << " does not exist.";
            return false;
        }

        asn1Files.append(datamodel);
    }

    for (const auto &subtypesFilepath : m_subtypesFilepaths) {
        QFileInfo subtypesFileInfo(subtypesFilepath);

        if (!subtypesFileInfo.exists()) {
            qCritical() << "File " << subtypesFileInfo.absoluteFilePath() << " with subtypes does not exist.";
            return false;
        }

        asn1Files.append(subtypesFileInfo.absoluteFilePath());
    }

    const QFileInfo outputDataview = outputFilepath("dataview.pml");
    convertDataview(asn1Files, outputOptimizedIvFileName, outputDataview.absoluteFilePath());

    const QFileInfo outputEnv = outputFilepath("env_inlines.pml");
    createEnvGenerationInlines(simuDataView, outputOptimizedIvFileName, outputEnv, environmentDatatypes);

    const QFileInfo outputSystemFile = outputFilepath("system.pml");
    convertInterfaceview(outputOptimizedIvFileName, outputSystemFile.absoluteFilePath(), asn1Files, modelFunctions,
            environmentFunctions);

    return true;
}

bool TmcConverter::convertStopConditions(const std::map<QString, ProcessMetadata> &allSdlFiles)
{
    const QDir outputDirectory = QDir(m_outputDirectory);

    QList<QFileInfo> inputFiles;

    for (const QString &filepath : m_stopConditionsFiles) {
        inputFiles.append(QFileInfo(filepath));
    }

    const QFileInfo output = outputFilepath(QString("scl.pml"));
    const bool includeObservers = !m_observerNames.isEmpty();

    SdlToPromelaConverter sdl2Promela;
    if (!sdl2Promela.convertStopConditions(inputFiles, output, allSdlFiles, includeObservers)) {
        return false;
    }
    return true;
}

bool TmcConverter::convertInterfaceview(const QString &inputFilepath, const QString &outputFilepath,
        const QList<QString> &asn1FilepathList, const QStringList &modelFunctions,
        const QStringList &environmentFunctions)
{
    qDebug() << "Converting InterfaceView " << inputFilepath << " to " << outputFilepath;

    Options options;

    if (m_isRealTypeEnabled) {
        options.add(PromelaOptions::enhancedSpinSupport);
    }
    options.add(IvOptions::inputFilepath, inputFilepath);
    options.add(IvOptions::configFilepath, shared::interfaceCustomAttributesFilePath());
    options.add(PromelaOptions::outputFilepath, outputFilepath);

    for (const QString &function : modelFunctions) {
        options.add(PromelaOptions::modelFunctionName, function);
    }

    for (const QString &function : environmentFunctions) {
        options.add(PromelaOptions::environmentFunctionName, function);
    }

    if (m_globalInputVectorLengthLimit) {
        options.add(PromelaOptions::globalInputVectorLengthLimit, *m_globalInputVectorLengthLimit);
    }

    for (const auto &[interfaceName, value] : m_interfaceInputVectorLengthLimits) {
        options.add(PromelaOptions::interfaceInputVectorLengthLimit.arg(interfaceName.toLower()), value);
    }

    if (m_processesBasePriority) {
        options.add(PromelaOptions::processesBasePriority, *m_processesBasePriority);
    }

    for (const auto &subtypesFilepath : m_subtypesFilepaths) {
        QFileInfo subtypesFileInfo(subtypesFilepath);
        options.add(PromelaOptions::subtypesFilepath, subtypesFileInfo.absoluteFilePath());
    }

    for (const QString &observer : m_observerNames) {
        options.add(PromelaOptions::observerFunctionName, observer);
    }

    for (const auto &info : m_observerAttachmentInfos) {
        options.add(PromelaOptions::observerAttachment, info);
    }

    if (!m_stopConditionsFiles.isEmpty() || !m_observerNames.isEmpty()) {
        options.add(PromelaOptions::additionalIncludes, "scl.pml");
    }

    for (const QString &inputFileName : asn1FilepathList) {
        options.add(Asn1Options::inputFilepath, inputFileName);
    }

    return convertModel(
            { ModelType::InterfaceView, ModelType::Asn1 }, ModelType::PromelaSystem, {}, std::move(options));
}

bool TmcConverter::convertDataview(
        const QList<QString> &inputFilepathList, const QString &ivFilepath, const QString &outputFilepath)
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

    for (const auto &subtypesFilepath : m_subtypesFilepaths) {
        QFileInfo subtypesFileInfo(subtypesFilepath);
        options.add(PromelaOptions::subtypesFilepath, subtypesFileInfo.absoluteFilePath());
    }

    if (m_isRealTypeEnabled) {
        options.add(PromelaOptions::enhancedSpinSupport);
    }

    options.add(IvOptions::inputFilepath, ivFilepath);
    options.add(IvOptions::configFilepath, shared::interfaceCustomAttributesFilePath());

    options.add(PromelaOptions::outputFilepath, outputFilepath);

    return convertModel({ ModelType::Asn1, ModelType::InterfaceView }, ModelType::PromelaData, {}, std::move(options));
}

bool TmcConverter::convertMscObservers(const QString &ivFilePath)
{
    for (const QString &mscFilePath : m_mscObserverFiles) {
        QFileInfo mscFile(mscFilePath);

        if (!mscFile.exists()) {
            qCritical() << "MSC file " << mscFilePath << " doesn't exist";
            return false;
        }

        const auto outputDir = mscFile.dir();
        const auto &outputPath = outputDir.path() + QDir::separator();

        Options options;
        if (m_isRealTypeEnabled) {
            options.add(PromelaOptions::enhancedSpinSupport);
        }
        options.add(MscOptions::inputFilepath, mscFilePath);
        options.add(Asn1Options::inputFilepath, simuDataViewLocation().absoluteFilePath());
        options.add(IvOptions::inputFilepath, ivFilePath);
        options.add(IvOptions::configFilepath, shared::interfaceCustomAttributesFilePath());
        options.add(MscOptions::simuDataViewFilepath, simuDataViewLocation().absoluteFilePath());
        options.add(SdlOptions::filepathPrefix, outputPath);

        qDebug() << "Converting MSC file" << mscFilePath << "to an SDL observer";

        if (!convertModel({ ModelType::Msc, ModelType::Asn1, ModelType::InterfaceView }, ModelType::Sdl, {},
                    std::move(options))) {
            qCritical() << "Unable to translate MSC file" << mscFilePath << "to SDL observer";
            return false;
        }

        QProcess process;
        process.setWorkingDirectory(outputPath);

        for (const auto &sdlFileName : QDir(outputPath).entryList({ "*.pr" })) {
            if (!generateObserverDatamodel(process, sdlFileName)) {
                return false;
            }

            const auto sdlFilePath = outputPath + sdlFileName;
            m_observerInfos.emplace_back(ObserverInfo(sdlFilePath, 1));
        }
    }

    return true;
}

bool TmcConverter::generateObserverDatamodel(QProcess &process, const QString &sdlFileName)
{
    QStringList arguments;
    arguments << "--toAda" << sdlFileName;

    qDebug() << "Executing:" << m_opengeodeCommand << "with args:" << arguments.join(", ");

    process.start(m_opengeodeCommand, arguments);

    if (!process.waitForStarted()) {
        qCritical() << "Cannot generate observer datamodel using opengeode.";
        process.terminate();
        return false;
    }

    if (!process.waitForFinished(m_commandTimeout)) {
        qCritical() << "Timeout while waiting for generating observer datamodel.";
        process.terminate();
        return false;
    }

    if (process.exitCode() != EXIT_SUCCESS) {
        qCritical() << "Observer datamodel generation finished with code: " << process.exitCode();
        return false;
    }

    return true;
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

void TmcConverter::saveOptimizedInterfaceView(const IVModel *ivModel, const QString outputFilePath)
{
    QByteArray modelData;
    QBuffer modelDataBuffer(&modelData);
    modelDataBuffer.open(QIODevice::WriteOnly);

    IVExporter exporter;
    exporter.exportObjects(ivModel->objects().values(), &modelDataBuffer);

    QSaveFile outputFile(outputFilePath);
    outputFile.open(QIODevice::WriteOnly);
    outputFile.write(modelData);
    outputFile.commit();
}

void TmcConverter::findFunctionsToConvert(const IVModel &model, QStringList &sdlFunctions,
        std::map<QString, ProcessMetadata> &sdlProcesses, QStringList &envFunctions)
{
    QStringList functionNames;

    QVector<IVFunctionType *> ivFunctionTypeList = model.allObjectsByType<IVFunctionType>();

    for (IVFunctionType *ivFunctionType : ivFunctionTypeList) {
        if (ivFunctionType->type() == IVObject::Type::FunctionType) {
            const QString name = ivFunctionType->property("name").toString();
            qDebug() << "Function Type " << name;
        }
    }

    QVector<IVFunction *> ivFunctionList = model.allObjectsByType<IVFunction>();
    for (IVFunction *ivFunction : ivFunctionList) {
        if (isSdlFunction(ivFunction)) {
            const QString ivFunctionName = ivFunction->property("name").toString();
            sdlFunctions.append(ivFunctionName);

            const QFileInfo systemStructure = sdlSystemStructureLocation(ivFunctionName);
            QList<QFileInfo> contextLocations;
            contextLocations.append(sdlFunctionContextLocation(ivFunctionName));

            if (ivFunction->instanceOf() != nullptr) {
                const QString ivFunctionTypeName = ivFunction->instanceOf()->property("name").toString();
                contextLocations.append(sdlFunctionContextLocation(ivFunctionTypeName));
                const QFileInfo sdlProcess = sdlImplementationLocation(ivFunctionTypeName);
                const QFileInfo functionDatamodel = sdlFunctionDatamodelLocation(ivFunctionName, ivFunctionTypeName);

                sdlProcesses.emplace(ivFunctionName,
                        ProcessMetadata(ivFunctionName, systemStructure, sdlProcess, functionDatamodel,
                                std::move(contextLocations)));
            } else {
                const QFileInfo sdlProcess = sdlImplementationLocation(ivFunctionName);
                const QFileInfo functionDatamodel = sdlFunctionDatamodelLocation(ivFunctionName);
                sdlProcesses.emplace(ivFunctionName,
                        ProcessMetadata(ivFunctionName, systemStructure, sdlProcess, functionDatamodel,
                                std::move(contextLocations)));
            }

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

bool TmcConverter::createEnvGenerationInlines(const QFileInfo &inputDataView, const QString &ivFilepath,
        const QFileInfo &outputFilepath, const QStringList &envDatatypes)
{
    qDebug() << "Converting ASN.1 environment value generators using " << inputDataView.absoluteFilePath();
    Options options;

    options.add(Asn1Options::inputFilepath, inputDataView.absoluteFilePath());

    for (const auto &subtypesFilepath : m_subtypesFilepaths) {
        QFileInfo subtypesFileInfo(subtypesFilepath);

        qDebug() << "Converting ASN.1 subtypes value generators using " << subtypesFileInfo.absoluteFilePath();
        options.add(Asn1Options::inputFilepath, subtypesFileInfo.absoluteFilePath());
        options.add(PromelaOptions::subtypesFilepath, subtypesFileInfo.absoluteFilePath());
    }

    options.add(IvOptions::inputFilepath, ivFilepath);
    options.add(IvOptions::configFilepath, shared::interfaceCustomAttributesFilePath());

    options.add(PromelaOptions::outputFilepath, outputFilepath.absoluteFilePath());

    options.add(PromelaOptions::asn1ValueGeneration);

    for (const QString &datatype : envDatatypes) {
        options.add(PromelaOptions::asn1ValueGenerationForType, datatype);
    }

    try {
        return convertModel(
                { ModelType::Asn1, ModelType::InterfaceView }, ModelType::PromelaData, {}, std::move(options));
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

QFileInfo TmcConverter::simuDataViewLocation() const
{
    return workDirectory().absoluteFilePath() + QDir::separator() + "simulation" + QDir::separator() + "observers"
            + QDir::separator() + "observer.asn";
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

QFileInfo TmcConverter::sdlFunctionDatamodelLocation(const QString &functionName, const QString &functionTypeName) const
{
    return sdlImplementationBaseDirectory(functionName).absoluteFilePath() + QDir::separator() + "code"
            + QDir::separator() + functionTypeName.toLower() + "_datamodel.asn";
}

QFileInfo TmcConverter::sdlFunctionContextLocation(const QString &functionName) const
{
    QFileInfo contextLocation = sdlImplementationBaseDirectory(functionName).absoluteFilePath() + QDir::separator()
            + "Context-" + functionName.toLower() + ".asn";
    if (contextLocation.exists()) {
        return contextLocation;
    } else {
        return QFileInfo();
    }
}

QFileInfo TmcConverter::outputFilepath(const QString &name)
{
    return m_outputDirectory.absolutePath() + QDir::separator() + name;
}
}
