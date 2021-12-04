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

#include "specialized/interfacecommandtranslator.h"

#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/asn1/SedsToAsn1Translator/specialized/dimensiontranslator.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <seds/SedsModel/components/interface.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>

using conversion::Escaper;
using conversion::UnhandledValueException;
using conversion::translator::TranslationException;

namespace conversion::iv::translator {

const QString InterfaceCommandTranslator::m_interfaceParameterEncoding = "ACN";
const QString InterfaceCommandTranslator::m_ivInterfaceNameTemplate = "%1_%2_%3";
const QString InterfaceCommandTranslator::m_arrayArgumentNameTemplate = "%1-Array";

InterfaceCommandTranslator::InterfaceCommandTranslator(
        const seds::model::Interface &sedsInterface, Asn1Acn::Definitions *asn1Definitions, ivm::IVFunction *ivFunction)
    : m_sedsInterface(sedsInterface)
    , m_asn1Definitions(asn1Definitions)
    , m_ivFunction(ivFunction)
{
}

QString InterfaceCommandTranslator::handleArgumentType(const seds::model::CommandArgument &sedsArgument) const
{
    const auto &sedsArgumentTypeName = findMappedType(sedsArgument.type().nameStr());
    const auto sedsArgumentTypeNameEscaped = Escaper::escapeAsn1TypeName(sedsArgumentTypeName);

    // Check if type exists
    const auto sedsArgumentTypeAssignment = m_asn1Definitions->type(sedsArgumentTypeNameEscaped);
    if (!sedsArgumentTypeAssignment) {
        auto errorMessage = QString("Argument %1 has an unknown type %2")
                                    .arg(sedsArgument.nameStr())
                                    .arg(sedsArgumentTypeNameEscaped);
        throw TranslationException(std::move(errorMessage));
    }

    // Just use type name if the type is not handled as an array
    if (sedsArgument.arrayDimensions().empty()) {
        return sedsArgumentTypeNameEscaped;
    }

    const auto arrayArgumentTypeName = m_arrayArgumentNameTemplate.arg(sedsArgumentTypeNameEscaped);

    // If the type was already created - use it
    if (m_asn1Definitions->type(arrayArgumentTypeName) != nullptr) {
        return arrayArgumentTypeName;
    }

    // Create new sequence type to use
    // TODO

    static const QString stub = "stub";
    return stub;
}

ivm::IVInterface *InterfaceCommandTranslator::createIvInterface(const seds::model::InterfaceCommand &sedsCommand,
        ivm::IVInterface::InterfaceType type, ivm::IVInterface::OperationKind kind) const
{
    ivm::IVInterface::CreationInfo creationInfo;
    creationInfo.function = m_ivFunction;
    creationInfo.type = type;
    creationInfo.name = getCommandName(m_sedsInterface.nameStr(), type, sedsCommand.nameStr());
    creationInfo.kind = kind;

    return ivm::IVInterface::createIface(creationInfo);
}

void InterfaceCommandTranslator::createAsn1SequenceComponent(
        const QString &name, const QString &typeName, Asn1Acn::Types::Sequence *sequence) const
{
    const auto *referencedTypeAssignment = m_asn1Definitions->type(typeName);
    const auto *referencedType = referencedTypeAssignment->type();

    auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(typeName, m_asn1Definitions->name());
    sequenceComponentType->setType(referencedType->clone());

    auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            name, name, false, std::nullopt, "", Asn1Acn::SourceLocation(), std::move(sequenceComponentType));
    sequence->addComponent(std::move(sequenceComponent));
}

ivm::IVInterface::InterfaceType InterfaceCommandTranslator::switchInterfaceType(
        ivm::IVInterface::InterfaceType interfaceType)
{
    switch (interfaceType) {
    case ivm::IVInterface::InterfaceType::Required:
        return ivm::IVInterface::InterfaceType::Provided;
    case ivm::IVInterface::InterfaceType::Provided:
        return ivm::IVInterface::InterfaceType::Required;
    default:
        throw UnhandledValueException("InterfaceType");
        break;
    }
}

const QString &InterfaceCommandTranslator::interfaceTypeToString(ivm::IVInterface::InterfaceType type)
{
    switch (type) {
    case ivm::IVInterface::InterfaceType::Required: {
        static QString name = "Ri";
        return name;
    }
    case ivm::IVInterface::InterfaceType::Provided: {
        static QString name = "Pi";
        return name;
    }
    case ivm::IVInterface::InterfaceType::Grouped: {
        static QString name = "Grp";
        return name;
    }
    default:
        throw UnhandledValueException("ivm::InterfaceType");
        break;
    }
}

QString InterfaceCommandTranslator::getCommandName(
        const QString &interfaceName, const ivm::IVInterface::InterfaceType type, const QString &commandName)
{
    return Escaper::escapeIvName(
            m_ivInterfaceNameTemplate.arg(interfaceName).arg(commandName).arg(interfaceTypeToString(type)));
}

const QString &InterfaceCommandTranslator::findMappedType(const QString &genericTypeName) const
{
    const auto &genericTypeMaps = m_sedsInterface.genericTypeMapSet().genericTypeMaps();

    if (genericTypeMaps.empty()) {
        return genericTypeName;
    }

    const auto foundMapping = std::find_if(genericTypeMaps.begin(), genericTypeMaps.end(),
            [&genericTypeName](
                    const seds::model::GenericTypeMap &typeMap) { return typeMap.nameStr() == genericTypeName; });

    if (foundMapping != genericTypeMaps.end()) {
        return foundMapping->type().nameStr();
    } else {
        return genericTypeName;
    }
}

} // namespace conversion::iv::translator
