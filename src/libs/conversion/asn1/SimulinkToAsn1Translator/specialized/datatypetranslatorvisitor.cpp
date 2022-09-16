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

#include "datatypetranslatorvisitor.h"

#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/simulink/matlabstandarddatatypes.h>
#include <conversion/common/simulink/tastestandarddatatypes.h>

using Asn1Acn::SourceLocation;
using Asn1Acn::TypeAssignment;
using Asn1Acn::Types::Endianness;
using Asn1Acn::Types::Enumerated;
using Asn1Acn::Types::EnumeratedItem;
using Asn1Acn::Types::IntegerEncoding;
using Asn1Acn::Types::UserdefinedType;
using conversion::Escaper;
using conversion::simulink::MatLabStandardDataTypes;
using conversion::simulink::TasteStandardDataTypes;
using simulink::model::AliasDataType;
using simulink::model::BusDataType;
using simulink::model::EnumDataType;

namespace conversion::asn1::translator {

DataTypeTranslatorVisitor::DataTypeTranslatorVisitor(Context &context)
    : m_context(context)
    , m_busDataTypeTranslator(context)
{
}

void DataTypeTranslatorVisitor::operator()(const EnumDataType &simulinkEnumType)
{
    const auto enumAsn1TypeName = Escaper::escapeAsn1TypeName(simulinkEnumType.name());
    auto enumAsn1EnumeratedType = std::make_unique<Enumerated>(enumAsn1TypeName);

    enumAsn1EnumeratedType->setEndianness(Endianness::unspecified);
    enumAsn1EnumeratedType->setSize(MatLabStandardDataTypes::getAsn1EnumBitSize());
    enumAsn1EnumeratedType->setAcnMinSizeInBits(MatLabStandardDataTypes::getAsn1EnumBitSize());
    enumAsn1EnumeratedType->setAcnMaxSizeInBits(MatLabStandardDataTypes::getAsn1EnumBitSize());
    enumAsn1EnumeratedType->setEncodeValues(true);

    const auto &simulinkEnumValues = simulinkEnumType.enumValues();
    auto doesEnumContainNegativeValue = false;

    for (std::size_t index = 0; index < simulinkEnumValues.size(); ++index) {
        const auto &simulinkEnumValue = simulinkEnumValues[index];
        const auto enumAsn1EnumeratedItemTypeName = Escaper::escapeAsn1FieldName(simulinkEnumValue.name());
        const auto enumAsn1EnumeratedItemType =
                EnumeratedItem(index, enumAsn1EnumeratedItemTypeName, simulinkEnumValue.value(), SourceLocation());

        enumAsn1EnumeratedType->addItem(enumAsn1EnumeratedItemType);

        if (simulinkEnumValue.value() < 0) {
            doesEnumContainNegativeValue = true;
        }
    }

    if (doesEnumContainNegativeValue) {
        enumAsn1EnumeratedType->setEncoding(IntegerEncoding::twos_complement);
    } else {
        enumAsn1EnumeratedType->setEncoding(IntegerEncoding::pos_int);
    }

    m_context.addSimulinkModelAsn1Type(std::move(enumAsn1EnumeratedType), simulinkEnumType.description());
}

void DataTypeTranslatorVisitor::operator()(const AliasDataType &simulinkAliasType)
{
    const auto aliasAsn1TypeName = Escaper::escapeAsn1TypeName(simulinkAliasType.name());

    if (TasteStandardDataTypes::isTasteType(aliasAsn1TypeName))
        return;

    const auto *aliasDataTypeAsn1Type = m_context.findAsn1Type(simulinkAliasType.baseType());

    auto aliasAsn1UserdefinedType = std::make_unique<UserdefinedType>(
            aliasDataTypeAsn1Type->identifier(), m_context.simulinkModelDefinitionsName());

    auto aliasAsn1TypeAssignment = std::make_unique<TypeAssignment>(
            aliasAsn1TypeName, aliasAsn1TypeName, SourceLocation(), std::move(aliasAsn1UserdefinedType));

    if (!simulinkAliasType.description().isEmpty())
        aliasAsn1TypeAssignment->setComment(simulinkAliasType.description());

    m_context.getSimulinkModelAsn1Definitions()->addType(std::move(aliasAsn1TypeAssignment));
}

void DataTypeTranslatorVisitor::operator()(const BusDataType &simulinkBusType)
{
    m_busDataTypeTranslator.translate(simulinkBusType);
}

} // namespace conversion::asn1::translator
