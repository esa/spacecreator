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

#include "rangevisitor.h"
#include "sizevisitor.h"

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
 * @brief Visitor for ASN.1 constraints
 *
 * This is a part of Asn1ToSedsTranslator.
 *
 * @tparam ValueType constrained value type
 */
template<typename ValueType>
class ConstraintVisitor : public ::Asn1Acn::Constraints::ConstraintVisitor<ValueType>
{
public:
    /// @brief Visit Asn1Acn::Constraints::RangeConstraint
    void visit(const ::Asn1Acn::Constraints::RangeConstraint<ValueType> &constraint) override
    {
        Q_UNUSED(constraint);
        constraint.accept(m_rangeVisitor);
    }

    /// @brief Visit Asn1Acn::Constraints::AndConstraint
    void visit(const ::Asn1Acn::Constraints::AndConstraint<ValueType> &constraint) override
    {
        constraint.leftChild()->accept(*this);
        constraint.rightChild()->accept(*this);
    }

    /// @brief Visit Asn1Acn::Constraints::OrConstraint
    void visit(const ::Asn1Acn::Constraints::OrConstraint<ValueType> &constraint) override
    {
        constraint.leftChild()->accept(*this);
        constraint.rightChild()->accept(*this);
    }

    /// @brief Visit Asn1Acn::Constraints::FromConstraint
    void visit(const ::Asn1Acn::Constraints::FromConstraint<ValueType> &constraint) override { Q_UNUSED(constraint); }

    /// @brief Visit Asn1Acn::Constraints::SizeConstraint
    void visit(const ::Asn1Acn::Constraints::SizeConstraint<ValueType> &constraint) override
    {
        constraint.innerConstraints()->accept(m_sizeVisitor);
    }

    /// @brief Visit Asn1Acn::Constraints::ConstraintList
    void visit(const ::Asn1Acn::Constraints::ConstraintList<ValueType> &constraint) override
    {
        for (const auto &c : constraint.constraints()) {
            c->accept(*this);
        }
    }

    /**
     * @brief Check if size constraint was visited.
     *
     * @return true if size constraint was visited, otherwise false
     */
    bool isSizeConstraintVisited() const noexcept { return m_sizeVisitor.isSizeConstraintVisited(); }

    /**
     * @brief Getter for found minimal size constraint
     *
     * @return minimal size constraint
     */
    size_t getMinSize() const noexcept { return m_sizeVisitor.getMinSize(); }

    /**
     * @brief Getter for found maximal size constraint
     *
     * @return maximal size constraint
     */
    size_t getMaxSize() const noexcept { return m_sizeVisitor.getMaxSize(); }

    /**
     * @brief Whether minimum and maximum size contraints differ
     *
     * @return true if maximum and minimum size constraints are different, false otherwise
     */
    bool isVariableSize() const noexcept { return m_sizeVisitor.getMaxSize() != m_sizeVisitor.getMinSize(); };

    /**
     * @brief Check if range constraint was visited.
     *
     * @return true if range constraint was visited, otherwise false
     */
    bool isRangeConstraintVisited() const noexcept { return m_rangeVisitor.isRangeConstraintVisited(); }

    Asn1Acn::Range<typename ValueType::Type> getRange() const noexcept { return m_rangeVisitor.getRange(); }

private:
    SizeVisitor m_sizeVisitor;
    RangeVisitor<ValueType> m_rangeVisitor;
};
} // namespace conversion::seds::translator
