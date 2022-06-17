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

#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QBuffer>
#include <fstream>

#include <messagemanager.h>
#include <messagestrings.h>
#include <modelloader.h>
#include <ivtools.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <editormanager/editormanager.h>
#include <shared/ui/veinteractiveobject.h>
#include <spacecreatorplugin/iv/iveditordocument.h>
#include <libdveditor/dvexporter.h>
#include <libiveditor/ivexporter.h>
#include <libiveditor/interfacedocument.h>
#include <conversion/iv/IvXmlExporter/exporter.h>
#include <conversion/common/model.h>
#include <testgenerator/testgenerator.h>
#include <testgenerator/datareconstructor/datareconstructor.h>
#include <testgenerator/gdbconnector/gdbconnector.h>

using namespace Core;
using namespace testgenerator;

using plugincommon::ModelLoader;
using testgenerator::DataReconstructor;
using testgenerator::GdbConnector;
using ive::IVExporter;

namespace spctr {

FunctionTesterPlugin::FunctionTesterPlugin()
{

}

FunctionTesterPlugin::~FunctionTesterPlugin()
{

}

auto FunctionTesterPlugin::initialize(const QStringList &arguments, QString *errorString) -> bool
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    addTestInterfaceOption();

    return true;
}

auto FunctionTesterPlugin::extensionsInitialized() -> void
{

}

auto FunctionTesterPlugin::aboutToShutdown() -> ExtensionSystem::IPlugin::ShutdownFlag
{
    return SynchronousShutdown;
}

auto FunctionTesterPlugin::functionTesterPluginMain() -> void
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
    functionTesterPluginCore(*interface, *csvModel, *asn1Model, delta);
}

auto FunctionTesterPlugin::functionTesterPluginCore(ivm::IVInterface &interface,
    const csv::CsvModel &csvModel, const Asn1Acn::Asn1Model &asn1Model, float delta) -> void
{
    projectDirectory = getBaseDirectory();
    generatedPath = projectDirectory + QDir::separator() + "generated";
    generatedCodePath = generatedPath + QDir::separator() + "testdriver.c";
    generatedIvPath = generatedPath + QDir::separator() + "interfaceview.xml";
    generatedDvPath = generatedPath + QDir::separator() + "deploymentview.dv.xml";

    QDir dir(generatedPath);
    dir.removeRecursively();
    dir.mkpath(".");

    std::stringstream outStream;
    try {
        outStream = TestDriverGenerator::generateTestDriver(csvModel, interface, asn1Model);
        std::ofstream outFile(generatedCodePath.toStdString(), std::ofstream::out);
        outFile << outStream.str();
        outFile.close();
    }
    catch (TestDriverGeneratorException& e) {
        MessageManager::write(GenMsg::msgInfo.arg("TestDriverGeneratorException: " + QString(e.what())));
        return;
    }

    const auto ivModelGenerated = IvGenerator::generate(&interface);
    if (ivModelGenerated == nullptr) {
        MessageManager::write(GenMsg::msgInfo.arg("IV model was not generated"));
        return;
    }

    QList<ivm::IVObject *> ivObjects = ivModelGenerated->visibleObjects();
    std::vector<ivm::IVFunction *> ivFunctions = {};

    for (const auto& ivObject : ivObjects) {
        if (ivObject->isFunction()) {
            ivFunctions.push_back(dynamic_cast<ivm::IVFunction *>(ivObject));
        }
    }

    exportIvModel(ivModelGenerated.get(), generatedIvPath);

    const std::unique_ptr<dvm::DVModel> dvModelGenerated =
        DvGenerator::generate(ivFunctions, "x86 Linux CPP", "x86_Linux_TestRunner", "Node_1", "hostPartition");

    if (dvModelGenerated == nullptr) {
        MessageManager::write(GenMsg::msgInfo.arg("DV model was not generated"));
        return;
    }

    exportDvModel(dvModelGenerated.get(), generatedDvPath);
    compileTest(ivFunctions[1]->title());
    //extractResult(interface, asn1Model.get());
}

auto FunctionTesterPlugin::copyRecursively(const QString &srcPath, const QString &dstPath) -> bool
{
    QDir parentDstDir(QFileInfo(dstPath).path());
    if (!parentDstDir.mkdir(QFileInfo(dstPath).fileName()))
        return false;

    QDir srcDir(srcPath);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        if (info.isDir()) {
            if (!copyRecursively(srcItemPath, dstItemPath)) {
                return false;
            }
        } else if (info.isFile()) {
            if (!QFile::copy(srcItemPath, dstItemPath)) {
                return false;
            }
        } else {
            qDebug() << "Unhandled item" << info.filePath() << "in copyRecursively";
        }
    }
    return true;
}

auto FunctionTesterPlugin::runProcess(QString cmd, QStringList args, QString workingPath) -> void
{
    QProcess process;
    process.setWorkingDirectory(workingPath);
    process.start(cmd, args);
    process.waitForFinished(-1);
    if (process.exitCode() != 0)
    {
        qDebug() << "Error: " << process.exitCode() << process.readAllStandardError();
        MessageManager::write(GenMsg::msgInfo.arg("Error: " + process.exitCode()));
    }
    else
    {
        MessageManager::write(GenMsg::msgInfo.arg("Command: " + cmd + " finished"));
    }
}

auto FunctionTesterPlugin::compileTest(const QString &functionName) -> void
{
    MessageManager::write(GenMsg::msgInfo.arg("Wait for compilation of the test..."));

    std::string baseDir = projectDirectory.toStdString();
    auto separator = QString(QDir::separator()).toStdString();
    QString projectName = QString::fromStdString(baseDir.substr(baseDir.find_last_of(separator) + 1));

    QString sourceFilePath = projectDirectory + QDir::separator() + projectName + ".asn";
    QString destFilePath = generatedPath + QDir::separator() + projectName + ".asn";
    QFile::copy(sourceFilePath, destFilePath);

    sourceFilePath = projectDirectory + QDir::separator() + projectName + ".acn";
    destFilePath = generatedPath + QDir::separator() + projectName + ".acn";
    QFile::copy(sourceFilePath, destFilePath);

    sourceFilePath = projectDirectory + QDir::separator() + "Makefile";
    destFilePath = generatedPath + QDir::separator() + "Makefile";
    QFile::copy(sourceFilePath, destFilePath);

    runProcess("taste-update-data-view", QStringList() << "*.asn", generatedPath);
    runProcess("make", QStringList() << "skeletons", generatedPath);

    sourceFilePath = projectDirectory + QDir::separator() + "work" + QDir::separator() + functionName;
    destFilePath = generatedPath + QDir::separator() + "work" + QDir::separator() + functionName;
    copyRecursively(sourceFilePath, destFilePath);

    sourceFilePath = generatedPath + QDir::separator() + "testdriver.c";
    destFilePath = generatedPath + QDir::separator() + "work/testdriver/C/src/testdriver.c";
    QFile::copy(sourceFilePath, destFilePath);

    runProcess("make", QStringList() << "deploymentview" << "debug", generatedPath);
    MessageManager::write(GenMsg::msgInfo.arg("Tests compilation finished"));
}

auto FunctionTesterPlugin::exportIvModel(ivm::IVModel *ivModel, const QString &outputFilename) -> void
{
    QByteArray modelData;
    QBuffer modelDataBuffer(&modelData);
    modelDataBuffer.open(QIODevice::WriteOnly);

    IVExporter exporter;
    exporter.exportObjects(ivModel->objects().values(), &modelDataBuffer);

    QSaveFile outputFile(outputFilename);
    outputFile.open(QIODevice::WriteOnly);
    outputFile.write(modelData);
    outputFile.commit();
}

auto FunctionTesterPlugin::exportDvModel(dvm::DVModel *dvModel, const QString &outputFilename) -> void
{
    const auto dvObjects = getDvObjectsFromModel(dvModel);

    dve::DVExporter exporter;
    QList<shared::VEObject *> objects;
    std::for_each(dvObjects->begin(), dvObjects->end(), //
            [&objects](const auto &obj) { objects.push_back(obj); });

    const int objectMaxSize = 1'000;
    QByteArray qba(objects.size() * objectMaxSize, '\00');
    QBuffer buf = QBuffer(&qba);

    exporter.exportObjects(objects, &buf);

    QFile file(outputFilename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    file.write(qba);
}

auto FunctionTesterPlugin::getDvObjectsFromModel(dvm::DVModel *const model) -> std::unique_ptr<QVector<dvm::DVObject *>>
{
    auto generatedDvObjects = std::make_unique<QVector<dvm::DVObject *>>();
    for (const auto &obj : model->objects()) {
        generatedDvObjects->append(static_cast<dvm::DVObject *>(obj));
    }

    return generatedDvObjects;
}

auto FunctionTesterPlugin::addTestInterfaceOption() -> void
{
    Context allContexts(Core::Constants::C_WELCOME_MODE, Core::Constants::C_EDIT_MODE, Core::Constants::C_DESIGN_MODE);

    ActionContainer *const acToolsFunctionTester = createActionContainerInTools(tr("&Test Interface"));

    const auto csvImportAction = new QAction(tr("Test using data from CSV"), this);
    connect(csvImportAction, &QAction::triggered, [=]() { this->functionTesterPluginMain(); });
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

auto FunctionTesterPlugin::setDeltaDialog() -> float
{
    float delta = 0.0;
    bool isOk;
    QString text = QInputDialog::getText(nullptr, tr("Set delta"), tr("Max error:"), QLineEdit::Normal,
            "0.0", &isOk, { 0U }, Qt::ImhFormattedNumbersOnly);
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
        return std::unique_ptr<csv::CsvModel>{};
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

auto FunctionTesterPlugin::getCurrentIvEditorCore() -> IVEditorCorePtr
{
    auto *const currentDocument = EditorManager::currentDocument();
    auto *const currentIvDocument = static_cast<IVEditorDocument *>(currentDocument);
    if (currentIvDocument == nullptr) {
        MessageManager::write(GenMsg::msgError.arg(GenMsg::ivFileNotSelected));
        return nullptr;
    }
    return currentIvDocument->ivEditorCore();
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

auto FunctionTesterPlugin::getBaseDirectory() -> QString
{
    QString ivDocumentPath = getCurrentIvEditorCore()->document()->path();
    return QFileInfo(ivDocumentPath).absoluteDir().absolutePath();
}

auto FunctionTesterPlugin::loadAsn1Model() -> std::unique_ptr<Asn1Acn::Asn1Model>
{
    QString baseDirectory = getBaseDirectory();
    QString workDirectory = baseDirectory + QDir::separator() + "work";
    QString asn1Path = workDirectory + QDir::separator() + "dataview"
        + QDir::separator() + "dataview-uniq.asn";

    auto modelPtr = std::unique_ptr<Asn1Acn::Asn1Model>{};
    try {
        modelPtr = ModelLoader::loadAsn1Model(asn1Path);
    }
    catch (...) {
        MessageManager::write(GenMsg::msgInfo.arg(tr("No ASN1 file found. Try to build the project first.")));
    }
    return modelPtr;
}

// TODO: this is draft
QString byteToHexStr(const char byte)
{
    const int hexBase = 16;

    const auto number = static_cast<uint_least8_t>(byte);
    if (number < hexBase) {
        return QString("0%1").arg(number, 1, hexBase);
    } else {
        return QString("%1").arg(number, 2, hexBase);
    }
}

void printQByteArrayInHex(const QByteArray &array)
{
    QString arrayInHex = QString("QByteArray size: %1\n").arg(array.size());
    for (int i = 0; i < array.size(); i++) {
        arrayInHex += byteToHexStr(array.at(i));
        arrayInHex += " ";

        if ((i + 1) % 8 == 0) {
            arrayInHex += "| ";
        }

        if ((i + 1) % 16 == 0) {
            arrayInHex += "\n";
        }
    }

    qDebug().noquote() << arrayInHex;
}

// TODO: fix and finish (doesnt work)
auto FunctionTesterPlugin::extractResult(ivm::IVInterface *const interface, Asn1Acn::Asn1Model *const asn1Model) -> void
{
    const QString binLocalization = generatedPath + QDir::separator()
            + "work" + QDir::separator() + "binaries" + QDir::separator();
    // TODO: it is now absolute but later will be changed
    const QString script = "/home/taste/SpaceCreator/spacecreator/src/libs/testgenerator/gdbconnector/scripts/x86-linux-cpp.gdb";
    const QString binToRun = "hostpartition";
    const QByteArray rawTestData =
            GdbConnector::getRawTestResults(binLocalization, { "-batch", "-x", script }, { "host:1234", binToRun });

    const DataReconstructor::TypeLayoutInfos typeLayoutInfos = {
        { "INTEGER", 4, 4 },
        { "BOOLEAN", 1, 7 },
        { "REAL", 8, 0 },
    };
    const QVector<QVariant> readTestData = DataReconstructor::getVariantVectorFromRawData(
            rawTestData, interface, asn1Model, QDataStream::LittleEndian, typeLayoutInfos);

    const int dataSize = readTestData.size();
    printQByteArrayInHex(rawTestData);
}

} // namespace spctr
