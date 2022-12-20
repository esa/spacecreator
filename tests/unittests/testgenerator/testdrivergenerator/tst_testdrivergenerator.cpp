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

#include <QDirIterator>
#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <conversion/common/model.h>
#include <conversion/common/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvXmlImporter/importer.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivobject.h>
#include <memory>
#include <modelloader.h>
#include <qtestcase.h>
#include <shared/common.h>
#include <shared/sharedlibrary.h>
#include <sstream>
#include <testgenerator/testgenerator.h>

using plugincommon::ModelLoader;
using testgenerator::TestDriverGenerator;
using testgenerator::TestDriverGeneratorException;

namespace tests::testgenerator {

class tst_testdrivergenerator : public QObject
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

void tst_testdrivergenerator::testEmpty()
{
    const auto csvModel = ModelLoader::loadCsvModel("resources/empty.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1Model = ModelLoader::loadAsn1Model("resources/testgenerator.asn");
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/nominal-interfaceview.xml");
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
            TestDriverGenerator::generateTestDriver(csvRef, interface, asn1ModelRef), TestDriverGeneratorException);
}

void tst_testdrivergenerator::testNominal()
{
    auto csvModel = ModelLoader::loadCsvModel("resources/test_data.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1Model = ModelLoader::loadAsn1Model("resources/testgenerator.asn");
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/nominal-interfaceview.xml");
    QVERIFY(ivModel != nullptr);

    const QString ifName = "PI_InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("Provided Interface named %1 not found in given IV file").arg(ifName).toStdString().c_str());
    }
    const ivm::IVInterface &interface = *ifUnderTest;

    const auto outStream = TestDriverGenerator::generateTestDriver(csvRef, interface, asn1ModelRef);

    checkStreamAgainstExpectedOut(outStream, "resources/testdriver.c.out");
}

void tst_testdrivergenerator::testNominalSwappedColumns()
{
    auto csvModel = ModelLoader::loadCsvModel("resources/test_data_swapped_columns.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1Model = ModelLoader::loadAsn1Model("resources/testgenerator.asn");
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/nominal-interfaceview.xml");
    QVERIFY(ivModel != nullptr);

    const QString ifName = "PI_InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("Provided Interface named %1 not found in given IV file").arg(ifName).toStdString().c_str());
    }
    const ivm::IVInterface &interface = *ifUnderTest;

    const auto outStream = TestDriverGenerator::generateTestDriver(csvRef, interface, asn1ModelRef);

    checkStreamAgainstExpectedOut(outStream, "resources/testdriver.c.out");
}

void tst_testdrivergenerator::testNominalTwoOutputs()
{
    auto csvModel = ModelLoader::loadCsvModel("resources/two_outputs-test_data.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1Model = ModelLoader::loadAsn1Model("resources/testgenerator.asn");
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/two_outputs-interfaceview.xml");
    QVERIFY(ivModel != nullptr);

    const QString ifName = "InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("Provided Interface named %1 not found in given IV file").arg(ifName).toStdString().c_str());
    }
    const ivm::IVInterface &interface = *ifUnderTest;

    const auto outStream = TestDriverGenerator::generateTestDriver(csvRef, interface, asn1ModelRef);

    checkStreamAgainstExpectedOut(outStream, "resources/two_outputs-testdriver.c.out");
}

void tst_testdrivergenerator::testCyclicInterface()
{
    auto csvModel = ModelLoader::loadCsvModel("resources/test_data.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1Model = ModelLoader::loadAsn1Model("resources/testgenerator.asn");
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/cyclicif-interfaceview.xml");
    QVERIFY(ivModel != nullptr);

    const QString ifName = "PI_InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("Provided Interface named %1 not found in given IV file").arg(ifName).toStdString().c_str());
    }
    const ivm::IVInterface &interface = *ifUnderTest;

    QVERIFY_EXCEPTION_THROWN(
            TestDriverGenerator::generateTestDriver(csvRef, interface, asn1ModelRef), TestDriverGeneratorException);
}

void tst_testdrivergenerator::testImplementationNotInC()
{
    auto csvModel = ModelLoader::loadCsvModel("resources/test_data.csv");
    const csv::CsvModel &csvRef = *csvModel;

    const auto asn1Model = ModelLoader::loadAsn1Model("resources/testgenerator.asn");
    QVERIFY(asn1Model != nullptr);
    const Asn1Acn::Asn1Model &asn1ModelRef = *asn1Model;

    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/implementation-interfaceview.xml");
    QVERIFY(ivModel != nullptr);

    const QString ifName = "PI_InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("Provided Interface named %1 not found in given IV file").arg(ifName).toStdString().c_str());
    }
    const ivm::IVInterface &interface = *ifUnderTest;

    const auto outStream = TestDriverGenerator::generateTestDriver(csvRef, interface, asn1ModelRef);

    checkStreamAgainstExpectedOut(outStream, "resources/testdriver.c.out");
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_testdrivergenerator)

#include "tst_testdrivergenerator.moc"
