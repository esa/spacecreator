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

#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <seds/SedsModel/components/interface.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>

using conversion::translator::UnhandledValueException;

namespace conversion::iv::translator {

const QString InterfaceCommandTranslator::m_ivInterfaceNameTemplate = "%1_%2_%3";

InterfaceCommandTranslator::InterfaceCommandTranslator(
        const seds::model::Interface &interface, ivm::IVFunction *ivFunction)
    : m_interface(interface)
    , m_ivFunction(ivFunction)
{
}

ivm::IVInterface *InterfaceCommandTranslator::createIvInterface(
        const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType type) const
{
    ivm::IVInterface::CreationInfo creationInfo;
    creationInfo.function = m_ivFunction;
    creationInfo.type = type;
    creationInfo.name = m_ivInterfaceNameTemplate.arg(m_interface.nameStr())
                                .arg(command.nameStr())
                                .arg(interfaceTypeToString(type));
    creationInfo.kind = ivm::IVInterface::OperationKind::Sporadic;

    return ivm::IVInterface::createIface(creationInfo);
}

const QString &InterfaceCommandTranslator::interfaceTypeToString(ivm::IVInterface::InterfaceType interfaceType) const
{
    switch (interfaceType) {
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
