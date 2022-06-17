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

#include <coreplugin/actionmanager/actioncontainer.h>
#include <extensionsystem/iplugin.h>
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
    auto functionTesterPluginCore(ivm::IVInterface &interface, const csv::CsvModel &csvModel,
            const Asn1Acn::Asn1Model &asn1Model, float delta) -> void;
    auto addTestInterfaceOption() -> void;
    auto createActionContainerInTools(const QString &title) -> Core::ActionContainer *;
    auto loadCsv() -> std::unique_ptr<csv::CsvModel>;
    auto getCurrentIvEditorCore() -> IVEditorCorePtr;
    auto getSelectedInterface() -> ivm::IVInterface *;
    auto setDeltaDialog() -> float;
    auto loadAsn1Model() -> std::unique_ptr<Asn1Acn::Asn1Model>;
    auto exportDvModel(dvm::DVModel *dvModel, const QString &outputFilename) -> void;
    auto exportIvModel(ivm::IVModel *ivModel, const QString &outputFilename) -> void;
    auto getDvObjectsFromModel(dvm::DVModel *const model) -> std::unique_ptr<QVector<dvm::DVObject *>>;
    auto getBaseDirectory() -> QString;
    auto compileTest(const QString &functionName) -> void;
    auto copyRecursively(const QString &srcPath, const QString &dstPath) -> bool;
    auto runProcess(QString cmd, QStringList args, QString workingPath) -> void;
    auto extractResult(ivm::IVInterface *const interface, Asn1Acn::Asn1Model *const asn1Model) -> void;

    QString projectDirectory;
    QString generatedPath;
    QString generatedCodePath;
    QString generatedIvPath;
    QString generatedDvPath;
};
}
