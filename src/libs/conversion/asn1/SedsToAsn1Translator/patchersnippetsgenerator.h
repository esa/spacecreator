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

#include <QString>
#include <asn1library/asn1/patchersnippet.h>
#include <seds/SedsModel/types/containerdatatype.h>
#include <seds/SedsModel/types/encodings/integerdataencoding.h>
#include <seds/SedsModel/types/entries/entrytype.h>
#include <vector>

namespace seds::model {
class Package;
} // namespace seds::model

namespace conversion::asn1::translator {

/**
 * @brief   Generator for functions for patching ErrorControlEntry and LengthEntry
 */
class PatcherSnippetsGenerator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sedsPackage         Parent SEDS package
     * @param   sedsPackages        List of SEDS packages
     */
    PatcherSnippetsGenerator(
            const seds::model::Package *sedsPackage, const std::vector<seds::model::Package> &sedsPackages);

public:
    /**
     * @brief   Generate patcher functions for given container
     *
     * @param   sedsType        Source container
     *
     * @return  Patcher functions
     */
    auto generate(const seds::model::ContainerDataType &sedsType) const -> std::vector<Asn1Acn::PatcherSnippet>;

private:
    auto buildErrorControlEntryFunction(const seds::model::ErrorControlEntry &errorControlEntry,
            const QString &sequenceName) const -> Asn1Acn::PatcherSnippet;
    auto buildLengthEntryFunction(const seds::model::LengthEntry &lengthEntry, const QString &sequenceName) const
            -> Asn1Acn::PatcherSnippet;

    auto buildErrorControlEntryEncodingFunction(const seds::model::ErrorControlEntry &entry, const uint64_t bits,
            const QString &sequenceName) const -> QString;
    auto buildErrorControlEntryDecodingValidator(const seds::model::ErrorControlEntry &entry, const uint64_t bits,
            const QString &sequenceName) const -> QString;
    auto buildLengthEntryEncodingFunction(const seds::model::IntegerDataEncoding &encoding, const QString &sequenceName,
            const QString &entryName) const -> QString;
    auto buildLengthEntryDecodingValidator(const seds::model::IntegerDataEncoding &encoding,
            const QString &sequenceName, const QString &entryName) const -> QString;

    auto getErrorControlBitCount(const seds::model::ErrorControlEntry &lengthEntry) const -> uint64_t;
    auto getLengthEncoding(const seds::model::LengthEntry &lengthEntry) const
            -> const seds::model::IntegerDataEncoding &;

private:
    /// @brief  Parent package
    const seds::model::Package *m_sedsPackage;
    /// @brief  List of SEDS packages
    const std::vector<seds::model::Package> &m_sedsPackages;
};

} // namespace conversion::asn1::translator
