/** @file
 * This file is part of the SpaceCreator.
 *l
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

#include <QObject>
#include <QtTest>
#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/asn1/SimulinkToAsn1Translator/datatypesdependencyresolver.h>
#include <conversion/asn1/SimulinkToAsn1Translator/translator.h>
#include <simulinkmodelbuilder/simulinkbusdatatypebuilder.h>
#include <simulinkmodelbuilder/simulinkmodelbuilder.h>
#include <unittests/common/verifyexception.h>

using Asn1Acn::Asn1Model;
using Asn1Acn::IntegerValue;
using Asn1Acn::Range;
using Asn1Acn::RealValue;
using Asn1Acn::TypeAssignment;
using Asn1Acn::Constraints::RangeConstraint;
using Asn1Acn::Constraints::SizeConstraint;
using Asn1Acn::Types::Boolean;
using Asn1Acn::Types::Endianness;
using Asn1Acn::Types::Enumerated;
using Asn1Acn::Types::Integer;
using Asn1Acn::Types::IntegerEncoding;
using Asn1Acn::Types::Real;
using Asn1Acn::Types::RealEncoding;
using Asn1Acn::Types::Sequence;
using Asn1Acn::Types::SequenceOf;
using Asn1Acn::Types::UserdefinedType;
using conversion::asn1::translator::simulink::DataTypesDependencyResolver;
using conversion::asn1::translator::simulink::SimulinkToAsn1Translator;
using conversion::translator::TranslationException;
using conversion::translator::UndeclaredDataTypeException;
using simulink::model::DataScope;
using simulink::model::Dimensions;
using simulink::model::PortDimension;
using simulink::model::VectorDimensions;
using tests::conversion::common::SimulinkBusDataTypeBuilder;
using tests::conversion::common::SimulinkModelBuilder;

namespace conversion::asn1::test {

enum Asn1ModelFileChooser
{
    MATLAB_STANDARD_TYPES = 0,
    SIMULINK_MODEL = 1,
};

class tst_SimulinkToAsn1Translator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testResolvingDataTypes();
    void testResolvingUndeclaredType();
    void testMissingModel();
    void testTooManyModels();
    void testMatLabStandardTypesAmount();
    void testMatLabBooleanStandardType();
    void testMatLabDoubleStandardType();
    void testMatLabSingleStandardType();
    void testMatLabInt8StandardType();
    void testMatLabUInt8StandardType();
    void testMatLabInt16StandardType();
    void testMatLabUInt16StandardType();
    void testMatLabInt32StandardType();
    void testMatLabUInt32StandardType();
    void testAliasDataType();
    void testEnumDataType();
    void testBusDataType();
    void testBusDataTypeDimensionality();

private:
    auto translateEmptySimulinkModel() -> std::vector<std::unique_ptr<conversion::Model>>;

    auto getTypeAssignment(const Asn1Model *asn1Model, Asn1ModelFileChooser asn1ModelFileChooser,
            std::size_t index) const -> const TypeAssignment *;
    auto getRangeConstraint(const SequenceOf *sequenceOf) const -> const Range<IntegerValue::Type> &;
};

void tst_SimulinkToAsn1Translator::testResolvingDataTypes()
{
    // clang-format off
    const auto simulinkModel = SimulinkModelBuilder("SimulinkModel")
                                    .withAliasDataType("Alias2", "Alias", DataScope::Auto, "", "")
                                    .withAliasDataType("Alias", "double", DataScope::Auto, "", "")
                                    .withBusDataType
                                    (
                                        SimulinkBusDataTypeBuilder("Bus", DataScope::Auto, "", "")
                                                    .withBusMember("member1", "Enum", "", Dimensions(1))
                                                    .withBusMember("member2", "Alias2", "", Dimensions(1))
                                                .build()
                                    )
                                    .withEnumDataType("Enum", DataScope::Auto, "", "", { "true", "false" })
                                    .build();
    // clang-format on

    DataTypesDependencyResolver resolver(simulinkModel->dataTypes());
    const auto &resolvedDataTypes = resolver.resolve();

    QCOMPARE(resolvedDataTypes.size(), 4);

    auto it = resolvedDataTypes.begin();

    QCOMPARE(dataTypeNameStr(*(*it)), "Alias");
    it++;

    QCOMPARE(dataTypeNameStr(*(*it)), "Alias2");
    it++;

    QCOMPARE(dataTypeNameStr(*(*it)), "Enum");
    it++;

    QCOMPARE(dataTypeNameStr(*(*it)), "Bus");
}

void tst_SimulinkToAsn1Translator::testResolvingUndeclaredType()
{
    // clang-format off
    const auto simulinkModel = SimulinkModelBuilder("SimulinkModel")
									.withAliasDataType("Alias", "AliasBase", DataScope::Auto, "", "")
									.build();
    // clang-format on

    DataTypesDependencyResolver resolver(simulinkModel->dataTypes());

    QVERIFY_EXCEPTION_THROWN(resolver.resolve(), UndeclaredDataTypeException);
}

void tst_SimulinkToAsn1Translator::testMissingModel()
{
    Options options;
    SimulinkToAsn1Translator translator;

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({}, options), TranslationException,
            "No models passed for translation for SIMULINK to ASN.1 translation");
}

void tst_SimulinkToAsn1Translator::testTooManyModels()
{
    // clang-format off
    const auto simulinkModel1 = SimulinkModelBuilder("SimulinkModel1")
                                    .withInport("Inport", "Inport", "double", "1", PortDimension(-1))
                                    .build();
    const auto simulinkModel2 = SimulinkModelBuilder("SimulinkModel2")
                                    .withInport("Inport", "Inport", "double", "1", PortDimension(-1))
                                    .build();
    // clang-format on

    Options options;
    SimulinkToAsn1Translator translator;

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            translator.translateModels({ simulinkModel1.get(), simulinkModel2.get() }, options), TranslationException,
            "Too many models passed for SIMULINK to ASN.1 translation");
}

void tst_SimulinkToAsn1Translator::testMatLabStandardTypesAmount()
{
    const auto resultModels = translateEmptySimulinkModel();
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Asn1);

    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());
    QVERIFY(asn1Model);

    const auto &asn1Files = asn1Model->data();
    const auto &definitionsList = asn1Files.at(MATLAB_STANDARD_TYPES)->definitionsList();
    const auto definitionsSize = definitionsList.size();
    QCOMPARE(definitionsSize, 1);

    const auto &definitions = definitionsList.at(0);
    const auto typeAssignmentsSize = definitions->types().size();
    QCOMPARE(typeAssignmentsSize, 9);
}

void tst_SimulinkToAsn1Translator::testMatLabBooleanStandardType()
{
    const auto resultModels = translateEmptySimulinkModel();
    const auto &resultModel = resultModels[0];
    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());

    const auto *booleanDataType = getTypeAssignment(asn1Model, MATLAB_STANDARD_TYPES, 0)->type();

    const auto *booleanType = dynamic_cast<const Boolean *>(booleanDataType);
    QVERIFY(booleanType);
    QCOMPARE(booleanType->identifier(), "MatLab-Boolean");
    QCOMPARE(booleanType->typeName(), "BOOLEAN");
    QCOMPARE(booleanType->trueValue(), "1");
    QCOMPARE(booleanType->falseValue(), "0");
}

void tst_SimulinkToAsn1Translator::testMatLabDoubleStandardType()
{
    const auto resultModels = translateEmptySimulinkModel();
    const auto &resultModel = resultModels[0];
    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());

    const auto *doubleDataType = getTypeAssignment(asn1Model, MATLAB_STANDARD_TYPES, 1)->type();

    const auto *doubleType = dynamic_cast<const Real *>(doubleDataType);
    QVERIFY(doubleType);
    QCOMPARE(doubleType->identifier(), "MatLab-Double");
    QCOMPARE(doubleType->typeName(), "REAL");
    QCOMPARE(doubleType->encoding(), RealEncoding::IEEE754_1985_64);
    QCOMPARE(doubleType->endianness(), Endianness::unspecified);

    const auto &doubleConstraints = doubleType->constraints().constraints();
    QCOMPARE(doubleConstraints.size(), 1);

    const auto &doubleSizeConstraint = doubleConstraints.at(0);
    QVERIFY(doubleSizeConstraint);

    const auto *doubleRangeConstraint = dynamic_cast<RangeConstraint<RealValue> *>(doubleSizeConstraint.get());
    QVERIFY(doubleRangeConstraint);

    const auto &doubleRange = doubleRangeConstraint->range();
    QCOMPARE(doubleRange.begin(), std::numeric_limits<double>::lowest());
    QCOMPARE(doubleRange.end(), std::numeric_limits<double>::max());
}

void tst_SimulinkToAsn1Translator::testMatLabSingleStandardType()
{
    const auto resultModels = translateEmptySimulinkModel();
    const auto &resultModel = resultModels[0];
    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());

    const auto *singleDataType = getTypeAssignment(asn1Model, MATLAB_STANDARD_TYPES, 2)->type();

    const auto *singleType = dynamic_cast<const Real *>(singleDataType);
    QVERIFY(singleType);
    QCOMPARE(singleType->identifier(), "MatLab-Single");
    QCOMPARE(singleType->typeName(), "REAL");
    QCOMPARE(singleType->encoding(), RealEncoding::IEEE754_1985_32);
    QCOMPARE(singleType->endianness(), Endianness::unspecified);

    const auto &singleConstraints = singleType->constraints().constraints();
    QCOMPARE(singleConstraints.size(), 1);

    const auto &singleSizeConstraint = singleConstraints.at(0);
    QVERIFY(singleSizeConstraint);

    const auto *singleRangeConstraint = dynamic_cast<RangeConstraint<RealValue> *>(singleSizeConstraint.get());
    QVERIFY(singleRangeConstraint);

    const auto &singleRange = singleRangeConstraint->range();
    QCOMPARE(singleRange.begin(), static_cast<double>(std::numeric_limits<float>::lowest()));
    QCOMPARE(singleRange.end(), static_cast<double>(std::numeric_limits<float>::max()));
}

void tst_SimulinkToAsn1Translator::testMatLabInt8StandardType()
{
    const auto resultModels = translateEmptySimulinkModel();
    const auto &resultModel = resultModels[0];
    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());

    const auto *int8DataType = getTypeAssignment(asn1Model, MATLAB_STANDARD_TYPES, 3)->type();

    const auto *int8Type = dynamic_cast<const Integer *>(int8DataType);
    QVERIFY(int8Type);
    QCOMPARE(int8Type->identifier(), "MatLab-Int8");
    QCOMPARE(int8Type->typeName(), "INTEGER");
    QCOMPARE(int8Type->size(), 8);
    QCOMPARE(int8Type->acnMinSizeInBits(), 8);
    QCOMPARE(int8Type->acnMaxSizeInBits(), 8);
    QCOMPARE(int8Type->encoding(), IntegerEncoding::twos_complement);
    QCOMPARE(int8Type->endianness(), Endianness::unspecified);

    const auto &int8Constraints = int8Type->constraints().constraints();
    QCOMPARE(int8Constraints.size(), 1);

    const auto &int8SizeConstraint = int8Constraints.at(0);
    QVERIFY(int8SizeConstraint);

    const auto *int8RangeConstraint = dynamic_cast<RangeConstraint<IntegerValue> *>(int8SizeConstraint.get());
    QVERIFY(int8RangeConstraint);

    const auto &int8Range = int8RangeConstraint->range();
    QCOMPARE(int8Range.begin(), -128);
    QCOMPARE(int8Range.end(), 127);
}

void tst_SimulinkToAsn1Translator::testMatLabUInt8StandardType()
{
    const auto resultModels = translateEmptySimulinkModel();
    const auto &resultModel = resultModels[0];
    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());

    const auto *uint8DataType = getTypeAssignment(asn1Model, MATLAB_STANDARD_TYPES, 4)->type();

    const auto *uint8Type = dynamic_cast<const Integer *>(uint8DataType);
    QVERIFY(uint8Type);
    QCOMPARE(uint8Type->identifier(), "MatLab-Uint8");
    QCOMPARE(uint8Type->typeName(), "INTEGER");
    QCOMPARE(uint8Type->size(), 8);
    QCOMPARE(uint8Type->acnMinSizeInBits(), 8);
    QCOMPARE(uint8Type->acnMaxSizeInBits(), 8);
    QCOMPARE(uint8Type->encoding(), IntegerEncoding::pos_int);
    QCOMPARE(uint8Type->endianness(), Endianness::unspecified);

    const auto &uint8Constraints = uint8Type->constraints().constraints();
    QCOMPARE(uint8Constraints.size(), 1);

    const auto &uint8SizeConstraint = uint8Constraints.at(0);
    QVERIFY(uint8SizeConstraint);

    const auto *uint8RangeConstraint = dynamic_cast<RangeConstraint<IntegerValue> *>(uint8SizeConstraint.get());
    QVERIFY(uint8RangeConstraint);

    const auto &uint8Range = uint8RangeConstraint->range();
    QCOMPARE(uint8Range.begin(), 0);
    QCOMPARE(uint8Range.end(), 255);
}

void tst_SimulinkToAsn1Translator::testMatLabInt16StandardType()
{
    const auto resultModels = translateEmptySimulinkModel();
    const auto &resultModel = resultModels[0];
    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());

    const auto *int16DataType = getTypeAssignment(asn1Model, MATLAB_STANDARD_TYPES, 5)->type();

    const auto *int16Type = dynamic_cast<const Integer *>(int16DataType);
    QVERIFY(int16Type);
    QCOMPARE(int16Type->identifier(), "MatLab-Int16");
    QCOMPARE(int16Type->typeName(), "INTEGER");
    QCOMPARE(int16Type->size(), 16);
    QCOMPARE(int16Type->acnMinSizeInBits(), 16);
    QCOMPARE(int16Type->acnMaxSizeInBits(), 16);
    QCOMPARE(int16Type->encoding(), IntegerEncoding::twos_complement);
    QCOMPARE(int16Type->endianness(), Endianness::unspecified);

    const auto &int16Constraints = int16Type->constraints().constraints();
    QCOMPARE(int16Constraints.size(), 1);

    const auto &int16SizeConstraint = int16Constraints.at(0);
    QVERIFY(int16SizeConstraint);

    const auto *int16RangeConstraint = dynamic_cast<RangeConstraint<IntegerValue> *>(int16SizeConstraint.get());
    QVERIFY(int16RangeConstraint);

    const auto &int16Range = int16RangeConstraint->range();
    QCOMPARE(int16Range.begin(), -32768);
    QCOMPARE(int16Range.end(), 32767);
}

void tst_SimulinkToAsn1Translator::testMatLabUInt16StandardType()
{
    const auto resultModels = translateEmptySimulinkModel();
    const auto &resultModel = resultModels[0];
    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());

    const auto *uint16DataType = getTypeAssignment(asn1Model, MATLAB_STANDARD_TYPES, 6)->type();

    const auto *uint16Type = dynamic_cast<const Integer *>(uint16DataType);
    QVERIFY(uint16Type);
    QCOMPARE(uint16Type->identifier(), "MatLab-Uint16");
    QCOMPARE(uint16Type->typeName(), "INTEGER");
    QCOMPARE(uint16Type->size(), 16);
    QCOMPARE(uint16Type->acnMinSizeInBits(), 16);
    QCOMPARE(uint16Type->acnMaxSizeInBits(), 16);
    QCOMPARE(uint16Type->encoding(), IntegerEncoding::pos_int);
    QCOMPARE(uint16Type->endianness(), Endianness::unspecified);

    const auto &uint16Constraints = uint16Type->constraints().constraints();
    QCOMPARE(uint16Constraints.size(), 1);

    const auto &uint16SizeConstraint = uint16Constraints.at(0);
    QVERIFY(uint16SizeConstraint);

    const auto *uint16RangeConstraint = dynamic_cast<RangeConstraint<IntegerValue> *>(uint16SizeConstraint.get());
    QVERIFY(uint16RangeConstraint);

    const auto &uint16Range = uint16RangeConstraint->range();
    QCOMPARE(uint16Range.begin(), 0);
    QCOMPARE(uint16Range.end(), 65535);
}

void tst_SimulinkToAsn1Translator::testMatLabInt32StandardType()
{
    const auto resultModels = translateEmptySimulinkModel();
    const auto &resultModel = resultModels[0];
    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());

    const auto *int32DataType = getTypeAssignment(asn1Model, MATLAB_STANDARD_TYPES, 7)->type();

    const auto *int32Type = dynamic_cast<const Integer *>(int32DataType);
    QVERIFY(int32Type);
    QCOMPARE(int32Type->identifier(), "MatLab-Int32");
    QCOMPARE(int32Type->typeName(), "INTEGER");
    QCOMPARE(int32Type->size(), 32);
    QCOMPARE(int32Type->acnMinSizeInBits(), 32);
    QCOMPARE(int32Type->acnMaxSizeInBits(), 32);
    QCOMPARE(int32Type->encoding(), IntegerEncoding::twos_complement);
    QCOMPARE(int32Type->endianness(), Endianness::unspecified);

    const auto &int32Constraints = int32Type->constraints().constraints();
    QCOMPARE(int32Constraints.size(), 1);

    const auto &int32SizeConstraint = int32Constraints.at(0);
    QVERIFY(int32SizeConstraint);

    const auto *int32RangeConstraint = dynamic_cast<RangeConstraint<IntegerValue> *>(int32SizeConstraint.get());
    QVERIFY(int32RangeConstraint);

    const auto &int32Range = int32RangeConstraint->range();
    QCOMPARE(int32Range.begin(), -2147483648);
    QCOMPARE(int32Range.end(), 2147483647);
}

void tst_SimulinkToAsn1Translator::testMatLabUInt32StandardType()
{
    const auto resultModels = translateEmptySimulinkModel();
    const auto &resultModel = resultModels[0];
    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());

    const auto *uint32DataType = getTypeAssignment(asn1Model, MATLAB_STANDARD_TYPES, 8)->type();

    const auto *uint32Type = dynamic_cast<const Integer *>(uint32DataType);
    QVERIFY(uint32Type);
    QCOMPARE(uint32Type->identifier(), "MatLab-Uint32");
    QCOMPARE(uint32Type->typeName(), "INTEGER");
    QCOMPARE(uint32Type->size(), 32);
    QCOMPARE(uint32Type->acnMinSizeInBits(), 32);
    QCOMPARE(uint32Type->acnMaxSizeInBits(), 32);
    QCOMPARE(uint32Type->encoding(), IntegerEncoding::pos_int);
    QCOMPARE(uint32Type->endianness(), Endianness::unspecified);

    const auto &uint32Constraints = uint32Type->constraints().constraints();
    QCOMPARE(uint32Constraints.size(), 1);

    const auto &uint32SizeConstraint = uint32Constraints.at(0);
    QVERIFY(uint32SizeConstraint);

    const auto *uint32RangeConstraint = dynamic_cast<RangeConstraint<IntegerValue> *>(uint32SizeConstraint.get());
    QVERIFY(uint32RangeConstraint);

    const auto &uint32Range = uint32RangeConstraint->range();
    QCOMPARE(uint32Range.begin(), 0);
    QCOMPARE(uint32Range.end(), 4294967295);
}

void tst_SimulinkToAsn1Translator::testAliasDataType()
{
    // clang-format off
    const auto simulinkModel = SimulinkModelBuilder("SimulinkModel")
                                    .withAliasDataType("Alias1", "double", DataScope::Auto, "Alias1Description", "")
                                    .withAliasDataType("Alias2", "Alias1", DataScope::Auto, "Alias2Description", "")
                                    .withAliasDataType("Alias3", "Alias2", DataScope::Auto, "Alias3Description", "")
                                    .build();
    // clang-format on

    Options options;
    SimulinkToAsn1Translator translator;

    const auto resultModels = translator.translateModels({ simulinkModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Asn1);

    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());
    QVERIFY(asn1Model);

    const auto &asn1Files = asn1Model->data();
    const auto &definitionsList = asn1Files.at(SIMULINK_MODEL)->definitionsList();
    const auto definitionsSize = definitionsList.size();
    QCOMPARE(definitionsSize, 1);

    const auto &definitions = definitionsList.at(0);
    const auto typeAssignmentsSize = definitions->types().size();
    QCOMPARE(typeAssignmentsSize, 3);

    // Alias1

    const auto *alias1TypeAssignment = getTypeAssignment(asn1Model, SIMULINK_MODEL, 0);
    QVERIFY(alias1TypeAssignment);
    QCOMPARE(alias1TypeAssignment->name(), "Alias1");
    QCOMPARE(alias1TypeAssignment->comment(), "Alias1Description");

    const auto *alias1DataType = dynamic_cast<const UserdefinedType *>(alias1TypeAssignment->type());
    QVERIFY(alias1DataType);
    QCOMPARE(alias1DataType->typeName(), "MatLab-Double");

    // Alias2

    const auto *alias2TypeAssignment = getTypeAssignment(asn1Model, SIMULINK_MODEL, 1);
    QVERIFY(alias2TypeAssignment);
    QCOMPARE(alias2TypeAssignment->name(), "Alias2");
    QCOMPARE(alias2TypeAssignment->comment(), "Alias2Description");

    const auto *alias2DataType = dynamic_cast<const UserdefinedType *>(alias2TypeAssignment->type());
    QVERIFY(alias2DataType);
    QCOMPARE(alias2DataType->typeName(), "Alias1");

    // Alias3

    const auto *alias3TypeAssignment = getTypeAssignment(asn1Model, SIMULINK_MODEL, 2);
    QVERIFY(alias3TypeAssignment);
    QCOMPARE(alias3TypeAssignment->name(), "Alias3");
    QCOMPARE(alias3TypeAssignment->comment(), "Alias3Description");

    const auto *alias3DataType = dynamic_cast<const UserdefinedType *>(alias3TypeAssignment->type());
    QVERIFY(alias3DataType);
    QCOMPARE(alias3DataType->typeName(), "Alias2");
}

void tst_SimulinkToAsn1Translator::testEnumDataType()
{
    // clang-format off
    const auto simulinkModel = SimulinkModelBuilder("SimulinkModel")
                                    .withEnumDataType("Enum", DataScope::Auto, "EnumDescription", "", {"false", "true"})
                                    .build();
    // clang-format on

    Options options;
    SimulinkToAsn1Translator translator;

    const auto resultModels = translator.translateModels({ simulinkModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Asn1);

    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());
    QVERIFY(asn1Model);

    const auto &asn1Files = asn1Model->data();
    const auto &definitionsList = asn1Files.at(SIMULINK_MODEL)->definitionsList();
    const auto definitionsSize = definitionsList.size();
    QCOMPARE(definitionsSize, 1);

    const auto &definitions = definitionsList.at(0);
    const auto typeAssignmentsSize = definitions->types().size();
    QCOMPARE(typeAssignmentsSize, 1);

    const auto *enumTypeAssignment = getTypeAssignment(asn1Model, SIMULINK_MODEL, 0);
    QVERIFY(enumTypeAssignment);

    const auto *enumType = dynamic_cast<const Enumerated *>(enumTypeAssignment->type());
    QVERIFY(enumType);
    QCOMPARE(enumType->identifier(), "Enum");
    QCOMPARE(enumType->typeName(), "ENUMERATED");

    const auto &enumItems = enumType->items();
    QCOMPARE(enumItems.size(), 2);

    const auto &falseEnumItem = enumItems["false"];
    QCOMPARE(falseEnumItem.name(), "false");
    QCOMPARE(falseEnumItem.value(), 0);
    QCOMPARE(falseEnumItem.index(), 0);

    const auto &trueEnumItem = enumItems["true"];
    QCOMPARE(trueEnumItem.name(), "true");
    QCOMPARE(trueEnumItem.value(), 1);
    QCOMPARE(trueEnumItem.index(), 1);
}

void tst_SimulinkToAsn1Translator::testBusDataType()
{
    // clang-format off
    const auto simulinkModel = SimulinkModelBuilder("SimulinkModel")
                                    .withAliasDataType("Alias", "double", DataScope::Auto, "Alias1Description", "")
                                    .withEnumDataType("Enum", DataScope::Auto, "EnumDescription", "", {"false", "true"})
                                    .withBusDataType(
                                        SimulinkBusDataTypeBuilder("Bus", DataScope::Auto, "BusDescription", "")
                                            .withBusMember("member1", "Alias", "", Dimensions(1))
                                            .withBusMember("member2", "Enum", "", Dimensions(1))
                                            .withBusMember("member3", "int8", "", Dimensions(1))
                                            .build()
                                    )
                                    .build();
    // clang-format on

    Options options;
    SimulinkToAsn1Translator translator;

    const auto resultModels = translator.translateModels({ simulinkModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Asn1);

    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());
    QVERIFY(asn1Model);

    const auto &asn1Files = asn1Model->data();
    const auto &definitionsList = asn1Files.at(SIMULINK_MODEL)->definitionsList();
    const auto definitionsSize = definitionsList.size();
    QCOMPARE(definitionsSize, 1);

    const auto &definitions = definitionsList.at(0);
    const auto typeAssignmentsSize = definitions->types().size();
    QCOMPARE(typeAssignmentsSize, 3);

    const auto *busDataType = getTypeAssignment(asn1Model, SIMULINK_MODEL, 2);
    QVERIFY(busDataType);
    QCOMPARE(busDataType->name(), "Bus");
    QCOMPARE(busDataType->comment(), "BusDescription");

    const auto *busType = dynamic_cast<const Sequence *>(busDataType->type());
    QVERIFY(busType);
    QCOMPARE(busType->typeName(), "SEQUENCE");
    QCOMPARE(busType->components().size(), 3);

    // Bus Member1

    const auto *busMember1 = busType->component("member1");
    QVERIFY(busMember1);
    QCOMPARE(busMember1->name(), "member1");

    const auto *busMember1Type = dynamic_cast<const UserdefinedType *>(busMember1->type());
    QVERIFY(busMember1Type);
    QCOMPARE(busMember1Type->typeName(), "Alias");

    // Bus Member2

    const auto *busMember2 = busType->component("member2");
    QVERIFY(busMember2);
    QCOMPARE(busMember2->name(), "member2");

    const auto *busMember2Type = dynamic_cast<const UserdefinedType *>(busMember2->type());
    QVERIFY(busMember2Type);
    QCOMPARE(busMember2Type->typeName(), "Enum");

    // Bus Member3

    const auto *busMember3 = busType->component("member3");
    QVERIFY(busMember3);
    QCOMPARE(busMember3->name(), "member3");

    const auto *busMember3Type = dynamic_cast<const UserdefinedType *>(busMember3->type());
    QVERIFY(busMember3Type);
    QCOMPARE(busMember3Type->typeName(), "MatLab-Int8");
}

void tst_SimulinkToAsn1Translator::testBusDataTypeDimensionality()
{
    Dimensions vecDimensions(std::in_place_type<VectorDimensions>);
    auto &vecDimensions2x2 = std::get<VectorDimensions>(vecDimensions);
    vecDimensions2x2.addValue(2);
    vecDimensions2x2.addValue(2);

    // clang-format off
    const auto simulinkModel = SimulinkModelBuilder("SimulinkModel")
                                    .withBusDataType(
                                        SimulinkBusDataTypeBuilder("Bus", DataScope::Auto, "BusDescription", "")
                                            .withBusMember("member1", "int8", "", Dimensions(1))
                                            .withBusMember("member2", "int8", "", Dimensions(2))
                                            .withBusMember("member3", "int8", "", std::move(vecDimensions))
                                            .build()
                                    )
                                    .build();
    // clang-format on

    Options options;
    SimulinkToAsn1Translator translator;

    const auto resultModels = translator.translateModels({ simulinkModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Asn1);

    const auto *asn1Model = dynamic_cast<Asn1Model *>(resultModel.get());
    QVERIFY(asn1Model);

    const auto &asn1Files = asn1Model->data();
    const auto &definitionsList = asn1Files.at(SIMULINK_MODEL)->definitionsList();
    const auto definitionsSize = definitionsList.size();
    QCOMPARE(definitionsSize, 1);

    const auto &definitions = definitionsList.at(0);
    const auto typeAssignmentsSize = definitions->types().size();
    QCOMPARE(typeAssignmentsSize, 1);

    // Bus

    const auto *busDataType = getTypeAssignment(asn1Model, SIMULINK_MODEL, 0);
    QVERIFY(busDataType);
    QCOMPARE(busDataType->name(), "Bus");
    QCOMPARE(busDataType->comment(), "BusDescription");

    const auto *busType = dynamic_cast<const Sequence *>(busDataType->type());
    QVERIFY(busType);
    QCOMPARE(busType->typeName(), "SEQUENCE");
    QCOMPARE(busType->components().size(), 3);

    // Bus Member1

    const auto *busMember1 = busType->component("member1");
    QVERIFY(busMember1);
    QCOMPARE(busMember1->name(), "member1");

    const auto *busMember1Type = dynamic_cast<const UserdefinedType *>(busMember1->type());
    QVERIFY(busMember1Type);
    QCOMPARE(busMember1Type->typeName(), "MatLab-Int8");

    // Bus Member2

    const auto *busMember2 = busType->component("member2");
    QVERIFY(busMember2);
    QCOMPARE(busMember2->name(), "member2");

    const auto *busMember2Type = dynamic_cast<const SequenceOf *>(busMember2->type());
    QVERIFY(busMember2Type);
    QCOMPARE(busMember2Type->identifier(), "member2");
    QCOMPARE(busMember2Type->typeName(), "SEQUENCE OF");

    const auto &busMember2Range = getRangeConstraint(busMember2Type);
    QCOMPARE(busMember2Range.begin(), 2);
    QCOMPARE(busMember2Range.end(), 2);

    const auto *busMember2SequenceOfItemsType = busMember2Type->itemsType();
    QVERIFY(busMember2SequenceOfItemsType);

    const auto *busMember2SequenceOfReferencedItemsType =
            dynamic_cast<const UserdefinedType *>(busMember2SequenceOfItemsType);
    QVERIFY(busMember2SequenceOfReferencedItemsType);
    QCOMPARE(busMember2SequenceOfReferencedItemsType->identifier(), "");
    QCOMPARE(busMember2SequenceOfReferencedItemsType->typeName(), "MatLab-Int8");

    // Bus Member3

    const auto *busMember3 = busType->component("member3");
    QVERIFY(busMember3);
    QCOMPARE(busMember3->name(), "member3");

    const auto *busMember3Type = dynamic_cast<const SequenceOf *>(busMember3->type());
    QVERIFY(busMember3Type);
    QCOMPARE(busMember3Type->identifier(), "MatLab-Int8");
    QCOMPARE(busMember3Type->typeName(), "SEQUENCE OF");

    const auto &busMember3Range = getRangeConstraint(busMember3Type);
    QCOMPARE(busMember3Range.begin(), 2);
    QCOMPARE(busMember3Range.end(), 2);

    const auto *busMember3SequenceOfItemsType = busMember3Type->itemsType();
    QVERIFY(busMember3SequenceOfItemsType);

    const auto *busMember3SequenceOfReferencedItemsType =
            dynamic_cast<const SequenceOf *>(busMember3SequenceOfItemsType);
    QVERIFY(busMember3SequenceOfReferencedItemsType);
    QCOMPARE(busMember3SequenceOfReferencedItemsType->identifier(), "");
    QCOMPARE(busMember3SequenceOfReferencedItemsType->typeName(), "SEQUENCE OF");

    const auto &busMember3ReferenceItemsTypeRange = getRangeConstraint(busMember3SequenceOfReferencedItemsType);
    QCOMPARE(busMember3ReferenceItemsTypeRange.begin(), 2);
    QCOMPARE(busMember3ReferenceItemsTypeRange.end(), 2);

    const auto *busMember3SequenceOfSequenceOfItemsType = busMember3SequenceOfReferencedItemsType->itemsType();
    QVERIFY(busMember3SequenceOfSequenceOfItemsType);

    const auto *busMember3SequenceOfSequenceOfReferencedItemsType =
            dynamic_cast<const UserdefinedType *>(busMember3SequenceOfSequenceOfItemsType);
    QVERIFY(busMember3SequenceOfSequenceOfReferencedItemsType);
    QCOMPARE(busMember3SequenceOfSequenceOfReferencedItemsType->identifier(), "");
    QCOMPARE(busMember3SequenceOfSequenceOfReferencedItemsType->typeName(), "MatLab-Int8");
}

std::vector<std::unique_ptr<conversion::Model>> tst_SimulinkToAsn1Translator::translateEmptySimulinkModel()
{
    const auto emptySimulinkModel = SimulinkModelBuilder("SimulinkModel").build();

    Options options;
    SimulinkToAsn1Translator translator;

    auto resultModels = translator.translateModels({ emptySimulinkModel.get() }, options);

    return resultModels;
}

const TypeAssignment *tst_SimulinkToAsn1Translator::getTypeAssignment(
        const Asn1Model *asn1Model, Asn1ModelFileChooser asn1ModelFileChooser, std::size_t index) const
{
    const auto &asn1Files = asn1Model->data();
    const auto &definitions = asn1Files.at(asn1ModelFileChooser)->definitionsList().at(0);
    const auto &typeAssignments = definitions->types();
    const auto *type = typeAssignments.at(index).get();

    return type;
}

const Range<IntegerValue::Type> &tst_SimulinkToAsn1Translator::getRangeConstraint(const SequenceOf *sequenceOf) const
{
    const auto &constraints = sequenceOf->constraints().constraints();
    const auto &constraint = constraints.at(0);
    const auto *sizeConstraint = dynamic_cast<SizeConstraint<IntegerValue> *>(constraint.get());
    const auto *rangeConstraint = dynamic_cast<RangeConstraint<IntegerValue> *>(sizeConstraint->innerConstraints());

    return rangeConstraint->range();
}

} // namespace conversion::asn1::test

QTEST_MAIN(conversion::asn1::test::tst_SimulinkToAsn1Translator)

#include "tst_simulinktoasn1translator.moc"
