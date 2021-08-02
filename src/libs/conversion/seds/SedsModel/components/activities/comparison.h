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

#include <variant>

#include "components/activities/basecondition.h"
#include "components/activities/comparisonoperator.h"
#include "components/activities/valueoperand.h"
#include "components/activities/variablerefoperand.h"

namespace seds::model {

class Comparison final : public BaseCondition
{
  public:
    using SecondOperand = std::variant<VariableRefOperand, ValueOperand>;

  public:
    Comparison() = default;
    Comparison(Comparison&&) = default;
    Comparison& operator=(Comparison&&) = default;

  public:
    auto firstOperand() const -> const VariableRefOperand&;
    auto setFirstOperand(VariableRefOperand firstOperand) -> void;

    auto comparisonOperator() const -> const ComparisonOperator&;
    auto setComparisonOperator(const ComparisonOperator& comparisonOperator) -> void;

    auto secondOperand() const -> const SecondOperand&;
    auto setSecondOperand(SecondOperand secondOperand) -> void;

  private:
    VariableRefOperand m_firstOperand;
    ComparisonOperator m_comparisonOperator;
    SecondOperand m_secondOperand;
};

} // namespace seds::model
