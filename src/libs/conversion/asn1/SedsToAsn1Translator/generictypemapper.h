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

struct TypeMapping final {
    struct Concrete {
        Asn1Acn::Types::Type *type;
        std::optional<QString> fixedValue;
        std::optional<QString> determinantValue;

        friend auto operator==(const Concrete &lhs, const Concrete &rhs) -> bool
        {
            return lhs.type->identifier() == rhs.type->identifier() && lhs.fixedValue == rhs.fixedValue;
        }

        friend auto operator!=(const Concrete &lhs, const Concrete &rhs) -> bool { return !(lhs == rhs); }
    };

    std::vector<Concrete> concreteMappings;
};

class GenericTypeMapper final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context         Current translation context
     * @param   interfaceName   Parent SEDS interface name
     */
    explicit GenericTypeMapper(Context &context, QString interfaceName);
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
    auto addMappings(const seds::model::GenericTypeMapSet &typeMapSet) -> void;
    auto getMapping(const QString &genericTypeName) const -> const TypeMapping *;

    auto determinantName() const -> const std::optional<QString> &;
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
    QString m_interfaceName;
    std::optional<QString> m_determinantName;
    std::optional<QString> m_determinantTypePath;

    std::unordered_map<QString, TypeMapping> m_mappings;
};

} // namespace conversion::asn1::translator
