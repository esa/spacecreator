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

#include "specialized/rangetranslatorvisitor.h"

#include <asn1library/asn1/constraints/rangecombiner.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <conversion/common/translation/exceptions.h>
#include <cstdint>

namespace Asn1Acn {
class Definitions;
namespace Types {
class Integer;
class Real;
class Sequence;
class Type;
} // namespace Asn1Acn
} // namespace Types

namespace seds::model {
class EntryRef;
class ContainerRangeConstraint;
class ContainerTypeConstraint;
class ContainerValueConstraint;
class Package;
} // namespace seds::model

namespace conversion::asn1::translator {

/**
 * @brief   Translator visitor for SEDS container constraints
 *
 * Translated constraints will be applied to the passed ASN.1 sequence
 */
class ContainerConstraintTranslatorVisitor final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   asn1Sequence        ASN.1 sequence to which the translated constraints will be applied
     * @param   asn1Definitions     Parent ASN.1 definitions
     * @param   sedsPackage         Parent SEDS package
     */
    ContainerConstraintTranslatorVisitor(
            Asn1Acn::Types::Sequence *asn1Sequence, const seds::model::Package *sedsPackage);
    /**
     * @brief   Deleted copy constructor
     */
    ContainerConstraintTranslatorVisitor(const ContainerConstraintTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    ContainerConstraintTranslatorVisitor(ContainerConstraintTranslatorVisitor &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    ContainerConstraintTranslatorVisitor &operator=(const ContainerConstraintTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    ContainerConstraintTranslatorVisitor &operator=(ContainerConstraintTranslatorVisitor &&) = delete;

public:
    /**
     * @brief   Translates range container constraint
     *
     * @param   rangeConstraint     Constraint to translate
     */
    auto operator()(const seds::model::ContainerRangeConstraint &rangeConstraint) -> void;
    /**
     * @brief   Translates type container constraint
     *
     * @param   typeConstraint      Constraint to translate
     */
    auto operator()(const seds::model::ContainerTypeConstraint &typeConstraint) -> void;
    /**
     * @brief   Translates value container constraint
     *
     * @param   valueConstraint     Constraint to translate
     */
    auto operator()(const seds::model::ContainerValueConstraint &valueConstraint) -> void;

private:
    auto applyContainerRangeConstraint(
            const seds::model::ContainerRangeConstraint &rangeConstraint, Asn1Acn::Types::Type *asn1Type) const -> void;
    auto applyContainerTypeConstraint(
            const seds::model::ContainerTypeConstraint &typeConstraint, Asn1Acn::Types::Type *asn1Type) const -> void;
    auto applyContainerValueConstraint(
            const seds::model::ContainerValueConstraint &valueConstraint, Asn1Acn::Types::Type *asn1Type) const -> void;

    template<typename Type, typename ValueType>
    auto addTypeRangeConstraint(Asn1Acn::Types::Type *asn1Type, const Type *referencedType) const -> void;

    auto getConstrainedType(const seds::model::EntryRef &entry) const -> Asn1Acn::Types::Type *;

private:
    Asn1Acn::Types::Sequence *m_asn1Sequence;

    const seds::model::Package *m_sedsPackage;
};

template<typename Type, typename ValueType>
void ContainerConstraintTranslatorVisitor::addTypeRangeConstraint(
        Asn1Acn::Types::Type *asn1Type, const Type *referencedType) const
{
    const auto &constraints = referencedType->constraints();
    const auto resultRange = Asn1Acn::Constraints::RangeCombiner<ValueType>::combineRanges(&constraints);

    if (!resultRange) {
        auto errorMessage = QString("Type %1 used as a type constraint doesn't have a range that can be used")
                                    .arg(referencedType->identifier());
        throw conversion::translator::TranslationException(std::move(errorMessage));
    }

    RangeTranslatorVisitor<Type, ValueType> rangeTranslator(asn1Type);
    rangeTranslator.addRangeConstraint(*resultRange);
}

} // namespace conversion::asn1::translator
