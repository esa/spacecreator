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

namespace Asn1Acn::Constraints {

template<typename ValueType>
class RangeCombiner final
{
public:
    static std::optional<Range<typename ValueType::Type>> combineRanges(const ConstraintList<ValueType> *constraintList);
    static std::optional<Range<typename ValueType::Type>> combineRange(const Constraint<ValueType> *constraint);
};

template<typename ValueType>
std::optional<Range<typename ValueType::Type>> RangeCombiner<ValueType>::combineRanges(const ConstraintList<ValueType> *constraintList)
{
    const auto &constraints = constraintList->constraints();

    if (constraints.empty()) {
        return std::nullopt;
    }

    return std::accumulate(std::next(constraints.begin()), constraints.end(),
            combineRange(constraints[0].get()), [&](const auto &range, const auto &constraint) {
                auto combinedRange = combineRange(constraint.get());

                if (!combinedRange) {
                    return range;
                } else if (!range) {
                    return combinedRange;
                } else {
                    return std::make_optional(range->intersection(*combinedRange));
                }
            });
}

template<typename ValueType>
std::optional<Range<typename ValueType::Type>> RangeCombiner<ValueType>::combineRange(const Constraint<ValueType> *constraint)
{
    if (!constraint) {
        return std::nullopt;
    }

    if (const auto *constraintList = dynamic_cast<const ConstraintList<ValueType> *>(constraint); constraintList) {
        return combineRanges(constraintList);
    } else if (const auto *andConstraint = dynamic_cast<const Constraints::AndConstraint<ValueType> *>(constraint);
               andConstraint) {
        const auto leftRange = combineRange(andConstraint->leftChild());
        const auto rightRange = combineRange(andConstraint->rightChild());

        if (!leftRange) {
            return rightRange;
        } else if (!rightRange) {
            return leftRange;
        } else {
            return leftRange->intersection(*rightRange);
        }
    } else if (const auto *orConstraint = dynamic_cast<const Constraints::OrConstraint<ValueType> *>(constraint);
               orConstraint) {
        const auto leftRange = combineRange(andConstraint->leftChild());
        const auto rightRange = combineRange(andConstraint->rightChild());

        if (!leftRange) {
            return rightRange;
        } else if (!rightRange) {
            return leftRange;
        } else {
            return leftRange->merge(*rightRange);
        }
    } else if (const auto *rangeConstraint = dynamic_cast<const Constraints::RangeConstraint<ValueType> *>(constraint);
               rangeConstraint) {
        return rangeConstraint->range();
    } else {
        return std::nullopt;
    }
}

} // namespace Asn1Acn::Constraints
