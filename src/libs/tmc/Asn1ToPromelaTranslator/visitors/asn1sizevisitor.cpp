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

#include "asn1sizevisitor.h"

using Asn1Acn::IntegerValue;
using Asn1Acn::Range;

namespace tmc::translator {
Asn1SizeVisitor::Asn1SizeVisitor()
    : m_sizeVisited(false)
    , m_minSize(0)
    , m_maxSize(0)
{
}

void Asn1SizeVisitor::visit(const ::Asn1Acn::Constraints::RangeConstraint<IntegerValue> &constraint)
{
    const Range<IntegerValue::Type> &range = constraint.range();
    if (!m_sizeVisited) {
        m_sizeVisited = true;
        if (range.isSingleItem()) {
            m_minSize = static_cast<size_t>(range.begin());
            m_maxSize = m_minSize;
        } else {
            m_minSize = static_cast<size_t>(range.begin());
            m_maxSize = static_cast<size_t>(range.end());
        }
    } else {
        if (range.isSingleItem()) {
            m_maxSize = std::max(static_cast<size_t>(range.begin()), m_maxSize);
        } else {
            m_minSize = std::min(static_cast<size_t>(range.begin()), m_minSize);
            m_maxSize = std::max(static_cast<size_t>(range.end()), m_maxSize);
        }
    }
}

void Asn1SizeVisitor::visit(const ::Asn1Acn::Constraints::AndConstraint<IntegerValue> &constraint)
{
    constraint.leftChild()->accept(*this);
    constraint.rightChild()->accept(*this);
}

void Asn1SizeVisitor::visit(const ::Asn1Acn::Constraints::OrConstraint<IntegerValue> &constraint)
{
    constraint.leftChild()->accept(*this);
    constraint.rightChild()->accept(*this);
}

void Asn1SizeVisitor::visit(const ::Asn1Acn::Constraints::FromConstraint<IntegerValue> &constraint)
{
    Q_UNUSED(constraint);
}

void Asn1SizeVisitor::visit(const ::Asn1Acn::Constraints::SizeConstraint<IntegerValue> &constraint)
{
    Q_UNUSED(constraint);
}

void Asn1SizeVisitor::visit(const ::Asn1Acn::Constraints::ConstraintList<IntegerValue> &constraint)
{
    for (const auto &c : constraint.constraints()) {
        c->accept(*this);
    }
}

bool Asn1SizeVisitor::isSizeConstraintVisited() const noexcept
{
    return m_sizeVisited;
}

size_t Asn1SizeVisitor::getMinSize() const noexcept
{
    return m_minSize;
}

size_t Asn1SizeVisitor::getMaxSize() const noexcept
{
    return m_maxSize;
}
}
