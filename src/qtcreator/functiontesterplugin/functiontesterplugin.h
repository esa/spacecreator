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

#include <coreplugin/actionmanager/actioncontainer.h>
#include <extensionsystem/iplugin.h>
#include <csv/CsvModel/csvmodel.h>
#include <asn1library/asn1/asn1model.h>
#include <qlistwidget.h>

namespace spctr {

class FunctionTesterPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "FunctionTesterPlugin.json")

    struct BoardLaunchConfig
    {
        QString scriptPath;
        QString clientName;
        QString clientParams;
        QString serverName;
        QString serverParams;
    };

public:
    FunctionTesterPlugin();
    ~FunctionTesterPlugin() override;

    auto initialize(const QStringList &arguments, QString *errorString) -> bool override;
    auto extensionsInitialized() -> void override;
    auto aboutToShutdown() -> ShutdownFlag override;

    /**
     * @brief Test the selected interface using user provided data
     *
     */
    auto testUsingDataFromCsvGui(const QString &boardName) -> void;
private:
    auto addTestInterfaceOption() -> void;
    auto createActionContainerInTools(const QString &title) -> Core::ActionContainer *;
    auto loadCsv() -> std::unique_ptr<csv::CsvModel>;
    auto setDeltaDialog() -> float;
    auto getSelectedInterface() -> ivm::IVInterface *;
    auto loadAsn1Model() -> std::unique_ptr<Asn1Acn::Asn1Model>;
    auto getBaseDirectory() -> QString;
    auto getCurrentIvEditorCore() -> IVEditorCorePtr;
    auto displayResultHtml(const QString &resultFileName) -> void;
    auto selectBoardDialog() -> void;
    auto boardOptionsDialog(QWidget *parent, const QString &boardName) -> void;
    auto selectScriptDialog(QWidget *parent, const QString &boardName) -> void;
    auto loadBoardsConfiguration() -> QMap<QString, BoardLaunchConfig>;
    auto saveBoardConfiguration(const QString &boardName, const QString &gdbScriptPath) -> bool;

    QMap<QString, BoardLaunchConfig> boardsConfiguration;
};

}
