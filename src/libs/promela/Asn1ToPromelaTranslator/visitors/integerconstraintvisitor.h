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

#pragma once

#include "integersubset.h"

#include <QtGlobal>
#include <algorithm>
#include <asn1library/asn1/constraints/constraintlist.h>
#include <asn1library/asn1/constraints/constraintvisitor.h>
#include <asn1library/asn1/constraints/fromconstraint.h>
#include <asn1library/asn1/constraints/logicoperators.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/values.h>
#include <optional>

namespace promela::translator {
/**
 * @brief Visitor for ASN.1 integer constraints
 *
 * This is a part of Asn1ToPromelaTranslator.
 *
 * Visitor finds a subset of allowed values for integer or for size constraints
 */
class IntegerConstraintVisitor : public ::Asn1Acn::Constraints::ConstraintVisitor<::Asn1Acn::IntegerValue>
{
public:
    /**
     * @brief Constructor.
     *
     * Initializes empty visitor
     */
    IntegerConstraintVisitor();

    /**
     * @brief Visit Asn1Acn::Constraints::RangeConstraint
     *
     * RangeConstraints contains actual sizes
     * Calculate subset of allowed values or sizes.
     *
     * @param constraint element to visit
     */
    void visit(const ::Asn1Acn::Constraints::RangeConstraint<::Asn1Acn::IntegerValue> &constraint) override;
    /// @brief Visit Asn1Acn::Constraints::AndConstraint
    void visit(const ::Asn1Acn::Constraints::AndConstraint<::Asn1Acn::IntegerValue> &constraint) override;
    /// @brief Visit Asn1Acn::Constraints::OrConstraint
    void visit(const ::Asn1Acn::Constraints::OrConstraint<::Asn1Acn::IntegerValue> &constraint) override;
    /// @brief Visit Asn1Acn::Constraints::FromConstraint
    void visit(const ::Asn1Acn::Constraints::FromConstraint<::Asn1Acn::IntegerValue> &constraint) override;
    /// @brief Visit Asn1Acn::Constraints::SizeConstraint
    void visit(const ::Asn1Acn::Constraints::SizeConstraint<::Asn1Acn::IntegerValue> &constraint) override;
    /// @brief Visit Asn1Acn::Constraints::ConstraintList
    void visit(const ::Asn1Acn::Constraints::ConstraintList<::Asn1Acn::IntegerValue> &constraint) override;

    /**
     * @brief Check if constraint was calculated.
     *
     * @return true if constraint was visited, otherwise false
     */
    bool isSizeConstraintVisited() const noexcept;

    /**
     * @brief Getter for found minimal value constraint
     *
     * @return minimal allowed value
     */
    size_t getMinValue() const noexcept;

    /**
     * @brief Getter for found maximal value constraint
     *
     * @return maximal allowed value
     */
    size_t getMaxValue() const noexcept;

    /**
     * @brief Getter for result IntegerSubset which describes allowed size values
     *
     * @return IntegerSubset with allowed size values
     */
    const std::optional<IntegerSubset> &getResultSubset() const;

private:
    std::optional<IntegerSubset> m_subset;
};
} // promela::translator
