/****************************************************************************
**
** Copyright (C) 2022 N7 Space sp. z o. o.
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

#include "values.h"

namespace Asn1Acn::Constraints {

template<typename ValueType>
class SizeCombiner final
{
public:
    static std::optional<Range<int64_t>> combineSizes(const ConstraintList<ValueType> *constraintList);
    static std::optional<Range<int64_t>> combineSize(const Constraint<ValueType> *constraint);
};

template<typename ValueType>
std::optional<Range<int64_t>> SizeCombiner<ValueType>::combineSizes(const ConstraintList<ValueType> *constraintList)
{
    const auto &constraints = constraintList->constraints();

    if (constraints.empty()) {
        return std::nullopt;
    }

    return std::accumulate(std::next(constraints.begin()), constraints.end(),
            combineSize(constraints[0].get()), [&](const auto &range, const auto &constraint) {
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
std::optional<Range<int64_t>> SizeCombiner<ValueType>::combineSize(const Constraint<ValueType> *constraint)
{
    if (!constraint) {
        return std::nullopt;
    }

    if (const auto *constraintList = dynamic_cast<const ConstraintList<ValueType> *>(constraint); constraintList) {
        return combineSizes(constraintList);
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
        return RangeCombiner<IntegerValue>::combineRange(sizeConstraint->innerConstraints());
    } else {
        return std::nullopt;
    }

    return std::nullopt;
}

} // namespace Asn1Acn::Constraints
