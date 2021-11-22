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

StatementTranslatorVisitor::StatementTranslatorVisitor(const seds::model::Package &sedsPackage,
        Asn1Acn::Asn1Model *asn1Model, ivm::IVModel *ivModel, ::sdl::Process *sdlProcess,
        ::sdl::Procedure *sdlProcedure, ::sdl::Transition *sdlTransition)
    : m_sedsPackage(sedsPackage)
    , m_asn1Model(asn1Model)
    , m_ivModel(ivModel)
    , m_sdlProcess(sdlProcess)
    , m_sdlProcedure(sdlProcedure)
    , m_sdlTransition(sdlTransition)
{
}

auto StatementTranslatorVisitor::operator()(const seds::model::ActivityInvocation &invocation) -> void
{
    auto task = translateActivityCall(m_sdlProcess, invocation);
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
    Q_UNUSED(conditional);
    throw TranslationException("Conditional activity not implemented");
}

auto StatementTranslatorVisitor::operator()(const seds::model::Iteration &iteration) -> void
{
    Q_UNUSED(iteration);
    throw TranslationException("Iteration activity not implemented");
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
    const auto interface = findInterfaceDeclaration(m_ivModel, m_sdlProcess->name(), callName);

    if (interface->kind() == ivm::IVInterface::OperationKind::Sporadic) {
        auto outputActions = translateOutput(m_sdlProcess, m_sdlProcedure, callName, sendCommand);
        for (auto &action : outputActions) {
            m_sdlTransition->addAction(std::move(action));
        }
    } else if (interface->kind() == ivm::IVInterface::OperationKind::Protected
            || interface->kind() == ivm::IVInterface::OperationKind::Unprotected) {
        auto call = translateCall(m_sdlProcess, m_sdlProcedure, callName, sendCommand);
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
    Q_UNUSED(hostProcess);
    Q_UNUSED(hostProcedure);

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

}
