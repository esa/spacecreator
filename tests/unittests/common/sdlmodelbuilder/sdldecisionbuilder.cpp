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

#include "sdldecisionbuilder.h"

#include <memory>

namespace tests::common {

SdlDecisionBuilder::SdlDecisionBuilder()
    : m_decision(std::make_unique<Decision>())
{
}

std::unique_ptr<Decision> SdlDecisionBuilder::build()
{
    return std::move(m_decision);
}

SdlDecisionBuilder &SdlDecisionBuilder::withName(QString name)
{
    m_decision->setName(std::move(name));

    return *this;
}

SdlDecisionBuilder &SdlDecisionBuilder::withExpression(std::unique_ptr<Expression> expression)
{
    m_decision->setExpression(std::move(expression));

    return *this;
}

SdlDecisionBuilder &SdlDecisionBuilder::withAnswer(std::unique_ptr<Answer> answer)
{
    m_decision->addAnswer(std::move(answer));

    return *this;
}

} // namespace tests::common
