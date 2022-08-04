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

#include "enumvalueconstraintvisitor.h"

using Asn1Acn::EnumValue;

namespace promela::translator {

void EnumValueConstraintVisitor::visit(const Asn1Acn::Constraints::RangeConstraint<EnumValue> &constraint)
{
    const auto &range = constraint.range();

    if (range.isSingleItem()) {
        m_allowedValues.push_back(range.begin());
    }
}

void EnumValueConstraintVisitor::visit(const Asn1Acn::Constraints::AndConstraint<EnumValue> &constraint)
{
    EnumValueConstraintVisitor leftVisitor;
    constraint.leftChild()->accept(leftVisitor);
    auto leftValues = leftVisitor.allowedValues();
    std::sort(leftValues.begin(), leftValues.end());

    EnumValueConstraintVisitor rightVisitor;
    constraint.rightChild()->accept(rightVisitor);
    auto rightValues = rightVisitor.allowedValues();
    std::sort(rightValues.begin(), rightValues.end());

    std::set_intersection(leftValues.begin(), leftValues.end(), rightValues.begin(), rightValues.end(),
            std::back_inserter(m_allowedValues));
}

void EnumValueConstraintVisitor::visit(const Asn1Acn::Constraints::OrConstraint<EnumValue> &constraint)
{
    constraint.leftChild()->accept(*this);
    constraint.rightChild()->accept(*this);
}

void EnumValueConstraintVisitor::visit(const Asn1Acn::Constraints::FromConstraint<EnumValue> &constraint)
{
    Q_UNUSED(constraint);
}

void EnumValueConstraintVisitor::visit(const Asn1Acn::Constraints::SizeConstraint<EnumValue> &constraint)
{
    Q_UNUSED(constraint);
}

void EnumValueConstraintVisitor::visit(const Asn1Acn::Constraints::ConstraintList<EnumValue> &constraint)
{
    for (const auto &c : constraint.constraints()) {
        c->accept(*this);
    }
}

const std::vector<QString> &EnumValueConstraintVisitor::allowedValues() const
{
    return m_allowedValues;
}

} // namespace promela::translator
