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

#include "dvcore/dvhwlibraryreader.h"
#include "pluginconstants.h"

#include <QApplication>
#include <QBoxLayout>
#include <QComboBox>
#include <QDesktopServices>
#include <QDirIterator>
#include <QFileDialog>
#include <QFormLayout>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <coreplugin/actionmanager/actionmanager.h>
#include <editormanager/editormanager.h>
#include <libiveditor/interfacedocument.h>
#include <messagestrings.h>
#include <modelloader.h>
#include <shared/ui/veinteractiveobject.h>
#include <spacecreatorplugin/common/messagemanager.h>
#include <spacecreatorplugin/iv/iveditordocument.h>

using namespace Core;
using namespace testgenerator;

using dvm::DVObject;
using ive::IVExporter;
using plugincommon::ModelLoader;

namespace spctr {

const QString resultFileName = "Results.html";
const QString boardsConfigFileName = "boards_config.txt";

FunctionTesterPlugin::FunctionTesterPlugin()
    : boardsConfigLoader(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator()
            + boardsConfigFileName)
{
}

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

auto FunctionTesterPlugin::testUsingDataFromCsvGui(const LaunchConfiguration &launchConfig) -> void
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

    if (launchConfig.scriptPath.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg("Path to the GDB file is empty"));
        return;
    }

    TestGenerator testGenerator(getBaseDirectory());
    testGenerator.testUsingDataFromCsv(*interface, *csvModel, *asn1Model, delta, launchConfig);
    displayResultHtml(resultFileName);
}

auto FunctionTesterPlugin::addTestInterfaceOption() -> void
{
    Context allContexts(Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    ActionContainer *const acToolsFunctionTester = createActionContainerInTools(tr("&Test Interface"));

    const auto csvImportAction = new QAction(tr("Test using data from CSV"), this);
    connect(csvImportAction, &QAction::triggered, [this]() { selectBoardDialog(); });
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
    QString text = QInputDialog::getText(nullptr, tr("Set delta"), tr("Maximum allowed absolute error:"),
            QLineEdit::Normal, "0.0", &isOk, Qt::WindowFlags(), Qt::ImhFormattedNumbersOnly);
    if (isOk && !text.isEmpty()) {
        delta = text.toFloat();
    }
    return delta;
}

auto FunctionTesterPlugin::loadCsv() -> std::unique_ptr<csv::CsvModel>
{
    const QString inputFilePath = QFileDialog::getOpenFileName(
            nullptr, tr("Select CSV file to import test vectors from..."), getBaseDirectory(), tr("*.csv"));
    if (inputFilePath.isEmpty()) {
        MessageManager::write(GenMsg::msgInfo.arg(GenMsg::fileToImportNotSelected));
        return std::unique_ptr<csv::CsvModel> {};
    }
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
                        MessageManager::write(GenMsg::msgInfo.arg("Selected interface: " + entity->title()));
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
        MessageManager::write(GenMsg::msgInfo.arg(tr("No ASN.1 file found. Try to build the project first.")));
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

auto FunctionTesterPlugin::displayResultHtml(const QString &resultFileName) -> void
{
    qDebug() << "Displaying html";
    QString filepath = getBaseDirectory() + QDir::separator() + "generated" + QDir::separator() + resultFileName;
    if (QFile::exists(filepath)) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));
    } else {
        MessageManager::write(GenMsg::msgError.arg("Could not find file with test results: " + filepath));
    }
}

auto loadHWLibraryObjects(const QString &directory) -> QVector<DVObject *>
{
    QVector<DVObject *> objects;
    QDirIterator it(directory, QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString fileName = it.next();
        dvm::DVHWLibraryReader reader;
        if (reader.readFile(fileName)) {
            objects << reader.parsedObjects();
        }
    }
    return objects;
}

auto FunctionTesterPlugin::selectBoardDialog() -> void
{
    int wndWidth = 800;
    int wndHeight = 600;

    boardsConfiguration = boardsConfigLoader.loadConfig().value();

    QWidget *chooseBoardWindow = new QWidget;
    chooseBoardWindow->resize(wndWidth, wndHeight);

    QListWidget *listWidget = new QListWidget(chooseBoardWindow);
    auto hwObjects = loadHWLibraryObjects(shared::hwLibraryPath());
    for (const auto &obj : hwObjects) {
        if (obj->type() == DVObject::Type::Board) {
            listWidget->addItem(obj->title());
        }
    }

    auto font = listWidget->font();
    font.setPointSize(15);
    listWidget->setFont(font);

    int listWidgetHeight = 0.9 * chooseBoardWindow->height();
    listWidget->resize(wndWidth, listWidgetHeight);
    listWidget->setCurrentRow(0);

    QWidget *bottomPanel = new QWidget(chooseBoardWindow);
    bottomPanel->setGeometry(0, listWidgetHeight, wndWidth, wndHeight - listWidgetHeight);

    QPushButton *okBtn = new QPushButton("OK", bottomPanel);
    okBtn->setFixedHeight(bottomPanel->height() * 0.5);

    QPushButton *optionsBtn = new QPushButton("Options", bottomPanel);
    optionsBtn->setFixedHeight(bottomPanel->height() * 0.5);

    QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::Direction::RightToLeft, bottomPanel);
    boxLayout->addWidget(okBtn);
    boxLayout->addSpacing(bottomPanel->width() * 0.05);
    boxLayout->addWidget(optionsBtn);
    boxLayout->addSpacing(bottomPanel->width() * 0.65);

    connect(okBtn, &QPushButton::clicked, this, [=] {
        QString boardName = listWidget->currentItem()->text();
        chooseBoardWindow->close();
        testUsingDataFromCsvGui(boardsConfiguration[boardName]);
    });
    connect(optionsBtn, &QPushButton::clicked, this,
            [=] { boardOptionsDialog(chooseBoardWindow, listWidget->currentItem()->text()); });

    chooseBoardWindow->setWindowTitle("Choose target board");
    chooseBoardWindow->show();
}

auto FunctionTesterPlugin::boardOptionsDialog(QWidget *parent, const QString &boardName) -> void
{
    QWidget *boardOptionsWindow = new QWidget;
    boardOptionsWindow->resize(500, 200);

    QLineEdit *scriptPathEdit = new QLineEdit;
    QLineEdit *clientNameEdit = new QLineEdit;
    QLineEdit *clientParamsEdit = new QLineEdit;
    QLineEdit *serverNameEdit = new QLineEdit;
    QLineEdit *serverParamsEdit = new QLineEdit;

    QBoxLayout *pathEditLayout = new QBoxLayout(QBoxLayout::Direction::RightToLeft);
    QPushButton *selectBtn = new QPushButton("Select");
    pathEditLayout->addWidget(selectBtn);
    pathEditLayout->addWidget(scriptPathEdit);

    const QString bigEndianStr = "Big Endian";
    const QString littleEndianStr = "Little Endian";

    QStringList endianessOptions = { bigEndianStr, littleEndianStr };
    QComboBox *endianessCombo = new QComboBox;
    endianessCombo->addItems(endianessOptions);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Script path", pathEditLayout);
    formLayout->addRow("Client", clientNameEdit);
    formLayout->addRow("Client params", clientParamsEdit);
    formLayout->addRow("Server", serverNameEdit);
    formLayout->addRow("Server params", serverParamsEdit);
    formLayout->addRow("Byte order", endianessCombo);

    QPushButton *okBtn = new QPushButton("OK");
    formLayout->addWidget(okBtn);

    boardOptionsWindow->setLayout(formLayout);

    scriptPathEdit->setText(boardsConfiguration[boardName].scriptPath);
    clientNameEdit->setText(boardsConfiguration[boardName].clientName);
    clientParamsEdit->setText(boardsConfiguration[boardName].clientArgs);
    serverNameEdit->setText(boardsConfiguration[boardName].serverName);
    serverParamsEdit->setText(boardsConfiguration[boardName].serverArgs);
    endianessCombo->setCurrentText(
            boardsConfiguration[boardName].endianess == QDataStream::BigEndian ? bigEndianStr : littleEndianStr);

    connect(selectBtn, &QPushButton::clicked, this,
            [=] { selectScriptDialog(boardOptionsWindow, boardName, scriptPathEdit); });
    connect(okBtn, &QPushButton::clicked, this, [=] {
        boardOptionsWindow->close();
        LaunchConfiguration boardConfig(boardName, scriptPathEdit->text(), clientNameEdit->text(),
                clientParamsEdit->text(), serverNameEdit->text(), serverParamsEdit->text(),
                endianessCombo->currentText() == bigEndianStr ? QDataStream::BigEndian : QDataStream::LittleEndian);
        boardsConfiguration[boardName] = boardConfig;
        boardsConfigLoader.saveConfig(boardsConfiguration);
    });

    boardOptionsWindow->setWindowTitle("Board options");
    boardOptionsWindow->show();
}

auto FunctionTesterPlugin::selectScriptDialog(QWidget *parent, const QString &boardName, QLineEdit *scriptPathEdit)
        -> void
{
    QString defaultDirPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString selectedScriptPath = QFileDialog::getOpenFileName(
            parent, tr("Select GDB script for running tests..."), defaultDirPath, tr("*.gdb"));
    if (!selectedScriptPath.isEmpty()) {
        scriptPathEdit->setText(selectedScriptPath);
    }
}

} // namespace spctr
