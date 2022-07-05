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

using ive::IVExporter;
using plugincommon::ModelLoader;

namespace testgenerator {

TestResultData::TestResultData(
        const QString &ifaceName, const CsvModel &csvModel, const QVector<QVariant> &results, float delta)
    : interfaceName(ifaceName)
    , maxDelta(delta)
{
    for (int i = 0; i < results.size(); i += 2) {
        orygValues.push_back(results[i]);
        resultValues.push_back(results[i + 1]);
    }
    for (const auto &row : csvModel.records()) {
        csv::Row csvRow = *row;
        auto fields = csvRow.fields();
        constexpr int EXPECTED_RESULT_INDEX = 1;
        expectedResults.push_back(fields[EXPECTED_RESULT_INDEX]);
    }
    for (int i = 0; i < resultValues.size(); i++) {
        isCorrectVector.push_back(abs(resultValues[i].toFloat() - expectedResults[i].toFloat()) <= maxDelta);
    }
}

TestGenerator::TestGenerator(const QString &baseDirectory)
{
    initializePaths(baseDirectory);
}

auto TestGenerator::testUsingDataFromCsv(
        ivm::IVInterface &interface, const csv::CsvModel &csvModel, Asn1Acn::Asn1Model &asn1Model, float delta) -> void
{
    Q_UNUSED(delta);

    QString testedFunctionName = prepareTestHarness(interface, csvModel, asn1Model);
    if (testedFunctionName.isEmpty()) {
        return;
    }
    prepareTasteProjectSkeleton();
    copyFunctionImplementations(testedFunctionName);
    compileSystemUnderTest();
    QVector<QVariant> testResults = extractResult(interface, asn1Model);
    generateResultHtmlFile(TestResultData(interface.title(), csvModel, testResults, delta));
}

auto TestGenerator::initializePaths(const QString &baseDirectory) -> void
{
    projectDirectory = baseDirectory;
    generatedPath = projectDirectory + QDir::separator() + "generated";
    generatedCodePath = generatedPath + QDir::separator() + "testdriver.c";
    generatedIvPath = generatedPath + QDir::separator() + "interfaceview.xml";
    generatedDvPath = generatedPath + QDir::separator() + "deploymentview.dv.xml";
}

auto TestGenerator::getAllFunctionsFromModel(const ivm::IVModel &ivModel) -> std::vector<ivm::IVFunction *>
{
    QList<ivm::IVObject *> ivObjects = ivModel.visibleObjects();
    std::vector<ivm::IVFunction *> ivFunctions = {};
    for (const auto &ivObject : ivObjects) {
        if (ivObject->isFunction()) {
            ivFunctions.push_back(dynamic_cast<ivm::IVFunction *>(ivObject));
        }
    }
    return ivFunctions;
}

auto TestGenerator::prepareTestHarness(
        ivm::IVInterface &interface, const csv::CsvModel &csvModel, Asn1Acn::Asn1Model &asn1Model) -> QString
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

    std::vector<ivm::IVFunction *> ivFunctions = getAllFunctionsFromModel(*ivModelGenerated);
    exportIvModel(ivModelGenerated.get(), generatedIvPath);

    const std::unique_ptr<dvm::DVModel> dvModelGenerated =
            DvGenerator::generate(ivFunctions, "x86 Linux CPP", "x86_Linux_TestRunner", "Node_1", "hostPartition");
    if (dvModelGenerated == nullptr) {
        qDebug() << "DV model was not generated";
        return {};
    }
    exportDvModel(dvModelGenerated.get(), generatedDvPath);
    constexpr int TESTED_FUNCTION_INDEX = 1;
    return ivFunctions[TESTED_FUNCTION_INDEX]->title();
}

auto TestGenerator::runProcess(QString cmd, QStringList args, QString workingPath) -> void
{
    Process process(cmd, args, workingPath);
    auto p = process.get();
    p->waitForFinished(-1);
    if (p->exitCode() != 0) {
        qDebug() << "Error: " << p->exitCode() << p->readAllStandardError();
    } else {
        qDebug() << "Command: " + cmd + " finished";
    }
}

auto TestGenerator::prepareTasteProjectSkeleton() -> void
{
    qDebug() << "Preparing project skeleton...";

    auto copy = [this](const QFileInfo &file) {
        if (file.suffix() == "asn" || file.suffix() == "acn" || file.fileName() == "Makefile") {
            QFile::copy(file.absoluteFilePath(), generatedPath + QDir::separator() + file.fileName());
        }
    };

    QFileInfoList qFileInfoList = QDir(projectDirectory).entryInfoList(QDir::Files);
    std::for_each(qFileInfoList.cbegin(), qFileInfoList.cend(), copy);

    runProcess("taste-update-data-view", QStringList() << "*.asn", generatedPath);
    runProcess("make", QStringList() << "skeletons", generatedPath);
}

auto TestGenerator::copyFunctionImplementations(const QString &functionName) -> void
{
    QString sourcePath = projectDirectory + QDir::separator() + "work" + QDir::separator() + functionName.toLower();
    QString destPath = generatedPath + QDir::separator() + "work" + QDir::separator();
    QDir(destPath + QDir::separator() + functionName.toLower()).removeRecursively();
    runProcess("cp", QStringList() << "-r" << sourcePath << destPath, generatedPath);

    sourcePath = generatedPath + QDir::separator() + "testdriver.c";
    destPath = generatedPath + QDir::separator() + "work/testdriver/C/src/testdriver.c";

    QFile::remove(destPath);
    QFile::copy(sourcePath, destPath);
}

auto TestGenerator::compileSystemUnderTest() -> void
{
    runProcess("make",
            QStringList() << "deploymentview"
                          << "debug",
            generatedPath);
    qDebug() << "Tests compilation finished";
}

auto TestGenerator::exportIvModel(ivm::IVModel *ivModel, const QString &outputFilename) -> void
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

auto TestGenerator::exportDvModel(dvm::DVModel *dvModel, const QString &outputFilename) -> void
{
    const auto dvObjects = getDvObjectsFromModel(dvModel);

    dve::DVExporter exporter;
    QList<shared::VEObject *> objects;
    std::for_each(dvObjects->begin(), dvObjects->end(), [&objects](const auto &obj) { objects.push_back(obj); });

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

auto TestGenerator::getDvObjectsFromModel(dvm::DVModel *const model) -> std::unique_ptr<QVector<dvm::DVObject *>>
{
    auto generatedDvObjects = std::make_unique<QVector<dvm::DVObject *>>();
    for (const auto &obj : model->objects()) {
        generatedDvObjects->append(static_cast<dvm::DVObject *>(obj));
    }

    return generatedDvObjects;
}

auto TestGenerator::generateTableRow(QTextStream &stream, const QString &columnName, QVector<QVariant> values,
        QVector<bool> isCorrectVector = {}) -> void
{
    stream << "        <tr>" << endl;
    stream << "            ";
    stream << "<th>" << columnName << "</th>";
    for (int i = 0; i < values.size(); i++) {
        if (!isCorrectVector.empty() && isCorrectVector[i] == false) {
            stream << "<td style='color:red'>";
        } else {
            stream << "<td>";
        }
        stream << values[i].toFloat();
        stream << "</td>";
    }
    stream << endl;
    stream << "        </tr>" << endl;
}

auto TestGenerator::generateResultHtmlStream(QTextStream &stream, const TestResultData &resultData) -> void
{
    stream << "<!DOCTYPE html>" << endl;
    stream << "<html lang='en'>" << endl;
    stream << " <head>" << endl;
    stream << "  <title>Test results for interface " << resultData.interfaceName << "</title>" << endl;
    stream << "  <meta charset='utf-8'>" << endl;
    stream << " </head>" << endl;
    stream << " <style> ";
    stream << "table, th, td { ";
    stream << "border: 1px solid black; border-collapse: collapse; font-size: 30px; } ";
    stream << "th { ";
    stream << "font-size: 18px; } ";
    stream << "</style>" << endl;
    stream << "<body>" << endl;
    stream << "<h2>Test results for interface " << resultData.interfaceName << "</h2>" << endl;
    stream << "    <table>" << endl;

    generateTableRow(stream, "Input", resultData.orygValues);
    generateTableRow(stream, "Expected", resultData.expectedResults);
    generateTableRow(stream, "Results", resultData.resultValues, resultData.isCorrectVector);

    stream << "    </table>" << endl;
    stream << "    <p style='font-size: 22px'>Maximum acceptable error: " << resultData.maxDelta << "</p>" << endl;
    stream << " </body>" << endl;
    stream << "</html>" << endl;
}

auto TestGenerator::generateResultHtmlFile(const TestResultData &resultData) -> void
{
    QString filename = this->projectDirectory + QDir::separator() + "work" + QDir::separator() + "Results.html";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        generateResultHtmlStream(stream, resultData);
    }
}

auto TestGenerator::extractResult(ivm::IVInterface &interface, Asn1Acn::Asn1Model &asn1Model) -> QVector<QVariant>
{

    const QString binLocalization =
            generatedPath + QDir::separator() + "work" + QDir::separator() + "binaries" + QDir::separator();
    // TODO: it is now absolute but later will be changed
    const QString script =
            "/home/taste/SpaceCreator/spacecreator/src/libs/testgenerator/gdbconnector/scripts/x86-linux-cpp.gdb";
    const QString binToRun = "/home/taste/SpaceCreator/TestProj3/generated/work/binaries/hostpartition";
    const QByteArray rawTestData = GdbConnector::getRawTestResults(
            binLocalization, { "-batch", "-x", script }, { "localhost:1234", binToRun });

    const DataReconstructor::TypeLayoutInfos typeLayoutInfos = {
        { "INTEGER", 4, 4 },
        { "BOOLEAN", 1, 7 },
        { "REAL", 8, 0 },
    };
    const QVector<QVariant> readTestData = DataReconstructor::getVariantVectorFromRawData(
            rawTestData, &interface, &asn1Model, QDataStream::LittleEndian, typeLayoutInfos);

    return readTestData;
}

} // namespace spctr
