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

#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/types/sequence.h>
#include <memory>
#include <seds/SedsModel/package/package.h>
#include <seds/SedsModel/types/containerdatatype.h>
#include <shared/qstringhash.h>
#include <unordered_map>

namespace conversion::asn1::translator {

class ContainerEntriesScope final
{
public:
    ContainerEntriesScope(Asn1Acn::Definitions *asn1Definitions, const seds::model::Package *sedsPackage,
            const Asn1Acn::Asn1Model::Data &asn1Files, const std::vector<seds::model::Package> &sedsPackages);

public:
    /**
     * @brief   Adds entries from given container data type to the current scope
     *
     * @param   sedsType    Type which entries should be saved
     */
    auto addContainer(const seds::model::ContainerDataType &sedsType) -> void;

    auto fetchComponents(const QString &sedsTypeName) const -> const Asn1Acn::Types::Sequence::Components &;
    auto fetchTrailerComponents(const QString &sedsTypeName) const -> const Asn1Acn::Types::Sequence::Components &;

    auto assertPresent(const QString &sedsTypeName) const -> void;

private:
    struct ScopeEntry final {
        std::unique_ptr<Asn1Acn::Types::Sequence> entries;
        std::unique_ptr<Asn1Acn::Types::Sequence> trailerEntries;
    };

    /// @brief  Parent definitions
    Asn1Acn::Definitions *m_asn1Definitions;
    /// @brief  Parent package
    const seds::model::Package *m_sedsPackage;

    /// @brief  List of already translated ASN.1 files
    const Asn1Acn::Asn1Model::Data &m_asn1Files;
    /// @brief  List of SEDS packages
    const std::vector<seds::model::Package> &m_sedsPackages;

    std::unordered_map<QString, ScopeEntry> m_scope;
};

} // namespace conversion::asn1::translator
