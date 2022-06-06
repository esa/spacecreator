/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "context.h"

#include <asn1library/asn1/types/type.h>
#include <memory>
#include <optional>
#include <seds/SedsModel/generics/generictypemapset.h>
#include <shared/qstringhash.h>
#include <unordered_map>
#include <vector>

namespace conversion::asn1::translator {

/**
 * @brief   Data structure for SEDS generic type mapping
 */
struct TypeMapping final {
    /**
     * @brief   Information about concrete type
     */
    struct Concrete {
        /** @brief  Concrete ASN.1 type */
        Asn1Acn::Types::Type *type;
        /** @brief  Optional fixed value */
        std::optional<QString> fixedValue;
        /** @brief  Determinent value if present */
        std::optional<QString> determinantValue;

        /**
         * @brief   Equal operator
         *
         * @param   lhs     Left element
         * @param   rhs     Right element
         *
         * @return  True if equal, false otherwise
         */
        friend auto operator==(const Concrete &lhs, const Concrete &rhs) -> bool
        {
            return lhs.type->identifier() == rhs.type->identifier() && lhs.fixedValue == rhs.fixedValue;
        }

        /**
         * @brief   Not equal operator
         *
         * @param   lhs     Left element
         * @param   rhs     Right element
         *
         * @return  True if not equal, false otherwise
         */
        friend auto operator!=(const Concrete &lhs, const Concrete &rhs) -> bool { return !(lhs == rhs); }
    };

    /** @brief   List of types that given generic type can be mapped to */
    std::vector<Concrete> concreteMappings;
};

/**
 * @brief   Helper for mapping SEDS generic types
 */
class GenericTypeMapper final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context     Current translation context
     * @param   parentName  Name of the parent
     */
    GenericTypeMapper(Context &context, QString parentName);
    /**
     * @brief   Deleted copy constructor
     */
    GenericTypeMapper(const GenericTypeMapper &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    GenericTypeMapper(GenericTypeMapper &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    GenericTypeMapper &operator=(const GenericTypeMapper &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    GenericTypeMapper &operator=(GenericTypeMapper &&) = delete;

public:
    /**
     * @brief   Add mappings from type map set
     *
     * @param   typeMapSet      Generic type mappings
     */
    auto addMappings(const seds::model::GenericTypeMapSet &typeMapSet) -> void;
    /**
     * @brief   Get mapping for given generic type
     *
     * @param   genericTypeName     Name of the generic type
     *
     * @return  Mapping for given generic type if any, nullptr otherwise
     */
    auto getMapping(const QString &genericTypeName) const -> const TypeMapping *;

    /**
     * @brief   Getter for name of the determinant entry
     *
     * @return  Name of the deterinant entry if present, nullopt otherwise
     */
    auto determinantName() const -> const std::optional<QString> &;
    /**
     * @brief   Getter for type of the determinant entry
     *
     * @return  Type of the deterinant entry if present, nullopt otherwise
     */
    auto determinantTypePath() const -> const std::optional<QString> &;

private:
    auto addSimpleMapping(const seds::model::GenericTypeMap &typeMap) -> void;
    auto addAlternateMapping(const seds::model::GenericAlternate &alternate) -> void;

    auto findDeterminant(const std::vector<seds::model::GenericAlternate> &alternates)
            -> std::pair<QString, seds::model::DataTypeRef>;
    auto getPossibleDeterminants(const seds::model::GenericAlternate &alternate)
            -> std::vector<std::pair<QString, seds::model::DataTypeRef>>;
    auto handleDeterminantTypePath(const seds::model::DataTypeRef &determinantTypeRef) -> QString;

private:
    Context &m_context;
    QString m_parentName;
    std::optional<QString> m_determinantName;
    std::optional<QString> m_determinantTypePath;

    std::unordered_map<QString, TypeMapping> m_mappings;
};

} // namespace conversion::asn1::translator
