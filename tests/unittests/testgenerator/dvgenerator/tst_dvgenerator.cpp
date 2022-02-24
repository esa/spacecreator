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
#include <algorithm>
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

static auto checkEntityProperties(const dvm::DVObject &actual, const dvm::DVObject &expected) -> void;
static auto checkObjVectors(QVector<dvm::DVObject *> *actual, QVector<dvm::DVObject *> *expected) -> void;
static auto checkEntityAttributes(const dvm::DVObject &actual, const dvm::DVObject &expected) -> void;

void tst_dvgenerator::initTestCase()
{
    //
}

void tst_dvgenerator::testNominal()
{
    std::vector<std::shared_ptr<ivm::IVFunction>> functions {
        std::make_shared<ivm::IVFunction>(),
        std::make_shared<ivm::IVFunction>(),
    };
    functions.front()->setTitle("TestDriver");
    functions.back()->setTitle("FunctionUnderTest");

    std::vector<ivm::IVFunction *> functionsToBind;
    std::for_each(functions.begin(), functions.end(),
            [&functionsToBind](const auto &f) { functionsToBind.push_back(f.get()); });

    const auto generatedModel = DvGenerator::generate(functionsToBind);

    QVERIFY(generatedModel != nullptr);
    const auto generatedDvObjects = dvtools::getDvObjectsFromModel(generatedModel.get());
    const auto expectedDvObjects = dvtools::getDvObjectsFromFile(dvPath.arg("deploymentview"));
    QVERIFY(generatedDvObjects != nullptr);
    QVERIFY(expectedDvObjects != nullptr);

    checkObjVectors(generatedDvObjects.get(), expectedDvObjects.get());
}

static void checkObjVectors(QVector<dvm::DVObject *> *actualObjs, QVector<dvm::DVObject *> *expectedObjs)
{
    QCOMPARE(actualObjs->size(), expectedObjs->size());
    const int objsSize = actualObjs->size();

    const QVector<int> map =
            createQVectorToQVectorMap<dvm::DVObject *>(*actualObjs, *expectedObjs, elementsEqualByTitle);
    for (int i = 0; i < objsSize; i++) {
        const auto &generatedObj = *actualObjs->at(i);
        const auto &expectedObj = *expectedObjs->at(map.at(i));

        QCOMPARE(generatedObj.title(), expectedObj.title());
        QCOMPARE(generatedObj.type(), expectedObj.type());

        checkEntityProperties(generatedObj, expectedObj);

        QCOMPARE(generatedObj.coordinates(), expectedObj.coordinates());
        qDebug() << "object: " << generatedObj.title();
        checkEntityAttributes(generatedObj, expectedObj);
        // check parameters?
    }
}

static void checkEntityProperties(const dvm::DVObject &actual, const dvm::DVObject &expected)
{
    const auto &actualProperties = actual.properties();
    const auto &expectedProperties = expected.properties();

    QCOMPARE(actualProperties.size(), expectedProperties.size());
    const int propertiesSize = actualProperties.size();

    for (int j = 0; j < propertiesSize; j++) {
        const auto &actualProperty = actualProperties.at(j);
        const auto &expectedProperty = expectedProperties.at(j);

        QCOMPARE(actualProperty.toString(), expectedProperty.toString());
        QCOMPARE(actualProperty.typeName(), expectedProperty.typeName());
        QCOMPARE(actualProperty.type(), expectedProperty.type());
        QCOMPARE(actualProperty.value<QString>(), expectedProperty.value<QString>());
    }
}

static void checkEntityAttributes(const dvm::DVObject &actual, const dvm::DVObject &expected)
{
    const auto &actualEntityAttributes = actual.entityAttributes();
    const auto &expectedEntityAttributes = expected.entityAttributes();

    qDebug() << "";
    qDebug() << "expected  entity attributes: ";
    for (const auto &expectedEntityAttribute : expectedEntityAttributes) {
        qDebug() << expectedEntityAttribute.name() << expectedEntityAttribute.value();
    }

    qDebug() << "";
    qDebug() << "generated entity attributes: ";
    for (const auto &generatedEntityAttribute : actualEntityAttributes) {
        qDebug() << generatedEntityAttribute.name() << generatedEntityAttribute.value();
    }

    QCOMPARE(actualEntityAttributes.size(), expectedEntityAttributes.size());

    for (const auto &expectedEntityAttribute : expectedEntityAttributes) {
        (void)expectedEntityAttribute;
        // QCOMPARE(actualEntityAttributes.value(expectedEntityAttribute.name()), expectedEntityAttribute.value());
    }
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_dvgenerator)

#include "tst_dvgenerator.moc"
