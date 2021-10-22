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

#include "sdlanswerbuilder.h"

#include <memory>

namespace tests::common {

SdlAnswerBuilder::SdlAnswerBuilder()
    : m_answer(std::make_unique<Answer>())
{
}

std::unique_ptr<Answer> SdlAnswerBuilder::build()
{
    return std::move(m_answer);
}

SdlAnswerBuilder &SdlAnswerBuilder::withName(QString name)
{
    m_answer->setName(std::move(name));

    return *this;
}

SdlAnswerBuilder &SdlAnswerBuilder::withTransition(std::unique_ptr<Transition> transition)
{
    m_answer->setTransition(std::move(transition));

    return *this;
}

SdlAnswerBuilder &SdlAnswerBuilder::withLiteral(VariableLiteral *literal)
{
    m_answer->setLiteral(literal);

    return *this;
}

} // namespace tests::common
