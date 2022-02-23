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

#include "../common.h"
#include "dvtools.h"

#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <dvcore/dvmodel.h>
#include <dvcore/dvobject.h>
#include <harness/dvgenerator/dvgenerator.h>
#include <ivcore/ivfunction.h>
#include <libdveditor/dvexporter.h>
#include <memory>
#include <qobjectdefs.h>
#include <testgenerator/testgenerator.h>

using testgenerator::DvGenerator;

namespace tests::testgenerator {

class tst_dvgenerator final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testNominal();

private:
    const QString dvDir = "resources";
    const QString dvPath =
            QString("%1%2%3.dv.xml").arg(dvDir).arg(QDir::separator()); // for example resources/deploymentview.dv.xml
    const QString dvConfig = QString("%1%2config.xml").arg(dvDir).arg(QDir::separator());

    std::unique_ptr<dve::DVExporter> m_exporter;
};

void tst_dvgenerator::initTestCase()
{
    //
}

void tst_dvgenerator::testNominal()
{
    std::vector<ivm::IVFunction *> functionsToBind;

    const auto generatedModel = DvGenerator::generate(functionsToBind);

    QVERIFY(generatedModel != nullptr);

    const auto generatedDvObjects = dvtools::getDvObjectsFromModel(generatedModel.get());
    const auto expectedDvObjects = dvtools::getDvObjectsFromFile(dvPath.arg("deploymentview"));
    QVERIFY(generatedDvObjects != nullptr);
    QVERIFY(expectedDvObjects != nullptr);

    QCOMPARE(generatedDvObjects->size(), expectedDvObjects->size());
    const int dvObjectsSize = generatedDvObjects->size();

    const QVector<int> map =
            createQVectorToQVectorMap<dvm::DVObject *>(*generatedDvObjects, *expectedDvObjects, elementsEqualByTitle);
    for (int i = 0; i < dvObjectsSize; i++) {
        const auto &generatedDvObj = *generatedDvObjects->at(i);
        const auto &expectedDvObj = *expectedDvObjects->at(map.at(i));

        QCOMPARE(generatedDvObj.title(), expectedDvObj.title());
        QCOMPARE(generatedDvObj.type(), expectedDvObj.type());

        // compare obj properties
        const auto &generatedProperties = generatedDvObj.properties();
        const auto &expectedProperties = expectedDvObj.properties();

        qDebug() << "gp: " << generatedProperties;
        qDebug() << "ep: " << expectedProperties;

        QCOMPARE(generatedProperties.size(), expectedProperties.size());
        const int propertiesSize = generatedProperties.size();

        for (int j = 0; j < propertiesSize; j++) {
            const auto &generatedProperty = generatedProperties.at(j);
            const auto &expectedProperty = expectedProperties.at(j);

            qDebug() << expectedProperty.toString();
            QCOMPARE(generatedProperty.toString(), expectedProperty.toString());
            QCOMPARE(generatedProperty.typeName(), expectedProperty.typeName());
            QCOMPARE(generatedProperty.type(), expectedProperty.type());
        }
        // TODO: compare other members of objects: object type, parameters etc.
    }
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_dvgenerator)

#include "tst_dvgenerator.moc"
