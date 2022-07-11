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

const QString resultFileName = "Results.html";
const QString gdbScriptPath = "~/.local/share/QtProject/QtCreator/x86-linux-cpp.gdb";

TestGenerator::TestGenerator(const QString &baseDirectory)
{
    initializePaths(baseDirectory);
}

auto TestGenerator::testUsingDataFromCsv(
        IVInterface &interface, const CsvModel &csvModel, Asn1Model &asn1Model, float delta) -> bool
{
    Q_UNUSED(delta);

    QString testedFunctionName = prepareTestHarness(interface, csvModel, asn1Model);
    if (testedFunctionName.isEmpty()) {
        return false;
    }
    if (!prepareTasteProjectSkeleton()) {
        qDebug() << "Error preparing taste project skeleton";
        return false;
    }
    copyFunctionImplementations(testedFunctionName);
    compileSystemUnderTest();
    const QString binToRun = generatedPath + "/work/binaries/hostpartition";
    QVector<QVariant> testResults = runTests(interface, asn1Model, binToRun);

    QString resultPath = this->projectDirectory + QDir::separator() + "work" + QDir::separator() + resultFileName;
    HtmlResultExporter exporter(interface, csvModel, testResults, delta);
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

auto TestGenerator::prepareTestHarness(IVInterface &interface, const CsvModel &csvModel, Asn1Model &asn1Model)
        -> QString
{
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

    const auto ivModelGenerated = IvGenerator::generate(&interface);
    if (ivModelGenerated == nullptr) {
        qDebug() << "IV model was not generated";
        return {};
    }

    std::vector<IVFunction *> ivFunctions = getAllFunctionsFromModel(*ivModelGenerated);
    if (!exportIvModel(ivModelGenerated.get(), generatedIvPath)) {
        return {};
    }

    const std::unique_ptr<DVModel> dvModelGenerated =
            DvGenerator::generate(ivFunctions, "x86 Linux CPP", "x86_Linux_TestRunner", "Node_1", "hostPartition");
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
    for (const auto &file : qFileInfoList) {
        if (file.suffix() == "asn" || file.suffix() == "acn" || file.fileName() == "Makefile") {
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
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);

    IVExporter exporter;
    if (!exporter.exportObjects(ivModel->objects().values(), &buffer)) {
        return false;
    }

    QSaveFile outputFile(outputFilename);
    outputFile.open(QIODevice::WriteOnly);
    outputFile.write(buffer.data());
    outputFile.commit();
    return true;
}

auto TestGenerator::exportDvModel(DVModel *dvModel, const QString &outputFilename) -> bool
{
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);

    DVExporter exporter;
    if (!exporter.exportObjects(dvModel->objects().values(), &buffer)) {
        return false;
    }

    QSaveFile outputFile(outputFilename);
    outputFile.open(QIODevice::WriteOnly);
    outputFile.write(buffer.data());
    outputFile.commit();
    return true;
}

auto TestGenerator::runTests(IVInterface &interface, Asn1Model &asn1Model, const QString &binaryPath)
        -> QVector<QVariant>
{
    const QString binLocalization = binaryPath.left(binaryPath.lastIndexOf("/"));
    const QByteArray rawTestData = GdbConnector::getRawTestResults(
            binLocalization, { "-batch", "-x", gdbScriptPath }, { "localhost:1234", binaryPath });

    const DataReconstructor::TypeLayoutInfos typeLayoutInfos = {
        { "INTEGER", 4, 4 },
        { "BOOLEAN", 1, 7 },
        { "REAL", 8, 0 },
    };
    const QVector<QVariant> readTestData = DataReconstructor::getVariantVectorFromRawData(
            rawTestData, &interface, &asn1Model, QDataStream::LittleEndian, typeLayoutInfos);

    for (const auto &readValue : readTestData) {
        qDebug() << readValue;
    }

    return readTestData;
}

} // namespace spctr
