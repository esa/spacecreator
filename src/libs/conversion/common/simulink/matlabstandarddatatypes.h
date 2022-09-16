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

#include <QStringList>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <conversion/common/escaper/escaper.h>
#include <memory>

namespace conversion::simulink {

/**
 * @brief  MatLabStandardDataTypes class provides various methods for handling MatLab's Standard Types
 */
class MatLabStandardDataTypes final
{
public:
    /**
     * @brief   Deleted constructor
     */
    MatLabStandardDataTypes() = delete;

public:
    /**
     * @brief   Method builds proper ASN.1 type name from MatLab Standard DataType name
     *
     * @param   typeName    Name of referenced data type
     *
     * @return  It returns built ASN.1 type name
     */
    static auto buildMatLabDataTypeName(const QString &dataType) -> QString;

    /**
     * @brief  Return escaped package name for standard MatLab data types
     */
    static inline auto getStandardDataTypesEscapedPackageName() -> QString
    {
        return Escaper::escapeAsn1PackageName("MatLab-Standard-DataTypes");
    }

    /**
     * @brief  Return prefix for standard MatLab data types
     */
    static inline auto getStandardDataTypesPrefix() -> QString { return "MatLab-"; }

    /**
     * @brief  Return bit size value for translated asn1 enumerated type
     */
    static inline auto getAsn1EnumBitSize() -> int { return 32; }

public:
    /**
     * @brief Create ASN.1 Boolean type related to MatLab Boolean Standard Type
     */
    static auto getStandardAsn1BooleanDataType() -> std::unique_ptr<Asn1Acn::Types::Boolean>;
    /**
     * @brief Create ASN.1 REAL type with proper constraints related to MatLab Double Standard Type
     */
    static auto getStandardAsn1DoubleDataType() -> std::unique_ptr<Asn1Acn::Types::Real>;
    /**
     * @brief Create ASN.1 REAL type with proper constraints related to MatLab Single Standard Type
     */
    static auto getStandardAsn1SingleDataType() -> std::unique_ptr<Asn1Acn::Types::Real>;
    /**
     * @brief Create ASN.1 INTEGER type with proper constraints related to MatLab Int8 Standard Type
     */
    static auto getStandardAsn1Int8DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;
    /**
     * @brief Create ASN.1 INTEGER type with proper constraints related to MatLab UInt8 Standard Type
     */
    static auto getStandardAsn1UInt8DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;
    /**
     * @brief Create ASN.1 INTEGER type with proper constraints related to MatLab Int16 Standard Type
     */
    static auto getStandardAsn1Int16DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;
    /**
     * @brief Create ASN.1 INTEGER type with proper constraints related to MatLab UInt16 Standard Type
     */
    static auto getStandardAsn1UInt16DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;
    /**
     * @brief Create ASN.1 INTEGER type with proper constraints related to MatLab Int32 Standard Type
     */
    static auto getStandardAsn1Int32DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;
    /**
     * @brief Create ASN.1 INTEGER type with proper constraints related to MatLab UInt32 Standard Type
     */
    static auto getStandardAsn1UInt32DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;

public:
    /**
     * @brief   Check if given data type name is MatLab's Standard Type
     *
     * @param   typeName    Name of referenced data type
     *
     * @return  true if given data type name is MatLab's Standard Type, false otherwise
     */
    static auto isMatLabType(const QString &typeName) -> bool;
    /**
     * @brief   Check if given data type name is MatLab's boolean Logical Type
     *
     * @param   typeName    Name of referenced data type
     *
     * @return  true if given data type name is MatLab's boolean Logical Type, false otherwise
     */
    static auto isBooleanType(const QString &typeName) -> bool;
    /**
     * @brief   Check if given data type name is one of MatLab's Numeric Type
     *
     * @param   typeName    Name of referenced data type
     *
     * @return  true if given data type name is one of MatLab's Numeric Type, false otherwise
     */
    static auto isNumericType(const QString &typeName) -> bool;

private:
    static const QStringList m_standardLogicalTypes;
    static const QStringList m_standardNumericTypes;
};

} // namespace conversion::simulink
