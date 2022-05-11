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

#include <memory>
#include <promela/PromelaModel/assignment.h>
#include <promela/PromelaModel/basictypes.h>
#include <promela/PromelaModel/constant.h>
#include <promela/PromelaModel/datatype.h>
#include <promela/PromelaModel/declaration.h>
#include <promela/PromelaModel/expression.h>
#include <promela/PromelaModel/forloop.h>
#include <promela/PromelaModel/inlinecall.h>

using promela::model::Assignment;
using promela::model::Constant;
using promela::model::Declaration;
using promela::model::Expression;
using promela::model::ForLoop;
using promela::model::InlineCall;
using promela::model::Sequence;

namespace promela::translator {

std::unique_ptr<ProctypeElement> ProctypeMaker::makeInlineCall(
        const QString &inlineName, const QString &argumentName, const QString &memberName)
{
    const QString inlineCallArgument =
            memberName.isEmpty() ? argumentName : QString("%1.%2").arg(argumentName).arg(memberName);
    const QList<InlineCall::Argument> args({ inlineCallArgument });

    auto inlineCall = InlineCall(inlineName, args);
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

std::unique_ptr<ProctypeElement> ProctypeMaker::makeVariableDeclaration(
        const model::BasicType &variableType, const QString &variableName)
{
    Declaration declaration(model::DataType(variableType), variableName);
    return std::make_unique<ProctypeElement>(std::move(declaration));
}

std::unique_ptr<ProctypeElement> ProctypeMaker::makeForLoop(
        const VariableRef &variable, const int first, const int last, std::unique_ptr<Sequence> sequence)
{
    ForLoop loop(variable, first, last, std::move(sequence));
    return std::make_unique<ProctypeElement>(std::move(loop));
}

} // namespace promela::translator
