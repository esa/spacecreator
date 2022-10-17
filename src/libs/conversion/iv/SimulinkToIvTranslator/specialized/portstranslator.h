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

#include <ivcore/ivinterface.h>
#include <shared/parameter.h>
#include <simulink/SimulinkModel/common/port.h>
#include <simulink/SimulinkModel/simulinkmodel.h>

namespace conversion::iv::translator::simulink {

/**
 * @brief   Translator from SIMULINK ports to InterfaceView parameters
 */
class PortsTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   simulinkModel       Model with ports to translate
     * @param   ivInterface         IVInterface where translated ports will be added
     */
    PortsTranslator(const ::simulink::model::SimulinkModel &simulinkModel, ivm::IVInterface &ivInterface);
    /**
     * @brief   Deleted copy constructor
     */
    PortsTranslator(const PortsTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    PortsTranslator(PortsTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    PortsTranslator &operator=(const PortsTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    PortsTranslator &operator=(PortsTranslator &&) = delete;

public:
    /**
     * @brief   Translates SIMULINK ports to InterfaceView parameters
     */
    auto translatePorts() -> void;

    /**
     * @brief  Return encoding for created InterfaceParameter
     */
    static inline auto getEncodingForInterfaceParameter() -> QString { return "NATIVE"; }

private:
    auto translatePort(const ::simulink::model::Port &port, shared::InterfaceParameter::Direction direction) -> void;

private:
    const ::simulink::model::SimulinkModel &m_simulinkModel;
    ivm::IVInterface &m_ivInterface;
};

} // namespace conversion::iv::translator::simulink
