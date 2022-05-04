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

#include "specialized/genericinterfacetypecreator.h"

#include "datatypetranslationhelper.h"
#include "generictypemapper.h"
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
#include <iostream>
#include <seds/SedsModel/package/package.h>

using conversion::UnhandledValueException;
using conversion::UnsupportedValueException;
using conversion::translator::MissingAsn1TypeDefinitionException;
using conversion::translator::TranslationException;
using conversion::translator::UndeclaredInterfaceException;
using seds::model::InterfaceCommandMode;

namespace conversion::asn1::translator {

GenericInterfaceTypeCreator::GenericInterfaceTypeCreator(
        Context &context, const seds::model::Interface &interface, const seds::model::Component &component)
    : m_context(context)
    , m_interface(interface)
    , m_component(component)
    , m_typeMapper(context, interface.nameStr())
{
    if (!interface.genericTypeMapSet()) {
        auto message = QString("Failed to create concrete types for interface '%1' because it has no type maps")
                               .arg(interface.nameStr());
        throw TranslationException(std::move(message));
    }

    m_typeMapper.addMappings(*interface.genericTypeMapSet());

    m_interfaceDeclaration = findInterfaceDeclaration(m_interface.type());
}

void GenericInterfaceTypeCreator::createTypes()
{
    const auto &genericTypes = m_interfaceDeclaration->genericTypes();
    for (const auto &genericType : genericTypes) {
        createTypeForGeneric(genericType);
    }

    for (const auto &command : m_interfaceDeclaration->commands()) {
        switch (command.mode()) {
        case InterfaceCommandMode::Sync:
            createTypesForSyncCommand(command);
            break;
        case InterfaceCommandMode::Async:
            createTypesForAsyncCommand(command);
            break;
        default:
            throw UnhandledValueException("InterfaceCommandMode");
        }
    }
}

void GenericInterfaceTypeCreator::createTypeForGeneric(const seds::model::GenericType &genericType)
{
    const auto &genericName = genericType.nameStr();

    const auto mapping = m_typeMapper.getMapping(genericName);
    if (mapping == nullptr || mapping->concreteMappings.empty()) {
        auto errorMessage = QString("Missing mapping for generic type '%1' in interface '%2'")
                                    .arg(genericName)
                                    .arg(m_interface.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    const auto &concreteMappings = mapping->concreteMappings;
    const auto allTheSame = std::all_of(concreteMappings.begin(), concreteMappings.end(),
            [&](const auto &concreteMapping) { return concreteMapping == concreteMappings.front(); });

    if (allTheSame) {
        createConcreteTypeAlias(genericType, concreteMappings.front());
    } else {
        const auto determinantName = m_typeMapper.determinantName();

        if (genericName == determinantName) {
            createConcreteTypeAlias(genericType, concreteMappings.front());
        } else {
            createConcreteChoice(genericType, mapping);
        }
    }
}

void GenericInterfaceTypeCreator::createConcreteTypeAlias(
        const seds::model::GenericType &genericType, const TypeMapping::Concrete &concreteMapping)
{
    const auto &genericName = genericType.nameStr();
    const auto concreteTypeName =
            DataTypeTranslationHelper::buildConcreteTypeName(m_component.nameStr(), m_interface.nameStr(), genericName);
    const auto concreteType = concreteMapping.type;

    auto aliasType = std::make_unique<Asn1Acn::TypeAssignment>(
            concreteTypeName, concreteTypeName, Asn1Acn::SourceLocation(), concreteType->clone());

    m_context.getAsn1Definitions()->addType(std::move(aliasType));
}

void GenericInterfaceTypeCreator::createConcreteChoice(
        const seds::model::GenericType &genericType, const TypeMapping *mapping)
{
    const auto &genericName = genericType.nameStr();
    const auto concreteTypeName =
            DataTypeTranslationHelper::buildConcreteTypeName(m_component.nameStr(), m_interface.nameStr(), genericName);

    const auto &concreteMappings = mapping->concreteMappings;

    auto choice = std::make_unique<Asn1Acn::Types::Choice>(concreteTypeName);

    for (std::size_t i = 0; i < concreteMappings.size(); ++i) {
        const auto &concreteMapping = concreteMappings[i];
        createConcreteChoiceAlternative(concreteMapping, choice.get(), i);
    }

    // Add an ACN parameter for determinant
    const auto &determinantTypeName = mapping->determinantTypeName.value();
    auto acnParameter = std::make_unique<Asn1Acn::AcnParameter>("determinant", "determinant", determinantTypeName);
    choice->addParameter(std::move(acnParameter));

    m_context.addAsn1Type(std::move(choice), nullptr);
}

void GenericInterfaceTypeCreator::createConcreteChoiceAlternative(
        const TypeMapping::Concrete &concreteMapping, Asn1Acn::Types::Choice *choice, const std::size_t count)
{
    const auto concreteType = concreteMapping.type;

    const auto choiceAlternativeName = QString("concrete%1").arg(count);
    auto choiceAlternativeType = concreteType->clone();

    if (concreteMapping.fixedValue) {
        handleFixedValue(choiceAlternativeType.get(), *concreteMapping.fixedValue);
    }

    const auto presentWhen = QString("determinant==%1").arg(*concreteMapping.determinantValue);

    auto choiceAlternative = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(choiceAlternativeName,
            choiceAlternativeName, choiceAlternativeName, choiceAlternativeName, presentWhen, Asn1Acn::SourceLocation(),
            std::move(choiceAlternativeType));

    choice->addComponent(std::move(choiceAlternative));
}

void GenericInterfaceTypeCreator::handleFixedValue(Asn1Acn::Types::Type *type, const QString &fixedValue)
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

void GenericInterfaceTypeCreator::createTypesForSyncCommand(const seds::model::InterfaceCommand &command)
{
    for (const auto &argument : command.arguments()) {
        createSyncArgumentType(argument);
    }
}

void GenericInterfaceTypeCreator::createSyncArgumentType(const seds::model::CommandArgument &argument)
{
    const auto argumentTypeRef = argument.type();

    const auto isGeneric = isTypeGeneric(argumentTypeRef);

    if (isGeneric) {
        if (!argument.arrayDimensions().empty()) {
            auto errorMessage = QString("Command argument '%1' could not be translated, array arguments with generic "
                                        "types are not supported because of the ACN limitations")
                                        .arg(argument.nameStr());
            throw TranslationException(std::move(errorMessage));
        }
    } else {
        DataTypeTranslationHelper::handleArrayArgumentType(m_context, argumentTypeRef, argument.arrayDimensions());
    }
}

void GenericInterfaceTypeCreator::createTypesForAsyncCommand(const seds::model::InterfaceCommand &command)
{
    const auto commandName = Escaper::escapeAsn1TypeName(command.nameStr());

    // Process command based on its commands
    switch (command.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly: {
        // In arguments are 'native', so they are handles as-is
        const auto bundledTypeName = DataTypeTranslationHelper::buildGenericBundledTypeName(
                m_component.nameStr(), m_interface.nameStr(), commandName);
        createAsyncCommandBundledType(command, bundledTypeName, seds::model::CommandArgumentMode::In);
    } break;
    case seds::model::ArgumentsCombination::OutOnly: {
        // Out arguments aren't supported by TASTE sporadic interface.
        // We cannot change the argument direction, so we switch interface type (provided <-> required)
        const auto bundledTypeName = DataTypeTranslationHelper::buildGenericBundledTypeName(
                m_component.nameStr(), m_interface.nameStr(), commandName);
        createAsyncCommandBundledType(command, bundledTypeName, seds::model::CommandArgumentMode::Out);
    } break;
    case seds::model::ArgumentsCombination::NoArgs: {
        // No arguments, no problems
        break;
    }
    case seds::model::ArgumentsCombination::InAndNotify:
    case seds::model::ArgumentsCombination::NotifyOnly:
    case seds::model::ArgumentsCombination::InAndOut:
    case seds::model::ArgumentsCombination::OutAndNotify:
    case seds::model::ArgumentsCombination::All: {
        const auto message = QString("Interface command arguments combination '%1' is not supported for TASTE "
                                     "InterfaceView async generic interface")
                                     .arg(argumentsCombinationToString(command.argumentsCombination()));
        throw TranslationException(message);
    } break;
    default:
        throw UnhandledValueException("ArgumentsCombination");
        break;
    }
}

void GenericInterfaceTypeCreator::createAsyncCommandBundledType(const seds::model::InterfaceCommand &command,
        const QString &bundledTypeName, const seds::model::CommandArgumentMode requestedArgumentMode)
{
    auto bundledType = std::make_unique<Asn1Acn::Types::Sequence>(bundledTypeName);

    const auto determinantArgumentName = findDeterminantArgument(command.arguments());

    for (const auto &argument : command.arguments()) {
        if (argument.mode() != requestedArgumentMode) {
            continue;
        }

        createAsyncCommandBundledTypeComponent(argument, bundledType.get(), determinantArgumentName);
    }

    m_context.addAsn1Type(std::move(bundledType), nullptr);
}

void GenericInterfaceTypeCreator::createAsyncCommandBundledTypeComponent(const seds::model::CommandArgument &argument,
        Asn1Acn::Types::Sequence *bundledType, const QString &determinantArgumentName)
{
    const auto argumentName = Escaper::escapeAsn1FieldName(argument.nameStr());
    const auto argumentTypeRef = argument.type();

    Asn1Acn::Types::Type *argumentType = nullptr;

    const auto isGeneric = isTypeGeneric(argumentTypeRef);

    if (isGeneric) {
        if (!argument.arrayDimensions().empty()) {
            auto errorMessage = QString("Command argument '%1' could not be translated, array arguments with generic "
                                        "types are not supported because of the ACN limitations")
                                        .arg(argument.nameStr());
            throw TranslationException(std::move(errorMessage));
        }

        const auto argumentConcreteTypeName = DataTypeTranslationHelper::buildConcreteTypeName(
                m_component.nameStr(), m_interface.nameStr(), argumentTypeRef.nameStr());
        argumentType = m_context.findAsn1Type(argumentConcreteTypeName);

        auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(
                argumentType->identifier(), m_context.definitionsName());
        sequenceComponentType->setType(argumentType->clone());

        if (argumentName == determinantArgumentName) {
            auto sequenceComponent = std::make_unique<Asn1Acn::AcnSequenceComponent>(
                    argumentName, argumentName, std::move(sequenceComponentType));

            bundledType->addComponent(std::move(sequenceComponent));
        } else {
            auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(argumentName, argumentName, false,
                    std::nullopt, "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
                    std::move(sequenceComponentType));

            if (argumentType->typeEnum() == Asn1Acn::Types::Type::ASN1Type::CHOICE) {
                sequenceComponent->addAcnParameter(determinantArgumentName);
            }

            bundledType->addComponent(std::move(sequenceComponent));
        }
    } else {
        argumentType = DataTypeTranslationHelper::handleArrayArgumentType(
                m_context, argumentTypeRef, argument.arrayDimensions());

        auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(
                argumentType->identifier(), m_context.definitionsName());
        sequenceComponentType->setType(argumentType->clone());

        auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(argumentName, argumentName, false,
                std::nullopt, "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
                std::move(sequenceComponentType));

        bundledType->addComponent(std::move(sequenceComponent));
    }
}

const seds::model::InterfaceDeclaration *GenericInterfaceTypeCreator::findInterfaceDeclaration(
        const seds::model::InterfaceDeclarationRef &interfaceRef)
{
    const auto &name = interfaceRef.nameStr();

    const auto namesEqual = [&name](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
        return interfaceDeclaration.nameStr() == name;
    };

    if (interfaceRef.packageStr()) {
        const auto package = m_context.getSedsPackage(*interfaceRef.packageStr());
        const auto &interfaceDeclarations = package->declaredInterfaces();

        const auto found = std::find_if(interfaceDeclarations.begin(), interfaceDeclarations.end(), namesEqual);
        if (found != interfaceDeclarations.end()) {
            return &(*found);
        }
    } else {
        const auto &componentInterfaceDeclarations = m_component.declaredInterfaces();
        auto found =
                std::find_if(componentInterfaceDeclarations.begin(), componentInterfaceDeclarations.end(), namesEqual);
        if (found != componentInterfaceDeclarations.end()) {
            return &(*found);
        }

        const auto package = m_context.getSedsPackage();
        const auto &packageInterfaceDeclarations = package->declaredInterfaces();
        found = std::find_if(packageInterfaceDeclarations.begin(), packageInterfaceDeclarations.end(), namesEqual);
        if (found != packageInterfaceDeclarations.end()) {
            return &(*found);
        }
    }

    throw UndeclaredInterfaceException(interfaceRef.value().pathStr());
}

QString GenericInterfaceTypeCreator::findDeterminantArgument(const std::vector<seds::model::CommandArgument> &arguments)
{
    const auto &determinantName = m_typeMapper.determinantName();
    const auto foundArgument = std::find_if(arguments.begin(), arguments.end(),
            [&](const seds::model::CommandArgument &argument) { return argument.type().nameStr() == determinantName; });

    return Escaper::escapeAsn1FieldName(foundArgument->nameStr());
}

bool GenericInterfaceTypeCreator::isTypeGeneric(const seds::model::DataTypeRef &typeRef)
{
    if (typeRef.packageStr()) {
        return false;
    }

    const auto &typeName = typeRef.nameStr();
    const auto &genericTypes = m_interfaceDeclaration->genericTypes();

    const auto found = std::find_if(genericTypes.begin(), genericTypes.end(),
            [&](const seds::model::GenericType &genericType) { return genericType.nameStr() == typeName; });

    return found != genericTypes.end();
}

} // namespace conversion::asn1::translator
