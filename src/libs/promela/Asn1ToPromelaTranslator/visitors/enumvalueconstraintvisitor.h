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

#include <asn1library/asn1/constraints/constraintlist.h>
#include <asn1library/asn1/constraints/constraintvisitor.h>
#include <asn1library/asn1/constraints/fromconstraint.h>
#include <asn1library/asn1/constraints/logicoperators.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/values.h>
#include <vector>

namespace promela::translator {

/**
 * @brief   Visitor for handling enum constraints
 */
class EnumValueConstraintVisitor : public Asn1Acn::Constraints::ConstraintVisitor<Asn1Acn::EnumValue>
{
public:
    /// @brief Visit Asn1Acn::Constraints::RangeConstraint
    void visit(const Asn1Acn::Constraints::RangeConstraint<Asn1Acn::EnumValue> &constraint) override;
    /// @brief Visit Asn1Acn::Constraints::AndConstraint
    void visit(const Asn1Acn::Constraints::AndConstraint<Asn1Acn::EnumValue> &constraint) override;
    /// @brief Visit Asn1Acn::Constraints::OrConstraint
    void visit(const Asn1Acn::Constraints::OrConstraint<Asn1Acn::EnumValue> &constraint) override;
    /// @brief Visit Asn1Acn::Constraints::FromConstraint
    void visit(const Asn1Acn::Constraints::FromConstraint<Asn1Acn::EnumValue> &constraint) override;
    /// @brief Visit Asn1Acn::Constraints::SizeConstraint
    void visit(const Asn1Acn::Constraints::SizeConstraint<Asn1Acn::EnumValue> &constraint) override;
    /// @brief Visit Asn1Acn::Constraints::ConstraintList
    void visit(const Asn1Acn::Constraints::ConstraintList<Asn1Acn::EnumValue> &constraint) override;

    /**
     * @brief   Get allowed enum values
     */
    const std::vector<QString> &allowedValues() const;

private:
    std::vector<QString> m_allowedValues;
};

} // namespace promela::translator
