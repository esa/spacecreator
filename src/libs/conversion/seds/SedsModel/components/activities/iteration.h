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

#include "components/activities/operand.h"
#include "components/activities/statement.h"
#include "components/variableref.h"

#include <memory>

namespace seds::model {

class Body;

class Iteration final : public Statement
{
public:
    class NumericRange
    {
    public:
        auto startAt() const -> const Operand &;
        auto setStartAt(Operand startAt) -> void;

        auto step() const -> const std::optional<Operand> &;
        auto setStep(Operand step) -> void;

        auto endAt() const -> const Operand &;
        auto setEndAt(Operand endAt) -> void;

    private:
        Operand m_startAt;
        std::optional<Operand> m_step;
        Operand m_endAt;
    };

    using Range = std::variant<VariableRef, NumericRange>;

public:
    Iteration() = default;
    Iteration(Iteration &&) = default;
    Iteration &operator=(Iteration &&) = default;

public:
    auto range() const -> const Range &;
    auto setRange(Range range) -> void;

    auto doBody() const -> Body *;
    auto setDoBody(std::unique_ptr<Body> doBody) -> void;

    auto iteratorVariableRef() const -> const VariableRef &;
    auto setIteratorVariableRef(VariableRef iteratorVariableRef) -> void;

private:
    Range m_range;
    std::unique_ptr<Body> m_do;
    VariableRef m_iteratorVariableRef;
};

} // namespace seds::model
