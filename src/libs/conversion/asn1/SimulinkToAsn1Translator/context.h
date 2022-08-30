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

#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/types/type.h>
#include <conversion/common/options.h>
#include <simulink/SimulinkModel/simulinkmodel.h>

namespace conversion::asn1::translator {

/**
 * @brief   Context with current SIMULINK to ASN.1 translation state
 */
class Context final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   simulinkModel               Current simulink model
     * @param   matlabDefinitions           Current MatLab Standard DataTypes ASN.1 definitions
     * @param   simulinkModelDefinitions    Current simulink model ASN.1 definitions
     * @param   options                     Translation options
     */
    Context(const simulink::model::SimulinkModel &simulinkModel,
            std::unique_ptr<Asn1Acn::Definitions> &matlabDefinitions,
            std::unique_ptr<Asn1Acn::Definitions> &simulinkModelDefinitions, const Options &options);
    /**
     * @brief   Deleted copy constructor
     */
    Context(const Context &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    Context(Context &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    Context &operator=(const Context &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    Context &operator=(Context &&) = delete;

public:
    /**
     * @brief   Add a type to current simulink model ASN.1 definitions
     *
     * @param   asn1Type            Type to add
     */
    auto addSimulinkModelAsn1Type(std::unique_ptr<Asn1Acn::Types::Type> asn1Type, QString description = QString())
            -> void;
    /**
     * @brief   Add a type to current MatLab Standard DataTypes ASN.1 definitions
     *
     * @param   asn1Type            Type to add
     */
    auto addMatLabStandardAsn1Type(std::unique_ptr<Asn1Acn::Types::Type> asn1Type) -> void;
    /**
     * @brief   Find ASN.1 data type by name
     *
     * @param   dataType     data type name (simulink model data type name or MatLab Standard DataType name)
     *
     * @return  Pointer to the found type, nullptr otherwise
     */
    auto findAsn1Type(const QString &dataType) -> Asn1Acn::Types::Type *;

    /**
     * @brief   Getter for name of the current simulink model definitions
     *
     * @return  Current simulink model definitions name
     */
    auto simulinkModelDefinitionsName() const -> const QString &;
    /**
     * @brief   Getter for name of the current matlab definitions
     *
     * @return  Current MatLab Standard DataTypes definitions name
     */
    auto matlabStandardDataTypesDefinitionsName() const -> const QString &;

    /**
     * @brief   Getter for simulink model ASN.1 definitions
     *
     * @return  Pointer to the ASN.1 definitions
     */
    auto getSimulinkModelAsn1Definitions() const -> Asn1Acn::Definitions *;

private:
    const simulink::model::SimulinkModel &m_simulinkModel;
    std::unique_ptr<Asn1Acn::Definitions> &m_matlabDefinitions;
    std::unique_ptr<Asn1Acn::Definitions> &m_simulinkModelDefinitions;
    const Options &m_options;
};

} // namespace conversion::asn1::translator
