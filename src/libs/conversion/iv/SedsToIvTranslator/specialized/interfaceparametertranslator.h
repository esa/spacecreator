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

namespace seds::model {
class Interface;
class InterfaceParameter;
} // namespace seds::model

namespace ivm {
class IVFunction;
} // namespace ivm

namespace conversion::iv::translator {

/**
 * @brief   Translator from SEDS interface parameter to InterfaceView interface
 */
class InterfaceParameterTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sedsInterface   Parent interface
     * @param   ivFunction      Output interface view function
     */
    InterfaceParameterTranslator(const seds::model::Interface &sedsInterface, ivm::IVFunction *ivFunction);
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
            const ivm::IVInterface::InterfaceType interfaceType) const -> void;

private:
    enum class InterfaceMode
    {
        Getter,
        Setter
    };

private:
    auto translateGetterParameter(const seds::model::InterfaceParameter &sedsParameter,
            const ivm::IVInterface::InterfaceType interfaceType) const -> void;
    auto translateSetterParameter(const seds::model::InterfaceParameter &sedsParameter,
            const ivm::IVInterface::InterfaceType interfaceType) const -> void;

    auto createIvInterface(const InterfaceParameterTranslator::InterfaceMode mode,
            const seds::model::InterfaceParameter &sedsParameter, ivm::IVInterface::InterfaceType type,
            ivm::IVInterface::OperationKind kind, shared::InterfaceParameter::Direction direction) const -> void;
    auto switchInterfaceType(ivm::IVInterface::InterfaceType interfaceType) const -> ivm::IVInterface::InterfaceType;

    static auto getParameterName(const InterfaceMode mode, const QString &interfaceName,
            const ivm::IVInterface::InterfaceType type, const QString &parameterName) -> QString;
    static auto interfaceTypeToString(ivm::IVInterface::InterfaceType type) -> const QString &;

private:
    /// @brief  Parent SEDS interface
    const seds::model::Interface &m_sedsInterface;
    /// @brief  Output interface view function
    ivm::IVFunction *m_ivFunction;

    /// @brief  Interface parameter encoding name
    static const QString m_interfaceParameterEncoding;
    /// @brief  Prefix for getter interfaces
    static const QString m_getterInterfacePrefix;
    /// @brief  Prefix for setter interfaces
    static const QString m_setterInterfacePrefix;
    /// @brief  Template for interface view interfaces
    static const QString m_ivInterfaceNameTemplate;
    /// @brief  Name for the argument in the IV interface
    static const QString m_ivInterfaceParameterName;
};

} // namespace conversion::iv::translator
