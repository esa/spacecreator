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

#include "components/activities/iteration.h"

#include "components/activities/body.h"

namespace seds::model {

const Operand&
Iteration::NumericRange::startAt() const
{
    return m_startAt;
}

void
Iteration::NumericRange::setStartAt(Operand startAt)
{
    m_startAt = std::move(startAt);
}

const std::optional<Operand>&
Iteration::NumericRange::step() const
{
    return m_step;
}

void
Iteration::NumericRange::setStep(Operand step)
{
    m_step = std::move(step);
}

const Operand&
Iteration::NumericRange::endAt() const
{
    return m_endAt;
}

void
Iteration::NumericRange::setEndAt(Operand endAt)
{
    m_endAt = std::move(endAt);
}

const Iteration::Range&
Iteration::range() const
{
    return m_range;
}

void
Iteration::setRange(Iteration::Range range)
{
    m_range = std::move(range);
}

Body*
Iteration::doBody() const
{
    return m_do.get();
}

void
Iteration::setDoBody(std::unique_ptr<Body> doBody)
{
    m_do = std::move(doBody);
}

const VariableRef&
Iteration::iteratorVariableRef() const
{
    return m_iteratorVariableRef;
}

void
Iteration::setIteratorVariableRef(VariableRef iteratorVariableRef)
{
    m_iteratorVariableRef = std::move(iteratorVariableRef);
}

} // namespace seds::model
