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
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <memory>
#include <seds/SedsModel/types/containerdatatype.h>
#include <seds/SedsModel/types/entries/entry.h>
#include <seds/SedsModel/types/entries/entrytype.h>
#include <seds/SedsModel/types/entries/errorcontrolentry.h>
#include <seds/SedsModel/types/entries/fixedvalueentry.h>
#include <seds/SedsModel/types/entries/lengthentry.h>
#include <seds/SedsModel/types/entries/listentry.h>
#include <seds/SedsModel/types/entries/paddingentry.h>

namespace conversion::asn1::translator::seds {

/**
 * @brief   Translator visitor for SEDS entry type
 *
 * Translated entry will be added to the passed ASN.1 sequence
 */
class EntryTranslatorVisitor final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context         Current translation context
     * @param   sedsContainer   Parent SEDS container
     * @param   sequence        ASN.1 sequence where translated entries will be added
     */
    EntryTranslatorVisitor(Context &context, const ::seds::model::ContainerDataType &sedsContainer,
            Asn1Acn::Types::Sequence *sequence);
    /**
     * @brief   Deleted copy constructor
     */
    EntryTranslatorVisitor(const EntryTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    EntryTranslatorVisitor(EntryTranslatorVisitor &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    EntryTranslatorVisitor &operator=(const EntryTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    EntryTranslatorVisitor &operator=(EntryTranslatorVisitor &&) = delete;

public:
    /**
     * @brief   Translates SEDS entry
     *
     * @param   sedsEntry   Entry to translate
     */
    auto operator()(const ::seds::model::Entry &sedsEntry) -> void;
    /**
     * @brief   Translates SEDS error constrol entry
     *
     * @param   sedsEntry   Entry to translate
     */
    auto operator()(const ::seds::model::ErrorControlEntry &sedsEntry) -> void;
    /**
     * @brief   Translates SEDS fixed value entry
     *
     * @param   sedsEntry   Entry to translate
     */
    auto operator()(const ::seds::model::FixedValueEntry &sedsEntry) -> void;
    /**
     * @brief   Translates SEDS length entry
     *
     * @param   sedsEntry   Entry to translate
     */
    auto operator()(const ::seds::model::LengthEntry &sedsEntry) -> void;
    /**
     * @brief   Translates SEDS list entry
     *
     * @param   sedsEntry   Entry to translate
     */
    auto operator()(const ::seds::model::ListEntry &sedsEntry) -> void;
    /**
     * @brief   Translates SEDS padding entry
     *
     * @param   sedsEntry   Entry to translate
     */
    auto operator()(const ::seds::model::PaddingEntry &sedsEntry) -> void;

private:
    auto translateEntryType(const ::seds::model::DataTypeRef &sedsTypeRef)
            -> std::unique_ptr<Asn1Acn::Types::UserdefinedType>;
    auto translateErrorControl(const ::seds::model::ErrorControlEntry &sedsEntry) const
            -> std::unique_ptr<Asn1Acn::Types::Null>;
    auto translateLengthEntry(const ::seds::model::LengthEntry &sedsEntry) const
            -> std::unique_ptr<Asn1Acn::Types::Null>;

    auto translateFixedValue(
            const ::seds::model::FixedValueEntry &sedsEntry, Asn1Acn::Types::UserdefinedType *asn1Type) const -> void;
    auto translateCoreErrorControl(::seds::model::CoreErrorControl coreErrorControl,
            const ::seds::model::ErrorControlEntry &sedsEntry, Asn1Acn::Types::Null *asn1Type) const -> void;
    auto getErrorControlEntryBitCount(const ::seds::model::ErrorControlEntry &entry) const -> uint64_t;
    auto getLengthEntryEncoding(const ::seds::model::LengthEntry &entry) const
            -> const ::seds::model::IntegerDataEncoding &;
    auto addListSizeConstraint(Asn1Acn::Types::SequenceOf *asn1Type, const ::seds::model::ListEntry &sedsEntry) const
            -> void;
    auto getListLengthEntry(const QString &entryName, const ::seds::model::ContainerDataType &container) const
            -> const ::seds::model::EntryType *;
    auto updateListLengthEntry(const ::seds::model::Entry *sedsEntry) const -> void;
    auto getListLengthSequenceComponent(const ::seds::model::Entry *sedsEntry) const
            -> std::unique_ptr<Asn1Acn::SequenceComponent> &;

private:
    Context &m_context;

    const ::seds::model::ContainerDataType &m_container;
    Asn1Acn::Types::Sequence *m_sequence;

    const static uint64_t m_crc8BitSize = 8;
    const static uint64_t m_crc16BitSize = 16;
    const static uint64_t m_checksumBitSize = 32;
    const static uint64_t m_checksumLongitundinalSize = 8;
};

} // namespace conversion::asn1::translator::seds
