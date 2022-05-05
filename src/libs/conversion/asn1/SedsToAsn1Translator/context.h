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
#include <asn1library/asn1/file.h>
#include <asn1library/asn1/types/type.h>
#include <conversion/common/options.h>
#include <cstdint>
#include <list>
#include <memory>
#include <optional>
#include <seds/SedsModel/base/description.h>
#include <seds/SedsModel/types/datatype.h>
#include <seds/SedsModel/types/datatyperef.h>
#include <vector>

namespace seds::model {
class Package;
} // namespace seds::model

namespace conversion::asn1::translator {

class Context final
{
public:
    Context(const seds::model::Package *sedsPackage, Asn1Acn::Definitions *definitions,
            Asn1Acn::Definitions *parentDefinitions, const std::list<const seds::model::Package *> &sedsPackages,
            const std::vector<std::unique_ptr<Asn1Acn::File>> &asn1Files, const Options &options);
    Context(const Context &) = delete;
    Context(Context &&) = delete;

    Context &operator=(const Context &) = delete;
    Context &operator=(Context &&) = delete;

public:
    auto addAsn1Type(std::unique_ptr<Asn1Acn::Types::Type> asn1Type, const seds::model::Description *sedsDescription)
            -> void;

    auto findSedsType(const seds::model::DataTypeRef &typeRef) -> const seds::model::DataType *;
    auto findAsn1Type(const seds::model::DataTypeRef &typeRef) -> Asn1Acn::Types::Type *;
    auto findAsn1TypeDefinitions(const seds::model::DataTypeRef &typeRef) -> Asn1Acn::Definitions *;

    auto getSedsPackage() const -> const seds::model::Package *;
    auto getSedsPackage(const QString &packageName) const -> const seds::model::Package *;
    auto getAsn1Definitions() const -> Asn1Acn::Definitions *;
    auto getAsn1Definitions(const QString &asn1FileName) const -> Asn1Acn::Definitions *;

    auto definitionsName() const -> const QString &;
    auto arraySizeThreshold() const -> std::optional<uint64_t>;

private:
    const seds::model::Package *m_sedsPackage;
    Asn1Acn::Definitions *m_definitions;
    Asn1Acn::Definitions *m_parentDefinitions;

    const std::list<const seds::model::Package *> &m_sedsPackages;
    const std::vector<std::unique_ptr<Asn1Acn::File>> &m_asn1Files;

    const Options &m_options;
};

} // namespace conversion::asn1::translator
