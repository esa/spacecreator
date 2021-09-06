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

#pragma once

#include <ivcore/ivinterface.h>
#include <seds/SedsModel/types/datatype.h>
#include <vector>

namespace Asn1Acn {
class Definitions;
class AsnSequenceComponent;
} // namespace Asn1Acn

namespace ivm {
class IVFunction;
} // namespace ivm

namespace seds::model {
class CommandArgument;
class Component;
class DataTypeRef;
class GenericTypeMapSet;
class Interface;
class InterfaceCommand;
class Package;
enum class CommandArgumentMode : uint8_t;
} // namespace seds::model

namespace conversion::iv::translator {

class AsyncInterfaceCommandTranslator final
{
public:
    AsyncInterfaceCommandTranslator(const seds::model::Package &package, const seds::model::Component &component,
            const seds::model::Interface &interface, ivm::IVFunction *ivFunction,
            Asn1Acn::Definitions *asn1Definitions);
    AsyncInterfaceCommandTranslator(const AsyncInterfaceCommandTranslator &) = delete;
    AsyncInterfaceCommandTranslator(AsyncInterfaceCommandTranslator &&) = delete;
    AsyncInterfaceCommandTranslator &operator=(const AsyncInterfaceCommandTranslator &) = delete;
    AsyncInterfaceCommandTranslator &operator=(AsyncInterfaceCommandTranslator) = delete;

public:
    auto translateCommand(const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType interfaceType)
            -> void;
    auto translateArguments(const std::vector<seds::model::CommandArgument> &arguments,
            seds::model::CommandArgumentMode requestedArgumentMode, ivm::IVInterface *ivInterface) -> void;

private:
    auto createIvInterface(const QString &name, ivm::IVInterface::InterfaceType interfaceType) const
            -> ivm::IVInterface *;
    auto createAsn1SequenceComponent(const seds::model::CommandArgument &commandArgument) const
            -> std::unique_ptr<Asn1Acn::AsnSequenceComponent>;

    auto findMappedType(const QString &genericTypeName) const -> const seds::model::DataTypeRef &;
    auto findDataType(const QString &dataTypeName) const -> const seds::model::DataType &;

    auto switchInterfaceType(ivm::IVInterface::InterfaceType interfaceType) const -> ivm::IVInterface::InterfaceType;

private:
    const seds::model::Package &m_package;
    const seds::model::Component &m_component;
    const seds::model::Interface &m_interface;
    ivm::IVFunction *m_ivFunction;
    Asn1Acn::Definitions *m_asn1Definitions;
};

} // namespace conversion::iv::translator
