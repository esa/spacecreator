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

#include <conversion/simulink/SimulinkRegistrar/registrar.h>
#include <conversion/converter/exceptions.h>
#include <conversion/converter/converter.h>

#include <simulink/SimulinkOptions/options.h>
#include <simulink/matlabstandarddatatypes.h>

#include <shared/common.h>
#include <ivcore/ivfunction.h>
#include <libiveditor/interfacedocument.h>

using namespace Core;
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
    addSimulinkImportExport();

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

auto SimulinkImporterPlugin::addSimulinkImportExport() -> void
{
    Context allContexts(Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    ActionContainer *const acToolsSimulink = createActionContainerInTools(tr("&Simulink"));

    const auto importSlxAction = new QAction(tr("Import model from SLX"), this);
    connect(importSlxAction, &QAction::triggered, [=]() { this->importSlx(); });
    Command *const importSlxCommand = ActionManager::registerAction(importSlxAction, Constants::SLX_IMPORT_ID, allContexts);
    acToolsSimulink->addAction(importSlxCommand);

    const auto importXmlAction = new QAction(tr("Import interface specification from XML"), this);
    connect(importXmlAction, &QAction::triggered, [=]() { this->importXml(); });
    Command *const importXmlCommand = ActionManager::registerAction(importXmlAction, Constants::XML_IMPORT_ID, allContexts);
    acToolsSimulink->addAction(importXmlCommand);
}

auto SimulinkImporterPlugin::importSlx() -> void
{
    const QString inputFilePath =
            QFileDialog::getOpenFileName(nullptr, "Select file to import model from ...", QString(), tr("*.slx"));

    if (inputFilePath.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileToImportNotSelected));
        return;
    }
}

auto SimulinkImporterPlugin::importXml() -> void
{
    QString inputFilePath;
    QString functionBlockName;
    QStringList asn1FileNames;

    const QString ivConfig = shared::interfaceCustomAttributesFilePath();

    if( !askAboutFilePathAndCheck("Select file to import interface specification from ...", tr("*.xml"), inputFilePath)) {
        return;
    }

    if( !askAboutFunctionBlockNameAndCheck(functionBlockName)) {
        return;
    }

    if(!convertFileToAsn1(inputFilePath, asn1FileNames)) {
        return;
    }

    if(!convertFileToIv(inputFilePath, functionBlockName, ivConfig)) {
        return;
    }

    if(!tryAddIvToCurrentProject(ivConfig))
    {
        MessageManager::write(GenMsg::msgError.arg("Could not merge imported IV into current one"));
        QFile(m_tmpIvFileName).remove();
        // czyszczenie plikow ASN1
        return;
    }

    addAsn1FilesToCurrentProject(asn1FileNames);

    MessageManager::write(GenMsg::msgInfo.arg(GenMsg::filesImported));
}

auto SimulinkImporterPlugin::convert(const std::set<conversion::ModelType> &srcModelType,
        const conversion::ModelType targetModelType, const std::set<conversion::ModelType> &auxModelTypes,
        conversion::Options options) -> std::vector<std::unique_ptr<conversion::Model>>
{
    Converter converter(m_registry, std::move(options));
    converter.convert(srcModelType, targetModelType, auxModelTypes);
    return converter.extractCache();
}

auto SimulinkImporterPlugin::convertFileToAsn1(const QString &inputFilePath, QStringList &asn1FileNames) -> bool
{
    conversion::Options options;
    options.add(conversion::simulink::SimulinkOptions::inputFilepath, inputFilePath);
    options.add(conversion::asn1::Asn1Options::asn1FilepathPrefix, "");
    options.add(conversion::asn1::Asn1Options::acnFilepathPrefix, "");

    try {
        const auto outputModels = convert({ conversion::ModelType::Simulink }, conversion::ModelType::Asn1, {}, options);
        asn1FileNames = getAsnModelFilenames(outputModels);
    } catch (std::exception &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
        return false;
    }

    return true;
}

auto SimulinkImporterPlugin::convertFileToIv(const QString &inputFilePath, const QString &functionBlockName, const QString &ivConfig) -> bool
{
    conversion::Options options;
    options.add(conversion::simulink::SimulinkOptions::inputFilepath, inputFilePath);
    options.add(conversion::iv::IvOptions::outputFilepath, m_tmpIvFileName);
    options.add(conversion::iv::IvOptions::configFilepath, ivConfig);
    options.add(conversion::iv::SimulinkToIvOptions::ivFunctionName, functionBlockName);
    options.add(conversion::iv::SimulinkToIvOptions::ivFunctionImplementation, m_functionBlockDefaultImplementation);

    try {
        const auto outputModels = convert({ conversion::ModelType::Simulink }, conversion::ModelType::InterfaceView, {}, options);
    } catch (std::exception &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
        QFile(m_tmpIvFileName).remove();
        return false;
    }

    return true;
}

auto SimulinkImporterPlugin::askAboutFilePathAndCheck(QString caption, QString filter, QString &inputFilePath) -> bool
{
    const QString tmpFilePath = QFileDialog::getOpenFileName(nullptr, caption, QString(), filter);

    if (tmpFilePath.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileToImportNotSelected));
        return false;
    }

    inputFilePath = tmpFilePath;
    return true;
}

auto SimulinkImporterPlugin::askAboutFunctionBlockNameAndCheck(QString &functionBlockName) -> bool
{
    bool isOk;
    QString tmpFunctionBlockName = QInputDialog::getText(nullptr, tr("Choose function block name"),
                                         tr("Function Block Name : "), QLineEdit::Normal,
                                         tr("simulink"), &isOk);

    if (!isOk || tmpFunctionBlockName.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::ivNoFunctionBlockName));
        return false;
    }

    functionBlockName = tmpFunctionBlockName;
    return true;
}

auto SimulinkImporterPlugin::getAsnModelFilenames(const std::vector<std::unique_ptr<conversion::Model>> &models) -> QStringList
{
    auto *const asn1model = conversion::translator::Translator::getModel<Asn1Acn::Asn1Model>(models);
    if (asn1model != nullptr) {
        return conversion::asn1::exporter::Asn1Exporter::getFilenamesForModel(asn1model);
    } else {
        return QStringList();
    }
}

auto SimulinkImporterPlugin::addAsn1FilesToCurrentProject(QStringList asn1FileNames) -> void
{
    const QString currentProjectDir = m_currentProject->projectDirectory().toString();
    QStringList filePathsToAddToProject;
    bool wasFileReplaced = false;

    for(auto &asn1FileName : asn1FileNames) {
        QString asn1FilePath = QString("%1%2%3").arg(currentProjectDir).arg(QDir::separator()).arg(asn1FileName);

        QMessageBox::StandardButton ret = QMessageBox::StandardButton::Yes;
        wasFileReplaced = false;

        if(!isFileIsOneOfMatLabStandardDataTypesFiles(asn1FileName)) {
            if(QFileInfo::exists(asn1FilePath)) {
                ret = QMessageBox::question(nullptr, "File exists", asn1FilePath + " file exists in project directory. Do you want to override it ?");
                wasFileReplaced = true;
            }
        }

        if (ret == QMessageBox::StandardButton::Yes) {
            if(wasFileReplaced) {
                MessageManager::write(GenMsg::msgInfo.arg(asn1FileName + " file has been overrided"));
            }

            QFile(asn1FileName).copy(asn1FilePath);

            filePathsToAddToProject.append(asn1FilePath);
        }

        QFile(asn1FileName).remove();
    }

    m_currentProject->rootProjectNode()->removeFiles(filePathsToAddToProject); // dont want to duplicate files in .pro file
    m_currentProject->rootProjectNode()->addFiles(filePathsToAddToProject);
}

auto SimulinkImporterPlugin::tryAddIvToCurrentProject(const QString &ivConfig) -> bool
{
    std::unique_ptr<conversion::Model> model;

    try {
        model = plugincommon::ModelLoader::loadIvModel(ivConfig, m_tmpIvFileName);
    } catch (std::exception &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
        return false;
    }

    ivm::IVModel *const tmpIvModel = dynamic_cast<ivm::IVModel *>(model.get());
    if (tmpIvModel == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivTmpModelNotRead));
        return false;
    }

    ivm::IVModel *const currentIvModel = getCurrentIvModel();
    if (currentIvModel == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivModelNotRead));
        return false;
    }

    mergeIvModels(currentIvModel, tmpIvModel);

    QFile(m_tmpIvFileName).remove();

    return true;
}

auto SimulinkImporterPlugin::mergeIvModels(ivm::IVModel *const dstIvModel, ivm::IVModel *const srcIvModel) -> void
{
    for (auto &srcIvObject : srcIvModel->visibleObjects()) {
        auto *const srcFunction = dynamic_cast<ivm::IVFunction *>(srcIvObject);
        if (srcFunction == nullptr) {
            continue;
        } else if (doesModelContainFunction(dstIvModel, srcFunction)) {
           MessageManager::write(GenMsg::msgInfo.arg(QString("%1 - names are the same, Function will not be imported").arg(srcIvObject->title())));
        } else {
            addFunctionToModel(dynamic_cast<ivm::IVFunction *>(srcIvObject), dstIvModel);
        }
    }
}

auto SimulinkImporterPlugin::doesModelContainFunction(ivm::IVModel *const model, ivm::IVFunction *const function) -> bool
{
    const auto isObjectTitleEqualFunctionTitle = [&function](ivm::IVObject *const obj) -> bool {
        if (obj == nullptr) {
            return false;
        } else {
            return obj->isFunction() && obj->title() == function->title();
        }
    };

    for (ivm::IVObject *const obj : model->visibleObjects()) {
        if (isObjectTitleEqualFunctionTitle(obj)) {
            return true;
        }
    }

    return false;
}

auto SimulinkImporterPlugin::addFunctionToModel(ivm::IVFunction *const srcFun, ivm::IVModel *const model) -> void
{
    if (srcFun == nullptr) {
        MessageManager::write(GenMsg::msgError.arg("Source IV function could not be read"));
        return;
    }

    const auto curIvEditorCore = getCurrentIvEditorCore();
    if (curIvEditorCore == nullptr) {
        MessageManager::write(GenMsg::msgError.arg("Current IV Editor Core could not be read"));
        return;
    }

    ivm::IVFunction *const dstFun = curIvEditorCore->addFunction(srcFun->title(), nullptr);
    if (dstFun == nullptr) {
        MessageManager::write(GenMsg::msgError.arg("Destination IV function could not be created"));
        return;
    }

    dstFun->addImplementation("defaultImpl", "QGenC");
    dstFun->setDefaultImplementation("defaultImpl");

    //dstFun->removeImplementation(0);
    //dstFun->insertImplementation(0, EntityAttribute("default", "QGenC", EntityAttribute::Type::Attribute));
    //dstFun->setDefaultImplementation("default");

    for (ivm::IVInterface *const srcIf : srcFun->interfaces()) {
        ivm::IVInterface *const dstIf = curIvEditorCore->addInterface(srcIf->title(), dstFun->title());

        dstIf->setKind(srcIf->kind());
        dstIf->setDirection(srcIf->direction());
        dstIf->setParams(srcIf->params());

        dstIf->removeEntityAttribute("miat");
        dstIf->removeEntityAttribute("period");
    }

    for (const auto &srcContextParam : srcFun->contextParams()) {
        dstFun->addContextParam(srcContextParam);
    }
}

auto SimulinkImporterPlugin::getCurrentIvModel() -> ivm::IVModel *
{
    const auto curIvEditorCore = getCurrentIvEditorCore();
    if (curIvEditorCore == nullptr) {
        MessageManager::write(GenMsg::msgError.arg("IV Editor core could not be read"));
        return nullptr;
    }

    const auto curIvEditorCoreDocument = curIvEditorCore->document();
    if (curIvEditorCoreDocument == nullptr) {
        MessageManager::write(GenMsg::msgError.arg("No document in current IV Editor core"));
        return nullptr;
    }

    return curIvEditorCoreDocument->objectsModel();
}

auto SimulinkImporterPlugin::getCurrentIvEditorCore() -> IVEditorCorePtr
{
    auto *const currentDocument = EditorManager::currentDocument();
    auto *const currentIvDocument = static_cast<IVEditorDocument *>(currentDocument);
    if (currentIvDocument == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivFileNotSelected));
        return nullptr;
    }

    return currentIvDocument->ivEditorCore();
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

} // namespace spctr
