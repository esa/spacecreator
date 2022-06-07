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

#include "iveditorcore.h"
#include "ivfunction.h"
#include "ivmodel.h"
#include "model.h"
#include "options.h"
#include "dveditor.h"

#include <QStandardItemModel>
#include <QStringList>
#include <conversion/registry/registry.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <extensionsystem/iplugin.h>
#include <memory>
#include <shared/ui/listtreedialog.h>
#include <csv/CsvModel/csvmodel.h>
#include <asn1library/asn1/asn1model.h>

#include <dvcore/dvobject.h>

namespace spctr {

class FunctionTesterPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "FunctionTesterPlugin.json")

public:
    FunctionTesterPlugin();
    ~FunctionTesterPlugin() override;

    auto initialize(const QStringList &arguments, QString *errorString) -> bool override;
    auto extensionsInitialized() -> void override;
    auto aboutToShutdown() -> ShutdownFlag override;
    auto functionTesterPluginMain() -> void;
private:
    auto functionTesterPluginCore(ivm::IVInterface *interface,
        const std::unique_ptr<csv::CsvModel> &csvModel,
        const std::unique_ptr<Asn1Acn::Asn1Model> &asn1Model, float delta) -> void;
    auto addTestInterfaceOption() -> void;
    auto createActionContainerInTools(const QString &title) -> Core::ActionContainer *;
    auto loadCsv() -> std::unique_ptr<csv::CsvModel>;
    auto getCurrentIvEditorCore() -> IVEditorCorePtr;
    auto getSelectedInterface() -> ivm::IVInterface *;
    auto setDeltaDialog() -> float;
    auto loadAsn1Model() -> std::unique_ptr<Asn1Acn::Asn1Model>;
    auto exportDvModel(const std::unique_ptr<dvm::DVModel> &dvModel, const QString &outputFilename) -> void;
    auto exportIvModel(const std::unique_ptr<ivm::IVModel> &ivModel, QString outputFilepath) -> void;
    auto getDvObjectsFromModel(dvm::DVModel *const model) -> std::unique_ptr<QVector<dvm::DVObject *>>;
    auto getBaseDirectory() -> QString;

    // auto initializeRegistry() -> void;

    /** @brief  Internal registry */
    conversion::Registry m_registry;
};
}
