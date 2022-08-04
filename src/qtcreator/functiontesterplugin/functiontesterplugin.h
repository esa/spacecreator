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

#include <QGridLayout>
#include <QLabel>
#include <asn1library/asn1/asn1model.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <csv/CsvModel/csvmodel.h>
#include <extensionsystem/iplugin.h>
#include <qlistwidget.h>
#include <testgenerator/testgenerator.h>

namespace spctr {

using testgenerator::DataReconstructor;
using testgenerator::LaunchConfigLoader;
using testgenerator::LaunchConfiguration;

class FunctionTesterPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "FunctionTesterPlugin.json")

    struct TypeLayoutFormFields {
        QLabel *integerLabel;
        QLineEdit *integerSizeEdit;
        QLineEdit *integerPaddingEdit;

        QLabel *booleanLabel;
        QLineEdit *booleanSizeEdit;
        QLineEdit *booleanPaddingEdit;

        QLabel *realLabel;
        QLineEdit *realSizeEdit;
        QLineEdit *realPaddingEdit;
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
     * @param launchConfig tested binary launching params
     *
     */
    auto testUsingDataFromCsvGui(const LaunchConfiguration &launchConfig) -> void;

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
    auto selectScriptDialog(QWidget *parent, const QString &boardName, QLineEdit *scriptPathEdit) -> void;
    auto typeLayoutForm(const DataReconstructor::TypeLayoutInfos &typeLayout, TypeLayoutFormFields &formFields)
            -> QGridLayout *;
    auto readTypeInfos(const TypeLayoutFormFields &formFields) -> DataReconstructor::TypeLayoutInfos const;

    LaunchConfigLoader boardsConfigLoader;
    QMap<QString, LaunchConfiguration> boardsConfiguration;
};

}
