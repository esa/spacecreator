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

#pragma once

#include "simulinkimporterpluginconstants.h"

#include "iveditorcore.h"
#include <coreplugin/actionmanager/actioncontainer.h>
#include <extensionsystem/iplugin.h>
#include <conversion/registry/registry.h>
#include <QMenu>
#include <QMap>
#include <projectexplorer/project.h>

namespace spctr {

class SimulinkImporterPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "SimulinkImporterPlugin.json")

public:
    SimulinkImporterPlugin();
    ~SimulinkImporterPlugin() override;

    auto initialize(const QStringList &arguments, QString *errorString) -> bool override;
    auto extensionsInitialized() -> void override;
    auto aboutToShutdown() -> ShutdownFlag override;

private:
    auto initializeRegistry() -> void;
    auto configureSimulinkMenu() -> void;
    auto createActionContainerInTools(const QString &title) -> Core::ActionContainer *;
    auto onActiveProjectChanged(ProjectExplorer::Project *project) -> void;

private:
    auto importSlx() -> void;
    auto importXml() -> void;

private:
    auto askAboutAndCheckFilePath(QString caption, QString filter, QString &inputFilePath) -> bool;
    auto askAboutAndCheckFunctionBlockName(QString &functionBlockName) -> bool;
    auto searchAndCheckMatLabModelWorkspaceFile(const QString& inputFilePath, QFileInfo &workspaceFileInfo) -> bool;

    auto generateMatLabCommand(QFileInfo &workspaceFileInfo, const QString& inputFilePath) -> QString;
    auto generateWorkspaceLoadCallFunction(QFileInfo &workspaceFileInfo) -> QString;
    auto generateTasteExporterCallFunction(const QString& inputFilePath) -> QString;
    auto getWorkspaceLoadFunctionNameForWorkspaceFileExtension(const QString &workspaceFileExtension) -> QString;
    auto copyInputSlxFileToWorkDirectory(const QString &inputFilePath, const QString &functionBlockName) -> void;
    auto generateExportedXmlFilePath(const QString& inputFilePath) -> QString;

    auto importXmlFileAndRemoveTemporaries(const QString &inputFilePath, const QString &functionBlockName) -> bool;
    auto importXmlFile(const QString &inputFilePath, const QString &functionBlockName, QStringList &generatedAsn1FileNames) -> bool;

    auto convertXmlFileToAsn1(const QString &inputFilePath, QStringList &generatedAsn1FileNames) -> bool;
    auto getGeneratedAsn1FileNamesFromModels(const std::vector<std::unique_ptr<conversion::Model>> &models) -> QStringList;
    auto convertXmlFileToIv(const QString &inputFilePath, const QString &functionBlockName, const QString &ivConfig) -> bool;
    auto convert(const std::set<conversion::ModelType> &srcModelType, conversion::ModelType targetModelType,
            const std::set<conversion::ModelType> &auxModelTypes, conversion::Options options)
            -> std::vector<std::unique_ptr<conversion::Model>>;

    auto addIvToCurrentProject(const QString &ivConfig) -> bool;
    auto getCurrentIvModel() -> ivm::IVModel *;
    auto getCurrentIvEditorCore() -> IVEditorCorePtr;
    auto mergeIvModels(ivm::IVModel *const destinationIvModel, ivm::IVModel *const sourceIvModel) -> bool;
    auto doesModelContainIvFunction(ivm::IVModel *const model, ivm::IVFunction *const function) -> bool;
    auto addIvFunctionToIvModel(ivm::IVFunction *const srcFun, ivm::IVModel *const model) -> bool;

    auto addGeneratedAsn1FilesToCurrentProject(const QStringList &generatedAsn1FileNames) -> void;
    auto isFileIsOneOfMatLabStandardDataTypesFiles(const QString &fileName) -> bool;

    auto removeConvertersTemporaries(const QStringList &generatedAsn1FileNames) -> void;
    auto removeMatLabCommandTemporaries() -> void;

    auto checkIfFileExistsAndRemoveIt(const QString &filePath) -> void;

private:
    const QString m_temporaryIvFileName = "tmp-interfaceview.xml";
    const QString m_functionBlockDefaultImplementation = "QGenC";
    const QString m_tasteExporterOutputDirectory = "exported";
    const QString m_defaultFunctionBlockName = "simulink";
    const QString m_defaultFunctionBlockImplementationName = "default";

    const QStringList m_workspaceFilesExtensionsFilter = { "*.m", "*.mat" };
    const QMap<QString, QString> m_workspaceLoadFunctionsMap = {{"m", "run"}, {"mat", "load"}};

    const QString m_workspaceLoadCallFunctionTemplate = "%1('%2')";
    const QString m_matlabCommandWithoutWorkspaceLoadTemplate = "matlab -batch \"%1 exit;\"";
    const QString m_matlabCommandWithWorkspaceLoadTemplate = "matlab -batch \"%1 %2 exit;\"";
    const QString m_tasteExporterCallFunctionTemplate = "taste_export('%1', '%2');";
    const QString m_functionBlockInWorkDirectoryPathTemplate = "%1/work/%2/%3/src";

    conversion::Registry m_registry;

    QMenu *m_simulinkMenu = nullptr;
    ProjectExplorer::Project *m_currentProject = nullptr;
};
}
