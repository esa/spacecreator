/****************************************************************************
**
** Copyright (C) 2017-2021 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a programme and funded by
** European Space Agency.
**
** This Library is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#pragma once

#include "constraint.h"
#include "constraintvisitor.h"
#include "rangeconstraint.h"

#include <memory>
#include <vector>

namespace Asn1Acn {
namespace Constraints {

template<typename ValueType>
class ConstraintList : public Constraint<ValueType>
{
public:
    ConstraintList() {}
    ConstraintList(const ConstraintList &other);

    const std::vector<std::unique_ptr<Constraint<ValueType>>> &constraints() const { return m_constraints; }

    void append(std::unique_ptr<Constraint<ValueType>> c) { m_constraints.emplace_back(std::move(c)); }

    void append(const Range<typename ValueType::Type> &r) { append(std::make_unique<RangeConstraint<ValueType>>(r)); }

    void clear() { m_constraints.clear(); }

    void accept(ConstraintVisitor<ValueType> &visitor) const override { visitor.visit(*this); }

    std::unique_ptr<Constraint<ValueType>> clone() const override;

private:
    std::vector<std::unique_ptr<Constraint<ValueType>>> m_constraints;
};

template<typename ValueType>
ConstraintList<ValueType>::ConstraintList(const ConstraintList &other)
{
    for (const auto &c : other.constraints())
        append(c->clone());
}

template<typename ValueType>
std::unique_ptr<Constraint<ValueType>> ConstraintList<ValueType>::clone() const
{
    return std::make_unique<ConstraintList<ValueType>>(*this);
}

}
}
