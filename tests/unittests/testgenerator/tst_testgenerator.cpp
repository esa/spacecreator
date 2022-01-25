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

#include "ivlibrary.h"
#include "sharedlibrary.h"

#include <QDebug>
#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <conversion/common/model.h>
#include <conversion/common/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvXmlImporter/importer.h>
#include <csv/CsvImporter/csvimporter.h>
#include <csv/CsvModel/csvmodel.h>
#include <csv/CsvOptions/options.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivobject.h>
#include <memory>
#include <qdebug.h>
#include <qdiriterator.h>
#include <qstandardpaths.h>
#include <qtestcase.h>
#include <shared/common.h>
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
};

static std::unique_ptr<conversion::Model> loadIvModel(
        const QString &ivFilename, QString ivConfigFilename = shared::interfaceCustomAttributesFilePath())
{
    std::unique_ptr<conversion::Model> model;

    conversion::Options options;
    options.add(conversion::iv::IvOptions::inputFilepath, ivFilename);
    options.add(conversion::iv::IvOptions::configFilepath, std::move(ivConfigFilename));

    conversion::iv::importer::IvXmlImporter ivImporter;
    try {
        model = ivImporter.importModel(options);
    } catch (const std::exception &ex) {
        return nullptr;
    }

    return model;
}

void tst_testgenerator::testEmpty()
{
    shared::initSharedLibrary();
    ivm::initIVLibrary();

    csv::importer::CsvImporter importer;
    csv::importer::Options options;
    options.add(csv::importer::CsvOptions::separator, ",");
    options.add(csv::importer::CsvOptions::inputFilepath, "empty.csv");

    const auto csvModel = importer.importModel(options);
    QVERIFY(csvModel != nullptr);
    const csv::CsvModel &csvRef = *csvModel;
    QVERIFY(csvRef.header().fields().empty());

    const auto model = loadIvModel("interfaceview.xml", "config.xml");

    const auto ivModel = dynamic_cast<ivm::IVModel *>(model.get());
    if (ivModel == nullptr) {
        QFAIL("no model");
    }
    const QString ifName = "PI_InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("provided if named not %1 found in given IV file").arg(ifName).toStdString().c_str());
    }

    ivm::IVInterface::CreationInfo ci;
    const ivm::IVInterface &interface = *ifUnderTest;
    QVERIFY_EXCEPTION_THROWN(TestGenerator::generateTestDriver(csvRef, interface), TestGeneratorException);
}

void tst_testgenerator::testNominal()
{
    csv::importer::CsvImporter importer;
    csv::importer::Options options;
    options.add(csv::importer::CsvOptions::separator, ",");
    options.add(csv::importer::CsvOptions::inputFilepath, "test_data.csv");

    auto csvModel = importer.importModel(options);
    QVERIFY(csvModel != nullptr);
    const csv::CsvModel &csvRef = *csvModel;
    QVERIFY(csvRef.header().fields().size() == 4);

    QString headerText = "";
    const auto &headerFields = csvModel->header().fields();
    headerText = *headerFields.begin();
    for (auto it = std::next(headerFields.begin()); it != headerFields.end(); it++) {
        headerText = QString("%1,%2").arg(headerText, *it);
    }
    QCOMPARE(headerText, "active,temperature,posX,posY");

    const auto model = loadIvModel("interfaceview.xml", "config.xml");

    const auto ivModel = dynamic_cast<ivm::IVModel *>(model.get());
    if (ivModel == nullptr) {
        QFAIL("The model could not be read");
    }
    const QString ifName = "PI_InterfaceUnderTest";
    const auto ifUnderTest = ivModel->getIfaceByName(ifName, ivm::IVInterface::InterfaceType::Provided);
    if (ifUnderTest == nullptr) {
        QFAIL(QString("provided if named not %1 found in given IV file").arg(ifName).toStdString().c_str());
    }

    ivm::IVInterface::CreationInfo ci;
    const ivm::IVInterface &interface = *ifUnderTest;

    // TODO: verify that generator with correct inputs creates a correct source file
    // - csv data read
    // - function's interface from interfaceview (protected or unprotected)
    auto testDriver = TestGenerator::generateTestDriver(csvRef, interface);

    const QString testDriverStr = QString::fromStdString(testDriver.str());
    const QStringList testDriverStrList = testDriverStr.split("\n");

    auto expectedFile = QFile("testdriver.c.out");
    expectedFile.open(QFile::ReadOnly | QFile::Text);
    const QStringList expectedStrList = QTextStream(&expectedFile).readAll().split("\n");

    for (int i = 0; i < expectedStrList.size(); i++) {
        if (testDriverStrList.size() == i) {
            qDebug() << (QString("missing line: %1").arg(expectedStrList[i]).toStdString().c_str());
            QFAIL("Actual size too short");
        }
        if (testDriverStrList[i].compare(expectedStrList[i]) != 0) {
            qDebug() << (QString("in line no %1").arg(i).toStdString().c_str());
            qDebug() << (QString("expected %1").arg(expectedStrList[i]).toStdString().c_str());
            qDebug() << (QString("but was  %1").arg(testDriverStrList[i]).toStdString().c_str());
            QFAIL("Lines not equal");
        }
    }
}

// TODO: test case to verify that the generator throws an exception on cyclic interface

// TODO: test case to verify that the generator throws an exception on sporadic interface

// TODO: test case to verify that the generator throws an exception on interface with implementation
//       NOT in C

// TODO:

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_testgenerator)

#include "tst_testgenerator.moc"
