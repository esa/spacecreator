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
#include <QTest>
#include <QtTest/qtestcase.h>
#include <dvcore/dvfunction.h>
#include <exception>
#include <ivcore/ivfunction.h>
#include <memory>
#include <qobjectdefs.h>
#include <shared/common.h>
#include <stdexcept>
#include <testgenerator/testgenerator.h>

using testgenerator::DvGenerator;

namespace tests::testgenerator {

class tst_dvgenerator final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testLinuxX86();
    void testArmV71();

private:
    const QString dvDir = "resources";
    const QString dvPath =
            QString("%1%2%3.dv.xml").arg(dvDir).arg(QDir::separator()); // for example resources/deploymentview.dv.xml
    const QString dvConfig = QString("%1%2config.xml").arg(dvDir).arg(QDir::separator());

    std::vector<std::unique_ptr<ivm::IVFunction>> ivFunctions;
    std::vector<ivm::IVFunction *> ivFunctionsRaw;

    auto makeIvFunctionsForDv() -> std::vector<std::unique_ptr<ivm::IVFunction>>;
    auto getRawPointersVector(const std::vector<std::unique_ptr<ivm::IVFunction>> &uniquePointersVector)
            -> std::vector<ivm::IVFunction *>;

public:
    const std::vector<QString> functionTitles = {
        "TestDriver",
        "FunctionUnderTest",
    };
};

static auto checkEntityProperties(const dvm::DVObject &actual, const dvm::DVObject &expected) -> void;
static auto checkEntityAttributes(const dvm::DVObject &actual, const dvm::DVObject &expected) -> void;
static auto checkObjVectors(QVector<dvm::DVObject *> *actual, QVector<dvm::DVObject *> *expected) -> void;
static auto checkParentObject(dvm::DVObject *actual, dvm::DVObject *expected) -> void;
static auto checkPartitions(const dvm::DVPartition &actualPartition, const dvm::DVPartition &expectedPartition) -> void;

void tst_dvgenerator::initTestCase()
{
    ivFunctions = makeIvFunctionsForDv();
    ivFunctionsRaw = getRawPointersVector(ivFunctions);
}

void tst_dvgenerator::testLinuxX86()
{
    const QString outputFileName = "deploymentview-linux-x86.dv.xml";
    const QString expectedOutputFilename = dvPath.arg("deploymentview-linux-x86");

    const std::unique_ptr<dvm::DVModel> generatedModel =
            DvGenerator::generate(ivFunctionsRaw, "x86 Linux C++", "x86_Linux_TestRunner", "Node_1", "hostPartition");

    QVERIFY(generatedModel != nullptr);
    const auto generatedDvObjects = dvtools::getDvObjectsFromModel(generatedModel.get());
    QVERIFY(generatedDvObjects != nullptr);

    dvtools::exportModel(generatedDvObjects.get(), outputFileName);

    const auto expectedDvObjects = dvtools::getDvObjectsFromFile(expectedOutputFilename);
    QVERIFY(expectedDvObjects != nullptr);

    checkObjVectors(generatedDvObjects.get(), expectedDvObjects.get());
}

void tst_dvgenerator::testArmV71()
{
    const QString outputFileName = "deploymentview-samv71.dv.xml";
    const QString expectedOutputFilename = dvPath.arg("deploymentview-samv71");

    const std::unique_ptr<dvm::DVModel> generatedModel = DvGenerator::generate(
            ivFunctionsRaw, "SAM V71 FreeRTOS N7S", "SAM V71 FreeRTOS N7S_1", "Node_1", "hostPartition");

    QVERIFY(generatedModel != nullptr);
    const auto generatedDvObjects = dvtools::getDvObjectsFromModel(generatedModel.get());
    QVERIFY(generatedDvObjects != nullptr);
    QVERIFY(!generatedDvObjects->isEmpty());

    dvtools::exportModel(generatedDvObjects.get(), outputFileName);

    const auto expectedDvObjects = dvtools::getDvObjectsFromFile(expectedOutputFilename);
    QVERIFY(expectedDvObjects != nullptr);
    checkObjVectors(generatedDvObjects.get(), expectedDvObjects.get());
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

        checkParentObject(generatedObj.parentObject(), expectedObj.parentObject());
        checkEntityProperties(generatedObj, expectedObj);
        checkEntityAttributes(generatedObj, expectedObj);
        if (generatedObj.type() == dvm::DVObject::Type::Partition) {
            const auto *const actualPartition = static_cast<const dvm::DVPartition *>(&generatedObj);
            const auto *const expectedPartition = static_cast<const dvm::DVPartition *>(&expectedObj);
            if (actualPartition == nullptr || expectedPartition == nullptr) {
                throw std::runtime_error("Specified DV object type is Partition, but cannot cast to DVPartition type");
            }
            checkPartitions(*actualPartition, *expectedPartition);
        }
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

void checkPartitions(const dvm::DVPartition &actualPartition, const dvm::DVPartition &expectedPartition)
{
    const auto &actualFunctions = actualPartition.functions();
    const auto &expectedFunctions = expectedPartition.functions();

    QCOMPARE(actualFunctions.size(), expectedFunctions.size());
    const int functionsSize = actualPartition.functions().size();

    for (int j = 0; j < functionsSize; j++) {
        const auto &actualFunction = actualFunctions.at(j);
        const auto &expectedFunction = expectedFunctions.at(j);

        QVERIFY(actualFunction != nullptr);
        QVERIFY(expectedFunction != nullptr);

        QCOMPARE(actualFunction->title(), expectedFunction->title());
        QCOMPARE(actualFunction->implementation(), expectedFunction->implementation());
    }
}

void checkParentObject(dvm::DVObject *const actual, dvm::DVObject *const expected)
{
    if (expected != nullptr) {
        if (actual != nullptr) {
            QCOMPARE(actual->title(), expected->title());
        } else {
            QFAIL("generated parent object set to nullptr but expected non-nullptr");
        }
    } else {
        if (actual != nullptr) {
            QFAIL("generated parent object is non-nullptr but expected nullptr");
        }
    }
}

std::vector<ivm::IVFunction *> tst_dvgenerator::getRawPointersVector(
        const std::vector<std::unique_ptr<ivm::IVFunction>> &uniquePointersVector)
{
    std::vector<ivm::IVFunction *> rawPtrsVector;

    std::for_each(uniquePointersVector.begin(), uniquePointersVector.end(),
            [&rawPtrsVector](const auto &f) { rawPtrsVector.push_back(f.get()); });

    return rawPtrsVector;
}

std::vector<std::unique_ptr<ivm::IVFunction>> tst_dvgenerator::makeIvFunctionsForDv()
{
    std::vector<std::unique_ptr<ivm::IVFunction>> functions;
    functions.push_back(std::make_unique<ivm::IVFunction>());
    functions.push_back(std::make_unique<ivm::IVFunction>());
    functions.front()->setTitle(tst_dvgenerator::functionTitles.front());
    functions.back()->setTitle(tst_dvgenerator::functionTitles.back());

    return functions;
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_dvgenerator)

#include "tst_dvgenerator.moc"
