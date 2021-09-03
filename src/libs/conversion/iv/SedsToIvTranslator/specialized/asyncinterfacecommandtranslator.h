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
#include <vector>

namespace ivm {
class IVFunction;
} // namespace ivm

namespace seds::model {
class CommandArgument;
class Component;
class InterfaceCommand;
enum class CommandArgumentMode : uint8_t;
} // namespace seds::model

namespace conversion::iv::translator {

class AsyncInterfaceCommandTranslator final
{
public:
    explicit AsyncInterfaceCommandTranslator(ivm::IVFunction *ivFunction);
    AsyncInterfaceCommandTranslator(const AsyncInterfaceCommandTranslator &) = delete;
    AsyncInterfaceCommandTranslator(AsyncInterfaceCommandTranslator &&) = delete;
    AsyncInterfaceCommandTranslator &operator=(const AsyncInterfaceCommandTranslator &) = delete;
    AsyncInterfaceCommandTranslator &operator=(AsyncInterfaceCommandTranslator) = delete;

public:
    auto translateCommand(
            const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType interfaceType) const -> void;
    auto translateArguments(const std::vector<seds::model::CommandArgument> &arguments,
            seds::model::CommandArgumentMode requestedArgumentMode, ivm::IVInterface *ivInterface) const -> void;
    auto translateArgument(const seds::model::CommandArgument &argument, ivm::IVInterface *ivInterface) const -> void;

private:
    auto createIvInterface(const QString &name, ivm::IVInterface::InterfaceType interfaceType) const
            -> ivm::IVInterface *;
    auto switchInterfaceType(ivm::IVInterface::InterfaceType interfaceType) const -> ivm::IVInterface::InterfaceType;

private:
    ivm::IVFunction *m_ivFunction;
};

} // namespace conversion::iv::translator
