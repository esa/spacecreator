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

#include <QBuffer>
#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <cstddef>
#include <dvcore/dvfunction.h>
#include <dvcore/dvmodel.h>
#include <dvcore/dvobject.h>
#include <dvcore/dvpartition.h>
#include <dvcore/dvport.h>
#include <harness/dvgenerator/dvgenerator.h>
#include <ivcore/ivfunction.h>
#include <libdveditor/dveditor.h>
#include <libdveditor/dvexporter.h>
#include <memory>
#include <qobjectdefs.h>
#include <shared/veobject.h>
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
static auto checkPartitions(const dvm::DVObject &actualObj, const dvm::DVObject &expectedObj) -> void;
static auto checkTypeSpecificMembers(const dvm::DVObject &generatedObj, const dvm::DVObject &expectedObj) -> void;
static auto exportModel(QVector<dvm::DVObject *> *objects, const QString &outputFilename) -> void;
static auto getRawPointersVector(const std::vector<std::unique_ptr<ivm::IVFunction>> &functions)
        -> std::vector<ivm::IVFunction *>;

void tst_dvgenerator::initTestCase()
{
    dve::initDvEditor();
}

void tst_dvgenerator::testNominal()
{
    const QString &nominalOutputFileName = "nominal.dv.xml";
    const std::vector<QString> functionTitles = {
        "TestDriver",
        "FunctionUnderTest",
    };

    std::vector<std::unique_ptr<ivm::IVFunction>> functions;
    functions.push_back(std::make_unique<ivm::IVFunction>());
    functions.push_back(std::make_unique<ivm::IVFunction>());
    functions.front()->setTitle(functionTitles.front());
    functions.back()->setTitle(functionTitles.back());

    std::vector<ivm::IVFunction *> functionsToBind = getRawPointersVector(functions);

    const std::unique_ptr<dvm::DVModel> generatedModel = DvGenerator::generate(functionsToBind);
    QVERIFY(generatedModel != nullptr);
    const auto generatedDvObjects = dvtools::getDvObjectsFromModel(generatedModel.get());
    QVERIFY(generatedDvObjects != nullptr);

    exportModel(generatedDvObjects.get(), nominalOutputFileName);

    const auto expectedDvObjects = dvtools::getDvObjectsFromFile(dvPath.arg("deploymentview-linux-x86"));
    QVERIFY(expectedDvObjects != nullptr);
    checkObjVectors(generatedDvObjects.get(), expectedDvObjects.get());
}

void checkObjVectors(QVector<dvm::DVObject *> *actualObjs, QVector<dvm::DVObject *> *expectedObjs)
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

        if (expectedObj.parentObject() != nullptr) {
            if (generatedObj.parentObject() != nullptr) {
                QCOMPARE(generatedObj.parentObject()->title(), expectedObj.parentObject()->title());
            } else {
                QFAIL("generated parent object set to nullptr but expected non-nullptr");
            }
        } else {
            if (generatedObj.parentObject() != nullptr) {
                QFAIL("generated parent object is non-nullptr but expected nullptr");
            }
        }

        QCOMPARE(generatedObj.coordinates(), expectedObj.coordinates());
        checkEntityProperties(generatedObj, expectedObj);
        checkEntityAttributes(generatedObj, expectedObj);
        checkTypeSpecificMembers(generatedObj, expectedObj);
    }
}

void checkEntityProperties(const dvm::DVObject &actual, const dvm::DVObject &expected)
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

void checkEntityAttributes(const dvm::DVObject &actual, const dvm::DVObject &expected)
{
    const auto &actualEntityAttributes = actual.entityAttributes();
    const auto &expectedEntityAttributes = expected.entityAttributes();

    QCOMPARE(actualEntityAttributes.size(), expectedEntityAttributes.size());

    for (const auto &expectedEntityAttribute : expectedEntityAttributes) {
        const auto &expectedValue = expectedEntityAttribute.value().value<QString>();
        const auto &actualValue = actualEntityAttributes.value(expectedEntityAttribute.name()).value<QString>();

        QCOMPARE(actualValue, expectedValue);
    }
}

void checkTypeSpecificMembers(const dvm::DVObject &actualObj, const dvm::DVObject &expectedObj)
{
    switch (actualObj.type()) {
    case dvm::DVObject::Type::Node:
        break;
    case dvm::DVObject::Type::Function:
        break;
    case dvm::DVObject::Type::Connection:
        break;
    case dvm::DVObject::Type::Message:
        break;
    case dvm::DVObject::Type::Bus:
        break;
    case dvm::DVObject::Type::Board:
        break;
    case dvm::DVObject::Type::Port:
        break;
    case dvm::DVObject::Type::SystemInterface:
        break;
    case dvm::DVObject::Type::SystemFunction:
        break;
    case dvm::DVObject::Type::Device:
        break;
    case dvm::DVObject::Type::Partition:
        checkPartitions(actualObj, expectedObj);
        break;
    case dvm::DVObject::Type::Unknown:
        QFAIL("Unknown type in generated object");
    }
}

void checkPartitions(const dvm::DVObject &actualObj, const dvm::DVObject &expectedObj)
{
    const auto *const actualPartition = static_cast<const dvm::DVPartition *>(&actualObj);
    const auto *const expectedPartition = static_cast<const dvm::DVPartition *>(&expectedObj);

    QVERIFY(actualPartition != nullptr);
    QVERIFY(expectedPartition != nullptr);

    const auto &actualFunctions = actualPartition->functions();
    const auto &expectedFunctions = expectedPartition->functions();

    QCOMPARE(actualFunctions.size(), expectedFunctions.size());
    const int functionsSize = actualPartition->functions().size();

    for (int j = 0; j < functionsSize; j++) {
        const auto &actualFunction = actualFunctions.at(j);
        const auto &expectedFunction = expectedFunctions.at(j);

        QVERIFY(actualFunction != nullptr);
        QVERIFY(expectedFunction != nullptr);

        QCOMPARE(actualFunction->title(), expectedFunction->title());
        QCOMPARE(actualFunction->implementation(), expectedFunction->implementation());
    }
}

void exportModel(QVector<dvm::DVObject *> *const generatedDvObjects, const QString &outputFilename)
{
    dve::DVExporter exporter;
    QList<shared::VEObject *> objects;
    std::for_each(generatedDvObjects->begin(), generatedDvObjects->end(),
            [&objects](const auto &obj) { objects.push_back(obj); });

    QByteArray qba(objects.size() * 1'000, '\00');
    QBuffer buf = QBuffer(&qba);

    exporter.exportObjects(objects, &buf);

    QFile file(outputFilename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    file.write(qba);
}

std::vector<ivm::IVFunction *> getRawPointersVector(
        const std::vector<std::unique_ptr<ivm::IVFunction>> &uniquePointersVector)
{
    std::vector<ivm::IVFunction *> rawePointersVector;

    std::for_each(uniquePointersVector.begin(), uniquePointersVector.end(),
            [&rawePointersVector](const auto &f) { rawePointersVector.push_back(f.get()); });

    return rawePointersVector;
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_dvgenerator)

#include "tst_dvgenerator.moc"
