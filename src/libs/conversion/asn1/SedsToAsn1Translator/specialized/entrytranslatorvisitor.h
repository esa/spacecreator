/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/types/sequence.h>
#include <optional>
#include <seds/SedsModel/types/datatype.h>
#include <seds/SedsModel/types/entries/entrytype.h>

namespace Asn1Acn {
class Definitions;
namespace Types {
class Null;
} // namespace Types
} // namespace Asn1Acn

namespace seds::model {
class Component;
class DataTypeRef;
class Entry;
class EntryRef;
class ErrorControlEntry;
class FixedValueEntry;
class LengthEntry;
class ListEntry;
class Package;
class PaddingEntry;
enum class CoreErrorControl;
} // namespace seds::model

namespace conversion::asn1::translator {

/**
 * @brief   Translator visitor for SEDS entry type
 *
 * Translated entry will be added to the passed ASN.1 sequence
 */
struct EntryTranslatorVisitor final {
public:
    /**
     * @brief   Constructor
     *
     * @param   asn1Sequence            ASN.1 sequence to which the translated entries will be added
     * @param   asn1Definitions         Parent ASN.1 definitions
     * @param   sedsParentContainer     Parent SEDS container
     * @param   sedsPackage             Parent SEDS package
     * @param   asn1Files               List of already translated ASN.1 files
     * @param   sedsPackages            List of SEDS packages
     * @param   sequenceSizeThreshold   ASN.1 sequence size threshold
     */
    EntryTranslatorVisitor(Asn1Acn::Types::Sequence *asn1Sequence, Asn1Acn::Definitions *asn1Definitions,
            const seds::model::ContainerDataType *sedsParentContainer, const seds::model::Package *sedsPackage,
            const Asn1Acn::Asn1Model::Data &asn1Files, const std::vector<seds::model::Package> &sedsPackages,
            const std::optional<uint64_t> &sequenceSizeThreshold);
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
    auto operator()(const seds::model::Entry &sedsEntry) -> void;
    /**
     * @brief   Translates SEDS error constrol entry
     *
     * @param   sedsEntry   Entry to translate
     */
    auto operator()(const seds::model::ErrorControlEntry &sedsEntry) -> void;
    /**
     * @brief   Translates SEDS fixed value entry
     *
     * @param   sedsEntry   Entry to translate
     */
    auto operator()(const seds::model::FixedValueEntry &sedsEntry) -> void;
    /**
     * @brief   Translates SEDS length entry
     *
     * @param   sedsEntry   Entry to translate
     */
    auto operator()(const seds::model::LengthEntry &sedsEntry) -> void;
    /**
     * @brief   Translates SEDS list entry
     *
     * @param   sedsEntry   Entry to translate
     */
    auto operator()(const seds::model::ListEntry &sedsEntry) -> void;
    /**
     * @brief   Translates SEDS padding entry
     *
     * @param   sedsEntry   Entry to translate
     */
    auto operator()(const seds::model::PaddingEntry &sedsEntry) -> void;

private:
    auto translateEntryType(const seds::model::DataTypeRef &sedsTypeRef) const
            -> std::unique_ptr<Asn1Acn::Types::UserdefinedType>;

    auto translateFixedValue(
            const seds::model::FixedValueEntry &sedsEntry, Asn1Acn::Types::UserdefinedType *asn1Type) const -> void;
    auto translateErrorControl(const seds::model::ErrorControlEntry &sedsEntry) const
            -> std::unique_ptr<Asn1Acn::Types::Null>;
    auto translateLengthField(const seds::model::LengthEntry &sedsEntry) const -> std::unique_ptr<Asn1Acn::Types::Null>;

    auto translateCoreErrorControl(seds::model::CoreErrorControl coreErrorControl,
            const seds::model::ErrorControlEntry &sedsEntry, Asn1Acn::Types::Null *asn1Type) const -> void;

    auto getLengthEntryEncoding(const seds::model::LengthEntry &lengthEntry) const
            -> const seds::model::IntegerDataEncoding &;
    auto getErrorControlEntryBitCount(const seds::model::ErrorControlEntry &entry) const -> uint64_t;

    auto updateListLengthEntry(const seds::model::Entry *sedsEntry) const -> void;
    auto getListLengthField(const QString &listLengthFieldName,
            const seds::model::ContainerDataType *sedsContainer) const -> const seds::model::EntryType *;
    auto getListLengthSequenceComponent(const seds::model::Entry *sedsEntry) const
            -> std::unique_ptr<Asn1Acn::SequenceComponent> &;
    auto addListSizeConstraint(Asn1Acn::Types::SequenceOf *asn1Type, const seds::model::ListEntry &sedsEntry) const
            -> void;

private:
    /// @brief  Where translated entry will be saved
    Asn1Acn::Types::Sequence *m_asn1Sequence;

    /// @brief  Parent definitions
    Asn1Acn::Definitions *m_asn1Definitions;
    /// @brief  Parent container
    const seds::model::ContainerDataType *m_sedsParentContainer;
    /// @brief  Parent package
    const seds::model::Package *m_sedsPackage;

    /// @brief  List of alerady translated ASN.1 files
    const Asn1Acn::Asn1Model::Data &m_asn1Files;
    /// @brief  List of SEDS packages
    const std::vector<seds::model::Package> &m_sedsPackages;

    /// @brief  ASN.1 sequence size threshold
    const std::optional<uint64_t> &m_sequenceSizeThreshold;

    const static uint64_t m_crc8BitSize = 8;
    const static uint64_t m_crc16BitSize = 16;
    const static uint64_t m_checksumBitSize = 32;
};

} // namespace conversion::asn1::translator
