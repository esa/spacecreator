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
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/type.h>
#include <asn1library/asn1/values.h>
#include <conversion/asn1/SedsToAsn1Translator/datatypesdependencyresolver.h>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
#include <conversion/common/options.h>
#include <seds/SedsModel/sedsmodel.h>
#include <sedsmodelbuilder/sedscontainerdatatypebuilder.h>
#include <sedsmodelbuilder/sedsdatatypefactory.h>
#include <sedsmodelbuilder/sedsmodelbuilder.h>
#include <unittests/common/verifyexception.h>

using namespace Asn1Acn;
using namespace seds::model;

using conversion::Options;
using conversion::asn1::translator::DataTypesDependencyResolver;
using conversion::asn1::translator::NotDAGException;
using conversion::asn1::translator::SedsToAsn1Translator;
using conversion::translator::TranslationException;
using tests::conversion::common::SedsContainerDataTypeBuilder;
using tests::conversion::common::SedsDataTypeFactory;
using tests::conversion::common::SedsModelBuilder;

namespace conversion::asn1::test {

class MockModel final : public Model
{
    virtual auto modelType() const -> ModelType override { return ModelType::Unspecified; }
};

class tst_SedsToAsn1Translator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testMissingModel();
    void testTooManyModels();
    void testWrongModel();

    void testResolvingArrayDataType();
    void testResolvingContainerDataType();
    void testResolvingCyclicDependency();

    void testTranslateBinaryDataType();
    void testTranslateBooleanDataType();
    void testTranslateEnumeratedDataType();
    void testTranslateIntegerDataType();
    void testTranslateFloatDataType();
    void testTranslateStringDataType();

private:
    const Types::Type *getType(const Asn1Model *asn1Model, std::size_t index);
};

void tst_SedsToAsn1Translator::testMissingModel()
{
    Options options;
    SedsToAsn1Translator translator;

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({}, options), TranslationException,
            "No models passed for translation for SEDS to ASN.1 translation");
}

void tst_SedsToAsn1Translator::testTooManyModels()
{
    Options options;
    SedsToAsn1Translator translator;

    const auto sedsModel1 = SedsModelBuilder("Model1").withIntegerDataType("SignedInteger16").build();
    const auto sedsModel2 = SedsModelBuilder("Model2").withFloatDataType("Float64").build();

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({ sedsModel1.get(), sedsModel2.get() }, options),
            TranslationException, "Too many models passed for SEDS to ASN.1 translation");
}

void tst_SedsToAsn1Translator::testWrongModel()
{
    Options options;
    SedsToAsn1Translator translator;

    const auto mockModel = std::make_unique<MockModel>();

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({ mockModel.get() }, options), TranslationException,
            "Missing source Unspecified model");
}

void tst_SedsToAsn1Translator::testResolvingArrayDataType()
{
    const seds::model::DataType arrayDataType = SedsDataTypeFactory::createArray("Array", "DataItem");
    const seds::model::DataType integerDataType = SedsDataTypeFactory::createInteger("DataItem");

    std::vector<const DataType *> dataTypes;
    dataTypes.push_back(&arrayDataType);
    dataTypes.push_back(&integerDataType);

    DataTypesDependencyResolver resolver;
    auto resolvedDataTypes = resolver.resolve(&dataTypes);

    QCOMPARE(resolvedDataTypes.size(), 2);

    QCOMPARE(dataTypeNameStr(*resolvedDataTypes.front()), "DataItem");
    resolvedDataTypes.pop_front();

    QCOMPARE(dataTypeNameStr(*resolvedDataTypes.front()), "Array");
    resolvedDataTypes.pop_front();
}

void tst_SedsToAsn1Translator::testResolvingContainerDataType()
{
    // clang-format off
    const seds::model::DataType containerDataType = SedsContainerDataTypeBuilder("Container")
                                   .withEntry("fieldA", "DataTypeA")
                                   .withEntry("fieldB", "DataTypeB")
                               .build();
    // clang-format on
    const seds::model::DataType dataTypeA = SedsDataTypeFactory::createInteger("DataTypeA");
    const seds::model::DataType dataTypeB = SedsDataTypeFactory::createFloat("DataTypeB");

    std::vector<const DataType *> dataTypes;
    dataTypes.push_back(&containerDataType);
    dataTypes.push_back(&dataTypeA);
    dataTypes.push_back(&dataTypeB);

    DataTypesDependencyResolver resolver;
    auto resolvedDataTypes = resolver.resolve(&dataTypes);

    QCOMPARE(resolvedDataTypes.size(), 3);

    QCOMPARE(dataTypeNameStr(*resolvedDataTypes.front()), "DataTypeA");
    resolvedDataTypes.pop_front();

    QCOMPARE(dataTypeNameStr(*resolvedDataTypes.front()), "DataTypeB");
    resolvedDataTypes.pop_front();

    QCOMPARE(dataTypeNameStr(*resolvedDataTypes.front()), "Container");
    resolvedDataTypes.pop_front();
}

void tst_SedsToAsn1Translator::testResolvingCyclicDependency()
{
    // clang-format off
    const seds::model::DataType containerDataType1 = SedsContainerDataTypeBuilder("Container1")
                                   .withEntry("field", "Container2")
                               .build();
    const seds::model::DataType containerDataType2 = SedsContainerDataTypeBuilder("Container2")
                                   .withEntry("field", "Container1")
                               .build();
    // clang-format on

    std::vector<const DataType *> dataTypes;
    dataTypes.push_back(&containerDataType1);
    dataTypes.push_back(&containerDataType2);

    DataTypesDependencyResolver resolver;

    QVERIFY_EXCEPTION_THROWN(resolver.resolve(&dataTypes), NotDAGException);
}

void tst_SedsToAsn1Translator::testTranslateBinaryDataType()
{
    const auto sedsModel = SedsModelBuilder("Model").withBinaryDataType("Bitstring").build();

    Options options;
    SedsToAsn1Translator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Asn1);

    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());
    QVERIFY(asn1Model);

    const auto *type = getType(asn1Model, 0);
    QVERIFY(type);

    const auto *stringType = dynamic_cast<const Types::BitString *>(type);
    QVERIFY(stringType);

    QCOMPARE(stringType->identifier(), "Bitstring");
    QCOMPARE(stringType->typeName(), "BIT STRING");

    const auto &constraints = stringType->constraints().constraints();
    QCOMPARE(constraints.size(), 1);

    const auto &constraint = constraints[0];
    QVERIFY(constraint);

    const auto *sizeConstraint = dynamic_cast<Constraints::SizeConstraint<BitStringValue> *>(constraint.get());
    QVERIFY(sizeConstraint);

    const auto *innerConstraints = sizeConstraint->innerConstraints();
    QVERIFY(innerConstraints);

    const auto *rangeConstraint = dynamic_cast<const Constraints::RangeConstraint<IntegerValue> *>(innerConstraints);
    QVERIFY(rangeConstraint);

    const auto &range = rangeConstraint->range();
    QVERIFY(!range.isSingleItem());
    QCOMPARE(range.begin(), 0);
    QCOMPARE(range.end(), 42);
}

void tst_SedsToAsn1Translator::testTranslateBooleanDataType()
{
    const auto sedsModel = SedsModelBuilder("Model").withBooleanDataType("Boolean").build();

    Options options;
    SedsToAsn1Translator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Asn1);

    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());
    QVERIFY(asn1Model);

    const auto *type = getType(asn1Model, 0);
    QVERIFY(type);

    const auto *booleanType = dynamic_cast<const Types::Boolean *>(type);
    QVERIFY(booleanType);

    QCOMPARE(booleanType->identifier(), "Boolean");
    QCOMPARE(booleanType->typeName(), "BOOLEAN");
    QCOMPARE(booleanType->trueValue(), "0");
}

void tst_SedsToAsn1Translator::testTranslateEnumeratedDataType()
{
    const auto sedsModel =
            SedsModelBuilder("Model").withEnumeratedDataType("Enumeration", { "One", "Two", "Three" }).build();

    Options options;
    SedsToAsn1Translator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Asn1);

    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());
    QVERIFY(asn1Model);

    const auto *type = getType(asn1Model, 0);
    QVERIFY(type);

    const auto *enumType = dynamic_cast<const Types::Enumerated *>(type);
    QVERIFY(enumType);

    QCOMPARE(enumType->identifier(), "Enumeration");
    QCOMPARE(enumType->typeName(), "ENUMERATED");
    QCOMPARE(enumType->size(), 8);
    QCOMPARE(enumType->encoding(), Types::IntegerEncoding::twos_complement);
    QCOMPARE(enumType->endianness(), Types::Endianness::big);

    const auto &items = enumType->items();
    QCOMPARE(items.size(), 3);

    const auto &item = items["Two"];
    QCOMPARE(item.index(), 1);
    QCOMPARE(item.name(), "Two");
    QCOMPARE(item.value(), 2);
}

void tst_SedsToAsn1Translator::testTranslateIntegerDataType()
{
    const auto sedsModel = SedsModelBuilder("Model").withIntegerDataType("SignedInteger16").build();

    Options options;
    SedsToAsn1Translator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Asn1);

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
    const auto sedsModel = SedsModelBuilder("Model").withFloatDataType("Float64").build();

    Options options;
    SedsToAsn1Translator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Asn1);

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

void tst_SedsToAsn1Translator::testTranslateStringDataType()
{
    const auto sedsModel = SedsModelBuilder("Model").withStringDataType("String20").build();

    Options options;
    SedsToAsn1Translator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Asn1);

    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModels[0].get());
    QVERIFY(asn1Model);

    const auto *type = getType(asn1Model, 0);
    QVERIFY(type);

    const auto *stringType = dynamic_cast<const Types::IA5String *>(type);
    QVERIFY(stringType);

    QCOMPARE(stringType->identifier(), "String20");
    QCOMPARE(stringType->typeName(), "IA5String");
    QCOMPARE(stringType->encoding(), Types::AsciiStringEncoding::ASCII);
    QCOMPARE(stringType->terminationPattern(), "X");

    const auto &constraints = stringType->constraints().constraints();
    QCOMPARE(constraints.size(), 1);

    const auto &constraint = constraints[0];
    QVERIFY(constraint);

    const auto *sizeConstraint = dynamic_cast<Constraints::SizeConstraint<StringValue> *>(constraint.get());
    QVERIFY(sizeConstraint);

    const auto *innerConstraints = sizeConstraint->innerConstraints();
    QVERIFY(innerConstraints);

    const auto *rangeConstraint = dynamic_cast<const Constraints::RangeConstraint<IntegerValue> *>(innerConstraints);
    QVERIFY(rangeConstraint);

    const auto &range = rangeConstraint->range();
    QVERIFY(range.isSingleItem());
    QCOMPARE(range.begin(), 20);
}

const Types::Type *tst_SedsToAsn1Translator::getType(const Asn1Model *asn1Model, std::size_t index)
{
    const auto &asn1Files = asn1Model->data();
    const auto &definitions = asn1Files.at(0)->definitionsList();
    const auto &typeAssignments = definitions.at(0)->types();
    const auto *type = typeAssignments.at(index)->type();

    return type;
}

} // namespace conversion::asn1::test

QTEST_MAIN(conversion::asn1::test::tst_SedsToAsn1Translator)

#include "tst_sedstoasn1translator.moc"
