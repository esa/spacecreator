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
    auto addSimulinkImportExport() -> void;

private:
    auto importSlx() -> void;
    auto importXml() -> void;

private:
    auto convert(const std::set<conversion::ModelType> &srcModelType, conversion::ModelType targetModelType,
            const std::set<conversion::ModelType> &auxModelTypes, conversion::Options options)
            -> std::vector<std::unique_ptr<conversion::Model>>;
    auto convertFileToAsn1(const QString &inputFilePath, QStringList &asn1FileNames) -> bool;
    auto convertFileToIv(const QString &inputFilePath, const QString &functionBlockName, const QString &ivConfig) -> bool;

    auto askAboutFilePathAndCheck(QString caption, QString filter, QString &inputFilePath) -> bool;
    auto askAboutFunctionBlockNameAndCheck(QString &functionBlockName) -> bool;

    auto getAsnModelFilenames(const std::vector<std::unique_ptr<conversion::Model>> &models) -> QStringList;
    auto addAsn1FilesToCurrentProject(QStringList fileNames) -> void;
    auto tryAddIvToCurrentProject(const QString &ivConfig) -> bool;

    auto mergeIvModels(ivm::IVModel *const dstIvModel, ivm::IVModel *const srcIvModel) -> void;
    auto doesModelContainFunction(ivm::IVModel *const model, ivm::IVFunction *const function) -> bool;
    auto addFunctionToModel(ivm::IVFunction *const srcFun, ivm::IVModel *const model) -> void;
    auto getCurrentIvModel() -> ivm::IVModel *;
    auto getCurrentIvEditorCore() -> IVEditorCorePtr;
    auto isFileIsOneOfMatLabStandardDataTypesFiles(const QString &fileName) -> bool;
    auto createActionContainerInTools(const QString &title) -> Core::ActionContainer *;
    auto onActiveProjectChanged(ProjectExplorer::Project *project) -> void;

private:
    conversion::Registry m_registry;
    
    const QString m_tmpIvFileName = "tmp-interfaceview.xml";
    const QString m_functionBlockDefaultImplementation = "QGenC";

    QMenu *m_simulinkMenu = nullptr;
    ProjectExplorer::Project *m_currentProject = nullptr;
};
}
