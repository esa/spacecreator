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
#include <asn1library/asn1scc.h>
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
#include <conversion/simulatortrail/SimulatorTrailRegistrar/registrar.h>
#include <conversion/spintrail/SpinTrailRegistrar/registrar.h>
#include <iostream>
#include <ivcore/ivfunction.h>
#include <ivcore/ivxmlreader.h>
#include <ivcore/ivxmlwriter.h>
#include <promela/PromelaOptions/options.h>
#include <sdl/SdlOptions/options.h>
#include <simulatortrail/SimulatorTrailOptions/options.h>
#include <spintrail/SpinTrailOptions/options.h>
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
using conversion::simulatortrail::SimulatorTrailOptions;
using conversion::simulatortrail::SimulatorTrailRegistrar;
using conversion::spintrail::SpinTrailOptions;
using conversion::spintrail::SpinTrailRegistrar;
using conversion::translator::TranslationException;
using ivm::IVFunction;
using ivm::IVFunctionType;
using ivm::IVInterface;
using ivm::IVModel;
using ivm::IVObject;
using ivm::IVXMLWriter;
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

TmcConverter::TmcConverter(const QString &inputIvFilepath, const QString &outputDirectory, QObject *parent)
    : QObject(parent)
    , m_inputIvFilepath(inputIvFilepath)
    , m_outputDirectoryFilepath(outputDirectory)
    , m_ivBaseDirectory(QFileInfo(m_inputIvFilepath).dir())
    , m_outputDirectory(outputDirectory)
    , m_isRealTypeEnabled(false)
    , m_isMulticastEnabled(false)
    , m_commandTimeout(12000)
    , m_numberOfProctypes(0)
    , m_process(new QProcess(this))
    , m_timer(new QTimer(this))
    , m_sdlToPromelaConverter(new SdlToPromelaConverter(this))
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
    SpinTrailRegistrar spinTrailRegistrar;
    result = spinTrailRegistrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::SpinTrail);
    }
    SimulatorTrailRegistrar simulatorTrailRegistrar;
    result = simulatorTrailRegistrar.registerCapabilities(m_registry);
    if (!result) {
        throw RegistrationFailedException(ModelType::SimulatorTrail);
    }
    m_dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    m_dynPropConfig->init(shared::interfaceCustomAttributesFilePath());

    connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(processStderrReady()));
    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStdoutReady()));
    connect(m_process, SIGNAL(started()), this, SLOT(processStarted()));
    connect(m_process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(m_sdlToPromelaConverter, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
}

TmcConverter::~TmcConverter()
{
    m_timer->stop();
    if (m_process->state() != QProcess::ProcessState::NotRunning) {
        m_process->kill();
        m_process->waitForFinished();
    }
}

bool TmcConverter::prepare()
{
    m_numberOfProctypes = 0;
    const QFileInfo ivFileInfo(m_inputIvFilepath);

    if (!ivFileInfo.exists()) {
        QString text = QString("File %1 does not exist.\n").arg(ivFileInfo.absoluteFilePath());
        Q_EMIT message(text);
        return false;
    }

    const QDir outputDirectory = QDir(m_outputDirectory);
    if (!outputDirectory.exists()) {
        QString text = QString("Directory %1 does not exists.\n").arg(outputDirectory.path());
        Q_EMIT message(text);
        return false;
    }

    const QDir ivBaseDirectory = ivFileInfo.dir();

    Q_EMIT message(QString("Reading InterfaceView from %1\n").arg(ivFileInfo.absoluteFilePath()));

    std::unique_ptr<IVModel> inputIv = readInterfaceView(ivFileInfo.absoluteFilePath());

    if (!inputIv) {
        Q_EMIT message(QString("Unable to read InterfaceView.\n"));
        return false;
    }

    if (!m_environmentFunctions.empty() && !m_keepFunctions.empty()) {
        Q_EMIT message(QString("Environment and keep functions shouldn't be specified at once\n"));
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

    m_outputOptimizedIvFileName = outputDirectory.absolutePath() + QDir::separator() + "interfaceview.xml";
    saveOptimizedInterfaceView(inputIv.get(), m_outputOptimizedIvFileName);
    findFunctionsToConvert(*inputIv, m_modelFunctions, m_allSdlFiles, m_finalEnvironmentFunctions);
    findEnvironmentDatatypes(*inputIv, m_finalEnvironmentFunctions, m_environmentDatatypes);

    Q_EMIT message(QString("Using the following SDL functions: %1\n").arg(m_modelFunctions.join(", ")));
    Q_EMIT message(QString("Using the following ENV functions: %1\n").arg(m_finalEnvironmentFunctions.join(", ")));
    Q_EMIT message(QString("Using the following ENV data types: %1\n").arg(m_environmentDatatypes.join(", ")));

    for (const QString &ivFunction : m_modelFunctions) {
        const ProcessMetadata &processMetadata = m_allSdlFiles.at(ivFunction);
        m_functionsToConvert.push_back(processMetadata);
    }

    const QFileInfo simuDataView = simuDataViewLocation();
    if (!simuDataView.exists()) {
        Q_EMIT message(QString("File %1 does not exist.\n").arg(simuDataView.absoluteFilePath()));
        Q_EMIT conversionFinished(false);
        return false;
    }

    m_asn1Files.append(simuDataView.absoluteFilePath());

    for (const auto &subtypesFilepath : m_subtypesFilepaths) {
        QFileInfo subtypesFileInfo(subtypesFilepath);

        if (!subtypesFileInfo.exists()) {
            Q_EMIT message(QString("File %1 with subtypes does not exist.\n").arg(subtypesFileInfo.absoluteFilePath()));
            Q_EMIT conversionFinished(false);
            return false;
        }

        m_asn1Files.append(subtypesFileInfo.absoluteFilePath());
    }

    return true;
}

void TmcConverter::convert()
{
    std::copy(m_mscObserverFiles.begin(), m_mscObserverFiles.end(), std::back_inserter(m_mscObserversToConvert));
    convertNextFunction();
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

void TmcConverter::setDelta(std::optional<QString> value)
{
    m_delta = std::move(value);
}

void TmcConverter::setMulticastEnabled(bool isMulticastEnabled)
{
    m_isMulticastEnabled = isMulticastEnabled;
}

void TmcConverter::setSdl2PromelaTimeout(int timeout)
{
    m_sdlToPromelaConverter->setSdl2PromelaTimeout(timeout);
}

void TmcConverter::setCommandTimeout(int timeout)
{
    m_commandTimeout = timeout * 1000; // convert to ms
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

const QStringList &TmcConverter::getStopConditionFiles() const
{
    return m_stopConditionsFiles;
}

bool TmcConverter::attachObserver(const QString &observerPath, const uint32_t priority)
{
    m_observerInfos.emplace_back(ObserverInfo(observerPath, priority));
    return true;
}

bool TmcConverter::convertTrace(const QString &inputFile, const QString &outputFile)
{
    m_observerAttachmentInfos.clear();
    m_observerNames.clear();
    for (const auto &info : m_observerInfos) {
        const auto process = QFileInfo(info.path());
        const auto processName = process.baseName();
        const auto directory = process.absoluteDir();
        const auto datamodel =
                QFileInfo(directory.absolutePath() + QDir::separator() + processName.toLower() + "_datamodel.asn");
        ProcessMetadata meta(
                Escaper::escapePromelaName(processName), std::nullopt, process, datamodel, QList<QFileInfo>());

        m_observerNames.append(Escaper::escapePromelaIV(processName));
        const auto infoPath = outputFilepath(Escaper::escapePromelaIV(processName) + ".info");

        QFile infoFile(infoPath.absoluteFilePath());
        if (infoFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&infoFile);
            while (!in.atEnd()) {
                m_observerAttachmentInfos.append(in.readLine() + ":p" + QString::number(info.priority()));
            }
            infoFile.close();
        } else {
            const auto errorMessage =
                    QString("Could not open observer info file %1\n").arg(infoPath.absoluteFilePath());
            Q_EMIT message(errorMessage);
            Q_EMIT conversionFinished(false);
            return false;
        }
    }

    const QFileInfo outputSystemFile = outputFilepath("system.pml");
    conversion::Options options;
    options.add(SimulatorTrailOptions::outputFilepath, outputFile);
    options.add(SpinTrailOptions::inputFilepath, inputFile);
    options.add(IvOptions::configFilepath, shared::interfaceCustomAttributesFilePath());
    if (m_isMulticastEnabled) {
        options.add(PromelaOptions::supportMulticast);
    }

    options.add(IvOptions::inputFilepath, m_outputOptimizedIvFileName);
    options.add(PromelaOptions::outputFilepath, outputSystemFile.absoluteFilePath());

    for (const QString &function : m_modelFunctions) {
        options.add(PromelaOptions::modelFunctionName, function);
    }

    for (const QString &function : m_finalEnvironmentFunctions) {
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

    for (const QString &inputFileName : m_asn1Files) {
        options.add(Asn1Options::inputFilepath, inputFileName);
    }

    return convertModel({ ModelType::SpinTrail, ModelType::InterfaceView, ModelType::Asn1 }, ModelType::SimulatorTrail,
            {}, std::move(options));
}

size_t TmcConverter::getNumberOfProctypes() const
{
    return m_numberOfProctypes;
}

bool TmcConverter::convertDataview(const QList<QString> &inputFilepathList, const QString &outputFilepath)
{
    Q_EMIT message(QString("Converting ASN.1 files:\n"));
    for (const QString &file : inputFilepathList) {
        Q_EMIT message(QString("    %1\n").arg(file));
    }
    Q_EMIT message(QString("  to:\n"));
    Q_EMIT message(QString("    %1\n").arg(outputFilepath));

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
        options.add(PromelaOptions::realGeneratorDelta, m_delta.value_or(""));
    }

    options.add(IvOptions::configFilepath, shared::interfaceCustomAttributesFilePath());

    options.add(PromelaOptions::outputFilepath, outputFilepath);

    // when option PromelaOptions::asn1ValueGeneration is disabled
    // then option IvOptions::inputFilepath is not required
    return convertModel({ ModelType::Asn1 }, ModelType::PromelaData, {}, std::move(options));
}

bool TmcConverter::convertModel(const std::set<conversion::ModelType> &sourceModelTypes,
        conversion::ModelType targetModelType, const std::set<conversion::ModelType> &auxilaryModelTypes,
        conversion::Options options)
{
    try {
        Converter converter(m_registry, std::move(options));
        converter.convert(sourceModelTypes, targetModelType, auxilaryModelTypes);
        return true;
    } catch (const ImportException &ex) {
        const auto errorMessage = QString("Import failure: %1\n").arg(ex.errorMessage());
        Q_EMIT message(errorMessage);
        Q_EMIT conversionFinished(false);
    } catch (const TranslationException &ex) {
        const auto errorMessage = QString("Translation failure: %1\n").arg(ex.errorMessage());
        Q_EMIT message(errorMessage);
        Q_EMIT conversionFinished(false);
    } catch (const ExportException &ex) {
        const auto errorMessage = QString("Export failure: %1\n").arg(ex.errorMessage());
        Q_EMIT message(errorMessage);
        Q_EMIT conversionFinished(false);
    } catch (const ConversionException &ex) {
        const auto errorMessage = QString("Conversion failure: %1\n").arg(ex.errorMessage());
        Q_EMIT message(errorMessage);
        Q_EMIT conversionFinished(false);
    }
    return false;
}

void TmcConverter::integrateObserver(const ObserverInfo &info, QStringList &observerNames, QStringList &asn1Files,
        std::map<QString, ProcessMetadata> &allSdlFiles)
{
    const auto process = QFileInfo(info.path());
    const auto processName = process.baseName();
    const auto directory = process.absoluteDir();
    const auto datamodel =
            QFileInfo(directory.absolutePath() + QDir::separator() + processName.toLower() + "_datamodel.asn");

    ProcessMetadata meta(Escaper::escapePromelaName(processName), std::nullopt, process, datamodel, QList<QFileInfo>());

    if (m_conversionFinishedConnection) {
        disconnect(m_conversionFinishedConnection);
    }
    m_conversionFinishedConnection = connect(
            m_sdlToPromelaConverter, SIGNAL(conversionFinished(bool)), this, SLOT(observerConversionFinished(bool)));

    const auto promelaFilename = Escaper::escapePromelaIV(processName) + ".pml";
    const auto infoPath = outputFilepath(Escaper::escapePromelaIV(processName) + ".info");
    observerNames.append(Escaper::escapePromelaIV(processName));
    allSdlFiles.emplace(processName, meta);
    asn1Files.append(datamodel.absoluteFilePath());

    m_sdlToPromelaConverter->convertObserverSdl(meta, outputFilepath(promelaFilename), infoPath);
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

    if (m_conversionFinishedConnection) {
        disconnect(m_conversionFinishedConnection);
    }
    m_conversionFinishedConnection = connect(m_sdlToPromelaConverter, SIGNAL(conversionFinished(bool)), this,
            SLOT(stopConditionConversionFinished(bool)));
    if (!m_sdlToPromelaConverter->convertStopConditions(inputFiles, output, allSdlFiles, includeObservers)) {
        Q_EMIT conversionFinished(false);
        return false;
    }
    return true;
}

bool TmcConverter::convertInterfaceview(const QString &inputFilepath, const QString &outputFilepath,
        const QList<QString> &asn1FilepathList, const QStringList &modelFunctions,
        const QStringList &environmentFunctions)
{
    Q_EMIT message(QString("Converting InterfaceView %1 to %2\n").arg(inputFilepath).arg(outputFilepath));

    Options options;

    if (m_isRealTypeEnabled) {
        options.add(PromelaOptions::enhancedSpinSupport);
        options.add(PromelaOptions::realGeneratorDelta, m_delta.value_or(""));
    }
    if (m_isMulticastEnabled) {
        options.add(PromelaOptions::supportMulticast);
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

void TmcConverter::generateObserverDatamodel(QProcess &process, const QString &sdlFileName)
{
    QStringList arguments;
    arguments << "--toAda" << sdlFileName;

    Q_EMIT message(QString("Executing: %1 with args: %2\n").arg(m_opengeodeCommand).arg(arguments.join(", ")));

    process.start(m_opengeodeCommand, arguments);
}

std::unique_ptr<IVModel> TmcConverter::readInterfaceView(const QString &filepath)
{
    ivm::IVXMLReader reader;

    const bool ok = reader.readFile(filepath);
    if (ok) {
        auto model = std::make_unique<IVModel>(m_dynPropConfig);
        model->initFromObjects(reader.parsedObjects(), reader.externalAttributes());
        return model;
    } else {
        return {};
    }

    return {};
}

void TmcConverter::saveOptimizedInterfaceView(const IVModel *ivModel, const QString outputFilePath)
{
    IVXMLWriter exporter;
    exporter.exportObjectsSilently(ivModel->objects().values(), outputFilePath);
}

void TmcConverter::findFunctionsToConvert(const IVModel &model, QStringList &sdlFunctions,
        std::map<QString, ProcessMetadata> &sdlProcesses, QStringList &envFunctions)
{
    QStringList functionNames;

    QVector<IVFunctionType *> ivFunctionTypeList = model.allObjectsByType<IVFunctionType>();

    QVector<IVFunction *> ivFunctionList = model.allObjectsByType<IVFunction>();
    for (IVFunction *ivFunction : ivFunctionList) {
        if (isSdlFunction(ivFunction)) {
            const QString ivFunctionName = ivFunction->property("name").toString();
            sdlFunctions.append(ivFunctionName);

            const QFileInfo systemStructure = sdlSystemStructureLocation(ivFunctionName);
            QList<QFileInfo> contextLocations;
            contextLocations.append(sdlFunctionContextLocation(ivFunctionName));

            if (ivFunction->inheritsFunctionType()) {
                const QString ivFunctionTypeName = ivFunction->instanceOf()->property("name").toString();
                contextLocations.append(sdlFunctionContextLocation(ivFunctionTypeName));
                const QFileInfo sdlProcess = sdlImplementationLocation(ivFunctionTypeName);
                const QFileInfo functionDatamodel = sdlFunctionDatamodelLocation(ivFunctionName, ivFunctionTypeName);

                sdlProcesses.emplace(ivFunctionName,
                        ProcessMetadata(ivFunctionName, systemStructure, sdlProcess, functionDatamodel,
                                std::move(contextLocations)));

                m_numberOfProctypes += static_cast<size_t>(ivFunction->pis().size());
            } else {
                const QFileInfo sdlProcess = sdlImplementationLocation(ivFunctionName);
                const QFileInfo functionDatamodel = sdlFunctionDatamodelLocation(ivFunctionName);
                sdlProcesses.emplace(ivFunctionName,
                        ProcessMetadata(ivFunctionName, systemStructure, sdlProcess, functionDatamodel,
                                std::move(contextLocations)));
                m_numberOfProctypes += static_cast<size_t>(ivFunction->pis().size());
            }

        } else {
            m_numberOfProctypes += static_cast<size_t>(ivFunction->ris().size());
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
    // Based on required and provided interfaces of environment functions,
    // the envDataTypes is filled with type names for which the values will be generated by environment.
    //
    // For required sporadic and sync interfaces, the env shall generate values for input parameters.
    // For provided sync interfaces, the env shall generate values for output parameters.
    // NOTE:
    // Sporadic interface has at most one input parameter.
    // Sync interfaces have both input and output parameters.
    for (const QString &functionName : envFunctions) {
        IVFunction *function = model.getFunction(functionName, Qt::CaseSensitivity::CaseInsensitive);

        QVector<IVInterface *> requiredInterfaces = function->ris();
        for (const IVInterface *interface : requiredInterfaces) {
            QVector<InterfaceParameter> parameters = interface->params();
            for (const auto &param : parameters) {
                if (param.direction() == InterfaceParameter::Direction::IN) {
                    envDataTypes.append(param.paramTypeName());
                }
            }
        }

        QVector<IVInterface *> providedInterfaces = function->pis();
        for (const IVInterface *interface : providedInterfaces) {
            if (interface->kind() == IVInterface::OperationKind::Protected
                    || interface->kind() == IVInterface::OperationKind::Unprotected) {
                QVector<InterfaceParameter> parameters = interface->params();
                for (const auto &param : parameters) {
                    if (param.direction() == InterfaceParameter::Direction::OUT) {
                        envDataTypes.append(param.paramTypeName());
                    }
                }
            }
        }
    }
}

bool TmcConverter::createEnvGenerationInlines(const QFileInfo &inputDataView, const QString &ivFilepath,
        const QFileInfo &outputFilepath, const QStringList &envDatatypes)
{
    Q_EMIT message(
            QString("Converting ASN.1 environment value generators using %1\n").arg(inputDataView.absoluteFilePath()));
    Options options;

    options.add(Asn1Options::inputFilepath, inputDataView.absoluteFilePath());

    for (const auto &subtypesFilepath : m_subtypesFilepaths) {
        QFileInfo subtypesFileInfo(subtypesFilepath);

        Q_EMIT message(QString("Converting ASN.1 subtypes value generators using %1\n")
                               .arg(subtypesFileInfo.absoluteFilePath()));

        options.add(Asn1Options::inputFilepath, subtypesFileInfo.absoluteFilePath());
        options.add(PromelaOptions::subtypesFilepath, subtypesFileInfo.absoluteFilePath());
    }

    options.add(IvOptions::configFilepath, shared::interfaceCustomAttributesFilePath());

    options.add(PromelaOptions::outputFilepath, outputFilepath.absoluteFilePath());

    // when option PromelaOptions::asn1ValueGeneration is enabled
    // then option IvOptions::inputFilepath is required
    options.add(PromelaOptions::asn1ValueGeneration);
    options.add(IvOptions::inputFilepath, ivFilepath);
    options.add(PromelaOptions::realGeneratorDelta, m_delta.value_or(""));

    for (const QString &datatype : envDatatypes) {
        options.add(PromelaOptions::asn1ValueGenerationForType, datatype);
    }

    return convertModel({ ModelType::Asn1, ModelType::InterfaceView }, ModelType::PromelaData, {}, std::move(options));
}

QFileInfo TmcConverter::workDirectory() const
{
    return QFileInfo(m_ivBaseDirectory.absolutePath() + QDir::separator() + "work");
}

QFileInfo TmcConverter::simuDataViewLocation() const
{
    return QFileInfo(workDirectory().absoluteFilePath() + QDir::separator() + "simulation" + QDir::separator()
            + "observers" + QDir::separator() + "observer.asn");
}

QFileInfo TmcConverter::dataviewUniqLocation() const
{
    return QFileInfo(workDirectory().absoluteFilePath() + QDir::separator() + "dataview" + QDir::separator()
            + "dataview-uniq.asn");
}

QFileInfo TmcConverter::sdlImplementationBaseDirectory(const QString &functionName) const
{
    return QFileInfo(workDirectory().absoluteFilePath() + QDir::separator() + functionName.toLower() + QDir::separator()
            + "SDL");
}

QFileInfo TmcConverter::sdlImplementationLocation(const QString &functionName) const
{
    return QFileInfo(sdlImplementationBaseDirectory(functionName).absoluteFilePath() + QDir::separator() + "src"
            + QDir::separator() + functionName.toLower() + ".pr");
}

QFileInfo TmcConverter::sdlSystemStructureLocation(const QString &functionName) const
{
    return QFileInfo(sdlImplementationBaseDirectory(functionName).absoluteFilePath() + QDir::separator() + "src"
            + QDir::separator() + "system_structure.pr");
}

QFileInfo TmcConverter::sdlFunctionDatamodelLocation(const QString &functionName) const
{
    return QFileInfo(sdlImplementationBaseDirectory(functionName).absoluteFilePath() + QDir::separator() + "code"
            + QDir::separator() + functionName.toLower() + "_datamodel.asn");
}

QFileInfo TmcConverter::sdlFunctionDatamodelLocation(const QString &functionName, const QString &functionTypeName) const
{
    return QFileInfo(sdlImplementationBaseDirectory(functionName).absoluteFilePath() + QDir::separator() + "code"
            + QDir::separator() + functionTypeName.toLower() + "_datamodel.asn");
}

QFileInfo TmcConverter::sdlFunctionContextLocation(const QString &functionName) const
{
    QFileInfo contextLocation = QFileInfo(sdlImplementationBaseDirectory(functionName).absoluteFilePath()
            + QDir::separator() + "Context-" + functionName.toLower() + ".asn");
    if (contextLocation.exists()) {
        return contextLocation;
    } else {
        return QFileInfo();
    }
}

void TmcConverter::convertNextFunction()
{
    if (m_functionsToConvert.empty()) {
        convertMscPropertiesToObservers();
        return;
    }

    const ProcessMetadata processMetadata = m_functionsToConvert.front();
    m_functionsToConvert.pop_front();
    const QFileInfo outputFile = outputFilepath(processMetadata.getName().toLower() + ".pml");

    if (m_conversionFinishedConnection) {
        disconnect(m_conversionFinishedConnection);
    }
    m_conversionFinishedConnection = connect(
            m_sdlToPromelaConverter, SIGNAL(conversionFinished(bool)), this, SLOT(functionConversionFinished(bool)));
    if (!m_sdlToPromelaConverter->convertSdl(processMetadata, outputFile)) {
        Q_EMIT conversionFinished(false);
    }
    return;
}

void TmcConverter::convertMscPropertiesToObservers()
{
    while (!m_mscObserversToConvert.empty()) {
        const QString mscFilePath = m_mscObserversToConvert.front();
        m_mscObserversToConvert.pop_front();

        QFileInfo mscFile(mscFilePath);

        if (!mscFile.exists()) {
            Q_EMIT message(QString("MSC file %1 doesn't exist\n").arg(mscFilePath));
            Q_EMIT conversionFinished(false);
            return;
        }

        const auto outputDir = mscFile.dir();
        const auto &outputPath = outputDir.path() + QDir::separator();

        Options options;

        options.add(MscOptions::inputFilepath, mscFilePath);
        options.add(Asn1Options::inputFilepath, simuDataViewLocation().absoluteFilePath());
        options.add(IvOptions::inputFilepath, m_outputOptimizedIvFileName);
        options.add(IvOptions::configFilepath, shared::interfaceCustomAttributesFilePath());
        options.add(MscOptions::simuDataViewFilepath, simuDataViewLocation().absoluteFilePath());
        options.add(SdlOptions::filepathPrefix, outputPath);

        Q_EMIT message(QString("Converting MSC file %1 to an SDL observer\n").arg(mscFilePath));

        if (!convertModel({ ModelType::Msc, ModelType::Asn1, ModelType::InterfaceView }, ModelType::Sdl, {},
                    std::move(options))) {
            return;
        }

        QStringList sdlFiles = QDir(outputPath).entryList({ "*.pr" });
        for (const auto &sdlFileName : sdlFiles) {
            const auto sdlFilePath = outputPath + sdlFileName;
            m_observerInfos.emplace_back(ObserverInfo(sdlFilePath, 1));
        }
    }

    std::copy(m_observerInfos.begin(), m_observerInfos.end(), std::back_inserter(m_observersToConvert));
    prepareNextObserverDatamodel();
}

void TmcConverter::prepareNextObserverDatamodel()
{
    if (m_observersToConvert.empty()) {
        generateCDataview();
        return;
    }

    const ObserverInfo info = m_observersToConvert.front();

    QFileInfo observerFile(info.path());
    m_process->setWorkingDirectory(observerFile.absolutePath());

    m_timer->setSingleShot(true);
    m_timer->start(m_commandStartTimeout);
    generateObserverDatamodel(*m_process, observerFile.fileName());
}

void TmcConverter::convertNextObserver()
{
    const ObserverInfo info = m_observersToConvert.front();

    integrateObserver(info, m_observerNames, m_asn1Files, m_allSdlFiles);
}

void TmcConverter::attachNextObserver()
{
    const ObserverInfo info = m_observersToConvert.front();
    m_observersToConvert.pop_front();

    const auto process = QFileInfo(info.path());
    const auto processName = process.baseName();

    const auto infoPath = outputFilepath(Escaper::escapePromelaIV(processName) + ".info");

    QFile infoFile(infoPath.absoluteFilePath());
    if (infoFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&infoFile);
        while (!in.atEnd()) {
            m_observerAttachmentInfos.append(in.readLine() + ":p" + QString::number(info.priority()));
        }
        infoFile.close();
    } else {
        const auto errorMessage = QString("Could not open observer info file %1\n").arg(infoPath.absoluteFilePath());
        Q_EMIT message(errorMessage);
        Q_EMIT conversionFinished(false);
        return;
    }

    prepareNextObserverDatamodel();
}

void TmcConverter::generateCDataview()
{
    Asn1Acn::Asn1Scc asn1scc;
    QString asn1sccCommand = asn1scc.getAsn1SccCommand();

    QStringList arguments = QStringList() << "-uPER"
                                          << "-typePrefix"
                                          << "asn1Scc"
                                          << "-renamePolicy"
                                          << "3"
                                          << "-o" << m_outputDirectory.absolutePath() << "-c";
    arguments.append(dataviewUniqLocation().absoluteFilePath());

    Q_EMIT message(QString("Converting asn %1\n").arg(dataviewUniqLocation().absolutePath()));

    disconnect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(asn1sccProcessFinished(int, QProcess::ExitStatus)));
    disconnect(m_process, SIGNAL(started()), this, SLOT(processStarted()));
    connect(m_process, SIGNAL(started()), this, SLOT(asn1sccProcessStarted()));

    m_timer->setSingleShot(true);
    m_timer->start(m_commandStartTimeout);
    m_process->start(asn1sccCommand, arguments);
}

bool TmcConverter::generateMessageSizes(QFileInfo input, QFileInfo output)
{
    QFile inputFile(input.absoluteFilePath());
    if (!inputFile.open(QIODevice::ReadOnly)) {
        const auto errorMessage = QString("Could not open file %1\n").arg(input.absoluteFilePath());
        Q_EMIT message(errorMessage);
        Q_EMIT conversionFinished(false);
        return false;
    }
    QFile outputFile(output.absoluteFilePath());
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        const auto errorMessage = QString("Could not open file %1\n").arg(output.absoluteFilePath());
        Q_EMIT message(errorMessage);
        Q_EMIT conversionFinished(false);
        return false;
    }

    QTextStream in(&inputFile);
    QTextStream out(&outputFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains("_REQUIRED_BYTES_FOR_ENCODING")) {
            out << line << "\n";
        }
    }

    outputFile.close();
    inputFile.close();

    return true;
}

void TmcConverter::finishConversion()
{
    const QFileInfo simuDataView = simuDataViewLocation();

    const QFileInfo outputDataview = outputFilepath("dataview.pml");
    if (!convertDataview(m_asn1Files, outputDataview.absoluteFilePath())) {
        return;
    }

    const QFileInfo outputEnv = outputFilepath("env_inlines.pml");
    if (!createEnvGenerationInlines(simuDataView, m_outputOptimizedIvFileName, outputEnv, m_environmentDatatypes)) {
        return;
    }

    const QFileInfo outputSystemFile = outputFilepath("system.pml");
    if (!convertInterfaceview(m_outputOptimizedIvFileName, outputSystemFile.absoluteFilePath(), m_asn1Files,
                m_modelFunctions, m_finalEnvironmentFunctions)) {
        return;
    }

    const QFileInfo outputMessageSizesFile = outputFilepath("message_sizes.pml");
    const QFileInfo inputHeaderFile = outputFilepath("dataview-uniq.h");
    if (!generateMessageSizes(inputHeaderFile, outputMessageSizesFile)) {
        return;
    }

    convertStopConditions(m_allSdlFiles);
}

QFileInfo TmcConverter::outputFilepath(const QString &name)
{
    return QFileInfo(m_outputDirectory.absolutePath() + QDir::separator() + name);
}

void TmcConverter::processStderrReady()
{
    if (m_process != nullptr) {
        QByteArray buffer = m_process->readAllStandardError();
        QString text = QString(buffer);
        Q_EMIT message(text);
    }
}

void TmcConverter::processStdoutReady()
{
    if (m_process != nullptr) {
        QByteArray buffer = m_process->readAllStandardOutput();
        QString text = QString(buffer);
        Q_EMIT message(text);
    }
}

void TmcConverter::processStarted()
{
    m_timer->stop();
    m_timer->setSingleShot(true);
    m_timer->start(m_commandTimeout);
}

void TmcConverter::processError(QProcess::ProcessError error)
{
    m_process->terminate();
    m_timer->stop();
    switch (error) {
    case QProcess::ProcessError::FailedToStart:
        Q_EMIT message("External process failed to start.");
        Q_EMIT conversionFinished(false);
        break;
    case QProcess::ProcessError::Crashed:
        Q_EMIT message("External process crashed.");
        Q_EMIT conversionFinished(false);
        break;
    case QProcess::ProcessError::Timedout:
        Q_EMIT message("External process timeout.");
        Q_EMIT conversionFinished(false);
        break;
    case QProcess::ProcessError::WriteError:
        Q_EMIT message("External process write error.");
        Q_EMIT conversionFinished(false);
        break;
    case QProcess::ProcessError::ReadError:
        Q_EMIT message("External process read error.");
        Q_EMIT conversionFinished(false);
        break;
    case QProcess::ProcessError::UnknownError:
        Q_EMIT message("External process unknown error.");
        Q_EMIT conversionFinished(false);
        break;
    }
}

void TmcConverter::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    m_timer->stop();
    if (exitCode != EXIT_SUCCESS) {
        Q_EMIT conversionFinished(false);
        return;
    }
    convertNextObserver();
}

void TmcConverter::timeout()
{
    m_process->terminate();
    Q_EMIT message(QString("Timeout.\n"));
    Q_EMIT conversionFinished(false);
}

void TmcConverter::functionConversionFinished(bool success)
{
    if (success) {
        convertNextFunction();
    } else {
        Q_EMIT conversionFinished(false);
    }
}

void TmcConverter::observerConversionFinished(bool success)
{
    if (success) {
        attachNextObserver();
    } else {
        Q_EMIT conversionFinished(false);
    }
}

void TmcConverter::stopConditionConversionFinished(bool success)
{
    Q_EMIT conversionFinished(success);
}

void TmcConverter::asn1sccProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    m_timer->stop();
    if (exitCode != EXIT_SUCCESS) {
        Q_EMIT conversionFinished(false);
        return;
    }

    QTimer::singleShot(0, this, SLOT(finishConversion()));
}

void TmcConverter::asn1sccProcessStarted()
{
    Asn1Acn::Asn1Scc asn1scc;
    int asn1sccTimeout = asn1scc.getCompilerTimeout();
    m_timer->stop();
    m_timer->setSingleShot(true);
    m_timer->start(asn1sccTimeout);
}
}
