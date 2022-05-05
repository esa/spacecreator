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

#include "interfacetranslatorhelper.h"

#include "specialized/descriptiontranslator.h"

#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/sequence.h>
#include <conversion/asn1/SedsToAsn1Translator/specialized/datatypetranslatorvisitor.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/types/arraydatatype.h>
#include <seds/SedsModel/types/dimensionsize.h>
#include <shared/parameter.h>

using conversion::UnhandledValueException;
using conversion::translator::TranslationException;

namespace conversion::iv::translator {

ivm::IVInterface *InterfaceTranslatorHelper::createIvInterface(const QString &name,
        ivm::IVInterface::InterfaceType type, ivm::IVInterface::OperationKind kind,
        const seds::model::Description &sedsDescription, ivm::IVFunction *m_ivFunction)
{
    ivm::IVInterface::CreationInfo creationInfo;
    creationInfo.name = name;
    creationInfo.type = type;
    creationInfo.kind = kind;
    creationInfo.function = m_ivFunction;

    auto interface = ivm::IVInterface::createIface(creationInfo);
    DescriptionTranslator::translate(sedsDescription, interface);

    return interface;
}

shared::InterfaceParameter InterfaceTranslatorHelper::createInterfaceParameter(
        const QString &name, const QString &typeName, shared::InterfaceParameter::Direction direction)
{
    return shared::InterfaceParameter(
            name, shared::BasicParameter::Type::Other, typeName, m_interfaceParameterEncoding, direction);
}

QString InterfaceTranslatorHelper::buildParameterInterfaceName(const QString &sedsInterfaceName,
        const QString &sedsParameterName,
        const InterfaceTranslatorHelper::InterfaceParameterType interfaceParameterType,
        ivm::IVInterface::InterfaceType type)
{
    switch (interfaceParameterType) {
    case InterfaceParameterType::Getter:
        return Escaper::escapeIvName(m_ivParameterInterfaceNameTemplate.arg(m_getterInterfacePrefix)
                                             .arg(sedsInterfaceName)
                                             .arg(sedsParameterName)
                                             .arg(interfaceTypeToString(type)));
    case InterfaceParameterType::Setter:
        return Escaper::escapeIvName(m_ivParameterInterfaceNameTemplate.arg(m_setterInterfacePrefix)
                                             .arg(sedsInterfaceName)
                                             .arg(sedsParameterName)
                                             .arg(interfaceTypeToString(type)));
    default:
        throw UnhandledValueException("InterfaceMode");
        break;
    }
}

QString InterfaceTranslatorHelper::buildCommandInterfaceName(
        const QString &sedsInterfaceName, const QString &commandName, const ivm::IVInterface::InterfaceType type)
{
    return Escaper::escapeIvName(
            m_ivCommandInterfaceNameTemplate.arg(sedsInterfaceName).arg(commandName).arg(interfaceTypeToString(type)));
}

ivm::IVInterface::InterfaceType InterfaceTranslatorHelper::switchInterfaceType(
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

const QString &InterfaceTranslatorHelper::interfaceTypeToString(ivm::IVInterface::InterfaceType type)
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

} // namespace conversion::iv::translator
