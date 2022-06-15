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
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
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
#include <shared/ui/veinteractiveobject.h>
#include <spacecreatorplugin/iv/iveditordata.h>
#include <spacecreatorplugin/iv/iveditordocument.h>
#include <spacecreatorplugin/iv/iveditorfactory.h>
#include <spacecreatorplugin/iv/ivqtceditor.h>
#include <utils/fileutils.h>

#include <testgenerator/testgenerator.h>
#include <fstream>
#include <libdveditor/dvexporter.h>
#include <QBuffer>
#include <conversion/iv/IvXmlExporter/exporter.h>
#include <conversion/common/model.h>
#include <libiveditor/ivexporter.h>
#include <QProcess>

using namespace Core;
using conversion::Converter;
using conversion::ModelType;
using conversion::RegistrationFailedException;
using conversion::asn1::Asn1Registrar;
using conversion::iv::IvRegistrar;
using conversion::sdl::SdlRegistrar;
using conversion::seds::SedsRegistrar;
using ive::IVExporter;
using plugincommon::ModelLoader;
using namespace testgenerator;

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
    functionTesterPluginCore(interface, csvModel, asn1Model, delta);
}

auto FunctionTesterPlugin::functionTesterPluginCore(ivm::IVInterface *interface,
    const std::unique_ptr<csv::CsvModel> &csvModel,
    const std::unique_ptr<Asn1Acn::Asn1Model> &asn1Model, float delta) -> void
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
        outStream = TestDriverGenerator::generateTestDriver(*csvModel, *interface, *asn1Model);
        std::ofstream outFile(generatedCodePath.toStdString(), std::ofstream::out);
        outFile << outStream.str();
        outFile.close();
    }
    catch (TestDriverGeneratorException& e) {
        MessageManager::write(GenMsg::msgInfo.arg("TestDriverGeneratorException: " + QString(e.what())));
        return;
    }

    const auto ivModelGenerated = IvGenerator::generate(interface);
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

    exportIvModel(ivModelGenerated, generatedIvPath);

    const std::unique_ptr<dvm::DVModel> dvModelGenerated =
        DvGenerator::generate(ivFunctions, "x86 Linux CPP", "x86_Linux_TestRunner", "Node_1", "hostPartition");

    if (dvModelGenerated == nullptr) {
        MessageManager::write(GenMsg::msgInfo.arg("DV model was not generated"));
        return;
    }

    exportDvModel(dvModelGenerated, generatedDvPath);
    prepareCompilation(ivFunctions[1]->title());
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

auto FunctionTesterPlugin::prepareCompilation(const QString &functionName) -> void
{
    std::string baseDir = projectDirectory.toStdString();
    QString projectName = QString::fromStdString(baseDir.substr(baseDir.find_last_of("/") + 1));
    MessageManager::write(GenMsg::msgInfo.arg(projectName + ".asn"));

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

auto FunctionTesterPlugin::exportIvModel(const std::unique_ptr<ivm::IVModel> &ivModel, QString outputFilepath) -> void
{
    QByteArray modelData;
    QBuffer modelDataBuffer(&modelData);
    modelDataBuffer.open(QIODevice::WriteOnly);

    IVExporter exporter;
    exporter.exportObjects(ivModel->objects().values(), &modelDataBuffer);

    QSaveFile outputFile(outputFilepath);
    outputFile.open(QIODevice::WriteOnly);
    outputFile.write(modelData);
    outputFile.commit();
}


auto FunctionTesterPlugin::exportDvModel(const std::unique_ptr<dvm::DVModel> &dvModel, const QString &outputFilename) -> void
{
    const auto dvObjects = getDvObjectsFromModel(dvModel.get());

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

} // namespace spctr
