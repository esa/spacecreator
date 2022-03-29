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

#include "gdbconnectorstub.h"

#include <QObject>
#include <QTest>
#include <QVariant>
#include <QtTest/qtestcase.h>
#include <ivtools.h>
#include <modelloader.h>
#include <qobjectdefs.h>
#include <testgenerator/datareconstructor/datareconstructor.h>

using plugincommon::IvTools;
using plugincommon::ModelLoader;
using testgenerator::DataReconstructor;
using testgenerator::GdbConnector;

namespace tests::testgenerator {

class tst_datareconstructor final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testNominal() const;

private:
    const unsigned int numberOfTestVectors = 5;
};

void tst_datareconstructor::testNominal() const
{
    const QVector<QVariant> expectedTestData = {
        true, 2.3000, -2, 3, true, //
        true, 5.3444, -2, 3, true, //
        false, 2.3000, -8, 3, true, //
        false, 2.3000, -1, -1, true, //
        true, 2.3000, 4, 1, true, //
    };

    // these are just dummy values
    const QString binLocalization = qEnvironmentVariable("$HOME/testBinaries/");
    const QString script = "x86-linux-cpp.gdb";
    const QString binToRun = "hostpartition";
    const QByteArray rawTestData =
            GdbConnector::getRawTestResults(binLocalization, { "-batch", "-x", script }, { "host:1234", binToRun });

    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/interfaceview.xml");
    ivm::IVInterface *const ivIface = IvTools::getIfaceFromModel("InterfaceUnderTest", ivModel.get());

    const QString asn1Filepath = QString("%1%2%3").arg("resources").arg(QDir::separator()).arg("testharness.asn");
    const auto asn1Model = ModelLoader::loadAsn1Model(asn1Filepath);

    const QMap<QString, int> typeSizes = {
        { "INTEGER", 8 },
        { "BOOLEAN", 8 },
        { "REAL", 8 },
    };
    const QVector<QVariant> readTestData = DataReconstructor::getVariantVectorFromRawData(
            rawTestData, numberOfTestVectors, ivIface, asn1Model.get(), QDataStream::LittleEndian, typeSizes);

    QCOMPARE(readTestData.size(), expectedTestData.size());
    const int dataSize = readTestData.size();
    for (int i = 0; i < dataSize; i++) {
        QCOMPARE(readTestData.at(i), expectedTestData.at(i));
    }
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_datareconstructor)

#include "tst_datareconstructor.moc"
