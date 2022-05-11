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
#include "interfacetranslatorhelper.h"

#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <seds/SedsModel/interfaces/interfaceparameter.h>

namespace conversion::iv::translator {

/**
 * @brief   Translator from SEDS interface parameter to InterfaceView interface
 */
class InterfaceParameterTranslator final
{
public:
    /**
     * @brief Enumeration indicating whether the interface is a Setter of a Getter
     */
    enum class InterfaceMode
    {
        Getter,
        Setter
    };

    /**
     * @brief   Constructor
     *
     * @param   ivFunction          Output interface view function
     * @param   sedsInterfaceName   Parent interface name
     */
    InterfaceParameterTranslator(ivm::IVFunction *ivFunction, const QString &sedsInterfaceName);
    /**
     * @brief   Deleted copy constructor
     */
    InterfaceParameterTranslator(const InterfaceParameterTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    InterfaceParameterTranslator(InterfaceParameterTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    InterfaceParameterTranslator &operator=(const InterfaceParameterTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    InterfaceParameterTranslator &operator=(InterfaceParameterTranslator &&) = delete;

public:
    /**
     * @brief   Translates SEDS interface parameter to interface view interface
     *
     * This inserts result IV interface into member IV function
     *
     * @param   sedsParameter   SEDS interface parameter
     * @param   interfaceType   Interface type that will be created
     */
    auto translateParameter(const seds::model::InterfaceParameter &sedsParameter,
            const ivm::IVInterface::InterfaceType interfaceType) -> void;

private:
    auto translateGetterParameter(const seds::model::InterfaceParameter &sedsParameter,
            const ivm::IVInterface::InterfaceType interfaceType) -> void;
    auto translateSetterParameter(const seds::model::InterfaceParameter &sedsParameter,
            const ivm::IVInterface::InterfaceType interfaceType) -> void;

    auto buildParameter(const seds::model::InterfaceParameter &sedsParameter,
            InterfaceTranslatorHelper::InterfaceParameterType interfaceParameterType,
            const ivm::IVInterface::InterfaceType interfaceType, ivm::IVInterface::OperationKind interfaceKind,
            shared::InterfaceParameter::Direction interfaceDirection) -> void;
    auto handleParameterTypeName(const seds::model::InterfaceParameter &sedsParameter) const -> QString;

private:
    /// @brief  Output interface view function
    ivm::IVFunction *m_ivFunction;
    /// @brief  Parent SEDS interface name
    const QString &m_sedsInterfaceName;

    /// @brief  Name for the argument in the IV interface
    inline static const QString m_ivInterfaceParameterName = "Param";
};

} // namespace conversion::iv::translator
