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

namespace conversion::asn1::translator {

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
     * @brief These group of methods create ASN.1 types related to MatLab's Standard Types
     */
    static auto getStandardAsn1BooleanDataType() -> std::unique_ptr<Asn1Acn::Types::Boolean>;
    static auto getStandardAsn1DoubleDataType() -> std::unique_ptr<Asn1Acn::Types::Real>;
    static auto getStandardAsn1SingleDataType() -> std::unique_ptr<Asn1Acn::Types::Real>;
    static auto getStandardAsn1Int8DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;
    static auto getStandardAsn1UInt8DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;
    static auto getStandardAsn1Int16DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;
    static auto getStandardAsn1UInt16DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;
    static auto getStandardAsn1Int32DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;
    static auto getStandardAsn1UInt32DataType() -> std::unique_ptr<Asn1Acn::Types::Integer>;

public:
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

} // namespace conversion::asn1::translator
