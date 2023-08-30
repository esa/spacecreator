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

#include "testgenerator.h"

#include "datareconstructor/datareconstructor.h"
#include "gdbconnector/gdbconnector.h"
#include "gdbconnector/process.h"
#include "scversion.h"
#include "standardpaths.h"

#include <QBuffer>
#include <QDebug>
#include <QDir>
#include <conversion/common/model.h>
#include <conversion/iv/IvXmlExporter/exporter.h>
#include <fstream>
#include <ivcore/ivfunctiontype.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivobject.h>
#include <ivtools.h>
#include <libdveditor/dvexporter.h>
#include <libiveditor/ivexporter.h>
#include <modelloader.h>
#include <shared/ui/veinteractiveobject.h>

using dve::DVExporter;
using ive::IVExporter;
using ivm::IVObject;
using plugincommon::ModelLoader;

namespace testgenerator {

const QString TestGenerator::resultFileName = "Results.html";
const QString TestGenerator::defaultBinaryName = "hostpartition";

TestGenerator::TestGenerator(const QString &baseDirectory)
{
    initializePaths(baseDirectory);
}

auto TestGenerator::testUsingDataFromCsv(IVInterface &interface, const CsvModel &csvModel, Asn1Model &asn1Model,
        const float delta, const LaunchConfiguration &launchConfig) -> bool
{
    QString testedFunctionName =
            prepareTestHarness(interface, csvModel, asn1Model, launchConfig.boardName, launchConfig.stackSize);
    if (testedFunctionName.isEmpty()) {
        qWarning() << "Tested function name is empty";
        return false;
    }
    if (!prepareTasteProjectSkeleton()) {
        qWarning() << "Error preparing taste project skeleton";
        return false;
    }

    if (launchConfig.scriptPath.isEmpty()) {
        qWarning() << "Path to the GDB script is empty";
        return false;
    }

    copyFunctionImplementations(testedFunctionName);
    compileSystemUnderTest();
    const QString binToRun = generatedPath + "/work/binaries/hostpartition";
    QVector<QVariant> testResults = runTests(interface, asn1Model, launchConfig);

    QString resultPath = this->generatedPath + QDir::separator() + resultFileName;
    HtmlResultExporter exporter(launchConfig.boardName, interface, csvModel, testResults, delta);
    exporter.exportResult(resultPath);
    return true;
}

auto TestGenerator::initializePaths(const QString &baseDirectory) -> void
{
    projectDirectory = baseDirectory;
    generatedPath = projectDirectory + QDir::separator() + "generated";
    generatedCodePath = generatedPath + QDir::separator() + "testdriver.c";
    generatedIvPath = generatedPath + QDir::separator() + "interfaceview.xml";
    generatedDvPath = generatedPath + QDir::separator() + "deploymentview.dv.xml";
    binaryPath = generatedPath + QDir::separator() + "work" + QDir::separator() + "binaries" + QDir::separator();
}

auto TestGenerator::getAllFunctionsFromModel(const IVModel &ivModel) -> std::vector<IVFunction *>
{
    QList<IVObject *> ivObjects = ivModel.visibleObjects();
    std::vector<IVFunction *> ivFunctions = {};
    for (const auto &ivObject : ivObjects) {
        if (ivObject->isFunction()) {
            ivFunctions.push_back(dynamic_cast<IVFunction *>(ivObject));
        }
    }
    return ivFunctions;
}

auto TestGenerator::prepareTestHarness(IVInterface &interface, const CsvModel &csvModel, Asn1Model &asn1Model,
        const QString &boardName, const int stackSize) -> QString
{
    if (boardName.isEmpty()) {
        qWarning() << "TestGenerator::prepareTestHarness: Board name is empty";
        return "";
    }

    QDir dir(generatedPath);
    dir.removeRecursively();
    dir.mkpath(".");

    std::stringstream outStream;
    try {
        outStream = TestDriverGenerator::generateTestDriver(csvModel, interface, asn1Model);
        std::ofstream outFile(generatedCodePath.toStdString(), std::ofstream::out);
        outFile << outStream.str();
        outFile.close();
    } catch (TestDriverGeneratorException &e) {
        qDebug() << "TestDriverGeneratorException: " + QString(e.what());
        return {};
    }

    const auto ivModelGenerated = IvGenerator::generate(&interface, stackSize);
    if (ivModelGenerated == nullptr) {
        qDebug() << "IV model was not generated";
        return {};
    }

    std::vector<IVFunction *> ivFunctions = getAllFunctionsFromModel(*ivModelGenerated);
    if (!exportIvModel(ivModelGenerated.get(), generatedIvPath)) {
        return {};
    }

    const std::unique_ptr<DVModel> dvModelGenerated =
            DvGenerator::generate(ivFunctions, boardName, "TestRunner", "Node_1", defaultBinaryName);
    if (dvModelGenerated == nullptr) {
        qDebug() << "DV model was not generated";
        return {};
    }
    if (!exportDvModel(dvModelGenerated.get(), generatedDvPath)) {
        return {};
    }
    constexpr int TESTED_FUNCTION_INDEX = 1;
    return ivFunctions[TESTED_FUNCTION_INDEX]->title();
}

auto TestGenerator::runProcess(QString cmd, QStringList args, QString workingPath) -> bool
{
    Process process(cmd, args, workingPath);
    auto p = process.get();
    p->waitForFinished(-1);
    if (p->exitCode() != 0) {
        qDebug() << "Error: " << p->exitCode() << p->readAllStandardError();
        return false;
    } else {
        qDebug() << "Command: " + cmd + " finished";
        return true;
    }
}

auto TestGenerator::prepareTasteProjectSkeleton() -> bool
{
    qDebug() << "Preparing project skeleton...";

    QFileInfoList qFileInfoList = QDir(projectDirectory).entryInfoList(QDir::Files);
    if (qFileInfoList.empty()) {
        qDebug() << "No files found in project directory: " + projectDirectory;
        return false;
    }

    const QStringList suffixesToCopy { "asn", "acn", "modelcheck" };
    const QStringList namesToCopy { "Makefile" };
    for (const auto &file : qFileInfoList) {
        auto suffixIt = std::find(std::begin(suffixesToCopy), std::end(suffixesToCopy), file.suffix());
        auto namesIt = std::find(std::begin(namesToCopy), std::end(namesToCopy), file.fileName());
        if (suffixIt != std::end(suffixesToCopy) || namesIt != std::end(namesToCopy)) {
            if (!QFile::copy(file.absoluteFilePath(), generatedPath + QDir::separator() + file.fileName())) {
                qDebug() << "Error copying file: " << file.fileName();
                return false;
            }
        }
    }

    if (!runProcess("taste-update-data-view", { "*.asn" }, generatedPath)) {
        return false;
    }
    if (!runProcess("make", { "skeletons" }, generatedPath)) {
        return false;
    }
    return true;
}

auto TestGenerator::copyFunctionImplementations(const QString &functionName) -> void
{
    QString sourcePath = projectDirectory + QDir::separator() + "work" + QDir::separator() + functionName.toLower();
    QString destPath = generatedPath + QDir::separator() + "work" + QDir::separator();
    QDir(destPath + QDir::separator() + functionName.toLower()).removeRecursively();
    runProcess("cp", { "-r", sourcePath, destPath }, generatedPath);

    sourcePath = generatedPath + QDir::separator() + "testdriver.c";
    destPath = generatedPath + QDir::separator() + "work/testdriver/C/src/testdriver.c";

    QFile::remove(destPath);
    QFile::copy(sourcePath, destPath);
}

auto TestGenerator::compileSystemUnderTest() -> void
{
    runProcess("make", { "deploymentview", "debug" }, generatedPath);
    qDebug() << "Tests compilation finished";
}

auto TestGenerator::exportIvModel(IVModel *ivModel, const QString &outputFilename) -> bool
{
    IVExporter exporter;
    return exporter.exportObjectsSilently(ivModel->objects().values(), outputFilename);
}

auto TestGenerator::exportDvModel(DVModel *dvModel, const QString &outputFilename) -> bool
{
    DVExporter exporter;
    return exporter.exportObjectsSilently(dvModel->objects().values(), spaceCreatorGitHash, outputFilename);
}

auto TestGenerator::runTests(IVInterface &interface, Asn1Model &asn1Model, const LaunchConfiguration &launchConfig)
        -> QVector<QVariant>
{
    QByteArray rawTestData;
    try {
        rawTestData = GdbConnector::getRawTestResults(binaryPath, launchConfig.clientArgsParsed,
                launchConfig.serverArgsParsed, launchConfig.clientName, launchConfig.serverName);
    } catch (std::invalid_argument &e) {
        qCritical() << "Error while getting raw test results (invalid argument): " << e.what();
    } catch (std::runtime_error &e) {
        qCritical() << "Error while getting raw test results (runtime error): " << e.what();
    }

    constexpr int LOGGED_DATA_SIZE = 250;
    qDebug() << "Raw test data size: " << rawTestData.size();
    qDebug() << "Raw test data (first " << LOGGED_DATA_SIZE
             << " bytes): " << rawTestData.left(LOGGED_DATA_SIZE).toHex(' ');

    const QVector<QVariant> readTestData = DataReconstructor::getVariantVectorFromRawData(
            rawTestData, &interface, &asn1Model, launchConfig.endianess, launchConfig.typeLayoutInfos);

    for (int i = 0; i < readTestData.size(); i++) {
        if (i < LOGGED_DATA_SIZE) {
            qDebug() << readTestData[i];
        }
    }

    return readTestData;
}

} // namespace spctr
