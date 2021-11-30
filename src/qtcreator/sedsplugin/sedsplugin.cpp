/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "sedsplugin.h"

#include "../spacecreatorplugin/iv/iveditordata.h"
#include "../spacecreatorplugin/iv/iveditordocument.h"
#include "../spacecreatorplugin/iv/iveditorfactory.h"
#include "../spacecreatorplugin/iv/ivqtceditor.h"
#include "context/action/actionsmanager.h"
#include "conversion/asn1/Asn1Options/options.h"
#include "coreconstants.h"
#include "export/exceptions.h"
#include "import/exceptions.h"
#include "interfacedocument.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "sdl/SdlOptions/options.h"
#include "sharedlibrary.h"

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <asn1library/asn1/asn1model.h>
#include <conversion/asn1/Asn1Importer/importer.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/icore.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>
#include <limits>
#include <shared/ui/listtreedialog.h>

using namespace Core;

namespace spctr {

SedsPlugin::SedsPlugin()
{
    // Do not remove
}

SedsPlugin::~SedsPlugin()
{
    // Do not remove
}

auto SedsPlugin::initialize(const QStringList &arguments, QString *errorString) -> bool
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    addSedsImportExport();

    return true;
}

auto SedsPlugin::extensionsInitialized() -> void
{
    // Do not remove
}

auto SedsPlugin::aboutToShutdown() -> ExtensionSystem::IPlugin::ShutdownFlag
{
    return SynchronousShutdown;
}

auto SedsPlugin::updateModelWithFunctionNames(QStandardItemModel &model, const QStringList &ivFunctionsNames) -> void
{
    QStandardItemModel *const functionsListModel = &model;

    QList<QStandardItem *> ivFunctionNamesList;
    for (auto &name : ivFunctionsNames) {
        QStandardItem *item = new QStandardItem(name);
        item->setCheckable(true);
        ivFunctionNamesList.append(item);
    }
    functionsListModel->appendColumn(ivFunctionNamesList);
    functionsListModel->setHeaderData(0, Qt::Horizontal, "Functions");
}

auto SedsPlugin::addSedsImportExport() -> void
{
    Core::Context allContexts(
            Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    ActionContainer *const acToolsSeds = createActionContainerInTools(tr("&SEDS"));

    const auto ivImportAction = new QAction(tr("Import InterfaceView"), this);
    connect(ivImportAction, &QAction::triggered, [=]() { this->importInterfaceView(); });
    Core::Command *ivImport = Core::ActionManager::registerAction(ivImportAction, Constants::IV_IMPORT_ID, allContexts);
    acToolsSeds->addAction(ivImport);

    const auto sdlImportAction = new QAction(tr("Import SDL"), this);
    connect(sdlImportAction, &QAction::triggered, [=]() { this->importSdl(); });
    Core::Command *sdlImport =
            Core::ActionManager::registerAction(sdlImportAction, Constants::SDL_IMPORT_ID, allContexts);
    acToolsSeds->addAction(sdlImport);

    const auto asn1ImportAction = new QAction(tr("Import ASN.1"), this);
    connect(asn1ImportAction, &QAction::triggered, [=]() { this->importAsn1(); });
    Core::Command *asn1Import =
            Core::ActionManager::registerAction(asn1ImportAction, Constants::ASN1_IMPORT_ID, allContexts);
    acToolsSeds->addAction(asn1Import);

    const auto ivExportAction = new QAction(tr("Export InterfaceView"), this);
    connect(ivExportAction, &QAction::triggered, [=]() { this->exportInterfaceView(); });
    Core::Command *ivExport = Core::ActionManager::registerAction(ivExportAction, Constants::IV_EXPORT_ID, allContexts);
    acToolsSeds->addAction(ivExport);

    const auto asn1ExportAction = new QAction(tr("Export ASN.1"), this);
    connect(asn1ExportAction, &QAction::triggered, [=]() { this->exportAsn1(); });
    Core::Command *asn1Export =
            Core::ActionManager::registerAction(asn1ExportAction, Constants::ASN1_EXPORT_ID, allContexts);
    acToolsSeds->addAction(asn1Export);
}

auto SedsPlugin::createActionContainerInTools(const QString &title) -> ActionContainer *
{
    ActionContainer *const container = ActionManager::createMenu(Constants::M_TOOLS_SEDS);
    QMenu *const sedsMenu = container->menu();
    sedsMenu->setTitle(title);
    sedsMenu->setEnabled(true);

    ActionContainer *const tools = ActionManager::actionContainer(Core::Constants::M_TOOLS);
    tools->addMenu(container);

    return container;
}

auto SedsPlugin::importInterfaceView() -> void
{
    const QString inputFilePath = QFileDialog::getOpenFileName(
            nullptr, "Select SEDS file to import InterfaceView from...", QString(), tr("*.xml"));
    // TODO: implementation
}

auto SedsPlugin::importSdl() -> void
{
    const QString inputFilePath =
            QFileDialog::getOpenFileName(nullptr, "Select SEDS file to import SDL from...", QString(), tr("*.xml"));
    // TODO: implementation

    const QString sedsConverterPath =
            QString("%1/bin/sedsconverter")
                    .arg(QProcessEnvironment::systemEnvironment().value("SPACECREATOR_BUILD_DIR"));

    QStringList arguments;
    // clang-format off
    arguments << "--from" << "SEDS";
    arguments << "--to" << "SDL";
    arguments << "--skip-validation"; // remove this line
    arguments << "-i" << inputFilePath;
    arguments << "--sdl-filepath-prefix" << "work/sdl/";
    // clang-format on

    QProcess sedsConverterProcess;
    sedsConverterProcess.start(sedsConverterPath, arguments);
    if (!sedsConverterProcess.waitForStarted()) {
        qDebug() << "SedsConverter could not not started";
    } else if (sedsConverterProcess.waitForFinished()) {
        const QByteArray sedsConverterOutput =
                sedsConverterProcess.read(512 * 1024 * 1024); // read up to 512 MB of text output
        qDebug() << "sedsConverter output: " << QString::fromStdString(sedsConverterOutput.toStdString());
        qDebug() << "stderr: " << sedsConverterProcess.readAllStandardError();
        qDebug() << "stdout: " << sedsConverterProcess.readAllStandardOutput();
    }
}

auto SedsPlugin::importAsn1() -> void
{
    const QString inputFilePath = QFileDialog::getOpenFileName(
            nullptr, "Select SEDS file to import ASN.1 and ACN from...", QString(), tr("*.xml"));

    // TODO: implementation
}

auto SedsPlugin::exportInterfaceView() -> void
{
    auto *const currentDocument = Core::EditorManager::currentDocument();
    auto *const currentIvDocument = static_cast<IVEditorDocument *>(currentDocument);
    if (currentIvDocument == nullptr) {
        qWarning() << "InterfaceView file not selected";
        return;
        // TODO: Report warning in QTC IF
    }

    const auto ivEditorCore = currentIvDocument->ivEditorCore();
    const auto ivFunctionsNames = ivEditorCore->ivFunctionsNames();
    if (ivFunctionsNames.empty()) {
        qWarning() << "InterfaceView does not contain functions which could be exported";
        return;
        // TODO: Report warning in QTC IF
    }

    QStandardItemModel functionsListModel;
    updateModelWithFunctionNames(functionsListModel, ivFunctionsNames);

    ListTreeDialog ldDialog(&functionsListModel, "Export to SEDS", [&]() {
        QStandardItemModel *const model = ldDialog.model();
        const unsigned int rows = model->rowCount();
        const unsigned int cols = model->columnCount();

        for (unsigned int i = 0; i < cols; i++) {
            for (unsigned int j = 0; j < rows; j++) {
                QStandardItem *const item = model->takeItem(j, i);
                if (item != nullptr && item->checkState() == Qt::Checked) {
                    ldDialog.selectedItems()->append(item->text());
                }
            }
        }

        ldDialog.close();
    });
    ldDialog.setWindowTitle("IV functions to be exported");

    ldDialog.exec();

    QList<QString> *const selectedFunctions = ldDialog.selectedItems();
    if (!selectedFunctions->empty()) {
        for (auto &item : *selectedFunctions) {
            qDebug() << "Selected function: " << item;
        }

        QString outputDir = QFileDialog::getExistingDirectory(nullptr, "Select destination directory");
        qDebug() << "Selected directory: " << outputDir;
        // TODO: implementation
    } else {
        qWarning() << "No functions selected to export";
        return;
        // TODO: Report warning in QTC IF
    }
}

auto SedsPlugin::exportAsn1() -> void
{
    const auto names = QFileDialog::getOpenFileNames(
            nullptr, "Select ASN.1 and ACN files to export to SEDS", QString(), "*.asn *.acn");

    const auto outputDir = QFileDialog::getExistingDirectory(nullptr, "Select destination directory");

    for (auto &name : names) {
        (void)name;
        (void)outputDir;
        // TODO: implementation
    }
}
}
