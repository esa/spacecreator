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

#include "simulinkimporterplugin.h"

#include <QAction>
#include <QFileDialog>
#include <QInputDialog>
#include <QFile>
#include <QProcess>
#include <QMessageBox>
#include <projectexplorer.h>
#include <projectexplorer/projecttree.h>
#include <projectexplorer/session.h>
#include <projectnodes.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <messagemanager.h>
#include <messagestrings.h>
#include <modeltype.h>
#include <modelloader.h>
#include <entityattribute.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>
#include <spacecreatorplugin/iv/iveditordocument.h>

#include <conversion/asn1/Asn1Exporter/exporter.h>
#include <conversion/asn1/Asn1Importer/importer.h>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/asn1/Asn1Registrar/registrar.h>

#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvRegistrar/registrar.h>
#include <conversion/iv/IvXmlImporter/importer.h>
#include <conversion/iv/SimulinkToIvTranslator/options.h>

#include <conversion/common/escaper/escaper.h>
#include <conversion/simulink/SimulinkRegistrar/registrar.h>
#include <conversion/converter/exceptions.h>
#include <conversion/converter/converter.h>

#include <simulink/SimulinkOptions/options.h>
#include <simulink/matlabstandarddatatypes.h>

#include <shared/common.h>
#include <ivcore/ivfunction.h>
#include <libiveditor/interfacedocument.h>

using namespace Core;
using conversion::Escaper;
using conversion::Converter;
using conversion::ModelType;
using conversion::asn1::Asn1Registrar;
using conversion::iv::IvRegistrar;
using conversion::simulink::SimulinkRegistrar;
using conversion::RegistrationFailedException;
using conversion::simulink::MatLabStandardDataTypes;

namespace spctr {

SimulinkImporterPlugin::SimulinkImporterPlugin()
{
    // Do not remove
}

SimulinkImporterPlugin::~SimulinkImporterPlugin()
{
    // Do not remove
}

auto SimulinkImporterPlugin::initialize(const QStringList &arguments, QString *errorString) -> bool
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    initializeRegistry();
    configureSimulinkMenu();

    return true;
}

auto SimulinkImporterPlugin::extensionsInitialized() -> void
{
    // Do not remove
}

auto SimulinkImporterPlugin::aboutToShutdown() -> ExtensionSystem::IPlugin::ShutdownFlag
{
    return SynchronousShutdown;
}

auto SimulinkImporterPlugin::initializeRegistry() -> void
{
    Asn1Registrar asn1Registrar;
    if(!asn1Registrar.registerCapabilities(m_registry)) {
        throw RegistrationFailedException(ModelType::Asn1);
    }

    IvRegistrar ivRegistrar;
    if(!ivRegistrar.registerCapabilities(m_registry)) {
        throw RegistrationFailedException(ModelType::InterfaceView);
    }

    SimulinkRegistrar simulinkRegistrar;
    if(!simulinkRegistrar.registerCapabilities(m_registry)) {
        throw RegistrationFailedException(ModelType::Simulink);
    }
}

auto SimulinkImporterPlugin::configureSimulinkMenu() -> void
{
    Context context(Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    ActionContainer *const simulinkActionContainer = createActionContainerInTools(tr("&Simulink"));

    QAction *const importSlxAction = new QAction(tr("Import model from SLX"), this);
    connect(importSlxAction, &QAction::triggered, [=]() { this->importSlx(); });
    Command *const importSlxCommand = ActionManager::registerAction(importSlxAction, Constants::SLX_IMPORT_ID, context);
    simulinkActionContainer->addAction(importSlxCommand);

    QAction *const importXmlAction = new QAction(tr("Import interface specification from XML"), this);
    connect(importXmlAction, &QAction::triggered, [=]() { this->importXml(); });
    Command *const importXmlCommand = ActionManager::registerAction(importXmlAction, Constants::XML_IMPORT_ID, context);
    simulinkActionContainer->addAction(importXmlCommand);
}

auto SimulinkImporterPlugin::createActionContainerInTools(const QString &title) -> ActionContainer *
{
    ActionContainer *const container = ActionManager::createMenu(Constants::M_TOOLS_SIMULINK);
    m_simulinkMenu = container->menu();
    m_simulinkMenu->setTitle(title);
    m_simulinkMenu->setEnabled(false);

    ActionContainer *const tools = ActionManager::actionContainer(Core::Constants::M_TOOLS);
    tools->addMenu(container);

    connect(ProjectExplorer::SessionManager::instance(),
        &ProjectExplorer::SessionManager::startupProjectChanged,
        this,
        &SimulinkImporterPlugin::onActiveProjectChanged);

    return container;
}

void SimulinkImporterPlugin::onActiveProjectChanged(ProjectExplorer::Project *project)
{
    m_currentProject = project;
    if (m_currentProject != nullptr) {
        m_currentProjectDirectoryPath = project->projectDirectory().toString();
    } else {
        m_currentProjectDirectoryPath.clear();
    }

    m_simulinkMenu->setEnabled(project != nullptr);
}

auto SimulinkImporterPlugin::importSlx() -> void
{
    std::optional<QString> inputFilePath;
    std::optional<QString> functionBlockName;
    std::optional<QFileInfo> workspaceFileInfo;

    inputFilePath = askAboutAndCheckFilePath(tr("Select file to import model from ..."), tr("*.slx"));

    if(!inputFilePath.has_value()) {
        return;
    }

    printPluginSelfIntroductionInGeneralMessages();

    functionBlockName = askAboutAndCheckFunctionBlockName();

    if(!functionBlockName.has_value()) {
        return;
    }

    workspaceFileInfo = searchAndCheckMatLabModelWorkspaceFile(*inputFilePath);

    if(!workspaceFileInfo.has_value()) {
        return;
    }

    printInfoAboutInputs(*inputFilePath, *functionBlockName, workspaceFileInfo->absoluteFilePath());

    moveToProjectDirectoryAndPrepareTemporaryWorkingDirectory(m_matlabTemporaryWorkingDirectory);

    const QStringList matlabCallArguments = generateMatLabCallArguments(*workspaceFileInfo, *inputFilePath);

    printMatLabCommand(matlabCallArguments);

    if(QProcess().execute(m_matlabAppName, matlabCallArguments) != 0) {
        printErrorInGeneralMessages(GenMsg::matlabCommandHasFailed);

        removeMatLabTemporaryWorkingDirectory();
        return;
    }

    printInfoInGeneralMessages(GenMsg::matlabCommandHasExecuted);

    if(!importXmlFileAndRemoveTemporaries(generateExportedXmlFilePath(*inputFilePath), *functionBlockName)) {
        removeMatLabTemporaryWorkingDirectory();
        return;
    }

    copyInputSlxFileToWorkDirectory(*inputFilePath, *functionBlockName);

    removeMatLabTemporaryWorkingDirectory();
}

auto SimulinkImporterPlugin::importXml() -> void
{
    std::optional<QString> inputFilePath;
    std::optional<QString> functionBlockName;

    inputFilePath = askAboutAndCheckFilePath(tr("Select file to import interface specification from ..."), tr("*.xml"));

    if(!inputFilePath.has_value()) {
        return;
    }

    printPluginSelfIntroductionInGeneralMessages();

    functionBlockName = askAboutAndCheckFunctionBlockName();

    if(!functionBlockName.has_value()) {
        return;
    }

    printInfoAboutInputs(*inputFilePath, *functionBlockName);

    importXmlFileAndRemoveTemporaries(*inputFilePath, *functionBlockName);
}

auto SimulinkImporterPlugin::askAboutAndCheckFilePath(const QString &caption, const QString &filter) -> std::optional<QString>
{
    const QString temporaryFilePath = QFileDialog::getOpenFileName(nullptr, caption, m_currentProjectDirectoryPath, filter);

    if(temporaryFilePath.isEmpty()) {
        return std::nullopt;
    }

    return temporaryFilePath;
}

auto SimulinkImporterPlugin::askAboutAndCheckFunctionBlockName() -> std::optional<QString>
{
    bool isOk = false;
    const QString temporaryFunctionBlockName = QInputDialog::getText(nullptr, tr("Choose function block name"),
                                         tr("Function Block Name : "), QLineEdit::Normal,
                                         m_defaultFunctionBlockName, &isOk);

    if(!isOk && temporaryFunctionBlockName.isEmpty()) {
        printErrorInGeneralMessages(GenMsg::ivNoFunctionBlockName);
        return std::nullopt;
    }

    return Escaper::escapeIvName(temporaryFunctionBlockName);
}

auto SimulinkImporterPlugin::searchAndCheckMatLabModelWorkspaceFile(const QString& inputFilePath) -> std::optional<QFileInfo>
{
    QFileInfo inputFileInfo(inputFilePath);

    const QDir inputFileDirectory = inputFileInfo.absoluteDir();
    const QFileInfoList foundWorkspaceFiles = inputFileDirectory.entryInfoList(m_workspaceFilesExtensionsFilter, QDir::Files);
    const auto foundWorkspaceFilesAmount = foundWorkspaceFiles.size();

    /*
        if foundWorkspaceFilesAmount == 0 -> is fine, the model does not use any variables from workspace
        if foundWorkspaceFilesAmount == 1 -> is also fine, the model uses variables from one workspace file
        if foundWorkspaceFilesAmount >= 2 -> is an error, it is assumed that the model has at most one workspace file
    */

    if(foundWorkspaceFilesAmount == 1) {
        return foundWorkspaceFiles.front();
    } else if(foundWorkspaceFilesAmount >= 2) {
        printErrorInGeneralMessages(GenMsg::toMuchMatLabModelWorkspaceFiles);
        return std::nullopt;
    }

    return QFileInfo();
}

auto SimulinkImporterPlugin::printInfoAboutInputs(const QString& inputFilePath, const QString& functionBlockName, const QString& inputWorkspaceFilePath) -> void
{
    printInfoInGeneralMessages(GenMsg::inputFileWasChosen.arg(inputFilePath));
    printInfoInGeneralMessages(GenMsg::functionBlockNameWasChosen.arg(functionBlockName));

    if(inputWorkspaceFilePath.isEmpty()) {
        printInfoInGeneralMessages(GenMsg::workspaceFileIsNotUsed);
    } else {
        printInfoInGeneralMessages(GenMsg::workspaceFileWasFound.arg(inputWorkspaceFilePath));
    }
}

auto SimulinkImporterPlugin::generateMatLabCallArguments(QFileInfo &workspaceFileInfo, const QString& inputFilePath) -> QStringList
{
    const QString workspaceLoadFunction = generateWorkspaceLoadCallFunction(workspaceFileInfo);
    const QString tasteExporterFunction = generateTasteExporterCallFunction(inputFilePath);

    QString matlabBatchArgument;

    if(workspaceLoadFunction.isEmpty()) {
        matlabBatchArgument = m_matlabCommandWithoutWorkspaceLoadTemplate
                                .arg(m_matlabTemporaryWorkingDirectory)
                                .arg(tasteExporterFunction);
    } else {
        matlabBatchArgument = m_matlabCommandWithWorkspaceLoadTemplate
                                .arg(m_matlabTemporaryWorkingDirectory)
                                .arg(workspaceLoadFunction)
                                .arg(tasteExporterFunction);
    }

    return {"-batch", matlabBatchArgument};
}

auto SimulinkImporterPlugin::generateWorkspaceLoadCallFunction(QFileInfo &workspaceFileInfo) -> QString
{
    const QString workspaceFilePath = workspaceFileInfo.absoluteFilePath();

    if(!workspaceFilePath.isEmpty()) {
        const QString workspaceFileExtension = workspaceFileInfo.completeSuffix();
        const QString workspaceLoadFunction = getWorkspaceLoadFunctionNameForWorkspaceFileExtension(workspaceFileExtension);

        return m_workspaceLoadCallFunctionTemplate.arg(workspaceLoadFunction).arg(workspaceFilePath);
    }

    return "";
}

auto SimulinkImporterPlugin::generateTasteExporterCallFunction(const QString& inputFilePath) -> QString
{
    return m_tasteExporterCallFunctionTemplate.arg(inputFilePath).arg(m_tasteExporterOutputDirectory);
}

auto SimulinkImporterPlugin::getWorkspaceLoadFunctionNameForWorkspaceFileExtension(const QString &workspaceFileExtension) -> QString
{
    const QString workspaceLoadFunction = m_workspaceLoadFunctionsMap[workspaceFileExtension];

    return workspaceLoadFunction;
}

auto SimulinkImporterPlugin::copyInputSlxFileToWorkDirectory(const QString &inputFilePath, const QString &functionBlockName) -> void
{
    const QString escapedFunctionBlockName = functionBlockName.toLower();
    const QString functionBlockInWorkDirectoryPath = m_functionBlockInWorkDirectoryPathTemplate
                                                    .arg(m_currentProjectDirectoryPath)
                                                    .arg(escapedFunctionBlockName)
                                                    .arg(m_functionBlockDefaultImplementation);

    const QString inputFileName = QFileInfo(inputFilePath).fileName();
    const QString slxFilePathInWorkDirectory = QString("%1/%2").arg(functionBlockInWorkDirectoryPath).arg(inputFileName);

    const bool isFunctionBlockInWorkDirectoryExists = QDir().exists(functionBlockInWorkDirectoryPath);

    if(!isFunctionBlockInWorkDirectoryExists) { 
        QDir().mkpath(functionBlockInWorkDirectoryPath);
    }

    checkIfFileExistsAndRemoveIt(slxFilePathInWorkDirectory);

    QFile(inputFilePath).copy(slxFilePathInWorkDirectory);
}

auto SimulinkImporterPlugin::generateExportedXmlFilePath(const QString& inputFilePath) -> QString
{
    const QFileInfo inputFileInfo(inputFilePath);

    return QString("%1/%2/%3/%4.xml")
            .arg(m_currentProjectDirectoryPath)
            .arg(m_matlabTemporaryWorkingDirectory)
            .arg(m_tasteExporterOutputDirectory)
            .arg(inputFileInfo.baseName());
}

auto SimulinkImporterPlugin::importXmlFileAndRemoveTemporaries(const QString &inputFilePath, const QString &functionBlockName) -> bool
{
    const QString ivConfig = shared::interfaceCustomAttributesFilePath();

    moveToProjectDirectoryAndPrepareTemporaryWorkingDirectory(m_convertersTemporaryWorkingDirectory);
    QDir::setCurrent(m_convertersTemporaryWorkingDirectory);

    const QStringList generatedAsn1FileNames = convertXmlFileToAsn1(inputFilePath);

    if(generatedAsn1FileNames.isEmpty()) {
        return false;
    }

    if(!convertXmlFileToIv(inputFilePath, functionBlockName, ivConfig)) {
        moveToProjectDirectoryAndRemoveConvertersTemporaryWorkingDirectory();
        return false;
    }

    if(!addIvToCurrentProject(ivConfig)) {
        moveToProjectDirectoryAndRemoveConvertersTemporaryWorkingDirectory();
        return false;
    }

    addGeneratedAsn1FilesToCurrentProject(generatedAsn1FileNames);

    moveToProjectDirectoryAndRemoveConvertersTemporaryWorkingDirectory();

    printInfoInGeneralMessages(GenMsg::filesImported);

    return true;
}

auto SimulinkImporterPlugin::convertXmlFileToAsn1(const QString &inputFilePath) -> QStringList
{
    conversion::Options options;
    options.add(conversion::simulink::SimulinkOptions::inputFilepath, inputFilePath);
    options.add(conversion::asn1::Asn1Options::asn1FilepathPrefix, "");
    options.add(conversion::asn1::Asn1Options::acnFilepathPrefix, "");

    try {
        const auto outputModels = convert({ conversion::ModelType::Simulink }, conversion::ModelType::Asn1, {}, options);
        return getGeneratedAsn1FileNamesFromModels(outputModels);
    } catch (std::exception &ex) {
        printErrorInGeneralMessages(ex.what());
        return QStringList();
    }
}

auto SimulinkImporterPlugin::getGeneratedAsn1FileNamesFromModels(const std::vector<std::unique_ptr<conversion::Model>> &models) -> QStringList
{
    auto *const asn1model = conversion::translator::Translator::getModel<Asn1Acn::Asn1Model>(models);
    if(asn1model != nullptr) {
        return conversion::asn1::exporter::Asn1Exporter::getFilenamesForModel(asn1model);
    } else {
        return QStringList();
    }
}

auto SimulinkImporterPlugin::convertXmlFileToIv(const QString &inputFilePath, const QString &functionBlockName, const QString &ivConfig) -> bool
{
    conversion::Options options;
    options.add(conversion::simulink::SimulinkOptions::inputFilepath, inputFilePath);
    options.add(conversion::iv::IvOptions::outputFilepath, m_temporaryIvFileName);
    options.add(conversion::iv::IvOptions::configFilepath, ivConfig);
    options.add(conversion::iv::SimulinkToIvOptions::ivFunctionName, functionBlockName);
    options.add(conversion::iv::SimulinkToIvOptions::ivFunctionImplementation, m_functionBlockDefaultImplementation);

    try {
        const auto outputModels = convert({ conversion::ModelType::Simulink }, conversion::ModelType::InterfaceView, {}, options);
    } catch (std::exception &ex) {
        printErrorInGeneralMessages(ex.what());
        return false;
    }

    return true;
}

auto SimulinkImporterPlugin::convert(const std::set<conversion::ModelType> &srcModelType,
        const conversion::ModelType targetModelType, const std::set<conversion::ModelType> &auxModelTypes,
        conversion::Options options) -> std::vector<std::unique_ptr<conversion::Model>>
{
    Converter converter(m_registry, std::move(options));
    converter.convert(srcModelType, targetModelType, auxModelTypes);

    return converter.extractCache();
}

auto SimulinkImporterPlugin::addIvToCurrentProject(const QString &ivConfig) -> bool
{
    std::unique_ptr<conversion::Model> model;

    try {
        model = plugincommon::ModelLoader::loadIvModel(ivConfig, m_temporaryIvFileName);
    } catch (std::exception &ex) {
        printErrorInGeneralMessages(ex.what());
        return false;
    }

    ivm::IVModel *const temporaryIvModel = dynamic_cast<ivm::IVModel *>(model.get());
    if(temporaryIvModel == nullptr) {
        printErrorInGeneralMessages(GenMsg::ivTmpModelNotRead);
        return false;
    }

    ivm::IVModel *const currentIvModel = getCurrentIvModel();
    if(currentIvModel == nullptr) {
        printErrorInGeneralMessages(GenMsg::ivModelNotRead);
        return false;
    }

    if(!mergeIvModels(currentIvModel, temporaryIvModel)) {
        printErrorInGeneralMessages(GenMsg::ivModelsNotMerged);
        return false;
    }

    return true;
}

auto SimulinkImporterPlugin::getCurrentIvModel() -> ivm::IVModel *
{
    const IVEditorCorePtr currentIvEditorCore = getCurrentIvEditorCore();
    if(currentIvEditorCore == nullptr) {
        printErrorInGeneralMessages(GenMsg::ivEditorCoreCouldNotBeRead);
        return nullptr;
    }

    const ive::InterfaceDocument *const currentIvEditorCoreDocument = currentIvEditorCore->document();
    if(currentIvEditorCoreDocument == nullptr) {
        printErrorInGeneralMessages(GenMsg::ivEditorCoreNoDocument);
        return nullptr;
    }

    return currentIvEditorCoreDocument->objectsModel();
}

auto SimulinkImporterPlugin::getCurrentIvEditorCore() -> IVEditorCorePtr
{
    Core::IDocument *const currentDocument = EditorManager::currentDocument();
    IVEditorDocument *const currentIvDocument = static_cast<IVEditorDocument *>(currentDocument);
    if(currentIvDocument == nullptr) {
        printErrorInGeneralMessages(GenMsg::ivFileNotSelected);
        return nullptr;
    }

    return currentIvDocument->ivEditorCore();
}

auto SimulinkImporterPlugin::mergeIvModels(ivm::IVModel *const destinationIvModel, ivm::IVModel *const sourceIvModel) -> bool
{
    ivm::IVObject * sourceIvObject = nullptr;
    ivm::IVFunction * sourceIvFunction = nullptr;

    for (ivm::IVObject *const temporarySourceIvObject : sourceIvModel->visibleObjects()) {
        ivm::IVFunction *const temporarySourceIvFunction = dynamic_cast<ivm::IVFunction *>(temporarySourceIvObject);

        if(temporarySourceIvFunction != nullptr) {
            if(sourceIvObject == nullptr && sourceIvFunction == nullptr) {
                sourceIvObject = temporarySourceIvObject;
                sourceIvFunction = temporarySourceIvFunction;
            } else {
                printErrorInGeneralMessages(GenMsg::ivModelShouldContainOnlyOneIvFunction);
                return false;
            }
        }
    }

    if(sourceIvObject == nullptr || sourceIvFunction == nullptr ) {
        printErrorInGeneralMessages(GenMsg::ivModelDoesntContainIvFunction);
        return false;
    }

    if(doesModelContainIvFunction(destinationIvModel, sourceIvFunction)) {
        printErrorInGeneralMessages(GenMsg::ivFunctionWillNotBeImported.arg(sourceIvObject->title()));
        return false;
    } else {
        return addIvFunctionToIvModel(dynamic_cast<ivm::IVFunction *>(sourceIvObject), destinationIvModel);
    }
}

auto SimulinkImporterPlugin::doesModelContainIvFunction(ivm::IVModel *const ivModel, ivm::IVFunction *const ivFunction) -> bool
{
    const auto isObjectTitleEqualFunctionTitle = [&ivFunction](ivm::IVObject *const ivObj) -> bool {
        if(ivObj == nullptr) {
            return false;
        } else {
            return ivObj->isFunction() && ivObj->title() == ivFunction->title();
        }
    };

    for (ivm::IVObject *const obj : ivModel->visibleObjects()) {
        if(isObjectTitleEqualFunctionTitle(obj)) {
            return true;
        }
    }

    return false;
}

auto SimulinkImporterPlugin::addIvFunctionToIvModel(ivm::IVFunction *const sourceIvFunction, ivm::IVModel *const ivModel) -> bool
{
    if(sourceIvFunction == nullptr) {
        printErrorInGeneralMessages(GenMsg::ivFunctionSourceCouldNotBeRead);
        return false;
    }

    const auto currentIvEditorCore = getCurrentIvEditorCore();
    if(currentIvEditorCore == nullptr) {
        printErrorInGeneralMessages(GenMsg::ivEditorCoreCouldNotBeRead);
        return false;
    }

    ivm::IVFunction *const destinationIvFunction = currentIvEditorCore->addFunction(sourceIvFunction->title(), nullptr);
    if(destinationIvFunction == nullptr) {
        printErrorInGeneralMessages(GenMsg::ivFunctionDestinationCouldNotBeRead);
        return false;
    }

    destinationIvFunction->setImplementation(0, EntityAttribute(m_defaultFunctionBlockImplementationName, m_functionBlockDefaultImplementation, EntityAttribute::Type::Attribute));
    destinationIvFunction->setDefaultImplementation(m_defaultFunctionBlockImplementationName);

    for (ivm::IVInterface *const sourceIvInterface : sourceIvFunction->interfaces()) {
        ivm::IVInterface *const destinationIvInterface = currentIvEditorCore->addInterface(sourceIvInterface->title(), destinationIvFunction->title());

        destinationIvInterface->setKind(sourceIvInterface->kind());
        destinationIvInterface->setDirection(sourceIvInterface->direction());
        destinationIvInterface->setParams(sourceIvInterface->params());
    }

    for (const shared::ContextParameter &srcContextParam : sourceIvFunction->contextParams()) {
        destinationIvFunction->addContextParam(srcContextParam);
    }

    return true;
}

auto SimulinkImporterPlugin::addGeneratedAsn1FilesToCurrentProject(const QStringList &generatedAsn1FileNames) -> void
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QStringList asn1FilePathsToBeAddedToProject;
#else
    QList<Utils::FilePath> asn1FilePathsToBeAddedToProject;
#endif

    for(auto &asn1FileName : generatedAsn1FileNames) {
        QString destinationAsn1FilePath = QString("%1/%2").arg(m_currentProjectDirectoryPath).arg(asn1FileName);

        QMessageBox::StandardButton messageBoxQuestionAnswer = QMessageBox::StandardButton::Yes;
        bool isFileExists = false;

        if(!isFileIsOneOfMatLabStandardDataTypesFiles(asn1FileName)) {
            if(QFileInfo::exists(destinationAsn1FilePath)) {
                const QString question = QString("%1 file exists in project directory. Do you want to override it ?")
                                        .arg(destinationAsn1FilePath);
                messageBoxQuestionAnswer = QMessageBox::question(nullptr, "File exists", question);
                isFileExists = true;
            }
        }

        if(messageBoxQuestionAnswer == QMessageBox::StandardButton::Yes) {
            if(isFileExists) {
                printInfoInGeneralMessages(GenMsg::fileHasBeenOverridden.arg(asn1FileName));
            }

            checkIfFileExistsAndRemoveIt(destinationAsn1FilePath);

            QFile(asn1FileName).copy(destinationAsn1FilePath);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            asn1FilePathsToBeAddedToProject.append(destinationAsn1FilePath);
#else
            Utils::FilePath destinationAsn1UtilsFilePath = Utils::FilePath::fromString(destinationAsn1FilePath);
            asn1FilePathsToBeAddedToProject.append(destinationAsn1UtilsFilePath);
#endif
        } else {
            printInfoInGeneralMessages(GenMsg::fileHasNotBeenOverridden.arg(asn1FileName));
        }
    }

    m_currentProject->rootProjectNode()->removeFiles(asn1FilePathsToBeAddedToProject); // dont want to duplicate files in .pro file
    m_currentProject->rootProjectNode()->addFiles(asn1FilePathsToBeAddedToProject);
}

auto SimulinkImporterPlugin::isFileIsOneOfMatLabStandardDataTypesFiles(const QString &fileName) -> bool
{
    const QString matLabStandardDataAsn1FileName = MatLabStandardDataTypes::getStandardDataTypesEscapedPackageName() + ".asn";
    const QString matLabStandardDataAcnFileName = MatLabStandardDataTypes::getStandardDataTypesEscapedPackageName() + ".acn";

    if(fileName == matLabStandardDataAsn1FileName || 
        fileName == matLabStandardDataAcnFileName) {
            return true;
        }

    return false;
}

auto SimulinkImporterPlugin::moveToProjectDirectoryAndPrepareTemporaryWorkingDirectory(const QString& temporaryWorkingDirectoryName) -> void
{
    QDir::setCurrent(m_currentProjectDirectoryPath);

    const bool isTemporaryWorkingDirectoryExists = QDir().exists(temporaryWorkingDirectoryName);

    if(isTemporaryWorkingDirectoryExists) { 
        QDir(temporaryWorkingDirectoryName).removeRecursively();
    }

    QDir().mkdir(temporaryWorkingDirectoryName);
}

auto SimulinkImporterPlugin::moveToProjectDirectoryAndRemoveConvertersTemporaryWorkingDirectory() -> void
{
    QDir::setCurrent(m_currentProjectDirectoryPath);

    QDir convertersTemporaryWorkingDirectory(m_convertersTemporaryWorkingDirectory);
    
    if(convertersTemporaryWorkingDirectory.exists()) {
        convertersTemporaryWorkingDirectory.removeRecursively();
    }
}

auto SimulinkImporterPlugin::removeMatLabTemporaryWorkingDirectory() -> void
{
    QDir matlabTemporaryWorkingDirectory(m_matlabTemporaryWorkingDirectory);
    
    if(matlabTemporaryWorkingDirectory.exists()) {
        matlabTemporaryWorkingDirectory.removeRecursively();
    }
}

auto SimulinkImporterPlugin::checkIfFileExistsAndRemoveIt(const QString &filePath) -> void
{
    QFile fileToRemove(filePath);

    if(fileToRemove.exists()) {
        fileToRemove.remove();
    }

    fileToRemove.close();
}

auto SimulinkImporterPlugin::printPluginSelfIntroductionInGeneralMessages() -> void
{
    const QString completeMessage = GenMsg::pluginSelfIntroduction.arg(m_pluginNameInGeneralMessages);
    
    MessageManager::write("");
    MessageManager::write(completeMessage);
}

auto SimulinkImporterPlugin::printMatLabCommand(const QStringList& matlabCallArguments) -> void
{
    const QString matlabCommand = QString("%1 %2")
                                    .arg(m_matlabAppName)
                                    .arg(matlabCallArguments.join(' '));

    printInfoInGeneralMessages(GenMsg::executingMatlabCommand.arg(matlabCommand));
}

auto SimulinkImporterPlugin::printInfoInGeneralMessages(const QString &message) -> void
{
    const QString infoMessage = GenMsg::msgInfo.arg(message);
    const QString completeMessage = QString("%1 %2").arg(m_pluginNameInGeneralMessages).arg(infoMessage);

    MessageManager::write(completeMessage);
}

auto SimulinkImporterPlugin::printErrorInGeneralMessages(const QString &message) -> void
{
    const QString errorMessage = GenMsg::msgError.arg(message);
    const QString completeMessage = QString("%1 %2").arg(m_pluginNameInGeneralMessages).arg(errorMessage);
    
    MessageManager::write(completeMessage);
}

} // namespace spctr
