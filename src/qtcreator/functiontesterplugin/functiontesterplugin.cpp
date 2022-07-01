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
#include "pluginconstants.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QTextBrowser>
#include <coreplugin/actionmanager/actionmanager.h>
#include <editormanager/editormanager.h>
#include <libiveditor/interfacedocument.h>
#include <messagemanager.h>
#include <messagestrings.h>
#include <modelloader.h>
#include <shared/ui/veinteractiveobject.h>
#include <spacecreatorplugin/iv/iveditordocument.h>
#include <testgenerator/testgenerator.h>

using namespace Core;
using namespace testgenerator;

using ive::IVExporter;
using plugincommon::ModelLoader;

namespace spctr {

FunctionTesterPlugin::FunctionTesterPlugin() { }

FunctionTesterPlugin::~FunctionTesterPlugin() { }

auto FunctionTesterPlugin::initialize(const QStringList &arguments, QString *errorString) -> bool
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    addTestInterfaceOption();

    return true;
}

auto FunctionTesterPlugin::extensionsInitialized() -> void { }

auto FunctionTesterPlugin::aboutToShutdown() -> ExtensionSystem::IPlugin::ShutdownFlag
{
    return SynchronousShutdown;
}

auto FunctionTesterPlugin::testUsingDataFromCsvGui() -> void
{
    ivm::IVInterface *interface = getSelectedInterface();
    if (!interface) {
        QMessageBox msgBox;
        msgBox.setText(tr("No interface selected!"));
        msgBox.exec();
        return;
    }
    auto csvModel = loadCsv();
    if (!csvModel) {
        return;
    }
    auto asn1Model = loadAsn1Model();
    if (!asn1Model) {
        return;
    }
    float delta = setDeltaDialog();

    TestGenerator testGenerator(getBaseDirectory());
    testGenerator.testUsingDataFromCsv(*interface, *csvModel, *asn1Model, delta);
    displayResultHtml();
}

auto FunctionTesterPlugin::addTestInterfaceOption() -> void
{
    Context allContexts(Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    ActionContainer *const acToolsFunctionTester = createActionContainerInTools(tr("&Test Interface"));

    const auto csvImportAction = new QAction(tr("Test using data from CSV"), this);
    connect(csvImportAction, &QAction::triggered, [=]() { this->testUsingDataFromCsvGui(); });
    Command *const csvImport = ActionManager::registerAction(csvImportAction, Constants::CSV_IMPORT_ID, allContexts);
    acToolsFunctionTester->addAction(csvImport);
}

auto FunctionTesterPlugin::createActionContainerInTools(const QString &title) -> Core::ActionContainer *
{
    ActionContainer *const container = ActionManager::createMenu(Constants::M_TOOLS_FUNCTION_TESTER);
    QMenu *const functionTesterMenu = container->menu();
    functionTesterMenu->setTitle(title);
    functionTesterMenu->setEnabled(true);

    ActionContainer *const tools = ActionManager::actionContainer(Core::Constants::M_TOOLS);
    tools->addMenu(container);

    return container;
}

auto FunctionTesterPlugin::setDeltaDialog() -> float
{
    float delta = 0.0;
    bool isOk;
    QString text = QInputDialog::getText(nullptr, tr("Set delta"), tr("Max error:"), QLineEdit::Normal, "0.0", &isOk,
            { 0U }, Qt::ImhFormattedNumbersOnly);
    if (isOk && !text.isEmpty()) {
        delta = text.toFloat();
    }
    return delta;
}

auto FunctionTesterPlugin::loadCsv() -> std::unique_ptr<csv::CsvModel>
{
    const QString inputFilePath = QFileDialog::getOpenFileName(
            nullptr, tr("Select CSV file to import test vectors from..."), QString(), tr("*.csv"));
    if (inputFilePath.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileToImportNotSelected));
        return std::unique_ptr<csv::CsvModel> {};
    }

    std::unique_ptr<csv::CsvModel> model;
    try {
        model = ModelLoader::loadCsvModel(inputFilePath);
    } catch (std::exception &ex) {
        MessageManager::write(GenMsg::msgError.arg(ex.what()));
        return model;
    }

    MessageManager::write(GenMsg::msgInfo.arg(GenMsg::filesImported));
    return ModelLoader::loadCsvModel(inputFilePath);
}

auto FunctionTesterPlugin::getSelectedInterface() -> ivm::IVInterface *
{
    IVEditorCorePtr ivEditorCorePtr = getCurrentIvEditorCore();
    auto view = ivEditorCorePtr->chartView();
    if (!view) {
        return nullptr;
    }

    if (auto scene = view->scene()) {
        for (const auto &item : scene->selectedItems()) {
            if (auto iObj = qobject_cast<shared::ui::VEInteractiveObject *>(item->toGraphicsObject())) {
                if (auto entity = iObj->entity() ? iObj->entity()->as<ivm::IVObject *>() : nullptr) {
                    if (entity->isInterface()) {
                        MessageManager::write(GenMsg::msgInfo.arg(entity->title()));
                        return dynamic_cast<ivm::IVInterface *>(entity);
                    }
                }
            }
        }
    }

    return nullptr;
}

auto FunctionTesterPlugin::loadAsn1Model() -> std::unique_ptr<Asn1Acn::Asn1Model>
{
    QString baseDirectory = getBaseDirectory();
    QString workDirectory = baseDirectory + QDir::separator() + "work";
    QString asn1Path = workDirectory + QDir::separator() + "dataview" + QDir::separator() + "dataview-uniq.asn";

    auto modelPtr = std::unique_ptr<Asn1Acn::Asn1Model> {};
    try {
        modelPtr = ModelLoader::loadAsn1Model(asn1Path);
    } catch (...) {
        MessageManager::write(GenMsg::msgInfo.arg(tr("No ASN1 file found. Try to build the project first.")));
    }
    return modelPtr;
}

auto FunctionTesterPlugin::getBaseDirectory() -> QString
{
    QString ivDocumentPath = getCurrentIvEditorCore()->document()->path();
    return QFileInfo(ivDocumentPath).absoluteDir().absolutePath();
}

auto FunctionTesterPlugin::getCurrentIvEditorCore() -> IVEditorCorePtr
{
    auto *const currentDocument = EditorManager::currentDocument();
    auto *const currentIvDocument = static_cast<IVEditorDocument *>(currentDocument);
    if (currentIvDocument == nullptr) {
        qDebug() << GenMsg::ivFileNotSelected;
        return nullptr;
    }
    return currentIvDocument->ivEditorCore();
}

auto FunctionTesterPlugin::displayResultHtml() -> void
{
    qDebug() << "Displaying html";
    QString filepath = getBaseDirectory() + QDir::separator() + "work" + QDir::separator() + "Results.html";
    // QTextBrowser *textBrowser = new QTextBrowser();
    // textBrowser->setSource(QUrl::fromLocalFile(filepath));
    // textBrowser->show();

    // QWebEngineView *view = new QWebEngineView(parent);
    // view->load(QUrl(QUrl::fromLocalFile(filepath)));
    // view->show();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));
}

} // namespace spctr
