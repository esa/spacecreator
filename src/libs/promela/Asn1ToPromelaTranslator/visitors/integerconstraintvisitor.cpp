/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 - 2022 N7 Space Sp. z o.o.
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

#include "integerconstraintvisitor.h"

using Asn1Acn::IntegerValue;
using Asn1Acn::Range;

namespace promela::translator {
IntegerConstraintVisitor::IntegerConstraintVisitor() {}

void IntegerConstraintVisitor::visit(const ::Asn1Acn::Constraints::RangeConstraint<IntegerValue> &constraint)
{
    const Range<IntegerValue::Type> &range = constraint.range();
    if (range.isSingleItem()) {
        m_subset = IntegerSubset(range.begin());
    } else {
        m_subset = IntegerSubset(range.begin(), range.end());
    }
}

void IntegerConstraintVisitor::visit(const ::Asn1Acn::Constraints::AndConstraint<IntegerValue> &constraint)
{
    IntegerConstraintVisitor lhsVisitor;
    constraint.leftChild()->accept(lhsVisitor);
    std::optional<IntegerSubset> lhs = lhsVisitor.getResultSubset();

    IntegerConstraintVisitor rhsVisitor;
    constraint.rightChild()->accept(rhsVisitor);
    std::optional<IntegerSubset> rhs = rhsVisitor.getResultSubset();

    if (!lhs.has_value() || !rhs.has_value()) {
        m_subset = std::nullopt;
    }

    m_subset = lhs.value() & rhs.value();
}

void IntegerConstraintVisitor::visit(const ::Asn1Acn::Constraints::OrConstraint<IntegerValue> &constraint)
{
    IntegerConstraintVisitor lhsVisitor;
    constraint.leftChild()->accept(lhsVisitor);
    std::optional<IntegerSubset> lhs = lhsVisitor.getResultSubset();

    IntegerConstraintVisitor rhsVisitor;
    constraint.rightChild()->accept(rhsVisitor);
    std::optional<IntegerSubset> rhs = rhsVisitor.getResultSubset();

    if (!lhs.has_value() || !rhs.has_value()) {
        m_subset = std::nullopt;
    }

    m_subset = lhs.value() | rhs.value();
}

void IntegerConstraintVisitor::visit(const ::Asn1Acn::Constraints::FromConstraint<IntegerValue> &constraint)
{
    Q_UNUSED(constraint);
}

void IntegerConstraintVisitor::visit(const ::Asn1Acn::Constraints::SizeConstraint<IntegerValue> &constraint)
{
    Q_UNUSED(constraint);
}

void IntegerConstraintVisitor::visit(const ::Asn1Acn::Constraints::ConstraintList<IntegerValue> &constraint)
{
    std::optional<IntegerSubset> tmp;
    for (const auto &c : constraint.constraints()) {
        IntegerConstraintVisitor nestedVisitor;
        c->accept(nestedVisitor);
        std::optional<IntegerSubset> rhs = nestedVisitor.getResultSubset();

        if (!tmp.has_value()) {
            tmp = rhs;
        } else if (rhs.has_value()) {
            tmp = tmp.value() | rhs.value();
        }
    }

    m_subset = tmp;
}

bool IntegerConstraintVisitor::isSizeConstraintVisited() const noexcept
{
    return m_subset.has_value();
}

size_t IntegerConstraintVisitor::getMinValue() const noexcept
{
    int min = m_subset.value().getMin().value();
    if (min < 0) {
        return 0;
    }
    return static_cast<size_t>(min);
}

size_t IntegerConstraintVisitor::getMaxValue() const noexcept
{
    int max = m_subset.value().getMax().value();
    if (max < 0) {
        return 0;
    }
    return static_cast<size_t>(max);
}

const std::optional<IntegerSubset> &IntegerConstraintVisitor::getResultSubset() const
{
    return m_subset;
}
} // namespace promela::translator
