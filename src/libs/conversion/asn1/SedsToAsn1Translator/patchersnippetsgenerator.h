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

#include <asn1library/asn1/types/sequence.h>
#include <seds/SedsModel/types/containerdatatype.h>
#include <vector>

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
     * @param   context     Current translation context
     * @param   container   Parent SEDS container
     * @param   sequence    ASN.1 sequence where translated entries will be added
     */
    PatcherSnippetsGenerator(
            Context &context, const seds::model::ContainerDataType &container, Asn1Acn::Types::Sequence *sequence);
    /**
     * @brief   Deleted copy constructor
     */
    PatcherSnippetsGenerator(const PatcherSnippetsGenerator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    PatcherSnippetsGenerator(PatcherSnippetsGenerator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    PatcherSnippetsGenerator &operator=(const PatcherSnippetsGenerator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    PatcherSnippetsGenerator &operator=(PatcherSnippetsGenerator &&) = delete;

public:
    auto generate() -> void;

private:
    auto generatePatcherSnippets(const seds::model::ContainerDataType &container) -> void;

    auto buildErrorControlEntryFunction(const seds::model::ErrorControlEntry &errorControlEntry) -> void;
    auto buildLengthEntryFunction(const seds::model::LengthEntry &lengthEntry) -> void;

    auto buildErrorControlEntryEncodingFunction(
            const seds::model::ErrorControlEntry &entry, const uint64_t bitCount) const -> QString;
    auto buildErrorControlEntryDecodingValidator(
            const seds::model::ErrorControlEntry &entry, const uint64_t bitCount) const -> QString;
    auto buildLengthEntryEncodingFunction(
            const seds::model::IntegerDataEncoding &encoding, const QString &entryName) const -> QString;
    auto buildLengthEntryDecodingValidator(
            const seds::model::IntegerDataEncoding &encoding, const QString &entryName) const -> QString;

    auto getErrorControlBitCount(const seds::model::ErrorControlEntry &entry) const -> uint64_t;
    auto getLengthEncoding(const seds::model::LengthEntry &entry) const -> const seds::model::IntegerDataEncoding &;

private:
    Context &m_context;

    const seds::model::ContainerDataType &m_container;
    Asn1Acn::Types::Sequence *m_sequence;

    inline static const QString m_encodingFunctionTemplate = "%1-encoding-function";
    inline static const QString m_decodingValidatorTemplate = "%1-decoding-validator";
};

} // namespace conversion::asn1::translator
