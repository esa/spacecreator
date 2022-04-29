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
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/converter/converter.h>
#include <conversion/converter/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvRegistrar/registrar.h>
#include <conversion/promela/PromelaRegistrar/registrar.h>
#include <iostream>
#include <ivcore/ivfunction.h>
#include <ivcore/ivxmlreader.h>
#include <promela/PromelaOptions/options.h>
#include <tmc/SdlToPromelaConverter/converter.h>

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
using conversion::promela::PromelaOptions;
using conversion::promela::PromelaRegistrar;
using conversion::translator::TranslationException;
using ivm::IVFunction;
using ivm::IVFunctionType;
using ivm::IVInterface;
using ivm::IVModel;
using ivm::IVObject;
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

bool TmcConverter::attachObserver(const QString &attachmentSpecification)
{
    m_observerAttachments.append(attachmentSpecification);
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

auto TmcConverter::integrateObserver(QString observerSpecification, QStringList &observerNames, QStringList &asn1Files,
        std::map<QString, ProcessMetadata> &allSdlFiles, QStringList &attachmentInfos)
{
    std::cout << "spec -> " << observerSpecification.toStdString() << "\n";
    const auto separator = ":";
    const auto elements = observerSpecification.split(separator, QString::KeepEmptyParts);
    std::cout << "size -> " << elements.size() << "\n";
    for (const auto &element : elements) {
        std::cout << "element -> " << element.toStdString() << "\n";
    }
    const auto processPath = elements[0];
    bool ok = true;
    const auto priority = elements.size() > 1 ? elements[1].toInt(&ok) : 1;
    if (!ok) {
        throw "";
    }

    const auto process = QFileInfo(processPath);
    const auto processName = process.baseName();
    const auto directory = process.absoluteDir();
    const auto structure = QFileInfo(directory.absolutePath() + QDir::separator() + "system_structure.pr");
    const auto datamodel =
            QFileInfo(directory.absolutePath() + QDir::separator() + processName.toLower() + "_datamodel.asn");
    // const auto dataview = QFileInfo(directory.absolutePath() + QDir::separator() + "dataview-uniq.asn");

    ProcessMetadata meta(Escaper::escapePromelaName(processName), structure, process, datamodel, QList<QFileInfo>());
    SdlToPromelaConverter sdl2Promela;

    const auto promelaFilename = Escaper::escapePromelaName(processName) + ".pml";
    const auto infoPath = outputFilepath(Escaper::escapePromelaName(processName) + ".info");
    observerNames.append(Escaper::escapePromelaName(processName));
    sdl2Promela.convertObserverSdl(meta, outputFilepath(promelaFilename), infoPath);
    QFile infoFile(infoPath.absoluteFilePath());
    if (infoFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&infoFile);
        while (!in.atEnd()) {
            attachmentInfos.append(in.readLine() + ":" + QString::number(priority));
            std::cout << "Appended -> " << attachmentInfos.last().toStdString() << std::endl;
        }
        infoFile.close();
    } else {
        throw "";
    }
    // asn1Files.append(dataview.absoluteFilePath());

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
        /*if (!uniqueAsn1Files.contains(processMetadata.getDatamodel().fileName())) {
            uniqueAsn1Files.insert(
                    processMetadata.getDatamodel().fileName(), processMetadata.getDatamodel().absoluteFilePath());
        }*/
        for (const QFileInfo &fileInfo : allSdlFiles.at(ivFunction).getContext()) {
            if (fileInfo.exists()) {
                uniqueAsn1Files.insert(fileInfo.fileName(), fileInfo.absoluteFilePath());
            }
        }
        const QFileInfo outputFile = outputFilepath(processMetadata.getName().toLower() + ".pml");

        SdlToPromelaConverter sdl2Promela;

        if (!sdl2Promela.convertSdl(processMetadata, outputFile)) {
            return false;
        }
    }

    QStringList asn1Files;

    for (auto &attachment : m_observerAttachments) {
        integrateObserver(attachment, m_observerNames, asn1Files, allSdlFiles, m_observerAttachmentInfos);
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

    const QFileInfo outputDataview = outputFilepath("dataview.pml");

    convertDataview(asn1Files, outputDataview.absoluteFilePath());

    const QFileInfo outputEnv = outputFilepath("env_inlines.pml");

    createEnvGenerationInlines(simuDataView, outputEnv, environmentDatatypes);

    const QFileInfo outputSystemFile = outputFilepath("system.pml");

    convertInterfaceview(
            ivFileInfo.absoluteFilePath(), outputSystemFile.absoluteFilePath(), modelFunctions, environmentFunctions);

    return true;
}

bool TmcConverter::convertStopConditions(const std::map<QString, ProcessMetadata> &allSdlFiles)
{
    const QDir outputDirectory = QDir(m_outputDirectory);

    for (const QString &filepath : m_stopConditionsFiles) {
        const QFileInfo input(filepath);
        const QString base = input.baseName();
        const QFileInfo output = outputFilepath(base.toLower() + ".pml");

        SdlToPromelaConverter sdl2Promela;
        if (!sdl2Promela.convertStopCondition(input, output, allSdlFiles)) {
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
    options.add(IvOptions::configFilepath, shared::interfaceCustomAttributesFilePath());
    options.add(PromelaOptions::outputFilepath, outputFilepath);

    for (const QString &function : modelFunctions) {
        options.add(PromelaOptions::modelFunctionName, function);
    }

    for (const QString &function : environmentFunctions) {
        options.add(PromelaOptions::environmentFunctionName, function);
    }

    for (const QString &observer : m_observerNames) {
        options.add(PromelaOptions::observerFunctionName, observer);
    }

    for (auto &info : m_observerAttachmentInfos) {
        std::cout << "Attaching observer info -> " << info.toStdString() << std::endl;
        options.add(PromelaOptions::observerAttachment, info);
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

QFileInfo TmcConverter::simuDataViewLocation() const
{
    return workDirectory().absoluteFilePath() + QDir::separator() + "simulation" + QDir::separator() + "observer.asn";
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
