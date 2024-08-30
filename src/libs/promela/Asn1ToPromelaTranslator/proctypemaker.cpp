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
#include <promela/PromelaModel/basictypes.h>
#include <promela/PromelaModel/booleanconstant.h>
#include <promela/PromelaModel/constant.h>
#include <promela/PromelaModel/datatype.h>
#include <promela/PromelaModel/declaration.h>
#include <promela/PromelaModel/forloop.h>
#include <promela/PromelaModel/inlinecall.h>

using promela::model::Assignment;
using promela::model::BooleanConstant;
using promela::model::Constant;
using promela::model::Declaration;
using promela::model::Expression;
using promela::model::ForLoop;
using promela::model::InlineCall;
using promela::model::ProctypeElement;
using promela::model::Sequence;
using promela::model::VariableRef;

namespace promela::translator {

std::unique_ptr<ProctypeElement> ProctypeMaker::makeInlineCall(
        const QString &inlineName, const VariableRef &argumentName, VariableRef memberName)
{
    VariableRef argument = argumentName;
    for (VariableRef::Element &element : memberName.takeElements()) {
        argument.appendElement(std::move(element));
    }

    QList<InlineCall::Argument> args;
    args.emplaceBack(std::move(argument));

    auto inlineCall = InlineCall(inlineName, args);
    return std::make_unique<ProctypeElement>(std::move(inlineCall));
}

std::unique_ptr<Sequence> ProctypeMaker::makeNormalSequence()
{
    return std::make_unique<Sequence>(Sequence::Type::NORMAL);
}

std::unique_ptr<ProctypeElement> ProctypeMaker::makeTrueExpressionProctypeElement()
{
    return std::make_unique<ProctypeElement>(Expression(BooleanConstant(true)));
}

std::unique_ptr<ProctypeElement> ProctypeMaker::makeAssignmentProctypeElement(
        const QString &variableName, const Expression::Value &value)
{
    Assignment valueExistAssignment((VariableRef(variableName)), Expression(value));
    return std::make_unique<ProctypeElement>(std::move(valueExistAssignment));
}

std::unique_ptr<ProctypeElement> ProctypeMaker::makeVariableDeclaration(
        const model::BasicType &variableType, const QString &variableName)
{
    Declaration declaration(model::DataType(variableType), variableName);
    return std::make_unique<ProctypeElement>(std::move(declaration));
}

std::unique_ptr<ProctypeElement> ProctypeMaker::makeForLoop(const VariableRef &variable, const Expression &first,
        const Expression &last, std::unique_ptr<Sequence> sequence)
{
    ForLoop loop(variable, first, last, std::move(sequence));
    return std::make_unique<ProctypeElement>(std::move(loop));
}

std::unique_ptr<ProctypeElement> ProctypeMaker::makeCallForEachValue(const QString &functionToCallName,
        const VariableRef &objectName, const Expression &iteratorEndValue, const QString &iteratorVariableName)
{
    auto innerSequence = ProctypeMaker::makeNormalSequence();
    VariableRef memberName = VariableRef("data", std::make_unique<Expression>(VariableRef(iteratorVariableName)));
    innerSequence->appendElement(ProctypeMaker::makeInlineCall(functionToCallName, objectName, std::move(memberName)));

    return makeForLoop(VariableRef(iteratorVariableName), Expression(0), iteratorEndValue, std::move(innerSequence));
}

std::unique_ptr<ProctypeElement> ProctypeMaker::makeCallForEachValue(const QString &functionToCallName,
        const VariableRef &objectName, const Expression &iteratorStartValue, const Expression &iteratorEndValue,
        const QString &iteratorVariableName)
{
    auto innerSequence = ProctypeMaker::makeNormalSequence();
    VariableRef memberName = VariableRef("data", std::make_unique<Expression>(VariableRef(iteratorVariableName)));
    innerSequence->appendElement(ProctypeMaker::makeInlineCall(functionToCallName, objectName, std::move(memberName)));

    return makeForLoop(
            VariableRef(iteratorVariableName), iteratorStartValue, iteratorEndValue, std::move(innerSequence));
}

} // namespace promela::translator
