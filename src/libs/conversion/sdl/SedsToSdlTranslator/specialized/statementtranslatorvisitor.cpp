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

#include "statementtranslatorvisitor.h"

#include "components/activities/coremathoperator.h"
#include "mathoperationtranslator.h"
#include "statemachinetranslator.h"
#include "translation/exceptions.h"

#include <algorithm>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <conversion/iv/SedsToIvTranslator/specialized/interfacecommandtranslator.h>
#include <ivcore/ivfunction.h>

using conversion::Escaper;
using conversion::iv::translator::InterfaceCommandTranslator;
using conversion::translator::TranslationException;
using seds::model::CoreMathOperator;
using seds::model::Polynomial;
using seds::model::SplineCalibrator;

namespace conversion::sdl::translator {

StatementTranslatorVisitor::Context::Context(const seds::model::Package &sedsPackage, Asn1Acn::Asn1Model *asn1Model,
        ivm::IVModel *ivModel, ::sdl::Process *sdlProcess, ::sdl::Procedure *sdlProcedure)
    : m_sedsPackage(sedsPackage)
    , m_asn1Model(asn1Model)
    , m_ivModel(ivModel)
    , m_sdlProcess(sdlProcess)
    , m_sdlProcedure(sdlProcedure)
{
    m_labelCount = 0;
}

auto StatementTranslatorVisitor::Context::uniqueLabelName(const QString prefix) -> QString
{
    m_labelCount++;
    return prefix + QString::number(m_labelCount);
}

auto StatementTranslatorVisitor::Context::sedsPackage() -> const seds::model::Package &
{
    return m_sedsPackage;
}

auto StatementTranslatorVisitor::Context::asn1Model() -> Asn1Acn::Asn1Model *
{
    return m_asn1Model;
}

auto StatementTranslatorVisitor::Context::ivModel() -> ivm::IVModel *
{
    return m_ivModel;
}

auto StatementTranslatorVisitor::Context::sdlProcess() -> ::sdl::Process *
{
    return m_sdlProcess;
}
auto StatementTranslatorVisitor::Context::sdlProcedure() -> ::sdl::Procedure *
{
    return m_sdlProcedure;
}

StatementTranslatorVisitor::StatementTranslatorVisitor(Context &context, ::sdl::Transition *sdlTransition)
    : m_context(context)
    , m_sdlTransition(sdlTransition)
{
}

auto StatementTranslatorVisitor::operator()(const seds::model::ActivityInvocation &invocation) -> void
{
    auto task = translateActivityCall(m_context.sdlProcess(), invocation);
    m_sdlTransition->addAction(std::move(task));
}

auto StatementTranslatorVisitor::operator()(const seds::model::Assignment &assignment) -> void
{
    const auto targetName = Escaper::escapeAsn1FieldName(assignment.outputVariableRef().value().value());
    const auto &element = assignment.element();
    if (std::holds_alternative<seds::model::VariableRef>(element)) {
        const auto action =
                QString("%1 := %2").arg(targetName, std::get<seds::model::VariableRef>(element).value().value());
        m_sdlTransition->addAction(std::make_unique<::sdl::Task>("", action));

    } else if (std::holds_alternative<seds::model::ValueOperand>(element)) {
        const auto action =
                QString("%1 := %2").arg(targetName, std::get<seds::model::ValueOperand>(element).value().value());
        m_sdlTransition->addAction(std::make_unique<::sdl::Task>("", action));
    } else {
        throw TranslationException("Assignment not implemented");
    }
}

auto StatementTranslatorVisitor::operator()(const seds::model::Calibration &calibration) -> void
{
    const auto targetName = Escaper::escapeAsn1FieldName(calibration.outputVariableRef().value().value());
    const auto sourceName = Escaper::escapeAsn1FieldName(calibration.inputVariableRef().value().value());
    const auto &calibrator = calibration.calibrator();
    if (std::holds_alternative<Polynomial>(calibrator)) {
        const auto &polynomial = std::get<Polynomial>(calibrator);
        const auto action = QString("%1 := %2").arg(targetName, translatePolynomial(sourceName, polynomial));
        m_sdlTransition->addAction(std::make_unique<::sdl::Task>("", action));
    } else {
        // TODO Spline calibrator - postponed, as it requires generation of a custom procedure,
        // and possibly a custom type
        throw TranslationException("Calibration activity not implemented");
    }
}

auto StatementTranslatorVisitor::operator()(const seds::model::Conditional &conditional) -> void
{
    auto decision =
            translateBooleanExpression(m_context.sdlProcess(), m_context.sdlProcedure(), conditional.condition());
    auto label = std::make_unique<::sdl::Label>(Escaper::escapeSdlName("changema"));

    auto trueAnswer = translateAnswer(m_context, label.get(), "True", conditional.onConditionTrue());
    auto falseAnswer = translateAnswer(m_context, label.get(), "False", conditional.onConditionFalse());
    decision->addAnswer(std::move(trueAnswer));
    decision->addAnswer(std::move(falseAnswer));

    m_sdlTransition->addAction(std::move(decision));
    m_sdlTransition->addAction(std::move(label));
}

auto StatementTranslatorVisitor::operator()(const seds::model::Iteration &iteration) -> void
{
    auto startLabel = std::make_unique<::sdl::Label>("loop_start");
    auto endLabel = std::make_unique<::sdl::Label>("loop_end");
    auto loopBackJoin = std::make_unique<::sdl::Join>();
    loopBackJoin->setLabel(startLabel.get());
    const auto variable = findVariableDeclaration(m_context.sdlProcess(), m_context.sdlProcedure(),
            Escaper::escapeAsn1FieldName(iteration.iteratorVariableRef().value().value()));

    auto decision = std::make_unique<::sdl::Decision>();
    auto exitLoop = std::make_unique<::sdl::Answer>();
    auto continueLoop = std::make_unique<::sdl::Answer>();
    auto exitTransition = std::make_unique<::sdl::Transition>();
    auto continueTransition = std::make_unique<::sdl::Transition>();
    auto exitJoin = std::make_unique<::sdl::Join>();
    exitJoin->setLabel(endLabel.get());

    if (std::holds_alternative<seds::model::Iteration::NumericRange>(iteration.range())) {
        const auto &range = std::get<seds::model::Iteration::NumericRange>(iteration.range());
        const auto startValue = getOperandValue(m_context.sdlProcess(), m_context.sdlProcedure(), range.startAt());
        const auto endValue = getOperandValue(m_context.sdlProcess(), m_context.sdlProcedure(), range.endAt());
        m_sdlTransition->addAction(
                std::make_unique<::sdl::Task>("", QString("%1 := %2").arg(variable->name(), startValue)));

        m_sdlTransition->addAction(std::move(startLabel));
        decision->setExpression(
                std::make_unique<::sdl::Expression>(QString("%1 <= %2").arg(variable->name(), endValue)));
    } else {
        throw TranslationException("Variable range not implemented");
    }

    exitTransition->addAction(std::move(exitJoin));
    exitLoop->setLiteral(::sdl::VariableLiteral("False"));
    exitLoop->setTransition(std::move(exitTransition));

    continueLoop->setLiteral(::sdl::VariableLiteral("True"));
    continueLoop->setTransition(std::move(continueTransition));
    // This needs to be saved so that actual body actions can be added
    auto transitionPointer = continueLoop->transition();

    decision->addAnswer(std::move(exitLoop));
    decision->addAnswer(std::move(continueLoop));
    m_sdlTransition->addAction(std::move(decision));
    m_sdlTransition->addAction(std::move(endLabel));

    if (iteration.doBody() != nullptr) {
        StatementTranslatorVisitor nestedVisitor(m_context, transitionPointer);
        for (const auto &statement : iteration.doBody()->statements()) {
            std::visit(nestedVisitor, statement);
        }
    }

    if (std::holds_alternative<seds::model::Iteration::NumericRange>(iteration.range())) {
        const auto &range = std::get<seds::model::Iteration::NumericRange>(iteration.range());
        const auto stepValue = range.step().has_value()
                ? getOperandValue(m_context.sdlProcess(), m_context.sdlProcedure(), *range.step())
                : "1";
        transitionPointer->addAction(
                std::make_unique<::sdl::Task>("", QString("%1 := %1 + %2").arg(variable->name(), stepValue)));
    } else {
        throw TranslationException("Variable range not implemented");
    }
    transitionPointer->addAction(std::move(loopBackJoin));
}

auto StatementTranslatorVisitor::operator()(const seds::model::MathOperation &operation) -> void
{
    if (operation.elements().size() > 0 && std::holds_alternative<seds::model::Operator>(operation.elements()[0])) {
        // Check for a special case of the swap operator
        const auto &op = std::get<seds::model::Operator>(operation.elements()[0]);
        if (std::get<CoreMathOperator>(op.mathOperator()) == CoreMathOperator::Swap) {
            // TODO This needs proper knowledge of types
            throw TranslationException("Swap operator is not implemented");
        }
    }
    const auto targetName = Escaper::escapeAsn1FieldName(operation.outputVariableRef().value().value());
    const auto value = MathOperationTranslator::translateOperation(operation.elements());
    const auto action = QString("%1 := %2").arg(targetName, value);
    m_sdlTransition->addAction(std::make_unique<::sdl::Task>("", action));
}

auto StatementTranslatorVisitor::operator()(const seds::model::SendCommandPrimitive &sendCommand) -> void
{
    const auto commandName = sendCommand.command().value();
    const auto interfaceName = sendCommand.interface().value();

    const auto callName = InterfaceCommandTranslator::getCommandName(
            interfaceName, ivm::IVInterface::InterfaceType::Required, commandName);

    // Process name carries iv-escaped component name
    const auto interface = findInterfaceDeclaration(m_context.ivModel(), m_context.sdlProcess()->name(), callName);

    if (interface->kind() == ivm::IVInterface::OperationKind::Sporadic) {
        auto outputActions = translateOutput(m_context.sdlProcess(), m_context.sdlProcedure(), callName, sendCommand);
        for (auto &action : outputActions) {
            m_sdlTransition->addAction(std::move(action));
        }
    } else if (interface->kind() == ivm::IVInterface::OperationKind::Protected
            || interface->kind() == ivm::IVInterface::OperationKind::Unprotected) {
        auto call = translateCall(m_context.sdlProcess(), m_context.sdlProcedure(), callName, sendCommand);
        m_sdlTransition->addAction(std::move(call));
    }
}

auto StatementTranslatorVisitor::operator()(const seds::model::SendParameterPrimitive &sendPrimitive) -> void
{
    Q_UNUSED(sendPrimitive);
    throw TranslationException("SendPrimitive activity not implemented");
}

auto StatementTranslatorVisitor::translateActivityCall(::sdl::Process *process,
        const seds::model::ActivityInvocation &invocation) -> std::unique_ptr<::sdl::ProcedureCall>
{
    auto call = std::make_unique<::sdl::ProcedureCall>();
    const auto procedureName = Escaper::escapeSdlName(invocation.activity().value());
    const auto &procedure = std::find_if(process->procedures().begin(), process->procedures().end(),
            [procedureName](const auto &p) { return p->name() == procedureName; });

    if (procedure == process->procedures().end()) {
        throw TranslationException(QString("Procedure %1 not found").arg(procedureName));
    }
    call->setProcedure(procedure->get());

    for (const auto &argument : invocation.argumentValues()) {
        call->addArgument(std::move(translateArgument(process, nullptr, argument)));
    }
    return call;
}

auto StatementTranslatorVisitor::findInterfaceDeclaration(
        ivm::IVModel *model, const QString functionName, const QString interfaceName) -> ivm::IVInterface *
{
    const auto &function = model->getFunction(functionName, Qt::CaseSensitive);
    if (function == nullptr) {
        throw TranslationException(QString("Function %1 not found").arg(functionName));
    }

    // This form is shorter and easier to debug than find_if and for unknown reason works more reliably
    for (const auto &interface : function->interfaces()) {
        if (interface->ifaceLabel() == interfaceName) {
            return interface;
        }
    }

    throw TranslationException(QString("Interface %1 not found for function %2").arg(interfaceName, functionName));
    return nullptr;
}

auto StatementTranslatorVisitor::findVariableDeclaration(
        ::sdl::Process *process, ::sdl::Procedure *sdlProcedure, QString name) -> ::sdl::VariableDeclaration *
{
    const auto processVariableDeclaration = std::find_if(process->variables().begin(), process->variables().end(),
            [name](const auto &variable) { return variable->name() == name; });
    if (processVariableDeclaration != process->variables().end()) {
        return (*processVariableDeclaration).get();
    }
    if (sdlProcedure != nullptr) {
        const auto procedureVariableDeclaration = std::find_if(sdlProcedure->parameters().begin(),
                sdlProcedure->parameters().end(), [name](const auto &variable) { return variable->name() == name; });
        if (procedureVariableDeclaration != sdlProcedure->parameters().end()) {
            return (*procedureVariableDeclaration).get();
        }
    }

    throw TranslationException(QString("Variable %1 not found").arg(name));
    return nullptr;
}

auto StatementTranslatorVisitor::translateArgument(::sdl::Process *process, ::sdl::Procedure *sdlProcedure,
        const seds::model::NamedArgumentValue &argument) -> ::sdl::ProcedureCall::Argument
{
    if (std::holds_alternative<seds::model::ValueOperand>(argument.value())) {
        const auto &value = std::get<seds::model::ValueOperand>(argument.value());
        return std::make_unique<::sdl::VariableLiteral>(value.value().value());
    } else if (std::holds_alternative<seds::model::VariableRefOperand>(argument.value())) {
        const auto &value = std::get<seds::model::VariableRefOperand>(argument.value());
        const auto variableName = Escaper::escapeAsn1FieldName(value.variableRef().value().value());
        const auto variableDeclaration = findVariableDeclaration(process, sdlProcedure, variableName);
        return std::make_unique<::sdl::VariableReference>(variableDeclaration);
    } else {
        throw TranslationException("Argument not implemented");
        return std::unique_ptr<::sdl::VariableReference>();
    }
}

auto StatementTranslatorVisitor::translatePolynomial(const QString variable, const seds::model::Polynomial &polynomial)
        -> QString
{
    return std::accumulate(polynomial.terms().begin(), polynomial.terms().end(), QString(""),
            [variable](const auto &accumulator, const auto &element) {
                QString expression;
                switch (element.exponent()) {
                case 0:
                    expression.append(QString("%1").arg(element.coefficient()));
                    break;
                case 1:
                    expression.append(QString("%1 * %2").arg(QString::number(element.coefficient()), variable));
                    break;
                case 2:
                    expression.append(QString("%1 * %2 * %2").arg(QString::number(element.coefficient()), variable));
                    break;
                case 3:
                    expression.append(
                            QString("%1 * %2 * %2 * %2").arg(QString::number(element.coefficient()), variable));
                    break;
                default:
                    // From the 4th power and up, there are more efficient ways than naive multiplication
                    expression.append(QString("%1 * power(%2, %3)")
                                              .arg(QString::number(element.coefficient()), variable,
                                                      QString::number(element.exponent())));
                }
                return accumulator.size() == 0 ? expression : QString(accumulator + " + " + expression);
            });
}

auto StatementTranslatorVisitor::translateCall(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
        const QString callName, const seds::model::SendCommandPrimitive &sendCommand)
        -> std::unique_ptr<::sdl::ProcedureCall>
{
    auto call = std::make_unique<::sdl::ProcedureCall>();

    const auto &procedure = std::find_if(hostProcess->procedures().begin(), hostProcess->procedures().end(),
            [callName](const auto &p) { return p->name() == callName; });

    if (procedure == hostProcess->procedures().end()) {
        throw TranslationException(QString("Procedure %1 not found").arg(callName));
    }
    call->setProcedure(procedure->get());

    for (const auto &argument : sendCommand.argumentValues()) {
        call->addArgument(std::move(translateArgument(hostProcess, hostProcedure, argument)));
    }
    return std::move(call);
}

auto StatementTranslatorVisitor::translateOutput(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
        const QString callName, const seds::model::SendCommandPrimitive &sendCommand)
        -> std::vector<std::unique_ptr<::sdl::Action>>
{
    std::vector<std::unique_ptr<::sdl::Action>> result;

    auto output = std::make_unique<::sdl::Output>();
    output->setName(callName);
    if (sendCommand.argumentValues().size() > 0) {
        const auto ioVariable = StateMachineTranslator::ioVariableName(callName);
        output->setParameter(std::make_unique<::sdl::VariableReference>(
                findVariableDeclaration(hostProcess, hostProcedure, ioVariable)));
        for (const auto &argument : sendCommand.argumentValues()) {
            const auto fieldName = Escaper::escapeAsn1FieldName(argument.name().value());
            const auto source = std::visit(
                    overloaded { [](const seds::model::ValueOperand &operand) { return operand.value().value(); },
                            [](const seds::model::VariableRefOperand &operand) {
                                return Escaper::escapeAsn1FieldName(operand.variableRef().value().value());
                            } },
                    argument.value());
            auto assignment =
                    std::make_unique<::sdl::Task>("", QString("%1.%2 := %3").arg(ioVariable, fieldName, source));
            result.emplace_back(std::move(assignment));
        }
    }

    result.emplace_back(std::move(output));

    return std::move(result);
}

auto StatementTranslatorVisitor::translateBooleanExpression(
        ::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure, const seds::model::BooleanExpression &expression)
        -> std::unique_ptr<::sdl::Decision>
{
    const auto &condition = expression.condition();
    if (std::holds_alternative<seds::model::Comparison>(condition)) {
        const auto &comparision = std::get<seds::model::Comparison>(condition);
        return translateComparison(hostProcess, hostProcedure, comparision);
    } else if (std::holds_alternative<std::unique_ptr<seds::model::AndedConditions>>(condition)) {
        const auto &conditions = std::get<std::unique_ptr<seds::model::AndedConditions>>(condition);
        return translateAndedConditions(hostProcess, hostProcedure, *conditions);
    } else if (std::holds_alternative<std::unique_ptr<seds::model::OredConditions>>(condition)) {
        const auto &conditions = std::get<std::unique_ptr<seds::model::OredConditions>>(condition);
        return translateOredConditions(hostProcess, hostProcedure, *conditions);
    } else if (std::holds_alternative<seds::model::TypeCheck>(condition)) {
        const auto &check = std::get<seds::model::TypeCheck>(condition);
        return translateTypeCheck(hostProcess, hostProcedure, check);
    }
    throw TranslationException("Expression not implemented");
    return nullptr;
}

auto StatementTranslatorVisitor::translateComparison(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
        const seds::model::Comparison &comparison) -> std::unique_ptr<::sdl::Decision>
{
    Q_UNUSED(hostProcess);
    Q_UNUSED(hostProcedure);
    const auto left = comparison.firstOperand().variableRef().value().value();
    const auto &right = std::visit(
            overloaded {
                    [](const seds::model::VariableRefOperand &reference) {
                        return Escaper::escapeAsn1FieldName(reference.variableRef().value().value());
                    },
                    [](const seds::model::ValueOperand &value) { return value.value().value(); },
            },
            comparison.secondOperand());
    const auto op = comparisonOperatorToString(comparison.comparisonOperator());
    auto decision = std::make_unique<::sdl::Decision>();
    auto expression = std::make_unique<::sdl::Expression>(QString("%1 %2 %3").arg(left, op, right));
    decision->setExpression(std::move(expression));
    return decision;
}

auto StatementTranslatorVisitor::translateAndedConditions(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
        const seds::model::AndedConditions &conditions) -> std::unique_ptr<::sdl::Decision>
{
    Q_UNUSED(hostProcess);
    Q_UNUSED(hostProcedure);
    Q_UNUSED(conditions);
    throw TranslationException("Expression not implemented");
    return nullptr;
}

auto StatementTranslatorVisitor::translateOredConditions(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
        const seds::model::OredConditions &conditions) -> std::unique_ptr<::sdl::Decision>
{
    Q_UNUSED(hostProcess);
    Q_UNUSED(hostProcedure);
    Q_UNUSED(conditions);
    throw TranslationException("Expression not implemented");
    return nullptr;
}

auto StatementTranslatorVisitor::translateTypeCheck(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
        const seds::model::TypeCheck &check) -> std::unique_ptr<::sdl::Decision>
{
    Q_UNUSED(hostProcess);
    Q_UNUSED(hostProcedure);
    Q_UNUSED(check);
    throw TranslationException("Expression not implemented");
    return nullptr;
}

auto StatementTranslatorVisitor::comparisonOperatorToString(const seds::model::ComparisonOperator op) -> QString
{
    switch (op) {
    case seds::model::ComparisonOperator::Equals:
        return "=";
    case seds::model::ComparisonOperator::NotEquals:
        return "<>";
    case seds::model::ComparisonOperator::LessThan:
        return "<";
    case seds::model::ComparisonOperator::LessThanEquals:
        return "<=";
    case seds::model::ComparisonOperator::GreaterThan:
        return ">";
    case seds::model::ComparisonOperator::GreaterThanEquals:
        return ">=";
    }
    throw TranslationException("Expression not implemented");
    return "";
}

auto StatementTranslatorVisitor::translateAnswer(Context &context, ::sdl::Label *joinLabel, const QString value,
        const seds::model::Body *body) -> std::unique_ptr<::sdl::Answer>
{
    auto answer = std::make_unique<::sdl::Answer>();
    answer->setLiteral(::sdl::VariableLiteral(value));
    auto transition = std::make_unique<::sdl::Transition>();
    if (body != nullptr) {
        StatementTranslatorVisitor nestedVisitor(context, transition.get());
        for (const auto &statement : body->statements()) {
            std::visit(nestedVisitor, statement);
        }
    }
    auto join = std::make_unique<::sdl::Join>();
    join->setLabel(joinLabel);
    transition->addAction(std::move(join));
    answer->setTransition(std::move(transition));

    return answer;
}

auto StatementTranslatorVisitor::getOperandValue(
        ::sdl::Process *process, ::sdl::Procedure *sdlProcedure, const seds::model::Operand &operand) -> QString
{
    if (std::holds_alternative<seds::model::ValueOperand>(operand.value())) {
        const auto &value = std::get<seds::model::ValueOperand>(operand.value());
        return value.value().value();
    } else if (std::holds_alternative<seds::model::VariableRefOperand>(operand.value())) {
        const auto &value = std::get<seds::model::VariableRefOperand>(operand.value());
        const auto variableName = Escaper::escapeAsn1FieldName(value.variableRef().value().value());
        const auto variableDeclaration = findVariableDeclaration(process, sdlProcedure, variableName);
        return variableDeclaration->name();
    }
    throw TranslationException("Operand not implemented");
    return "";
}

}
