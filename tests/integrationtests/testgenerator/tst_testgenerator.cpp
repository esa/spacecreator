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
#include <ivtools.h>
#include <modelloader.h>
#include <qtestcase.h>
#include <testgenerator.h>

using plugincommon::IvTools;
using plugincommon::ModelLoader;
using testgenerator::TestGenerator;

namespace tests::testgenerator {

class tst_testgenerator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testPrepareTestHarness();
};

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
    QString testedFunctionName = testGenerator.prepareTestHarness(*interface, *csvModel, *asn1Model);

    QString generatedHarnessDirectory = "generated";
    auto generatedTestDriver = generatedHarnessDirectory + QDir::separator() + "testdriver.c";
    auto generatedIvFile = generatedHarnessDirectory + QDir::separator() + "interfaceview.xml";
    auto generatedDvFile = generatedHarnessDirectory + QDir::separator() + "deploymentview.dv.xml";

    QVERIFY(QFileInfo::exists(generatedTestDriver));
    QVERIFY(QFileInfo::exists(generatedIvFile));
    QVERIFY(QFileInfo::exists(generatedDvFile));

    QDir(generatedHarnessDirectory).removeRecursively();
}

}

QTEST_MAIN(tests::testgenerator::tst_testgenerator)

#include "tst_testgenerator.moc"
