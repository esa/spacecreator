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

#include "sedsactivitybuilder.h"

#include <assert.h>
#include <seds/SedsModel/components/activities/calibration.h>
#include <seds/SedsModel/components/activities/variablerefoperand.h>
#include <seds/SedsModel/sedsmodel.h>

using namespace seds::model;

namespace tests::conversion::common {

SedsActivityBuilder::SedsActivityBuilder(QString name)
{
    m_activity.setName(name);
    m_activity.setBody(std::make_unique<Body>());
}

auto SedsActivityBuilder::build() -> seds::model::Activity
{
    return std::move(m_activity);
}

auto SedsActivityBuilder::withValueAssignment(const QString target, const QString value) -> SedsActivityBuilder &
{

    ValueOperand operand;
    operand.setValue(LiteralValue(value));
    Assignment assignment;
    assignment.setElement(std::move(operand));
    assignment.setOutputVariableRef(VariableRef(target));
    m_activity.body()->addStatement(std::move(assignment));

    return *this;
}

auto SedsActivityBuilder::withMathOperation(
        const QString target, const seds::model::CoreMathOperator op, const QString argument) -> SedsActivityBuilder &
{

    seds::model::MathOperation operation;
    seds::model::Operator opElement;
    opElement.setMathOperator(op);
    operation.addElement(std::move(opElement));
    operation.addElement(seds::model::VariableRef(argument));
    operation.setOutputVariableRef(VariableRef(target));

    m_activity.body()->addStatement(std::move(operation));

    return *this;
}

auto SedsActivityBuilder::withPolynomialCalibration(const QString target, const QString source,
        const std::vector<double> coefficients, const std::vector<uint64_t> exponents) -> SedsActivityBuilder &
{
    seds::model::Calibration calibration;
    seds::model::Polynomial polynomial;

    assert(coefficients.size() == exponents.size());
    for (std::size_t i = 0; i < coefficients.size(); i++) {
        seds::model::PolynomialTerm term;
        term.setCoefficient(coefficients[i]);
        term.setExponent(exponents[i]);
        polynomial.addTerm(std::move(term));
    }

    calibration.setCalibrator(std::move(polynomial));
    calibration.setOutputVariableRef(VariableRef(target));
    calibration.setInputVariableRef(VariableRef(source));

    m_activity.body()->addStatement(std::move(calibration));

    return *this;
}

auto SedsActivityBuilder::withActivityCall(const QString target, const std::vector<QString> argumentNames,
        const std::vector<QString> argumentValues) -> SedsActivityBuilder &
{
    seds::model::ActivityInvocation invocation;

    invocation.setActivity(target);
    assert(argumentNames.size() == argumentValues.size());
    for (size_t i = 0; i < argumentNames.size(); i++) {
        seds::model::NamedArgumentValue namedValue;

        // Let's treat integers as literal values
        bool isInt;
        argumentValues[i].toInt(&isInt, 10);
        if (isInt) {
            seds::model::ValueOperand val;
            val.setValue(argumentValues[i]);
            namedValue.setValue(std::move(val));
        } else {
            seds::model::VariableRefOperand ref;
            ref.setVariableRef(VariableRef(argumentValues[i]));
            namedValue.setValue(std::move(ref));
        }

        namedValue.setName(argumentNames[i]);

        invocation.addArgumentValue(std::move(namedValue));
    }

    m_activity.body()->addStatement(std::move(invocation));

    return *this;
}

auto SedsActivityBuilder::withSendCommand(const QString interfaceName, const QString commandName,
        const std::vector<QString> argumentNames, const std::vector<QString> argumentValues) -> SedsActivityBuilder &
{
    seds::model::SendCommandPrimitive send;

    send.setInterface(interfaceName);
    send.setCommand(commandName);
    assert(argumentNames.size() == argumentValues.size());
    for (size_t i = 0; i < argumentNames.size(); i++) {
        seds::model::VariableRefOperand ref;
        seds::model::NamedArgumentValue namedValue;
        ref.setVariableRef(VariableRef(argumentValues[i]));
        namedValue.setValue(std::move(ref));
        namedValue.setName(argumentNames[i]);

        send.addArgumentValue(std::move(namedValue));
    }

    m_activity.body()->addStatement(std::move(send));

    return *this;
}

auto SedsActivityBuilder::withArgument(const QString name, const QString typeName) -> SedsActivityBuilder &
{
    ActivityArgument argument;
    argument.setName(name);
    argument.setType(DataTypeRef(typeName));
    m_activity.addArgument(std::move(argument));

    return *this;
}

}
