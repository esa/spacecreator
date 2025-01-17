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

#include <QObject>
#include <QTest>
#include <ivlibrary.h>
#include <ivtools.h>
#include <modelloader.h>
#include <qtestcase.h>
#include <sharedlibrary.h>
#include <standardpaths.h>
#include <testgenerator.h>

using plugincommon::IvTools;
using plugincommon::ModelLoader;
using testgenerator::DataReconstructor;
using testgenerator::HtmlResultExporter;
using testgenerator::LaunchConfigLoader;
using testgenerator::LaunchConfiguration;
using testgenerator::TestGenerator;

namespace tests::testgenerator {

class tst_testgenerator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();

    void testPrepareTestHarness();
    void testResultHtmlFile();
    void testResultHtmlData();
    void testParsingBoardSettings();
    void testStoringBoardsConfig();
};

void tst_testgenerator::initTestCase()
{
    shared::StandardPaths::setTestModeEnabled(true);
    ivm::initIVLibrary();
    shared::initSharedLibrary();
}

void tst_testgenerator::testPrepareTestHarness()
{
    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/interfaceview.xml");
    QVERIFY(ivModel);

    ivm::IVInterface *const interface = IvTools::getIfaceFromModel("CustomIface", ivModel.get());
    QVERIFY(interface);

    auto csvModel = ModelLoader::loadCsvModel("resources/test.csv");
    QVERIFY(csvModel);

    auto asn1Model = ModelLoader::loadAsn1Model("resources/dataview-uniq.asn");
    QVERIFY(asn1Model);

    TestGenerator testGenerator(QDir::currentPath());
    QString testedFunctionName =
            testGenerator.prepareTestHarness(*interface, *csvModel, *asn1Model, "x86 Linux CPP", 5000);

    QString generatedHarnessDirectory = "generated";
    auto generatedTestDriver = generatedHarnessDirectory + QDir::separator() + "testdriver.c";
    auto generatedIvFile = generatedHarnessDirectory + QDir::separator() + "interfaceview.xml";
    auto generatedDvFile = generatedHarnessDirectory + QDir::separator() + "deploymentview.dv.xml";

    QVERIFY(QFileInfo::exists(generatedTestDriver));
    QVERIFY(QFileInfo::exists(generatedIvFile));
    QVERIFY(QFileInfo::exists(generatedDvFile));

    QDir(generatedHarnessDirectory).removeRecursively();
}

void tst_testgenerator::testResultHtmlFile()
{
    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/interfaceview.xml");
    QVERIFY(ivModel);

    ivm::IVInterface *const interface = IvTools::getIfaceFromModel("CustomIface", ivModel.get());
    QVERIFY(interface);

    auto csvModel = ModelLoader::loadCsvModel("resources/test.csv");
    QVERIFY(csvModel);

    QVector<QVariant> testResults = { 1, 2, 3, 2, 5, 3, 4, 7, 12, 25 };
    const QString resultFileName = "Results.html";
    HtmlResultExporter exporter("Dummy board name", *interface, *csvModel, testResults, 0);
    exporter.exportResult(resultFileName);

    QVERIFY(QFileInfo::exists(resultFileName));
    QVERIFY(QFile(resultFileName).size() > 0);

    QFile::remove(resultFileName);
}

void tst_testgenerator::testResultHtmlData()
{
    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/interfaceview.xml");
    QVERIFY(ivModel);

    ivm::IVInterface *const interface = IvTools::getIfaceFromModel("CustomIface", ivModel.get());
    QVERIFY(interface);

    auto csvModel = ModelLoader::loadCsvModel("resources/test.csv");
    QVERIFY(csvModel);

    auto asn1Model = ModelLoader::loadAsn1Model("resources/dataview-uniq.asn");
    QVERIFY(asn1Model);

    QVector<QVariant> testResults = { 1, 2, 3, 2, 5, 3, 4, 7, 12, 25 };
    HtmlResultExporter exporter("Dummy board name", *interface, *csvModel, testResults, 0);
    auto cells = exporter.getData();

    constexpr auto G = HtmlResultExporter::CellColor::Green;
    constexpr auto B = HtmlResultExporter::CellColor::Black;

    const auto firstRow = QVector<HtmlResultExporter::Cell>({ { 1, B }, { 2, B }, { 3, B }, { 3, G }, { 0, G },
            { 2, B }, { 2, G }, { 0, G }, { 5, B }, { 5, G }, { 0, G } });

    const auto secondRow = QVector<HtmlResultExporter::Cell>({ { 3, B }, { 4, B }, { 7, B }, { 7, G }, { 0, G },
            { 12, B }, { 12, G }, { 0, G }, { 25, B }, { 25, G }, { 0, G } });

    QCOMPARE(cells[0], firstRow);
    QCOMPARE(cells[1], secondRow);
}

void tst_testgenerator::testParsingBoardSettings()
{
    const DataReconstructor::TypeLayoutInfos typeLayoutInfos = {
        { "INTEGER", 8, 0 },
        { "BOOLEAN", 1, 7 },
        { "REAL", 8, 0 },
    };
    LaunchConfiguration config("x86 Linux CPP", "/path/to/gdb/script", "gdb", "dummyClientParam1 $SCRIPT_PATH",
            "gdbserver", "dummyServerParam1 $BIN_PATH", typeLayoutInfos, QDataStream::LittleEndian, 5000);
    QCOMPARE(config.clientArgsParsed, QStringList({ "dummyClientParam1", "/path/to/gdb/script" }));
    QCOMPARE(config.serverArgsParsed, QStringList({ "dummyServerParam1", "hostpartition" }));
}

void tst_testgenerator::testStoringBoardsConfig()
{
    const QString configPath = "boards_config.txt";
    LaunchConfigLoader configLoader(configPath);
    QMap<QString, LaunchConfiguration> boardsConfig;
    const QString testBoardName = "x86 Linux CPP";

    const DataReconstructor::TypeLayoutInfos typeLayoutInfos = {
        { "INTEGER", 8, 0 },
        { "BOOLEAN", 1, 7 },
        { "REAL", 8, 0 },
    };

    LaunchConfiguration configToSave(testBoardName, "/path/to/script", "gdb", "dummyParam1 dummyParam2", "gdbserver",
            "dummyParam3 dummyParam4", typeLayoutInfos, QDataStream::LittleEndian, 5000);
    boardsConfig[testBoardName] = configToSave;
    configLoader.saveConfig(boardsConfig);

    boardsConfig = configLoader.loadConfig().value();
    LaunchConfiguration readConfig = boardsConfig[testBoardName];

    QCOMPARE(readConfig.scriptPath, "/path/to/script");
    QCOMPARE(readConfig.clientName, "gdb");
    QCOMPARE(readConfig.clientArgs, "dummyParam1 dummyParam2");
    QCOMPARE(readConfig.serverName, "gdbserver");
    QCOMPARE(readConfig.serverArgs, "dummyParam3 dummyParam4");
    QCOMPARE(readConfig.typeLayoutInfos["INTEGER"], typeLayoutInfos["INTEGER"]);
    QCOMPARE(readConfig.typeLayoutInfos["BOOLEAN"], typeLayoutInfos["BOOLEAN"]);
    QCOMPARE(readConfig.typeLayoutInfos["REAL"], typeLayoutInfos["REAL"]);
    QCOMPARE(readConfig.endianess, QDataStream::LittleEndian);
    QCOMPARE(readConfig.stackSize, 5000);

    QFile::remove(configPath);
}

}

QTEST_MAIN(tests::testgenerator::tst_testgenerator)

#include "tst_testgenerator.moc"
