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

#include <QtGlobal>
#include <algorithm>
#include <asn1library/asn1/constraints/constraintlist.h>
#include <asn1library/asn1/constraints/constraintvisitor.h>
#include <asn1library/asn1/constraints/fromconstraint.h>
#include <asn1library/asn1/constraints/logicoperators.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/values.h>

namespace conversion::seds::translator {
/**
 * @brief Visitor for ASN.1 range constraints
 *
 * This is a part of Asn1ToSedsTranslator.
 *
 * Visitor finds a range for a numeric type.
 * @tparam ValueType constrained value type
 */
template<typename ValueType>
class RangeVisitor : public ::Asn1Acn::Constraints::ConstraintVisitor<ValueType>
{
public:
    /**
     * @brief Constructor.
     *
     * Initializes empty visitor
     */
    RangeVisitor()
        : m_rangeVisited(false)
        , m_range(typename ValueType::Type())
    {
    }

    /**
     * @brief Visit Asn1Acn::Constraints::RangeConstraint
     *
     * RangeConstraints contains actual sizes
     * Calculate maximum and minimum size of an array using Constraint
     *
     * @param constraint element to visit
     */
    void visit(const ::Asn1Acn::Constraints::RangeConstraint<ValueType> &constraint) override
    {
        m_rangeVisited = true;
        m_range = constraint.range();
    }

    /// @brief Visit Asn1Acn::Constraints::AndConstraint
    void visit(const ::Asn1Acn::Constraints::AndConstraint<ValueType> &constraint) override { Q_UNUSED(constraint); }
    /// @brief Visit Asn1Acn::Constraints::OrConstraint
    void visit(const ::Asn1Acn::Constraints::OrConstraint<ValueType> &constraint) override { Q_UNUSED(constraint); }
    /// @brief Visit Asn1Acn::Constraints::FromConstraint
    void visit(const ::Asn1Acn::Constraints::FromConstraint<ValueType> &constraint) override { Q_UNUSED(constraint); }
    /// @brief Visit Asn1Acn::Constraints::SizeConstraint
    void visit(const ::Asn1Acn::Constraints::SizeConstraint<ValueType> &constraint) override { Q_UNUSED(constraint); }
    /// @brief Visit Asn1Acn::Constraints::ConstraintList
    void visit(const ::Asn1Acn::Constraints::ConstraintList<ValueType> &constraint) override { Q_UNUSED(constraint); }

    /**
     * @brief Check if size constraint was visited.
     *
     * @return true if size constraint was visited, otherwise false
     */
    bool isRangeConstraintVisited() const noexcept { return m_rangeVisited; }

    /**
     * @brief Getter for found range constraint
     *
     * @return range constraint
     */
    Asn1Acn::Range<typename ValueType::Type> getRange() const noexcept { return m_range; }

private:
    bool m_rangeVisited;
    Asn1Acn::Range<typename ValueType::Type> m_range;
};
} // namespace conversion::seds::translator
