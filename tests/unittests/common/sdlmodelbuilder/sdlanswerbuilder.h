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

#include <memory>
#include <sdl/SdlModel/answer.h>
#include <sdl/SdlModel/transition.h>
#include <sdl/SdlModel/variableliteral.h>

using sdl::Answer;
using sdl::Transition;
using sdl::VariableLiteral;

namespace tests::common {

class SdlAnswerBuilder final
{
public:
    SdlAnswerBuilder();

    auto build() -> std::unique_ptr<Answer>;

    auto withName(QString name) -> SdlAnswerBuilder &;

    auto withTransition(std::unique_ptr<Transition> transition) -> SdlAnswerBuilder &;

    auto withLiteral(VariableLiteral literal) -> SdlAnswerBuilder &;

private:
    std::unique_ptr<Answer> m_answer;
};

} // namespace tests::common
