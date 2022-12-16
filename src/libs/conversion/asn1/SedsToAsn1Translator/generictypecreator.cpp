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

#include "generictypecreator.h"

#include "datatypetranslationhelper.h"
#include "specialized/datatypetranslatorvisitor.h"
#include "specialized/rangetranslatorvisitor.h"

#include <asn1library/asn1/acnsequencecomponent.h>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/null.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/translation/exceptions.h>

using conversion::UnhandledValueException;
using conversion::UnsupportedValueException;
using conversion::translator::MissingAsn1TypeDefinitionException;
using conversion::translator::TranslationException;
using seds::model::GenericType;

namespace conversion::asn1::translator::seds {

GenericTypeCreator::GenericTypeCreator(Context &context, QString parentName, const GenericTypeMapper &typeMapper)
    : m_context(context)
    , m_parentName(std::move(parentName))
    , m_typeMapper(typeMapper)
{
}

QString GenericTypeCreator::createTypeForGeneric(const TypeMapping *mapping, const QString &genericName)
{
    if (mapping == nullptr || mapping->concreteMappings.empty()) {
        auto errorMessage =
                QString("Missing mapping for generic type '%1' in interface '%2'").arg(genericName).arg(m_parentName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto &concreteMappings = mapping->concreteMappings;
    const auto allTheSame = std::all_of(concreteMappings.begin(), concreteMappings.end(),
            [&](const auto &concreteMapping) { return concreteMapping == concreteMappings.front(); });

    if (allTheSame) {
        return createConcreteTypeAlias(genericName, concreteMappings.front());
    } else {
        const auto determinantName = m_typeMapper.determinantName();

        if (genericName == determinantName) {
            return createConcreteTypeAlias(genericName, concreteMappings.front());
        } else {
            return createConcreteChoice(genericName, mapping);
        }
    }
}

QString GenericTypeCreator::createConcreteTypeAlias(
        const QString &genericName, const TypeMapping::Concrete &concreteMapping)
{
    const auto concreteTypeName = DataTypeTranslationHelper::buildConcreteTypeName(m_parentName, genericName);

    if (m_context.hasAsn1Type(concreteTypeName)) {
        return concreteTypeName;
    }

    auto concreteType = concreteMapping.type->clone();

    DataTypeTranslationHelper::removeConstraints(concreteType.get());

    auto aliasType =
            std::make_unique<Asn1Acn::Types::UserdefinedType>(concreteType->identifier(), m_context.definitionsName());
    aliasType->setType(std::move(concreteType));

    auto aliasTypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
            concreteTypeName, concreteTypeName, Asn1Acn::SourceLocation(), std::move(aliasType));

    m_context.getAsn1Definitions()->addType(std::move(aliasTypeAssignment));

    return concreteTypeName;
}

QString GenericTypeCreator::createConcreteChoice(const QString &genericName, const TypeMapping *mapping)
{
    const auto concreteTypeName = DataTypeTranslationHelper::buildConcreteTypeName(m_parentName, genericName);

    if (m_context.hasAsn1Type(concreteTypeName)) {
        return concreteTypeName;
    }

    const auto &concreteMappings = mapping->concreteMappings;

    auto choice = std::make_unique<Asn1Acn::Types::Choice>(concreteTypeName);

    for (std::size_t i = 0; i < concreteMappings.size(); ++i) {
        const auto &concreteMapping = concreteMappings[i];
        createConcreteChoiceAlternative(concreteMapping, choice.get(), i);
    }

    // Add an ACN parameter for determinant
    auto acnParameter =
            std::make_unique<Asn1Acn::AcnParameter>("determinant", "determinant", *m_typeMapper.determinantTypePath());
    choice->addParameter(std::move(acnParameter));

    m_context.addAsn1Type(std::move(choice), nullptr);

    return concreteTypeName;
}

void GenericTypeCreator::createConcreteChoiceAlternative(
        const TypeMapping::Concrete &concreteMapping, Asn1Acn::Types::Choice *choice, const std::size_t count)
{
    const auto concreteType = concreteMapping.type;

    auto choiceAlternativeType = concreteType->clone();

    DataTypeTranslationHelper::removeConstraints(choiceAlternativeType.get());

    if (concreteMapping.fixedValue) {
        handleFixedValue(choiceAlternativeType.get(), *concreteMapping.fixedValue);
    }

    const auto presentWhen = QString("determinant==%1").arg(*concreteMapping.determinantValue);

    auto choiceAlternativeUserType = std::make_unique<Asn1Acn::Types::UserdefinedType>(
            choiceAlternativeType->identifier(), m_context.definitionsName());
    choiceAlternativeUserType->setType(std::move(choiceAlternativeType));

    const auto choiceAlternativeName = QString("concrete%1").arg(count);
    auto choiceAlternative = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(choiceAlternativeName,
            choiceAlternativeName, choiceAlternativeName, choiceAlternativeName, presentWhen, Asn1Acn::SourceLocation(),
            std::move(choiceAlternativeUserType));

    choice->addComponent(std::move(choiceAlternative));
}

void GenericTypeCreator::handleFixedValue(Asn1Acn::Types::Type *type, const QString &fixedValue)
{
    switch (type->typeEnum()) {
    case Asn1Acn::Types::Type::ASN1Type::INTEGER: {
        const auto value = Asn1Acn::IntegerValue::fromAstValue(fixedValue);
        RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue> rangeTranslator(type);
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::REAL: {
        const auto value = Asn1Acn::RealValue::fromAstValue(fixedValue);
        RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue> rangeTranslator(type);
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::BOOLEAN: {
        const auto value = Asn1Acn::BooleanValue::fromAstValue(fixedValue);
        RangeTranslatorVisitor<Asn1Acn::Types::Boolean, Asn1Acn::BooleanValue> rangeTranslator(type);
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::SEQUENCE:
        throw UnsupportedValueException("ASN1Type/GenericTypeMapFixedValue", "SEQUENCE");
        break;
    case Asn1Acn::Types::Type::ASN1Type::SEQUENCEOF:
        throw UnsupportedValueException("ASN1Type/GenericTypeMapFixedValue", "SEQUENCEOF");
        break;
    case Asn1Acn::Types::Type::ASN1Type::ENUMERATED: {
        const auto value = Asn1Acn::EnumValue::fromAstValue(fixedValue);
        RangeTranslatorVisitor<Asn1Acn::Types::Enumerated, Asn1Acn::EnumValue> rangeTranslator(type);
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::CHOICE:
        throw UnsupportedValueException("ASN1Type/GenericTypeMapFixedValue", "CHOICE");
        break;
    case Asn1Acn::Types::Type::ASN1Type::STRING:
        throw UnsupportedValueException("ASN1Type/GenericTypeMapFixedValue", "STRING");
        break;
    case Asn1Acn::Types::Type::ASN1Type::IA5STRING: {
        const auto value = Asn1Acn::StringValue::fromAstValue(fixedValue);
        RangeTranslatorVisitor<Asn1Acn::Types::IA5String, Asn1Acn::StringValue> rangeTranslator(type);
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::NUMERICSTRING: {
        const auto value = Asn1Acn::StringValue::fromAstValue(fixedValue);
        RangeTranslatorVisitor<Asn1Acn::Types::NumericString, Asn1Acn::StringValue> rangeTranslator(type);
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::NULLTYPE:
        throw UnsupportedValueException("ASN1Type/GenericTypeMapFixedValue", "NULLTYPE");
        break;
    case Asn1Acn::Types::Type::ASN1Type::BITSTRING: {
        const auto value = Asn1Acn::BitStringValue::fromAstValue(fixedValue);
        RangeTranslatorVisitor<Asn1Acn::Types::BitString, Asn1Acn::BitStringValue> rangeTranslator(type);
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::OCTETSTRING: {
        const auto value = Asn1Acn::OctetStringValue::fromAstValue(fixedValue);
        RangeTranslatorVisitor<Asn1Acn::Types::OctetString, Asn1Acn::OctetStringValue> rangeTranslator(type);
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::LABELTYPE:
        throw UnsupportedValueException("ASN1Type/GenericTypeMapFixedValue", "LABELTYPE");
        break;
    case Asn1Acn::Types::Type::ASN1Type::USERDEFINED:
        throw UnsupportedValueException("ASN1Type/GenericTypeMapFixedValue", "USERDEFINED");
        break;
    default:
        throw UnhandledValueException("ASN1Type/GenericTypeMapFixedValue");
        break;
    }
}

} // namespace conversion::asn1::translator::seds
