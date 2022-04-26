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
#include <seds/SedsModel/components/entryref.h>
#include <seds/SedsModel/types/constraints/containerrangeconstraint.h>
#include <seds/SedsModel/types/constraints/containertypeconstraint.h>
#include <seds/SedsModel/types/constraints/containervalueconstraint.h>

namespace conversion::asn1::translator {

class ContainerConstraintTranslatorVisitor final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context     Current translation context
     * @param   sequence    ASN.1 sequence to constraint
     */
    ContainerConstraintTranslatorVisitor(Context &context, Asn1Acn::Types::Sequence *sequence);
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

    auto getConstrainedType(const seds::model::EntryRef &entry) const -> Asn1Acn::Types::Type *;

private:
    Context &m_context;
    Asn1Acn::Types::Sequence *m_sequence;
};

} // namespace conversion::asn1::translator
