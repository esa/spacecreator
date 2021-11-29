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

#include "context/action/actionsmanager.h"
#include "conversion/asn1/Asn1Options/options.h"
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
#include <asn1library/asn1/asn1model.h>
#include <conversion/asn1/Asn1Importer/importer.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/icore.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>
#include "../spacecreatorplugin/iv/iveditordata.h"
#include "../spacecreatorplugin/iv/iveditordocument.h"
#include "../spacecreatorplugin/iv/iveditorfactory.h"
#include "../spacecreatorplugin/iv/ivqtceditor.h"
#include <shared/ui/listtreedialog.h>

using namespace Core;

void updateModelWithFunctionNames(QStandardItemModel &model, const QStringList &ivFunctionsNames)
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

namespace spctr {

SedsPlugin::SedsPlugin() {}

SedsPlugin::~SedsPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool SedsPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    addSedsImportExport();

    return true;
}

void SedsPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag SedsPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void SedsPlugin::addSedsImportExport()
{
    Core::Context allContexts(
            Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    ActionContainer *const acToolsSeds = createActionContainerInTools(tr("&SEDS"));

    const auto ivImportAction = new QAction(tr("Import InterfaceView"), this);
    connect(ivImportAction, &QAction::triggered, this, importInterfaceView);
    Core::Command *ivImport = Core::ActionManager::registerAction(ivImportAction, Constants::IV_IMPORT_ID, allContexts);
    acToolsSeds->addAction(ivImport);

    const auto sdlImportAction = new QAction(tr("Import SDL"), this);
    connect(sdlImportAction, &QAction::triggered, this, importSdl);
    Core::Command *sdlImport =
            Core::ActionManager::registerAction(sdlImportAction, Constants::SDL_IMPORT_ID, allContexts);
    acToolsSeds->addAction(sdlImport);

    const auto asn1ImportAction = new QAction(tr("Import ASN.1"), this);
    connect(asn1ImportAction, &QAction::triggered, this, importAsn1);
    Core::Command *asn1Import =
            Core::ActionManager::registerAction(asn1ImportAction, Constants::ASN1_IMPORT_ID, allContexts);
    acToolsSeds->addAction(asn1Import);

    const auto ivExportAction = new QAction(tr("Export InterfaceView"), this);
    connect(ivExportAction, &QAction::triggered, this, exportInterfaceView);
    Core::Command *ivExport = Core::ActionManager::registerAction(ivExportAction, Constants::IV_EXPORT_ID, allContexts);
    acToolsSeds->addAction(ivExport);

    const auto asn1ExportAction = new QAction(tr("Export ASN.1"), this);
    connect(asn1ExportAction, &QAction::triggered, this, exportAsn1);
    Core::Command *asn1Export =
            Core::ActionManager::registerAction(asn1ExportAction, Constants::ASN1_EXPORT_ID, allContexts);
    acToolsSeds->addAction(asn1Export);
}

ActionContainer *SedsPlugin::createActionContainerInTools(const QString &title)
{
    ActionContainer *const acToolsSeds = ActionManager::createMenu(Constants::M_TOOLS_SEDS);
    QMenu *const menuToolsSeds = acToolsSeds->menu();
    menuToolsSeds->setTitle(title);
    menuToolsSeds->setEnabled(true);

    ActionContainer *const acTools = ActionManager::actionContainer(Core::Constants::M_TOOLS);
    acTools->addMenu(acToolsSeds);

    return acToolsSeds;
}

void SedsPlugin::importInterfaceView()
{
    const QString inputFilePath = QFileDialog::getOpenFileName(
            nullptr, "Select SEDS file to import InterfaceView from...", QString(), tr("*.xml"));

    // TODO: implementation
}

void SedsPlugin::importSdl()
{
    const QString inputFilePath =
            QFileDialog::getOpenFileName(nullptr, "Select SEDS file to import SDL from...", QString(), tr("*.xml"));

    // TODO: implementation
}

void SedsPlugin::importAsn1()
{
    const QString inputFilePath =
            QFileDialog::getOpenFileName(nullptr, "Select SEDS file to import ASN.1 from...", QString(), tr("*.xml"));

    conversion::Options options;
    options.add(conversion::asn1::Asn1Options::inputFilepath, inputFilePath);

    conversion::asn1::importer::Asn1Importer asn1importer;
    try {
        const auto model = asn1importer.importModel(options);
        if (model) {
            const auto *asn1Model = dynamic_cast<Asn1Acn::Asn1Model *>(model.get());
            if (asn1Model) {
                const auto &files = asn1Model->data();
                if (files.size() != 1) {
                    qWarning() << "No data in imported file";
                    return;
                }
                // TODO: implementation
            }
        }
    } catch (conversion::importer::ImportException &ex) {
        // TODO: write an exception to a debug console
    }
}

void SedsPlugin::exportInterfaceView()
{
    auto *const currentDocument = Core::EditorManager::currentDocument();
    auto *const currentIvDocument = static_cast<IVEditorDocument *>(currentDocument);
    if (currentIvDocument == nullptr) {
        qWarning() << "InterfaceView file not selected";
        return;
    }

    const auto ivEditorCore = currentIvDocument->ivEditorCore();
    const auto ivFunctionsNames = ivEditorCore->ivFunctionsNames();
    if (ivFunctionsNames.empty()) {
        qWarning() << "InterfaceView does not contain functions which could be exported";
        return;
    }

    QStandardItemModel functionsListModel;
    updateModelWithFunctionNames(functionsListModel, ivFunctionsNames);

    ListTreeDialog ldDialog(&functionsListModel, "export to SEDS", [&]() {
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
            qDebug() << "selected function: " << item;
        }

        QString outputDir = QFileDialog::getExistingDirectory(nullptr, "Select destination directory");
        qDebug() << "selected directory: " << outputDir;

        // TODO: implementation (take selected functions and export them to a file in a selected directory
        //       using sedsConverter)
    } else {
        qWarning() << "No functions selected to export";
        return;
        // TODO: throw unhandled exception or write a warning visible to user and just return?
    }
}

void SedsPlugin::exportAsn1()
{
    const auto names = QFileDialog::getOpenFileNames(
            nullptr, "Select ASN.1 and ACN files to export to SEDS", QString(), "*.asn *.acn");

    const auto outputDir = QFileDialog::getExistingDirectory(nullptr, "Select destination directory");

    for (auto &name : names) {
        // TODO: implementation (call sedsConverter, from asn1 to seds)
        (void)name;
        (void)outputDir;
    }
}

}
