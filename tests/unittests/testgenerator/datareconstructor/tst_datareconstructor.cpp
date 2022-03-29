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
#include "asn1model.h"
#include "dataview-uniq.h"
#include "gdbconnectorstub.h"
#include "ivcommonprops.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "modelloader.h"
#include "testdriver.h"
#include "types/type.h"

#include <QDebug>
#include <QObject>
#include <QTest>
#include <QVariant>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <iostream>
#include <ivtools.h>
#include <memory>
#include <qglobal.h>
#include <qobjectdefs.h>
#include <qvariant.h>
#include <stdexcept>

using plugincommon::ModelLoader;
using testgenerator::GdbConnector;

namespace tests::testgenerator {

typedef struct {
    asn1SccMyBool active;
    asn1SccMyReal temperature;
    asn1SccMyInteger posX;
    asn1SccMyInteger posY;
    asn1SccMyBool result;
} TestVector;

class tst_datareconstructor final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testNominal();

private:
    const unsigned int TestVectorsNumber = 5;
    TestVector *testData = nullptr;
};

void tst_datareconstructor::initTestCase()
{
    const unsigned int testDataBytesNum = TestVectorsNumber * sizeof(TestVector);
    testData = static_cast<TestVector *>(malloc(testDataBytesNum));
    memset(testData, 0, testDataBytesNum);
}

QString byteToHexStr(char byte)
{
    const auto number = static_cast<uint_least8_t>(byte);
    if (number < 16) {
        return QString("0%1").arg(number, 1, 16);
    } else {
        return QString("%1").arg(number, 2, 16);
    }
}

void printQByteArrayInHex(const QByteArray &array)
{
    QString arrayInHex = QString("QByteArray size: %1\n").arg(array.size());
    for (int i = 0; i < array.size(); i++) {
        arrayInHex += byteToHexStr(array.at(i));
        arrayInHex += " ";

        if ((i + 1) % 8 == 0) {
            arrayInHex += "| ";
        }

        if ((i + 1) % 16 == 0) {
            arrayInHex += "\n";
        }
    }
    std::cerr << arrayInHex.toStdString();
}

QMap<QString, Asn1Acn::Types::Type *> mapAsnTypesToPlatformTypes(Asn1Acn::Asn1Model *const model)
{
    QMap<QString, Asn1Acn::Types::Type *> output;

    for (const auto &file : model->data()) {
        if (file == nullptr) {
            continue;
        }

        for (const auto &definition : file->definitionsList()) {
            if (definition == nullptr) {
                continue;
            }
            const auto &definitionTypeAssignmentNames = definition->typeAssignmentNames();
            if (definitionTypeAssignmentNames.isEmpty()) {
                continue;
            }
            if (definitionTypeAssignmentNames.size() == 1) {
                output.insert(definition->types().front()->name(), definition->types().front()->type());
            }

            for (const auto &type : definition->types()) {
                Asn1Acn::Types::Type *const typeType = type->type();
                if (typeType == nullptr) {
                    continue;
                }
                output.insert(type->name(), typeType);
            }
        }
    }

    return output;
}

// these values shall be configurable by a user
const int sizeOfInteger = 8;
const int sizeOfReal = 8;
const int sizeOfBoolean = 8;

int getSizeOfType(const Asn1Acn::Types::Type::ASN1Type &type)
{
    switch (type) {
    case Asn1Acn::Types::Type::ASN1Type::INTEGER:
        return sizeOfInteger;
    case Asn1Acn::Types::Type::ASN1Type::REAL:
        return sizeOfReal;
    case Asn1Acn::Types::Type::ASN1Type::BOOLEAN:
        return sizeOfBoolean;
    default:
        return 0;
    }
}

ivm::IVInterface *getIfaceFromModel(const QString &ifaceName, ivm::IVModel *const model)
{
    ivm::IVInterface *const ivIface = model->getIfaceByName(ifaceName, ivm::IVInterface::InterfaceType::Provided);
    if (ivIface == nullptr) {
        throw std::logic_error("requested interface not found");
    }
    return ivIface;
}

QByteArray popFrontQByteArray(int howMany, QByteArray &array)
{
    QByteArray output = array.left(howMany);
    array.remove(0, howMany);
    return output;
}

template<typename T>
void pushBackCopyToVariantVector(QVector<QVariant> &vv, QByteArray rawData)
{
    const T *const var = reinterpret_cast<const T *>(rawData.data());
    vv.push_back(QVariant(*var));
}

QVector<QVariant> getVariantVectorFromRawData(QByteArray rawData, const unsigned int TestVectorsNumber,
        ivm::IVInterface *const iface, Asn1Acn::Asn1Model *const asn1Model)
{
    QVector<QVariant> output;
    output.reserve(static_cast<int>(TestVectorsNumber));

    const auto map = mapAsnTypesToPlatformTypes(asn1Model);

    for (unsigned int i = 0; i < TestVectorsNumber; i++) {
        for (const auto &param : iface->params()) {
            const auto &type = map.value(param.paramTypeName());
            if (type == nullptr) {
                throw std::runtime_error("requested type cannot be found");
            }

            const int sizeOfVar = getSizeOfType(type->typeEnum());
            const QByteArray rawVar = popFrontQByteArray(sizeOfVar, rawData);

            const auto typeEnum = type->typeEnum();
            if (typeEnum == Asn1Acn::Types::Type::ASN1Type::INTEGER) {
                pushBackCopyToVariantVector<int>(output, rawVar);
            } else if (typeEnum == Asn1Acn::Types::Type::ASN1Type::REAL) {
                pushBackCopyToVariantVector<double>(output, rawVar);
            } else if (typeEnum == Asn1Acn::Types::Type::ASN1Type::BOOLEAN) {
                pushBackCopyToVariantVector<bool>(output, rawVar);
            }
        }
    }

    return output;
}

void tst_datareconstructor::testNominal()
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
    const QByteArray rawTestResults =
            GdbConnector::getRawTestResults(binLocalization, { "-batch", "-x", script }, { "host:1234", binToRun });

    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/interfaceview.xml");
    const QString ifaceUnderTest = "InterfaceUnderTest";
    ivm::IVInterface *const ivIface = getIfaceFromModel(ifaceUnderTest, ivModel.get());

    const QString asn1Filename = "testharness.asn";
    const QString asn1Filepath = QString("%1%2%3").arg("resources").arg(QDir::separator()).arg(asn1Filename);
    const auto asn1Model = ModelLoader::loadAsn1Model(asn1Filepath);

    const QVector<QVariant> allReadData =
            getVariantVectorFromRawData(rawTestResults, TestVectorsNumber, ivIface, asn1Model.get());

    QCOMPARE(allReadData.size(), expectedTestData.size());
    const int dataSize = allReadData.size();
    for (int i = 0; i < dataSize; i++) {
        QCOMPARE(allReadData.at(i), expectedTestData.at(i));
    }
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_datareconstructor)

#include "tst_datareconstructor.moc"
