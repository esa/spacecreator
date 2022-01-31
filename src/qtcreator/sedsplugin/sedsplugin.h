/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "sedspluginconstants.h"

#include <QStandardItemModel>
#include <QStringList>
#include <conversion/common/model.h>
#include <conversion/common/options.h>
#include <conversion/registry/registry.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <extensionsystem/iplugin.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <libiveditor/iveditorcore.h>
#include <memory>
#include <shared/ui/listtreedialog.h>

namespace spctr {

class SedsPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "SedsPlugin.json")

public:
    SedsPlugin();
    ~SedsPlugin() override;

    auto initialize(const QStringList &arguments, QString *errorString) -> bool override;
    auto extensionsInitialized() -> void override;
    auto aboutToShutdown() -> ShutdownFlag override;

private:
    auto addSedsImportExport() -> void;
    auto createActionContainerInTools(const QString &title) -> Core::ActionContainer *;
    auto importInterfaceView() -> void;
    auto importSdl() -> void;
    auto importAsn1() -> void;
    auto exportAsn1() -> void;
    auto exportInterfaceView() -> void;
    auto itemModelUpdateWithFunctionNames(QStandardItemModel &model, const QStringList &ivFunctionsNames) -> void;
    auto initializeRegistry() -> void;
    auto convert(const std::set<conversion::ModelType> &srcModelType, conversion::ModelType targetModelType,
            const std::set<conversion::ModelType> &auxModelTypes, conversion::Options options)
            -> std::vector<std::unique_ptr<conversion::Model>>;
    auto ltdialogUpdateWithItemModel(ListTreeDialog &ltdialog, QStandardItemModel *model) -> void;
    auto getCurrentIvEditorCore() -> IVEditorCorePtr;
    auto mergeIvModels(ivm::IVModel *dstIvModel, ivm::IVModel *srcIvModel) -> void;
    auto loadIvModel(const QString &ivConfigFilename, const QString &ivFilename) -> std::unique_ptr<conversion::Model>;
    auto loadSedsModel(const QString &sedsFilename) -> std::unique_ptr<conversion::Model>;
    auto getCurrentIvModel() -> ivm::IVModel *;
    auto loadAndMergeIvModelIntoCurrent(const QString &ivConfig, const QString &ivFilename) -> bool;
    auto addFunctionToModel(ivm::IVFunction *srcFun, ivm::IVModel *model) -> void;
    auto doesModelContainFunction(ivm::IVModel *model, ivm::IVFunction *function) -> bool;
    auto addFilesToCurrentProject(QStringList filenames, const QString &path) -> void;
    auto getAsnModelFilenames(const std::vector<std::unique_ptr<conversion::Model>> &models) -> QStringList;
    auto getSdlModelFilenames(const std::vector<std::unique_ptr<conversion::Model>> &models) -> QStringList;

    /** @brief  Internal registry */
    conversion::Registry m_registry;
};
}
