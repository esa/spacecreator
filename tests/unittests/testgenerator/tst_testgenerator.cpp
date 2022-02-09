/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include <QDebug>
#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <asn1library/asn1/asn1model.h>
#include <conversion/asn1/Asn1Importer/importer.h>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/common/model.h>
#include <conversion/common/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvXmlImporter/importer.h>
#include <csv/CsvImporter/csvimporter.h>
#include <csv/CsvModel/csvmodel.h>
#include <csv/CsvOptions/options.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivobject.h>
#include <memory>
#include <qdebug.h>
#include <qdiriterator.h>
#include <qstandardpaths.h>
#include <qtestcase.h>
#include <shared/common.h>
#include <shared/sharedlibrary.h>
#include <sstream>
#include <testgenerator/TestGenerator.h>
#include <testgenerator/TestGeneratorException.h>

using testgenerator::TestGenerator;
using testgenerator::TestGeneratorException;

namespace tests::testgenerator {

class tst_testgenerator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testEmpty();
    void testNominal();
    void testNominalSwappedColumns();
    void testNominalTwoOutputs();
    void testCyclicInterface();
    void testImplementationNotInC();
};

static std::unique_ptr<conversion::Model> loadAsn1Model(const QString &filename)
{
    std::unique_ptr<conversion::Model> model;

    conversion::Options options;
    options.add(conversion::asn1::Asn1Options::inputFilepath, filename);

    conversion::asn1::importer::Asn1Importer importer;
    try {
        model = importer.importModel(options);
    } catch (const std::exception &ex) {
        return nullptr;
    }

    return model;
}

static std::unique_ptr<conversion::Model> loadIvModel(
        const QString &filename, QString configFilename = shared::interfaceCustomAttributesFilePath())
{
    std::unique_ptr<conversion::Model> model;

    conversion::Options options;
    options.add(conversion::iv::IvOptions::inputFilepath, filename);
    options.add(conversion::iv::IvOptions::configFilepath, std::move(configFilename));

    conversion::iv::importer::IvXmlImporter ivImporter;
    try {
        model = ivImporter.importModel(options);
    } catch (const std::exception &ex) {
        return nullptr;
    }

    return model;
}

static std::unique_ptr<csv::CsvModel> loadCsvModel(const QString &filename)
{
    csv::importer::CsvImporter importer;
    csv::importer::Options options;
    options.add(csv::importer::CsvOptions::inputFilepath, filename);

    auto csvModel = importer.importModel(options);
    if (csvModel == nullptr) {
        throw "CSV file could not be read";
    }

    return csvModel;
}

static void checkStreamAgainstExpectedOut(const std::stringstream &stream, const QString &expectedOutFilename)
{
    auto expectedOutputFile = QFile(expectedOutFilename);
    expectedOutputFile.open(QFile::ReadOnly | QFile::Text);
    const QStringList expectedOutStrList = QTextStream(&expectedOutputFile).readAll().split("\n");

    const QStringList actualOutStrList = QString::fromStdString(stream.str()).split("\n");
    for (int i = 0; i < expectedOutStrList.size(); i++) {
        if (actualOutStrList.size() == i) {
            qDebug() << (QString("missing line: %1").arg(expectedOutStrList[i]).toStdString().c_str());
            QFAIL("Actual size too short");
        }
        if (actualOutStrList[i].compare(expectedOutStrList[i]) != 0) {
            qDebug() << (QString("in line no %1").arg(i + 1).toStdString().c_str());
            qDebug() << (QString("expected %1").arg(expectedOutStrList[i]).toStdString().c_str());
            qDebug() << (QString("but was  %1").arg(actualOutStrList[i]).toStdString().c_str());
            QFAIL("Lines not equal");
        }
    }
}

void tst_testgenerator::testEmpty()
{
    const auto csvModel = loadCsvModel("resources/empty.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1ModelRaw = loadAsn1Model("resources/testgenerator.asn");
    const auto asn1Model = dynamic_cast<Asn1Acn::Asn1Model *>(asn1ModelRaw.get());
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModelRaw = loadIvModel("resources/nominal-interfaceview.xml", "resources/config.xml");
    const auto ivModel = dynamic_cast<ivm::IVModel *>(ivModelRaw.get());
    if (ivModel == nullptr) {
        QFAIL("No model");
    }
    const QString ifName = "PI_InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("Provided interface named %1 not found in given IV file").arg(ifName).toStdString().c_str());
    }
    const ivm::IVInterface &interface = *ifUnderTest;

    QVERIFY_EXCEPTION_THROWN(
            TestGenerator::generateTestDriver(csvRef, interface, asn1ModelRef), TestGeneratorException);
}

void tst_testgenerator::testNominal()
{
    auto csvModel = loadCsvModel("resources/test_data.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1ModelRaw = loadAsn1Model("resources/testgenerator.asn");
    const auto asn1Model = dynamic_cast<Asn1Acn::Asn1Model *>(asn1ModelRaw.get());
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModelRaw = loadIvModel("resources/nominal-interfaceview.xml", "resources/config.xml");
    const auto ivModel = dynamic_cast<ivm::IVModel *>(ivModelRaw.get());
    QVERIFY(ivModel != nullptr);

    const QString ifName = "PI_InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("Provided Interface named %1 not found in given IV file").arg(ifName).toStdString().c_str());
    }
    const ivm::IVInterface &interface = *ifUnderTest;

    auto outStream = TestGenerator::generateTestDriver(csvRef, interface, asn1ModelRef);

    checkStreamAgainstExpectedOut(outStream, "resources/testdriver.c.out");
}

void tst_testgenerator::testNominalSwappedColumns()
{
    auto csvModel = loadCsvModel("resources/test_data_swapped_columns.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1ModelRaw = loadAsn1Model("resources/testgenerator.asn");
    const auto asn1Model = dynamic_cast<Asn1Acn::Asn1Model *>(asn1ModelRaw.get());
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModelRaw = loadIvModel("resources/nominal-interfaceview.xml", "resources/config.xml");
    const auto ivModel = dynamic_cast<ivm::IVModel *>(ivModelRaw.get());
    QVERIFY(ivModel != nullptr);

    const QString ifName = "PI_InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("Provided Interface named %1 not found in given IV file").arg(ifName).toStdString().c_str());
    }
    const ivm::IVInterface &interface = *ifUnderTest;

    auto outStream = TestGenerator::generateTestDriver(csvRef, interface, asn1ModelRef);

    checkStreamAgainstExpectedOut(outStream, "resources/testdriver.c.out");
}

void tst_testgenerator::testNominalTwoOutputs()
{
    auto csvModel = loadCsvModel("resources/two_outputs-test_data.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1ModelRaw = loadAsn1Model("resources/testgenerator.asn");
    const auto asn1Model = dynamic_cast<Asn1Acn::Asn1Model *>(asn1ModelRaw.get());
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModelRaw = loadIvModel("resources/two_outputs-interfaceview.xml", "resources/config.xml");
    const auto ivModel = dynamic_cast<ivm::IVModel *>(ivModelRaw.get());
    QVERIFY(ivModel != nullptr);

    const QString ifName = "InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("Provided Interface named %1 not found in given IV file").arg(ifName).toStdString().c_str());
    }
    const ivm::IVInterface &interface = *ifUnderTest;

    auto outStream = TestGenerator::generateTestDriver(csvRef, interface, asn1ModelRef);

    checkStreamAgainstExpectedOut(outStream, "resources/two_outputs-testdriver.c.out");
}

void tst_testgenerator::testCyclicInterface()
{
    auto csvModel = loadCsvModel("resources/test_data.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1ModelRaw = loadAsn1Model("resources/testgenerator.asn");
    const auto asn1Model = dynamic_cast<Asn1Acn::Asn1Model *>(asn1ModelRaw.get());
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModelRaw = loadIvModel("resources/cyclicif-interfaceview.xml", "resources/config.xml");
    const auto ivModel = dynamic_cast<ivm::IVModel *>(ivModelRaw.get());
    QVERIFY(ivModel != nullptr);

    const QString ifName = "PI_InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("Provided Interface named %1 not found in given IV file").arg(ifName).toStdString().c_str());
    }
    const ivm::IVInterface &interface = *ifUnderTest;

    QVERIFY_EXCEPTION_THROWN(
            TestGenerator::generateTestDriver(csvRef, interface, asn1ModelRef), TestGeneratorException);
}

void tst_testgenerator::testImplementationNotInC()
{
    auto csvModel = loadCsvModel("resources/test_data.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1ModelRaw = loadAsn1Model("resources/testgenerator.asn");
    const auto asn1Model = dynamic_cast<Asn1Acn::Asn1Model *>(asn1ModelRaw.get());
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModelRaw = loadIvModel("resources/implementation-interfaceview.xml", "resources/config.xml");
    const auto ivModel = dynamic_cast<ivm::IVModel *>(ivModelRaw.get());
    QVERIFY(ivModel != nullptr);

    const QString ifName = "PI_InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("Provided Interface named %1 not found in given IV file").arg(ifName).toStdString().c_str());
    }
    const ivm::IVInterface &interface = *ifUnderTest;

    auto outStream = TestGenerator::generateTestDriver(csvRef, interface, asn1ModelRef);

    checkStreamAgainstExpectedOut(outStream, "resources/testdriver.c.out");
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_testgenerator)

#include "tst_testgenerator.moc"
