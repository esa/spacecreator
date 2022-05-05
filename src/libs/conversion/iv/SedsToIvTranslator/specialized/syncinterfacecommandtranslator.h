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

#pragma once

#include "generictypemapper.h"

#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/definitions.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <seds/SedsModel/interfaces/interfacecommand.h>
#include <seds/SedsModel/package/package.h>

namespace conversion::iv::translator {

/**
 * @brief   Translator from SEDS async interface command to InterfaceView interface
 */
class SyncInterfaceCommandTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   ivFunction                  Output interface view function
     * @param   sedsInterfaceDeclaration    Declaration of the parent interface
     * @param   sedsComponentName           Parent component name
     * @param   sedsInterfaceName           Parent interface name
     */
    SyncInterfaceCommandTranslator(ivm::IVFunction *ivFunction,
            const seds::model::InterfaceDeclaration &sedsInterfaceDeclaration, const QString &sedsComponentName,
            const QString &sedsInterfaceName);
    /**
     * @brief   Deleted copy constructor
     */
    SyncInterfaceCommandTranslator(const SyncInterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    SyncInterfaceCommandTranslator(SyncInterfaceCommandTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    SyncInterfaceCommandTranslator &operator=(const SyncInterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    SyncInterfaceCommandTranslator &operator=(SyncInterfaceCommandTranslator &&) = delete;

public:
    /**
     * @brief   Translates SEDS interface command to interface view interface
     *
     * This inserts result IV interface into member IV function
     *
     * @param   sedsCommand     SEDS interface command
     * @param   interfaceType   Interface type that will be created
     */
    auto translateCommand(
            const seds::model::InterfaceCommand &sedsCommand, ivm::IVInterface::InterfaceType interfaceType) -> void;

private:
    auto translateArguments(
            const std::vector<seds::model::CommandArgument> &sedsArguments, ivm::IVInterface *ivInterface) -> void;
    auto handleArgumentTypeName(const seds::model::CommandArgument &sedsArgument) const -> QString;

private:
    /// @brief  Output interface view function
    ivm::IVFunction *m_ivFunction;
    /// @brief  Parent interface declaration
    const seds::model::InterfaceDeclaration &m_sedsInterfaceDeclaration;
    /// @brief  Parent SEDS component name
    const QString &m_sedsComponentName;
    /// @brief  Parent SEDS interface name
    const QString &m_sedsInterfaceName;
};

} // namespace conversion::iv::translator
