/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "specialized/datatypetranslatorvisitor.h"

#include "specialized/descriptiontranslator.h"

#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/package/package.h>

using conversion::Escaper;
using conversion::UnhandledValueException;
using conversion::UnsupportedValueException;
using conversion::translator::MissingAsn1TypeDefinitionException;
using conversion::translator::TranslationException;
using seds::model::ArrayDataType;
using seds::model::BinaryDataType;
using seds::model::BooleanDataType;
using seds::model::ContainerDataType;
using seds::model::DataType;
using seds::model::EnumeratedDataType;
using seds::model::EnumeratedDataTypeRange;
using seds::model::FloatDataType;
using seds::model::IntegerDataType;
using seds::model::StringDataType;
using seds::model::SubRangeDataType;

namespace conversion::asn1::translator {

DataTypeTranslatorVisitor::DataTypeTranslatorVisitor(
        Asn1Acn::Definitions *outputAsn1Definitions, const Context &context)
    : m_outputAsn1Definitions(outputAsn1Definitions)
    , m_context(context)
{
}

void DataTypeTranslatorVisitor::operator()(const seds::model::ArrayDataType &sedsType)
{
    Q_UNUSED(sedsType);
}

void DataTypeTranslatorVisitor::operator()(const seds::model::BinaryDataType &sedsType)
{
    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::BitString>(asn1TypeName);
    translateBitStringLength(sedsType, asn1Type.get());

    addType(std::move(asn1Type), &sedsType);
}

void DataTypeTranslatorVisitor::operator()(const seds::model::BooleanDataType &sedsType)
{
    Q_UNUSED(sedsType);
}

void DataTypeTranslatorVisitor::operator()(const seds::model::ContainerDataType &sedsType)
{
    Q_UNUSED(sedsType);
}

void DataTypeTranslatorVisitor::operator()(const seds::model::EnumeratedDataType &sedsType)
{
    Q_UNUSED(sedsType);
}

void DataTypeTranslatorVisitor::operator()(const seds::model::FloatDataType &sedsType)
{
    Q_UNUSED(sedsType);
}

void DataTypeTranslatorVisitor::operator()(const seds::model::IntegerDataType &sedsType)
{
    Q_UNUSED(sedsType);
}

void DataTypeTranslatorVisitor::operator()(const seds::model::StringDataType &sedsType)
{
    Q_UNUSED(sedsType);
}

void DataTypeTranslatorVisitor::operator()(const seds::model::SubRangeDataType &sedsType)
{
    Q_UNUSED(sedsType);
}

void DataTypeTranslatorVisitor::addType(
        std::unique_ptr<Asn1Acn::Types::Type> asn1Type, const seds::model::Description *sedsDescription)
{
    const auto &asn1TypeName = asn1Type->identifier();
    auto asn1TypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
            asn1TypeName, asn1TypeName, Asn1Acn::SourceLocation(), std::move(asn1Type));
    DescriptionTranslator::translate(sedsDescription, asn1TypeAssignment.get());

    m_outputAsn1Definitions->addType(std::move(asn1TypeAssignment));
}

void DataTypeTranslatorVisitor::translateBitStringLength(
        const BinaryDataType &sedsType, Asn1Acn::Types::BitString *asn1Type) const
{
    if (sedsType.bits() > std::numeric_limits<Asn1Acn::IntegerValue::Type>::max()) {
        throw TranslationException("Bit string length size overflows ASN.1 range");
    }

    auto sizeConstraint = std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::BitStringValue>>();

    if (sedsType.hasFixedSize()) {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
                { static_cast<Asn1Acn::IntegerValue::Type>(sedsType.bits()) });
        sizeConstraint->setInnerConstraints(std::move(constraint));
    } else {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
                { 0, static_cast<Asn1Acn::IntegerValue::Type>(sedsType.bits()) });
        sizeConstraint->setInnerConstraints(std::move(constraint));
    }

    asn1Type->constraints().append(std::move(sizeConstraint));
}

} // namespace conversion::asn1::translator
