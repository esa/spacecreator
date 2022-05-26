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

#include "functiontesterplugin.h"
#include "ftpluginconstants.h"

#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QMenu>
#include <algorithm>
#include <asn1library/asn1/asn1model.h>
#include <context/action/actionsmanager.h>
#include <conversion/asn1/Asn1Exporter/exporter.h>
#include <conversion/asn1/Asn1Importer/importer.h>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/asn1/Asn1Registrar/registrar.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/model.h>
#include <conversion/common/options.h>
#include <conversion/common/translation/translator.h>
#include <conversion/converter/converter.h>
#include <conversion/converter/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
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
#include <ivcore/ivfunction.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivobject.h>
#include <libiveditor/interfacedocument.h>
#include <libiveditor/iveditor.h>
#include <libiveditor/iveditorcore.h>
#include <memory>
#include <messagemanager.h>
#include <messagestrings.h>
#include <modelloader.h>
#include <modeltype.h>
#include <projectexplorer.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>
#include <projectnodes.h>
#include <sdl/SdlExporter/exporter.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <sdl/SdlOptions/options.h>
#include <seds/SedsModel/sedsmodel.h>
#include <seds/SedsOptions/options.h>
#include <seds/SedsXmlExporter/exporter.h>
#include <seds/SedsXmlImporter/importer.h>
#include <shared/common.h>
#include <shared/entityattribute.h>
#include <shared/graphicsviewutils.h>
#include <shared/sharedlibrary.h>
#include <spacecreatorplugin/iv/iveditordata.h>
#include <spacecreatorplugin/iv/iveditordocument.h>
#include <spacecreatorplugin/iv/iveditorfactory.h>
#include <spacecreatorplugin/iv/ivqtceditor.h>
#include <utils/fileutils.h>

using namespace Core;
using conversion::Converter;
using conversion::ModelType;
using conversion::RegistrationFailedException;
using conversion::asn1::Asn1Registrar;
using conversion::iv::IvRegistrar;
using conversion::sdl::SdlRegistrar;
using conversion::seds::SedsRegistrar;
using plugincommon::ModelLoader;

namespace spctr {

FunctionTesterPlugin::FunctionTesterPlugin()
{
    // Do not remove
}

FunctionTesterPlugin::~FunctionTesterPlugin()
{
    // Do not remove
}

auto FunctionTesterPlugin::initialize(const QStringList &arguments, QString *errorString) -> bool
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    // initializeRegistry();
    addTestInterfaceOption();

    return true;
}

auto FunctionTesterPlugin::extensionsInitialized() -> void
{
    // Do not remove
}

auto FunctionTesterPlugin::aboutToShutdown() -> ExtensionSystem::IPlugin::ShutdownFlag
{
    return SynchronousShutdown;
}

auto FunctionTesterPlugin::addTestInterfaceOption() -> void
{
    Context allContexts(Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    ActionContainer *const acToolsFunctionTester = createActionContainerInTools(tr("&Test Interface"));

    const auto csvImportAction = new QAction(tr("Import test vectors from CSV"), this);
    connect(csvImportAction, &QAction::triggered, [=]() { this->loadCsv(); });
    Command *const csvImport = ActionManager::registerAction(csvImportAction, Constants::CSV_IMPORT_ID, allContexts);
    acToolsFunctionTester->addAction(csvImport);
}

auto FunctionTesterPlugin::createActionContainerInTools(const QString &title) -> ActionContainer *
{
    ActionContainer *const container = ActionManager::createMenu(Constants::M_TOOLS_FUNCTION_TESTER);
    QMenu *const functionTesterMenu = container->menu();
    functionTesterMenu->setTitle(title);
    functionTesterMenu->setEnabled(true);

    ActionContainer *const tools = ActionManager::actionContainer(Core::Constants::M_TOOLS);
    tools->addMenu(container);

    return container;
}

auto FunctionTesterPlugin::loadCsv() -> void
{
    const QString inputFilePath = QFileDialog::getOpenFileName(
            nullptr, tr("Select CSV file to import test vectors from..."), QString(), tr("*.csv"));
    if (inputFilePath.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileToImportNotSelected));
        return;
    }

    std::unique_ptr<csv::CsvModel> model;
    try {
        model = ModelLoader::loadCsvModel(inputFilePath);
    } catch (std::exception &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
        return;
    }

    MessageManager::write(GenMsg::msgInfo.arg(GenMsg::filesImported));
}

} // namespace spctr
