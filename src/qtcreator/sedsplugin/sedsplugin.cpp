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
#include "conversion/iv/IvOptions/options.h"
#include "exceptions.h"
#include "export/exceptions.h"
#include "fileutils.h"
#include "graphicsviewutils.h"
#include "import/exceptions.h"
#include "interfacedocument.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "ivfunction.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivobject.h"
#include "model.h"
#include "modeltype.h"
#include "options.h"
#include "projectexplorer.h"
#include "projectnodes.h"
#include "sdl/SdlOptions/options.h"
#include "seds/SedsOptions/options.h"
#include "seds/SedsXmlImporter/importer.h"
#include "sedsmodel.h"
#include "sharedlibrary.h"

#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <algorithm>
#include <asn1library/asn1/asn1model.h>
#include <conversion/asn1/Asn1Importer/importer.h>
#include <conversion/asn1/Asn1Registrar/registrar.h>
#include <conversion/converter/converter.h>
#include <conversion/iv/IvRegistrar/registrar.h>
#include <conversion/iv/IvXmlImporter/importer.h>
#include <conversion/sdl/SdlRegistrar/registrar.h>
#include <conversion/seds/SedsRegistrar/registrar.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/icore.h>
#include <editormanager/editormanager.h>
#include <editormanager/ieditor.h>
#include <exception>
#include <memory>
#include <messagemanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>
#include <qdir.h>
#include <variant>

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
const QString ivFileNotSelected = "InterfaceView file not selected";
const QString ivNoFunctionsInIv = "InterfaceView does not contain functions which could be exported";
const QString ivNoFunctionsSelected = "No functions selected to export";
const QString filesExported = "File(s) exported";
const QString filesImported = "File(s) imported";
const QString functionsImported = "Function(s) imported";
const QString ivModelNotRead = "IV model could not be read";
const QString ivTmpModelNotRead = QString("Temporary %1").arg(ivModelNotRead);
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

auto SedsPlugin::itemModelUpdateWithFunctionNames(QStandardItemModel &model, const QStringList &ivFunctionsNames)
        -> void
{
    QStandardItemModel *const functionsListModel = &model;

    QList<QStandardItem *> ivFunctionNamesList;
    for (auto &name : ivFunctionsNames) {
        QStandardItem *const item = new QStandardItem(name);
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
    const QString tmpIvFilename = "tmp-interfaceview.xml";
    const QString ivConfig = "config.xml";

    const QString inputFilePath = QFileDialog::getOpenFileName(
            nullptr, "Select EDS file to import InterfaceView from...", QString(), tr("*.xml"));
    if (inputFilePath.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileToImportNotSelected));
        return;
    }

    conversion::Options options;
    options.add(conversion::iv::IvOptions::outputFilepath,
            QString("%1%2%3").arg(QDir::currentPath()).arg(QDir::separator()).arg(tmpIvFilename));
    options.add(conversion::iv::IvOptions::configFilepath,
            QString("%1%2%3").arg(QDir::currentPath()).arg(QDir::separator()).arg(ivConfig));
    options.add(conversion::seds::SedsOptions::inputFilepath, inputFilePath);

    try {
        convert({ conversion::ModelType::Seds }, conversion::ModelType::InterfaceView, { conversion::ModelType::Asn1 },
                options);
    } catch (conversion::ConverterException &ex) {
        MessageManager::write(GenMsg::msgWarning.arg(ex.what()));
        QFile(tmpIvFilename).remove();
        return;
    } catch (std::exception &ex) {
        MessageManager::write(GenMsg::msgWarning.arg(ex.what()));
        QFile(tmpIvFilename).remove();
        return;
    }

    if (!loadAndMergeIvModelIntoCurrent(ivConfig, tmpIvFilename)) {
        return;
    }

    MessageManager::write(GenMsg::msgInfo.arg(GenMsg::functionsImported));

    QFile(tmpIvFilename).remove();
}

auto SedsPlugin::getPackagenameFromPackageFile(const seds::model::PackageFile &file) -> QString
{
    QString name;

    auto &package = file.package();
    auto &components = package.components();
    if (components.empty()) {
        return "";
    }
    auto &firstComponent = components.front();
    auto &componentNameType = firstComponent.name();
    name = componentNameType.value();

    return name;
}

auto SedsPlugin::getPackagenameFromDatasheet(const seds::model::DataSheet &datasheet) -> QString
{
    QString name;

    auto &packages = datasheet.packages();
    if (packages.empty()) {
        return "";
    }
    auto &firstPackage = packages.front();
    name = firstPackage.nameStr();

    return name;
}

auto SedsPlugin::getPackagenameFromSedsModel(const QString &inputFilePath) -> QString
{
    const std::unique_ptr<conversion::Model> sedsModel = loadSedsModel(inputFilePath);
    seds::model::SedsModel *const sedsModelPtr = dynamic_cast<seds::model::SedsModel *>(sedsModel.get());
    if (sedsModelPtr == nullptr) {
        MessageManager::write(GenMsg::msgError.arg("SEDS model could not be read"));
        return "";
    }

    QString packageName;
    auto &variantData = sedsModelPtr->data();
    if (std::holds_alternative<seds::model::PackageFile>(variantData)) {
        packageName = getPackagenameFromPackageFile(std::get<seds::model::PackageFile>(variantData));
    } else if (std::holds_alternative<seds::model::DataSheet>(variantData)) {
        packageName = getPackagenameFromDatasheet(std::get<seds::model::DataSheet>(variantData));
    } else {
        MessageManager::write(GenMsg::msgError.arg("Selected file does not contain valid SEDS model"));
        return "";
    }

    return packageName;
}

auto SedsPlugin::importSdl() -> void
{
    const QString tmpIvFilename = "tmp-interfaceview.xml";
    const QString ivConfig = "config.xml";

    const QString inputFilePath =
            QFileDialog::getOpenFileName(nullptr, "Select EDS file to import SDL from...", QString(), tr("*.xml"));
    if (inputFilePath.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileToImportNotSelected));
        return;
    }

    const QString packageName = getPackagenameFromSedsModel(inputFilePath);
    if (packageName == "") {
        return;
    }

    const QString prefix = QString("work/%1/SDL/src/").arg(packageName.toLower());
    if (!QDir().mkpath(prefix)) {
        MessageManager::write(GenMsg::msgError.arg(QString("Could not create path %1").arg(prefix)));
        return;
    }

    conversion::Options options;
    options.add(conversion::sdl::SdlOptions::filepathPrefix, prefix);
    options.add(conversion::seds::SedsOptions::inputFilepath, inputFilePath);
    options.add(conversion::iv::IvOptions::outputFilepath, tmpIvFilename);
    options.add(conversion::iv::IvOptions::configFilepath, ivConfig);

    // TODO: get generated asn/acn file names
    const QStringList fileNames = {
        QString("%1%2GUARD.asn").arg(QDir::currentPath()).arg(QDir::separator()),
        QString("%1%2GUARD.acn").arg(QDir::currentPath()).arg(QDir::separator()),
    };
    for (const auto &filename : fileNames) {
        MessageManager::write(filename);
    };
    try {
        convert({ conversion::ModelType::Seds }, conversion::ModelType::Sdl,
                { conversion::ModelType::InterfaceView, conversion::ModelType::Asn1 }, options);
    } catch (std::exception &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
        QFile(tmpIvFilename).remove();
        return;
    }

    ProjectExplorer::Project *const project = ProjectExplorer::ProjectTree::currentProject();
    project->rootProjectNode()->addFiles(fileNames);

    if (!loadAndMergeIvModelIntoCurrent(ivConfig, tmpIvFilename)) {
        return;
    }

    MessageManager::write(GenMsg::msgInfo.arg(GenMsg::filesImported));

    QFile(tmpIvFilename).remove();
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
        const auto srcModelTypes = std::set<conversion::ModelType>({ conversion::ModelType::Seds });
        const auto targetModelType = conversion::ModelType::Asn1;
        const auto auxModelTypes = std::set<conversion::ModelType>({});
        Converter converter(m_registry, std::move(options));
        converter.convert(srcModelTypes, targetModelType, auxModelTypes);
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::filesImported));
    } catch (conversion::ConverterException &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
    } catch (conversion::FileNotFoundException &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
    } catch (std::exception &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
    }
}

auto SedsPlugin::exportInterfaceView() -> void
{
    const auto ivEditorCore = getCurIvEditorCore();
    const auto ivFunctionsNames = ivEditorCore->ivFunctionsNames();
    if (ivFunctionsNames.empty()) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivNoFunctionsInIv));
        return;
    }

    QStandardItemModel functionsListModel;
    itemModelUpdateWithFunctionNames(functionsListModel, ivFunctionsNames);

    ListTreeDialog ltDialog;
    ltdialogUpdateWithItemModel(ltDialog, &functionsListModel);

    ltDialog.exec();

    QList<QString> *const selectedFunctions = ltDialog.selectedItems();
    if (selectedFunctions->empty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::ivNoFunctionsSelected));
        return;
    }

    const QString outputDir = QFileDialog::getExistingDirectory(nullptr, "Select destination directory");

    const QVector<ivm::IVFunction *> allIvFunctions = ivEditorCore->allIVFunctions();
    if (!allIvFunctions.isEmpty()) {
        ivm::IVFunction *const ivFunction = allIvFunctions[0];
        ivm::IVModel *const ivModel = ivFunction->model();
        if (ivModel != nullptr) {
            conversion::Options options;
            options.add(conversion::iv::IvOptions::inputFilepath,
                    QString("%1%2%3").arg(QDir::currentPath()).arg(QDir::separator()).arg("interfaceview.xml"));
            options.add(conversion::iv::IvOptions::configFilepath,
                    QString("%1%2%3").arg(QDir::currentPath()).arg(QDir::separator()).arg("config.xml"));
            options.add(conversion::seds::SedsOptions::outputFilepath,
                    QString("%1%2%3.xml").arg(outputDir).arg(QDir::separator()).arg("seds_interfaceview"));
            for (auto &selectedFunction : *selectedFunctions) {
                options.add(conversion::iv::IvOptions::functionToConvert, selectedFunction);
            }

            try {
                convert({ conversion::ModelType::InterfaceView }, conversion::ModelType::Seds, {}, options);
                MessageManager::write(GenMsg::msgInfo.arg(GenMsg::filesExported));
            } catch (conversion::ConverterException &ex) {
                MessageManager::write(GenMsg::msgWarning.arg(ex.what()));
            } catch (std::exception &ex) {
                MessageManager::write(GenMsg::msgWarning.arg(ex.what()));
            }

        } else {
            MessageManager::write(GenMsg::msgError.arg(GenMsg::ivModelNotRead));
        }
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
        QString name = fullpath.split(QDir::separator()).last();
        name = name.split(".").first();

        return name;
    };

    for (unsigned int i = 0; i < asn1Names.size(); i++) {
        conversion::Options options;
        options.add(conversion::asn1::Asn1Options::inputFilepath, asn1Names[i]);
        options.add(conversion::asn1::Asn1Options::inputFilepath, acnNames[i]);
        options.add(conversion::seds::SedsOptions::outputFilepath,
                QString("%1%2%3.xml").arg(outputDir).arg(QDir::separator()).arg(getFileName(asn1Names[i])));
        try {
            const auto srcModelTypes = std::set<conversion::ModelType>({ conversion::ModelType::Asn1 });
            const auto targetModelType = conversion::ModelType::Seds;
            const auto auxModelTypes = std::set<conversion::ModelType>({});
            Converter converter(m_registry, std::move(options));
            converter.convert(srcModelTypes, targetModelType, auxModelTypes);
            MessageManager::write(GenMsg::msgInfo.arg(GenMsg::filesExported));
        } catch (conversion::ConverterException &ex) {
            MessageManager::write(GenMsg::msgError.arg(ex.what()));
        } catch (std::exception &ex) {
            MessageManager::write(GenMsg::msgError.arg(ex.what()));
        }
    }
}

auto SedsPlugin::initializeRegistry() -> void
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

auto SedsPlugin::convert(const std::set<conversion::ModelType> &srcModelType,
        const conversion::ModelType targetModelType, const std::set<conversion::ModelType> &auxModelTypes,
        conversion::Options options) -> void
{
    Converter converter(m_registry, std::move(options));
    converter.convert(srcModelType, targetModelType, auxModelTypes);
}

auto SedsPlugin::ltdialogUpdateWithItemModel(ListTreeDialog &ltdialog, QStandardItemModel *model) -> void
{
    ltdialog.setModel(model);
    ltdialog.setButtonText("Export to EDS");
    ltdialog.setButtonHandler([&]() {
        QStandardItemModel *const model = ltdialog.model();
        const auto rows = model->rowCount();
        const auto cols = model->columnCount();

        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                QStandardItem *const item = model->takeItem(j, i);
                if (item != nullptr && item->checkState() == Qt::Checked) {
                    ltdialog.selectedItems()->append(item->text());
                }
            }
        }

        ltdialog.close();
    });
    ltdialog.setWindowTitle("IV functions to be exported");
}

auto SedsPlugin::getCurIvEditorCore() -> IVEditorCorePtr
{
    auto *const currentDocument = EditorManager::currentDocument();
    auto *const currentIvDocument = static_cast<IVEditorDocument *>(currentDocument);
    if (currentIvDocument == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivFileNotSelected));
        return nullptr;
    }

    return currentIvDocument->ivEditorCore();
}

auto SedsPlugin::mergeIvModels(ivm::IVModel *const dstIvModel, ivm::IVModel *const srcIvModel) -> void
{
    for (auto &srcIvObject : srcIvModel->visibleObjects()) {
        auto *const srcFunction = dynamic_cast<ivm::IVFunction *>(srcIvObject);
        if (srcFunction == nullptr) {
            continue;
        } else if (doesModelContainFunction(dstIvModel, srcFunction)) {
            MessageManager::write(GenMsg::msgInfo.arg(
                    QString("%1 - names are the same, Function will not be imported").arg(srcIvObject->title())));
        } else {
            addFunctionToModel(dynamic_cast<ivm::IVFunction *>(srcIvObject), dstIvModel);
        }
    }
}

auto SedsPlugin::doesModelContainFunction(ivm::IVModel *const model, ivm::IVFunction *const function) -> bool
{
    const auto isObjectTitleEqualFunctionTitle = [&function](ivm::IVObject *const obj) -> bool {
        if (obj == nullptr) {
            return false;
        } else {
            return obj->isFunction() && obj->title() == function->title();
        }
    };

    for (ivm::IVObject *const obj : model->visibleObjects()) {
        if (isObjectTitleEqualFunctionTitle(obj)) {
            return true;
        }
    }

    return false;
}

auto SedsPlugin::loadIvModel(const QString &ivConfigFilename, const QString &ivFilename)
        -> std::unique_ptr<conversion::Model>
{
    std::unique_ptr<conversion::Model> model;

    conversion::Options options;
    options.add(conversion::iv::IvOptions::inputFilepath, ivFilename);
    options.add(conversion::iv::IvOptions::configFilepath, ivConfigFilename);

    conversion::iv::importer::IvXmlImporter ivImporter;
    try {
        model = ivImporter.importModel(options);
    } catch (const std::exception &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
        return nullptr;
    }

    return model;
}

auto SedsPlugin::loadSedsModel(const QString &sedsFilename) -> std::unique_ptr<conversion::Model>
{
    std::unique_ptr<conversion::Model> model;

    conversion::Options options;
    options.add(conversion::seds::SedsOptions::inputFilepath, sedsFilename);

    seds::importer::SedsXmlImporter sedsImporter;
    try {
        model = sedsImporter.importModel(options);
    } catch (const std::exception &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
        return nullptr;
    }

    return model;
}

auto SedsPlugin::getCurrentIvModel() -> ivm::IVModel *
{
    const auto curIvEditorCore = getCurIvEditorCore();
    if (curIvEditorCore == nullptr) {
        MessageManager::write(GenMsg::msgError.arg("IV Editor core could not be read"));
        return nullptr;
    }

    const auto curIvEditorCoreDocument = curIvEditorCore->document();
    if (curIvEditorCoreDocument == nullptr) {
        MessageManager::write(GenMsg::msgError.arg("No document in current IV Editor core"));
        return nullptr;
    }

    return curIvEditorCoreDocument->objectsModel();
}

auto SedsPlugin::loadAndMergeIvModelIntoCurrent(const QString &ivConfig, const QString &ivFilename) -> bool
{
    std::unique_ptr<conversion::Model> model = loadIvModel(ivConfig, ivFilename);

    ivm::IVModel *const tmpIvModel = dynamic_cast<ivm::IVModel *>(model.get());
    if (tmpIvModel == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivTmpModelNotRead));
        return false;
    }

    ivm::IVModel *const currentIvModel = getCurrentIvModel();
    if (currentIvModel == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivModelNotRead));
        return false;
    }

    mergeIvModels(currentIvModel, tmpIvModel);

    return true;
}

auto SedsPlugin::addFunctionToModel(ivm::IVFunction *const srcFun, ivm::IVModel *const model) -> void
{
    if (srcFun == nullptr) {
        MessageManager::write(GenMsg::msgError.arg("Source IV function could not be read"));
        return;
    }

    const auto curIvEditorCore = getCurIvEditorCore();
    if (curIvEditorCore == nullptr) {
        MessageManager::write(GenMsg::msgError.arg("Current IV Editor Core could not be read"));
        return;
    }

    ivm::IVFunction *const dstFun = curIvEditorCore->addFunction(srcFun->title(), nullptr);
    if (dstFun == nullptr) {
        MessageManager::write(GenMsg::msgError.arg("Destination IV function could not be created"));
        return;
    }

    QString srcFunDefaultImplementation = srcFun->defaultImplementation();
    dstFun->setDefaultImplementation(srcFunDefaultImplementation);

    for (ivm::IVInterface *const srcIf : srcFun->interfaces()) {
        ivm::IVInterface *const dstIf = curIvEditorCore->addInterface(srcIf->title(), dstFun->title());
        dstIf->setKind(srcIf->kind());
        dstIf->setDirection(srcIf->direction());
        dstIf->setParams(srcIf->params());
    }
}

} // namespace spctr
