/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "proctypemaker.h"

#include <promela/PromelaModel/assignment.h>
#include <promela/PromelaModel/constant.h>
#include <promela/PromelaModel/expression.h>
#include <promela/PromelaModel/variableref.h>

using promela::model::Assignment;
using promela::model::Constant;
using promela::model::Expression;
using promela::model::Sequence;
using promela::model::VariableRef;

namespace promela::translator {

std::unique_ptr<ProctypeElement> ProctypeMaker::makeInlineCall(
        const QString &inlineName, const QString &structureName, const QString &memberName)
{
    const QString inlineCallArgument = QString("%1.%2").arg(structureName).arg(memberName);
    QList<promela::model::InlineCall::Argument> args({ promela::model::InlineCall::Argument(inlineCallArgument) });

    auto inlineCall = promela::model::InlineCall(inlineName, args);
    return std::make_unique<ProctypeElement>(std::move(inlineCall));
}

std::unique_ptr<Sequence> ProctypeMaker::makeNormalSequence()
{
    return std::make_unique<Sequence>(Sequence::Type::NORMAL);
}

std::unique_ptr<ProctypeElement> ProctypeMaker::makeTrueExpressionProctypeElement()
{
    return std::make_unique<ProctypeElement>(Expression(VariableRef("true")));
}

std::unique_ptr<ProctypeElement> ProctypeMaker::makeAssignmentProctypeElement(
        const QString &variableName, const int32_t value)
{
    Assignment valueExistAssignment((VariableRef(variableName)), Expression(Constant(value)));

    return std::make_unique<ProctypeElement>(std::move(valueExistAssignment));
}

} // namespace promela::translator
