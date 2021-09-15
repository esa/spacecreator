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

namespace ivm {
class IVFunction;
} // namespace ivm

namespace seds::model {
class Interface;
class InterfaceCommand;
} // namespace seds::model

#include <ivcore/ivinterface.h>

namespace conversion::iv::translator {
/**
 * @brief   Interface for translators from SEDS interface command to InterfaceView interface
 */
class InterfaceCommandTranslator
{
public:
    /**
     * @brief   Constructor
     *
     * @param   ivFunction      Output interface view function
     * @param   interface       Parent interface
     */
    InterfaceCommandTranslator(const seds::model::Interface &interface, ivm::IVFunction *ivFunction);
    /**
     * @brief   Default destructor
     */
    virtual ~InterfaceCommandTranslator() = default;
    /**
     * @brief   Deleted copy constructor
     */
    InterfaceCommandTranslator(const InterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    InterfaceCommandTranslator(InterfaceCommandTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    InterfaceCommandTranslator &operator=(const InterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    InterfaceCommandTranslator &operator=(InterfaceCommandTranslator &&) = delete;

public:
    /**
     * @brief   Translates SEDS interface command to interface view interface
     *
     * This inserts result IV interface into member IV function
     *
     * @param   command         SEDS interface command
     * @param   interfaceType   Interface type
     */
    virtual auto translateCommand(
            const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType interfaceType) -> void = 0;

protected:
    /**
     * @brief   Creates new interface view interface
     *
     * @param   command     SEDS interface command
     * @param   type        Type of interface to create
     * @param   kind        Kind of interface to create
     *
     * @return   Interface view interface
     */
    auto createIvInterface(const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType type,
            ivm::IVInterface::OperationKind kind) const -> ivm::IVInterface *;

    /**
     * @brief   Converts interface view interface type to string
     *
     * @param   interfaceType   Interface type to convert
     *
     * @return   Interface type name
     */
    auto interfaceTypeToString(ivm::IVInterface::InterfaceType interfaceType) const -> const QString &;

protected:
    /// @brief  Parent SEDS interface
    const seds::model::Interface &m_interface;
    /// @brief  Output interface view function
    ivm::IVFunction *m_ivFunction;

    /// @brief  Interface parameter encoding name
    static const QString m_interfaceParameterEncoding;
    /// @brief  Template for interface view interfaces
    static const QString m_ivInterfaceNameTemplate;
};

} // namespace conversion::iv::translator
