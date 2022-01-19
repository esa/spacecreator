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
#include "values.h"

#include <memory>
#include <optional>
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

    std::optional<Range<typename ValueType::Type>> combineRanges() const;
    template<typename ConstraintType>
    std::optional<Range<typename ConstraintType::Type>> combineRange(
            const Constraint<ConstraintType> *constraint) const;

    std::optional<Range<int64_t>> combineSizes() const;
    std::optional<Range<int64_t>> combineSize(const Constraint<ValueType> *constraint) const;

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
std::optional<Range<typename ValueType::Type>> ConstraintList<ValueType>::combineRanges() const
{
    if (m_constraints.empty()) {
        return std::nullopt;
    }

    return std::accumulate(std::next(m_constraints.begin()), m_constraints.end(),
            combineRange<ValueType>(m_constraints[0].get()), [&](const auto &range, const auto &constraint) {
                auto combinedRange = combineRange<ValueType>(constraint.get());

                if (!combinedRange) {
                    return range;
                } else if (!range) {
                    return combinedRange;
                } else {
                    return std::make_optional(range->merge(*combinedRange));
                }
            });
}

template<typename ValueType>
template<typename ConstraintType>
std::optional<Range<typename ConstraintType::Type>> ConstraintList<ValueType>::combineRange(
        const Constraint<ConstraintType> *constraint) const
{
    if (!constraint) {
        return std::nullopt;
    }

    if (const auto *constraintList = dynamic_cast<const ConstraintList<ConstraintType> *>(constraint); constraintList) {
        return constraintList->combineRanges();
    } else if (const auto *andConstraint = dynamic_cast<const Constraints::AndConstraint<ConstraintType> *>(constraint);
               andConstraint) {
        const auto leftRange = combineRange<ConstraintType>(andConstraint->leftChild());
        const auto rightRange = combineRange<ConstraintType>(andConstraint->rightChild());

        if (!leftRange) {
            return rightRange;
        } else if (!rightRange) {
            return leftRange;
        } else {
            return leftRange->intersection(*rightRange);
        }
    } else if (const auto *orConstraint = dynamic_cast<const Constraints::OrConstraint<ConstraintType> *>(constraint);
               orConstraint) {
        const auto leftRange = combineRange<ConstraintType>(andConstraint->leftChild());
        const auto rightRange = combineRange<ConstraintType>(andConstraint->rightChild());

        if (!leftRange) {
            return rightRange;
        } else if (!rightRange) {
            return leftRange;
        } else {
            return leftRange->merge(*rightRange);
        }
    } else if (const auto *rangeConstraint =
                       dynamic_cast<const Constraints::RangeConstraint<ConstraintType> *>(constraint);
               rangeConstraint) {
        return rangeConstraint->range();
    } else {
        return std::nullopt;
    }
}

template<typename ValueType>
std::optional<Range<int64_t>> ConstraintList<ValueType>::combineSizes() const
{
    if (m_constraints.empty()) {
        return std::nullopt;
    }

    return std::accumulate(std::next(m_constraints.begin()), m_constraints.end(), combineSize(m_constraints[0].get()),
            [&](const auto &range, const auto &constraint) {
                auto combinedRange = combineSize(constraint.get());

                if (!combinedRange) {
                    return range;
                } else if (!range) {
                    return combinedRange;
                } else {
                    return std::make_optional(range->merge(*combinedRange));
                }
            });
}

template<typename ValueType>
std::optional<Range<int64_t>> ConstraintList<ValueType>::combineSize(const Constraint<ValueType> *constraint) const
{
    if (!constraint) {
        return std::nullopt;
    }

    if (const auto *constraintList = dynamic_cast<const ConstraintList<ValueType> *>(constraint); constraintList) {
        return constraintList->combineSizes();
    } else if (const auto *andConstraint = dynamic_cast<const Constraints::AndConstraint<ValueType> *>(constraint);
               andConstraint) {
        const auto leftRange = combineSize(andConstraint->leftChild());
        const auto rightRange = combineSize(andConstraint->rightChild());

        if (!leftRange) {
            return rightRange;
        } else if (!rightRange) {
            return leftRange;
        } else {
            return leftRange->intersection(*rightRange);
        }
    } else if (const auto *orConstraint = dynamic_cast<const Constraints::OrConstraint<ValueType> *>(constraint);
               orConstraint) {
        const auto leftRange = combineSize(andConstraint->leftChild());
        const auto rightRange = combineSize(andConstraint->rightChild());

        if (!leftRange) {
            return rightRange;
        } else if (!rightRange) {
            return leftRange;
        } else {
            return leftRange->merge(*rightRange);
        }
    } else if (const auto *sizeConstraint = dynamic_cast<const Constraints::SizeConstraint<ValueType> *>(constraint);
               sizeConstraint) {
        return combineRange<IntegerValue>(sizeConstraint->innerConstraints());
    } else {
        return std::nullopt;
    }
}

template<typename ValueType>
std::unique_ptr<Constraint<ValueType>> ConstraintList<ValueType>::clone() const
{
    return std::make_unique<ConstraintList<ValueType>>(*this);
}

}
}
