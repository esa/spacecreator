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

#include "realrangeconstraintvisitor.h"

#include <algorithm>

using Asn1Acn::Range;
using Asn1Acn::RealValue;

namespace promela::translator {

void RealRangeConstraintVisitor::visit(const Asn1Acn::Constraints::RangeConstraint<RealValue> &constraint)
{
    const Range<RealValue::Type> &range = constraint.range();
    if (range.isSingleItem()) {
        m_subset = RealSubset(range.begin());
    } else {
        m_subset = RealSubset(range.begin(), range.end());
    }
}

void RealRangeConstraintVisitor::visit(const Asn1Acn::Constraints::AndConstraint<RealValue> &constraint)
{
    RealRangeConstraintVisitor lhsVisitor;
    constraint.leftChild()->accept(lhsVisitor);
    std::optional<RealSubset> lhs = lhsVisitor.getResultSubset();

    RealRangeConstraintVisitor rhsVisitor;
    constraint.rightChild()->accept(rhsVisitor);
    std::optional<RealSubset> rhs = rhsVisitor.getResultSubset();

    if (!lhs.has_value() || !rhs.has_value()) {
        m_subset = std::nullopt;
    }

    m_subset = lhs.value() & rhs.value();
}

void RealRangeConstraintVisitor::visit(const Asn1Acn::Constraints::OrConstraint<RealValue> &constraint)
{
    RealRangeConstraintVisitor lhsVisitor;
    constraint.leftChild()->accept(lhsVisitor);
    std::optional<RealSubset> lhs = lhsVisitor.getResultSubset();

    RealRangeConstraintVisitor rhsVisitor;
    constraint.rightChild()->accept(rhsVisitor);
    std::optional<RealSubset> rhs = rhsVisitor.getResultSubset();

    if (!lhs.has_value() || !rhs.has_value()) {
        m_subset = std::nullopt;
    }

    m_subset = lhs.value() | rhs.value();
}

void RealRangeConstraintVisitor::visit(const Asn1Acn::Constraints::FromConstraint<RealValue> &constraint)
{
    Q_UNUSED(constraint);
}

void RealRangeConstraintVisitor::visit(const Asn1Acn::Constraints::SizeConstraint<RealValue> &constraint)
{
    Q_UNUSED(constraint);
}

void RealRangeConstraintVisitor::visit(const Asn1Acn::Constraints::ConstraintList<RealValue> &constraint)
{
    std::optional<RealSubset> subset;
    for (const auto &singleConstraint : constraint.constraints()) {
        RealRangeConstraintVisitor nestedVisitor;
        singleConstraint->accept(nestedVisitor);
        std::optional<RealSubset> rhs = nestedVisitor.getResultSubset();

        if (!subset.has_value()) {
            subset = rhs;
        } else if (rhs.has_value()) {
            subset = subset.value() & rhs.value();
        }
    }

    m_subset = subset;
}

bool RealRangeConstraintVisitor::isSizeConstraintVisited() const noexcept
{
    return m_subset.has_value();
}

size_t RealRangeConstraintVisitor::getMinSize() const noexcept
{
    int min = m_subset.value().getMin().value();
    return static_cast<size_t>(min);
}

size_t RealRangeConstraintVisitor::getMaxSize() const noexcept
{
    int max = m_subset.value().getMax().value();
    return static_cast<size_t>(max);
}

const std::optional<RealSubset> &RealRangeConstraintVisitor::getResultSubset() const
{
    return m_subset;
}
}
