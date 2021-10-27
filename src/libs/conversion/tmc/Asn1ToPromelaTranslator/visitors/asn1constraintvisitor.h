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

#include "asn1sizevisitor.h"

#include <QtGlobal>
#include <algorithm>
#include <asn1library/asn1/constraints/constraintlist.h>
#include <asn1library/asn1/constraints/constraintvisitor.h>
#include <asn1library/asn1/constraints/fromconstraint.h>
#include <asn1library/asn1/constraints/logicoperators.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/values.h>

namespace conversion::tmc::translator {
template<typename ValueType>
class Asn1ConstraintVisitor : public ::Asn1Acn::Constraints::ConstraintVisitor<ValueType>
{
public:
    void visit(const ::Asn1Acn::Constraints::RangeConstraint<ValueType> &constraint) override { Q_UNUSED(constraint); }

    void visit(const ::Asn1Acn::Constraints::AndConstraint<ValueType> &constraint) override
    {
        constraint.leftChild()->accept(*this);
    }

    void visit(const ::Asn1Acn::Constraints::OrConstraint<ValueType> &constraint) override
    {
        constraint.rightChild()->accept(*this);
    }

    void visit(const ::Asn1Acn::Constraints::FromConstraint<ValueType> &constraint) override { Q_UNUSED(constraint); }

    void visit(const ::Asn1Acn::Constraints::SizeConstraint<ValueType> &constraint) override
    {
        constraint.innerConstraints()->accept(m_sizeVisitor);
    }

    void visit(const ::Asn1Acn::Constraints::ConstraintList<ValueType> &constraint) override
    {
        for (const auto &c : constraint.constraints()) {
            c->accept(*this);
        }
    }

    bool isSizeConstraintVisited() const noexcept { return m_sizeVisitor.isSizeConstraintVisited(); }

    size_t getMinSize() const noexcept { return m_sizeVisitor.getMinSize(); }

    size_t getMaxSize() const noexcept { return m_sizeVisitor.getMaxSize(); }

private:
    Asn1SizeVisitor m_sizeVisitor;
};
}
