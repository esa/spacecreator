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

#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/types/sequence.h>
#include <seds/SedsModel/types/datatype.h>

namespace Asn1Acn {
class Definitions;
namespace Types {
class Null;
} // namespace Types
} // namespace Asn1Acn

namespace seds::model {
class Component;
class Entry;
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
    /// @brief  Parent definitions
    Asn1Acn::Definitions *m_asn1Definitions;
    /// @brief  Where translated entry will be saved
    Asn1Acn::Types::Sequence *m_asn1Sequence;

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
    auto translateEntryType(const QString &sedsTypeName) const -> std::unique_ptr<Asn1Acn::Types::UserdefinedType>;
    auto translateFixedValue(
            const seds::model::FixedValueEntry &sedsEntry, Asn1Acn::Types::UserdefinedType *asn1Type) const -> void;
    auto translateErrorControl(const seds::model::ErrorControlEntry &sedsEntry) const
            -> std::unique_ptr<Asn1Acn::Types::Null>;

    auto translateCoreErrorControl(seds::model::CoreErrorControl coreErrorControl, Asn1Acn::Types::Null *asn1Type) const
            -> void;

    template<typename Type, typename ValueType>
    auto createValueConstraint(const QString &value, Asn1Acn::Types::Type *asn1Type) const -> void;
};

template<typename Type, typename ValueType>
void EntryTranslatorVisitor::createValueConstraint(const QString &value, Asn1Acn::Types::Type *asn1Type) const
{
    auto *referencedType = dynamic_cast<Type *>(asn1Type);
    auto constraint = Asn1Acn::Constraints::RangeConstraint<ValueType>::create({ ValueType::fromAstValue(value) });
    referencedType->constraints().append(std::move(constraint));
}

} // namespace conversion::asn1::translator
