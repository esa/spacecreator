/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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
#include <QtTest>
#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/type.h>
#include <asn1library/asn1/values.h>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
#include <conversion/common/options.h>
#include <seds/SedsModel/sedsmodel.h>

using namespace Asn1Acn;
using namespace seds::model;

using conversion::Options;
using conversion::asn1::translator::SedsToAsn1Translator;

namespace seds::test {

class tst_SedsToAsn1Translator : public QObject
{
    Q_OBJECT

public:
    virtual ~tst_SedsToAsn1Translator() = default;

private Q_SLOTS:
    void testTranslateIntegerDataType();
    void testTranslateFloatDataType();

private:
    std::unique_ptr<SedsModel> createSignedInteger16();
    std::unique_ptr<SedsModel> createFloat64();

    const Types::Type *getType(const Asn1Model *asn1Model, std::size_t index);
};

void tst_SedsToAsn1Translator::testTranslateIntegerDataType()
{
    const auto sedsModel = createSignedInteger16();

    Options options;
    SedsToAsn1Translator translator;

    const auto resultModel = translator.translateModels({ sedsModel.get() }, options);
    QVERIFY(resultModel);

    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());
    QVERIFY(asn1Model);

    const auto *type = getType(asn1Model, 0);
    QVERIFY(type);

    const auto *integerType = dynamic_cast<const Types::Integer *>(type);
    QVERIFY(integerType);

    QCOMPARE(integerType->identifier(), "SignedInteger16");
    QCOMPARE(integerType->typeName(), "INTEGER");
    QCOMPARE(integerType->size(), 16);
    QCOMPARE(integerType->encoding(), Types::IntegerEncoding::twos_complement);
    QCOMPARE(integerType->endianness(), Types::Endianness::big);

    const auto &constraints = integerType->constraints().constraints();
    QCOMPARE(constraints.size(), 1);

    const auto &constraint = constraints[0];
    QVERIFY(constraint);

    const auto *rangeConstraint = dynamic_cast<Constraints::RangeConstraint<IntegerValue> *>(constraint.get());
    QVERIFY(rangeConstraint);

    const auto &range = rangeConstraint->range();
    QCOMPARE(range.begin(), -42);
    QCOMPARE(range.end(), 42);
}

void tst_SedsToAsn1Translator::testTranslateFloatDataType()
{
    const auto sedsModel = createFloat64();

    Options options;
    SedsToAsn1Translator translator;

    const auto resultModel = translator.translateModels({ sedsModel.get() }, options);
    QVERIFY(resultModel);

    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());
    QVERIFY(asn1Model);

    const auto *type = getType(asn1Model, 0);
    QVERIFY(type);

    const auto *realType = dynamic_cast<const Types::Real *>(type);
    QVERIFY(realType);

    QCOMPARE(realType->identifier(), "Float64");
    QCOMPARE(realType->typeName(), "REAL");
    QCOMPARE(realType->encoding(), Types::RealEncoding::IEEE754_1985_64);
    QCOMPARE(realType->endianness(), Types::Endianness::little);

    const auto &constraints = realType->constraints().constraints();
    QCOMPARE(constraints.size(), 1);

    const auto &constraint = constraints[0];
    QVERIFY(constraint);

    const auto *rangeConstraint = dynamic_cast<Constraints::RangeConstraint<RealValue> *>(constraint.get());
    QVERIFY(rangeConstraint);

    const auto &range = rangeConstraint->range();
    QCOMPARE(range.begin(), 2.22507e-308);
    QCOMPARE(range.end(), 1.79769e+308);
}

std::unique_ptr<SedsModel> tst_SedsToAsn1Translator::createSignedInteger16()
{
    MinMaxRange range;
    range.setMin(QString("-42"));
    range.setMax(QString("42"));

    IntegerDataEncoding encoding;
    encoding.setByteOrder(ByteOrder::BigEndian);
    encoding.setEncoding(CoreIntegerEncoding::TwosComplement);
    encoding.setBits(16);

    IntegerDataType integerDataType;
    integerDataType.setName("SignedInteger16");
    integerDataType.setRange(std::move(range));
    integerDataType.setEncoding(std::move(encoding));

    Package package;
    package.addDataType(std::move(integerDataType));

    PackageFile packageFile;
    packageFile.setPackage(std::move(package));

    return std::make_unique<SedsModel>(std::move(packageFile));
}

std::unique_ptr<SedsModel> tst_SedsToAsn1Translator::createFloat64()
{
    FloatPrecisionRange range = FloatPrecisionRange::Double;

    FloatDataEncoding encoding;
    encoding.setByteOrder(ByteOrder::LittleEndian);
    encoding.setEncoding(CoreEncodingAndPrecision::IeeeDouble);
    encoding.setBits(64);

    FloatDataType floatDataType;
    floatDataType.setName("Float64");
    floatDataType.setRange(range);
    floatDataType.setEncoding(std::move(encoding));

    Package package;
    package.addDataType(std::move(floatDataType));

    PackageFile packageFile;
    packageFile.setPackage(std::move(package));

    return std::make_unique<SedsModel>(std::move(packageFile));
}

const Types::Type *tst_SedsToAsn1Translator::getType(const Asn1Model *asn1Model, std::size_t index)
{
    const auto &asn1Files = asn1Model->data();
    const auto &definitions = asn1Files.at(0).definitionsList();
    const auto &typeAssignments = definitions.at(0)->types();
    const auto *type = typeAssignments.at(index)->type();

    return type;
}

} // namespace seds::test

QTEST_MAIN(seds::test::tst_SedsToAsn1Translator)

#include "tst_sedstoasn1translator.moc"
