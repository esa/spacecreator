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
    if (!asn1Registrar.registerCapabilities(m_registry)) {
        throw RegistrationFailedException(ModelType::Asn1);
    }

    IvRegistrar ivRegistrar;
    if (!ivRegistrar.registerCapabilities(m_registry)) {
        throw RegistrationFailedException(ModelType::InterfaceView);
    }

    SimulinkRegistrar simulinkRegistrar;
    if (!simulinkRegistrar.registerCapabilities(m_registry)) {
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

    m_simulinkMenu->setEnabled(project != nullptr);
}

auto SimulinkImporterPlugin::importSlx() -> void
{
    QString inputFilePath;
    QString functionBlockName;
    QFileInfo workspaceFileInfo;

    if( !askAboutAndCheckFilePath(tr("Select file to import model from ..."), tr("*.slx"), inputFilePath)) {
        return;
    }

    if( !askAboutAndCheckFunctionBlockName(functionBlockName)) {
        return;
    }

    if (!searchAndCheckMatLabModelWorkspaceFile(inputFilePath, workspaceFileInfo)) {
        return;
    }

    const QString matlabCmd = generateMatLabCommand(workspaceFileInfo, inputFilePath);

    MessageManager::write(GenMsg::msgInfo.arg(GenMsg::executingMatlabCommand));

    if (QProcess::execute(matlabCmd) != 0) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::matlabCommandHasFailed));
        return;
    }

    MessageManager::write(GenMsg::msgInfo.arg(GenMsg::done));

    copyInputSlxFileToWorkDirectory(inputFilePath, functionBlockName);

    importXmlFileAndRemoveTemporaries(generateExportedXmlFilePath(inputFilePath), functionBlockName);
}

auto SimulinkImporterPlugin::importXml() -> void
{
    QString inputFilePath;
    QString functionBlockName;

    if( !askAboutAndCheckFilePath(tr("Select file to import interface specification from ..."), tr("*.xml"), inputFilePath)) {
        return;
    }

    if( !askAboutAndCheckFunctionBlockName(functionBlockName)) {
        return;
    }

    importXmlFileAndRemoveTemporaries(inputFilePath, functionBlockName);
}

auto SimulinkImporterPlugin::askAboutAndCheckFilePath(QString caption, QString filter, QString &inputFilePath) -> bool
{
    const QString currentProjectDirectory = m_currentProject->projectDirectory().toString();
    const QString tmpFilePath = QFileDialog::getOpenFileName(nullptr, caption, currentProjectDirectory, filter);

    if (tmpFilePath.isEmpty()) {
        return false;
    }

    inputFilePath = tmpFilePath;
    return true;
}

auto SimulinkImporterPlugin::askAboutAndCheckFunctionBlockName(QString &functionBlockName) -> bool
{
    bool isOk = false;
    const QString tmpFunctionBlockName = QInputDialog::getText(nullptr, tr("Choose function block name"),
                                         tr("Function Block Name : "), QLineEdit::Normal,
                                         m_defaultFunctionBlockName, &isOk);

    if(!isOk && tmpFunctionBlockName.isEmpty()) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivNoFunctionBlockName));
        return false;
    }

    functionBlockName = Escaper::escapeIvName(tmpFunctionBlockName);
    return true;
}

auto SimulinkImporterPlugin::searchAndCheckMatLabModelWorkspaceFile(const QString& inputFilePath, QFileInfo &workspaceFileInfo) -> bool
{
    QFileInfo inputFileInfo(inputFilePath);

    const QDir inputFileDirectory = inputFileInfo.absoluteDir();
    const QFileInfoList foundWorkspaceFiles = inputFileDirectory.entryInfoList(m_workspaceFilesExtensionsFilter, QDir::Files);
    const int foundWorkspaceFilesAmount = foundWorkspaceFiles.size();

    if(foundWorkspaceFilesAmount == 1) {
        workspaceFileInfo = foundWorkspaceFiles.front();
    } else if(foundWorkspaceFilesAmount >= 2) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::toMuchMatLabModelWorkspaceFiles));
        return false;
    }

    return true;
}

auto SimulinkImporterPlugin::generateMatLabCommand(QFileInfo &workspaceFileInfo, const QString& inputFilePath) -> QString
{
    const QString workspaceLoadFunction = generateWorkspaceLoadCallFunction(workspaceFileInfo);
    const QString tasteExporterFunction = generateTasteExporterCallFunction(inputFilePath);

    if(workspaceLoadFunction.isEmpty()) {
        return m_matlabCommandWithoutWorkspaceLoadTemplate.arg(tasteExporterFunction);
    } else {
        return m_matlabCommandWithWorkspaceLoadTemplate.arg(workspaceLoadFunction).arg(tasteExporterFunction);
    }
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
    const QString currentProjectDirectory = m_currentProject->projectDirectory().toString();
    const QString escapedFunctionBlockName = functionBlockName.toLower();
    const QString functionBlockInWorkDirectoryPath = m_functionBlockInWorkDirectoryPathTemplate.arg(currentProjectDirectory).arg(escapedFunctionBlockName).arg(m_functionBlockDefaultImplementation);

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

    return QString("%1/%2.xml").arg(m_tasteExporterOutputDirectory).arg(inputFileInfo.baseName());
}

auto SimulinkImporterPlugin::importXmlFileAndRemoveTemporaries(const QString &inputFilePath, const QString &functionBlockName) -> void
{
    QStringList generatedAsn1FileNames;

    importXmlFile(inputFilePath, functionBlockName, generatedAsn1FileNames);

    removeConvertersTemporaries(generatedAsn1FileNames);
    removeMatLabCommandTemporaries();
}

auto SimulinkImporterPlugin::importXmlFile(const QString &inputFilePath, const QString &functionBlockName, QStringList &generatedAsn1FileNames) -> void
{
    const QString ivConfig = shared::interfaceCustomAttributesFilePath();

    if(!convertXmlFileToAsn1(inputFilePath, generatedAsn1FileNames)) {
        return;
    }

    if(!convertXmlFileToIv(inputFilePath, functionBlockName, ivConfig)) {
        return;
    }

    if(!tryAddIvToCurrentProject(ivConfig))
    {
        return;
    }

    addGeneratedAsn1FilesToCurrentProject(generatedAsn1FileNames);

    MessageManager::write(GenMsg::msgInfo.arg(GenMsg::filesImported));
}

auto SimulinkImporterPlugin::convertXmlFileToAsn1(const QString &inputFilePath, QStringList &generatedAsn1FileNames) -> bool
{
    conversion::Options options;
    options.add(conversion::simulink::SimulinkOptions::inputFilepath, inputFilePath);
    options.add(conversion::asn1::Asn1Options::asn1FilepathPrefix, "");
    options.add(conversion::asn1::Asn1Options::acnFilepathPrefix, "");

    try {
        const auto outputModels = convert({ conversion::ModelType::Simulink }, conversion::ModelType::Asn1, {}, options);
        generatedAsn1FileNames = getGeneratedAsn1FileNamesFromModels(outputModels);
    } catch (std::exception &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
        return false;
    }

    return true;
}

auto SimulinkImporterPlugin::getGeneratedAsn1FileNamesFromModels(const std::vector<std::unique_ptr<conversion::Model>> &models) -> QStringList
{
    auto *const asn1model = conversion::translator::Translator::getModel<Asn1Acn::Asn1Model>(models);
    if (asn1model != nullptr) {
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
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
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

auto SimulinkImporterPlugin::tryAddIvToCurrentProject(const QString &ivConfig) -> bool
{
    std::unique_ptr<conversion::Model> model;

    try {
        model = plugincommon::ModelLoader::loadIvModel(ivConfig, m_temporaryIvFileName);
    } catch (std::exception &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
        return false;
    }

    ivm::IVModel *const temporaryIvModel = dynamic_cast<ivm::IVModel *>(model.get());
    if (temporaryIvModel == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivTmpModelNotRead));
        return false;
    }

    ivm::IVModel *const currentIvModel = getCurrentIvModel();
    if (currentIvModel == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivModelNotRead));
        return false;
    }

    mergeIvModels(currentIvModel, temporaryIvModel);

    return true;
}

auto SimulinkImporterPlugin::getCurrentIvModel() -> ivm::IVModel *
{
    const IVEditorCorePtr currentIvEditorCore = getCurrentIvEditorCore();
    if (currentIvEditorCore == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivEditorCoreCouldNotBeRead));
        return nullptr;
    }

    const ive::InterfaceDocument *const currentIvEditorCoreDocument = currentIvEditorCore->document();
    if (currentIvEditorCoreDocument == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivEditorCoreNoDocument));
        return nullptr;
    }

    return currentIvEditorCoreDocument->objectsModel();
}

auto SimulinkImporterPlugin::getCurrentIvEditorCore() -> IVEditorCorePtr
{
    Core::IDocument *const currentDocument = EditorManager::currentDocument();
    IVEditorDocument *const currentIvDocument = static_cast<IVEditorDocument *>(currentDocument);
    if (currentIvDocument == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivFileNotSelected));
        return nullptr;
    }

    return currentIvDocument->ivEditorCore();
}

auto SimulinkImporterPlugin::mergeIvModels(ivm::IVModel *const destinationIvModel, ivm::IVModel *const sourceIvModel) -> void
{
    for (ivm::IVObject *const sourceIvObject : sourceIvModel->visibleObjects()) {
        ivm::IVFunction *const sourceFunction = dynamic_cast<ivm::IVFunction *>(sourceIvObject);
        
        if (sourceFunction == nullptr) {
            continue;
        } else if (doesModelContainIvFunction(destinationIvModel, sourceFunction)) {
           MessageManager::write(GenMsg::msgInfo.arg(GenMsg::ivFunctionWillNotBeImported.arg(sourceIvObject->title())));
        } else {
            addIvFunctionToIvModel(dynamic_cast<ivm::IVFunction *>(sourceIvObject), destinationIvModel);
        }
    }
}

auto SimulinkImporterPlugin::doesModelContainIvFunction(ivm::IVModel *const model, ivm::IVFunction *const function) -> bool
{
    const auto isObjectTitleEqualFunctionTitle = [&function](ivm::IVObject *const ivObj) -> bool {
        if (ivObj == nullptr) {
            return false;
        } else {
            return ivObj->isFunction() && ivObj->title() == function->title();
        }
    };

    for (ivm::IVObject *const obj : model->visibleObjects()) {
        if (isObjectTitleEqualFunctionTitle(obj)) {
            return true;
        }
    }

    return false;
}

auto SimulinkImporterPlugin::addIvFunctionToIvModel(ivm::IVFunction *const sourceIvFunction, ivm::IVModel *const ivModel) -> void
{
    if (sourceIvFunction == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivFunctionSourceCouldNotBeRead));
        return;
    }

    const auto currentIvEditorCore = getCurrentIvEditorCore();
    if (currentIvEditorCore == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivEditorCoreCouldNotBeRead));
        return;
    }

    ivm::IVFunction *const destinationIvFunction = currentIvEditorCore->addFunction(sourceIvFunction->title(), nullptr);
    if (destinationIvFunction == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivFunctionDestinationCouldNotBeRead));
        return;
    }

    // TODO
    destinationIvFunction->addImplementation("defaultImpl", "QGenC");
    destinationIvFunction->setDefaultImplementation("defaultImpl");

    for (ivm::IVInterface *const sourceIvInterface : sourceIvFunction->interfaces()) {
        ivm::IVInterface *const destinationIvInterface = currentIvEditorCore->addInterface(sourceIvInterface->title(), destinationIvFunction->title());

        destinationIvInterface->setKind(sourceIvInterface->kind());
        destinationIvInterface->setDirection(sourceIvInterface->direction());
        destinationIvInterface->setParams(sourceIvInterface->params());

        destinationIvInterface->removeEntityAttribute("miat");
        destinationIvInterface->removeEntityAttribute("period");
    }

    for (const shared::ContextParameter &srcContextParam : sourceIvFunction->contextParams()) {
        destinationIvFunction->addContextParam(srcContextParam);
    }
}

auto SimulinkImporterPlugin::addGeneratedAsn1FilesToCurrentProject(const QStringList &generatedAsn1FileNames) -> void
{
    const QString currentProjectDirectory = m_currentProject->projectDirectory().toString();
    QStringList asn1FilePathsToBeAddedToProject;

    for(auto &asn1FileName : generatedAsn1FileNames) {
        QString destinationAsn1FilePath = QString("%1/%2").arg(currentProjectDirectory).arg(asn1FileName);

        QMessageBox::StandardButton messageBoxQuestionAnswer = QMessageBox::StandardButton::Yes;
        bool isFileExists = false;

        if(!isFileIsOneOfMatLabStandardDataTypesFiles(asn1FileName)) {
            if(QFileInfo::exists(destinationAsn1FilePath)) {
                messageBoxQuestionAnswer = QMessageBox::question(nullptr, "File exists", destinationAsn1FilePath + " file exists in project directory. Do you want to override it ?");
                isFileExists = true;
            }
        }

        if (messageBoxQuestionAnswer == QMessageBox::StandardButton::Yes) {
            if(isFileExists) {
                MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileHasBeenOverrided.arg(asn1FileName)));
            }

            checkIfFileExistsAndRemoveIt(destinationAsn1FilePath);

            QFile(asn1FileName).copy(destinationAsn1FilePath);

            asn1FilePathsToBeAddedToProject.append(destinationAsn1FilePath);
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

auto SimulinkImporterPlugin::removeConvertersTemporaries(const QStringList &generatedAsn1FileNames) -> void
{
    checkIfFileExistsAndRemoveIt(m_temporaryIvFileName);
    
    for(const QString &asn1FileName : generatedAsn1FileNames) {
        checkIfFileExistsAndRemoveIt(asn1FileName);
    }
}

auto SimulinkImporterPlugin::removeMatLabCommandTemporaries() -> void
{
    QDir tasteExporterOutputDirectory(m_tasteExporterOutputDirectory);
    
    if(tasteExporterOutputDirectory.exists()) {
        tasteExporterOutputDirectory.removeRecursively();
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

} // namespace spctr
