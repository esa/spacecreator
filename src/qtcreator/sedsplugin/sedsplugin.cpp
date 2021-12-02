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
#include "conversion/converter/exceptions.h"
#include "export/exceptions.h"
#include "import/exceptions.h"
#include "interfacedocument.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "model.h"
#include "modeltype.h"
#include "options.h"
#include "sdl/SdlOptions/options.h"
#include "seds/SedsOptions/options.h"
#include "sharedlibrary.h"

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <asn1library/asn1/asn1model.h>
#include <conversion/asn1/Asn1Importer/importer.h>
#include <conversion/asn1/Asn1Registrar/registrar.h>
#include <conversion/converter/converter.h>
#include <conversion/iv/IvRegistrar/registrar.h>
#include <conversion/sdl/SdlRegistrar/registrar.h>
#include <conversion/seds/SedsRegistrar/registrar.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/icore.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>
#include <messagemanager.h>
#include <shared/ui/listtreedialog.h>

using namespace Core;
using conversion::Converter;
using conversion::ModelType;
using conversion::RegistrationFailedException;
using conversion::asn1::Asn1Registrar;
using conversion::iv::IvRegistrar;
using conversion::sdl::SdlRegistrar;
using conversion::seds::SedsRegistrar;

/// messages for General Messages GUI
namespace GenMsg {
const QString msgInfo = "INFO: %1";
const QString msgWarning = "WARNING: %1";
const QString msgError = "ERROR: %1";
const QString fileToImportNotSelected = "File to import not selected";
const QString spacecreatorDirEnvVarNotRead = "SPACECREATOR_BUILD_DIR environment variable could not be read";
const QString sedsconverterNotStarted = "SedsConverter could not be started";
const QString ivFileNotSelected = "InterfaceView file not selected";
const QString ivNoFunctionsInIv = "InterfaceView does not contain functions which could be exported";
const QString ivNoFunctionsSelected = "No functions selected to export";
const QString conversionFinished = "Conversion finished";
}

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

    initializeRegistry();
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
    Context allContexts(Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    ActionContainer *const acToolsSeds = createActionContainerInTools(tr("&Electronic Data Sheet"));

    const auto ivImportAction = new QAction(tr("Import InterfaceView from EDS"), this);
    connect(ivImportAction, &QAction::triggered, [=]() { this->importInterfaceView(); });
    Command *const ivImport = ActionManager::registerAction(ivImportAction, Constants::IV_IMPORT_ID, allContexts);
    acToolsSeds->addAction(ivImport);

    const auto sdlImportAction = new QAction(tr("Import SDL from EDS"), this);
    connect(sdlImportAction, &QAction::triggered, [=]() { this->importSdl(); });
    Command *const sdlImport = ActionManager::registerAction(sdlImportAction, Constants::SDL_IMPORT_ID, allContexts);
    acToolsSeds->addAction(sdlImport);

    const auto asn1ImportAction = new QAction(tr("Import ASN.1 from EDS"), this);
    connect(asn1ImportAction, &QAction::triggered, [=]() { this->importAsn1(); });
    Command *const asn1Import = ActionManager::registerAction(asn1ImportAction, Constants::ASN1_IMPORT_ID, allContexts);
    acToolsSeds->addAction(asn1Import);

    const auto ivExportAction = new QAction(tr("Export InterfaceView to EDS"), this);
    connect(ivExportAction, &QAction::triggered, [=]() { this->exportInterfaceView(); });
    Command *const ivExport = ActionManager::registerAction(ivExportAction, Constants::IV_EXPORT_ID, allContexts);
    acToolsSeds->addAction(ivExport);

    const auto asn1ExportAction = new QAction(tr("Export ASN.1 to EDS"), this);
    connect(asn1ExportAction, &QAction::triggered, [=]() { this->exportAsn1(); });
    Command *const asn1Export = ActionManager::registerAction(asn1ExportAction, Constants::ASN1_EXPORT_ID, allContexts);
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
            nullptr, "Select EDS file to import InterfaceView from...", QString(), tr("*.xml"));
    if (inputFilePath.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileToImportNotSelected));
        return;
    }

    // TODO: implementation
}

auto SedsPlugin::importSdl() -> void
{
    const QString inputFilePath =
            QFileDialog::getOpenFileName(nullptr, "Select EDS file to import SDL from...", QString(), tr("*.xml"));
    if (inputFilePath.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileToImportNotSelected));
        return;
    }

    // TODO: implementation
}

auto SedsPlugin::importAsn1() -> void
{
    const QString inputFilePath = QFileDialog::getOpenFileName(
            nullptr, "Select EDS file to import ASN.1 and ACN from...", QString(), tr("*.xml"));
    if (inputFilePath.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileToImportNotSelected));
        return;
    }

    conversion::Options options;
    options.add(conversion::seds::SedsOptions::inputFilepath, inputFilePath);
    options.add(conversion::asn1::Asn1Options::asn1FilepathPrefix, "");
    options.add(conversion::asn1::Asn1Options::acnFilepathPrefix, "");
    try {
        auto srcModelTypes = std::set<conversion::ModelType>({ conversion::ModelType::Seds });
        auto targetModelType = conversion::ModelType::Asn1;
        auto auxModelTypes = std::set<conversion::ModelType>({});
        Converter converter(m_registry, std::move(options));
        converter.convert(srcModelTypes, targetModelType, auxModelTypes);
        MessageManager::write(GenMsg::msgInfo.arg("file(s) imported"));
    } catch (conversion::ConverterException &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
    }
}

auto SedsPlugin::exportInterfaceView() -> void
{
    auto *const currentDocument = EditorManager::currentDocument();
    auto *const currentIvDocument = static_cast<IVEditorDocument *>(currentDocument);
    if (currentIvDocument == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivFileNotSelected));
        return;
    }

    const auto ivEditorCore = currentIvDocument->ivEditorCore();
    const auto ivFunctionsNames = ivEditorCore->ivFunctionsNames();
    if (ivFunctionsNames.empty()) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivNoFunctionsInIv));
        return;
    }

    QStandardItemModel functionsListModel;
    updateModelWithFunctionNames(functionsListModel, ivFunctionsNames);

    ListTreeDialog ldDialog(&functionsListModel, "Export to EDS", [&]() {
        QStandardItemModel *const model = ldDialog.model();
        const auto rows = model->rowCount();
        const auto cols = model->columnCount();

        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
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
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivNoFunctionsSelected));
        return;
    }
}

auto SedsPlugin::exportAsn1() -> void
{
    const auto names = QFileDialog::getOpenFileNames(
            nullptr, "Select ASN.1 and ACN files to export to EDS", QString(), "*.asn *.acn");
    if (names.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileToImportNotSelected));
        return;
    }

    QStringList asn1Names;
    std::copy_if(names.begin(), names.end(), std::back_inserter(asn1Names),
            [](const QString &name) -> bool { return name.endsWith(".asn"); });
    asn1Names.sort();

    QStringList acnNames;
    std::copy_if(names.begin(), names.end(), std::back_inserter(acnNames),
            [](const QString &name) -> bool { return name.endsWith(".acn"); });
    acnNames.sort();

    if (asn1Names.size() != acnNames.size()) {
        MessageManager::write(GenMsg::msgError.arg(".asn or .acn file missing!"));
        return;
    }

    const auto outputDir = QFileDialog::getExistingDirectory(nullptr, "Select destination directory");

    const auto getFileName = [](const auto &fullpath) -> QString {
        QString name = fullpath.split("/").last();
        name = name.split(".").first();

        return name;
    };

    for (unsigned int i = 0; i < asn1Names.size(); i++) {
        conversion::Options options;
        options.add(conversion::asn1::Asn1Options::inputFilepath, asn1Names[i]);
        options.add(conversion::asn1::Asn1Options::inputFilepath, acnNames[i]);
        options.add(conversion::seds::SedsOptions::outputFilepath,
                QString("%1/%2.xml").arg(outputDir).arg(getFileName(acnNames[i])));
        try {
            auto srcModelTypes = std::set<conversion::ModelType>({ conversion::ModelType::Asn1 });
            auto targetModelType = conversion::ModelType::Seds;
            auto auxModelTypes = std::set<conversion::ModelType>({});
            Converter converter(m_registry, std::move(options));
            converter.convert(srcModelTypes, targetModelType, auxModelTypes);
            MessageManager::write(GenMsg::msgInfo.arg("file(s) exported"));
        } catch (conversion::ConverterException &ex) {
            MessageManager::write(GenMsg::msgError.arg(ex.what()));
        }
    }
}

void SedsPlugin::initializeRegistry()
{
    Asn1Registrar asn1Registrar;
    if (!asn1Registrar.registerCapabilities(m_registry)) {
        throw RegistrationFailedException(ModelType::Asn1);
    }

    IvRegistrar ivRegistrar;
    if (!ivRegistrar.registerCapabilities(m_registry)) {
        throw RegistrationFailedException(ModelType::InterfaceView);
    }

    SedsRegistrar sedsRegistrar;
    if (!sedsRegistrar.registerCapabilities(m_registry)) {
        throw RegistrationFailedException(ModelType::Seds);
    }

    SdlRegistrar sdlRegistrar;
    if (!sdlRegistrar.registerCapabilities(m_registry)) {
        throw RegistrationFailedException(ModelType::Sdl);
    }
}

}
