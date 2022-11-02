/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "constants.h"
#include "specialized/descriptiontranslator.h"
#include "specialized/mathoperationtranslator.h"
#include "specialized/splinecalibratortranslator.h"
#include "specialized/statemachinetranslator.h"

#include <algorithm>
#include <conversion/asn1/SedsToAsn1Translator/constants.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/options.h>
#include <conversion/common/overloaded.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/SedsToIvTranslator/interfacetranslatorhelper.h>
#include <ivcore/ivfunction.h>
#include <regex>
#include <seds/SedsModel/components/activities/coremathoperator.h>
#include <seds/SedsOptions/options.h>

using conversion::Escaper;
using conversion::iv::translator::seds::InterfaceTranslatorHelper;
using conversion::seds::SedsOptions;
using conversion::translator::TranslationException;
using ::seds::model::CoreMathOperator;
using ::seds::model::Polynomial;
using ::seds::model::SplineCalibrator;

namespace conversion::sdl::translator {

static const QString LOOP_START_LABEL_PREFIX = "loop_start_";
static const QString LOOP_END_LABEL_PREFIX = "loop_end_";
static const QString CONDITION_END_LABEL_PREFIX = "condition_";
static const QString FALSE_LITERAL = "False";
static const QString TRUE_LITERAL = "True";

StatementTranslatorVisitor::StatementContext::StatementContext(
        Context &masterContext, ::sdl::Process *sdlProcess, ::sdl::Procedure *sdlProcedure)
    : m_labelCount(0)
    , m_rawSplinePointsVariableCount(0)
    , m_calibratedSplinePointsVariableCount(0)
    , m_masterContext(masterContext)
    , m_sdlProcess(sdlProcess)
    , m_sdlProcedure(sdlProcedure)
    , m_intervalIndexVariableCreated(false)
{
}

auto StatementTranslatorVisitor::StatementContext::uniqueLabelName(const QString &prefix) -> QString
{
    return QString("%1%2").arg(prefix).arg(++m_labelCount);
}

auto StatementTranslatorVisitor::StatementContext::uniqueRawSplinePointsVariableName() -> QString
{
    return QString("RawSplinePoints%1").arg(++m_rawSplinePointsVariableCount);
}

auto StatementTranslatorVisitor::StatementContext::uniqueCalibratedSplinePointsVariableName() -> QString
{
    return QString("CalibratedSplinePoints%1").arg(++m_calibratedSplinePointsVariableCount);
}

auto StatementTranslatorVisitor::StatementContext::sedsPackage() -> const ::seds::model::Package &
{
    return m_masterContext.sedsPackage();
}

auto StatementTranslatorVisitor::StatementContext::asn1Model() -> Asn1Acn::Asn1Model *
{
    return m_masterContext.asn1Model();
}

auto StatementTranslatorVisitor::StatementContext::ivFunction() -> ivm::IVFunction *
{
    return m_masterContext.ivFunction();
}

auto StatementTranslatorVisitor::StatementContext::sdlProcess() -> ::sdl::Process *
{
    return m_sdlProcess;
}

auto StatementTranslatorVisitor::StatementContext::sdlProcedure() -> ::sdl::Procedure *
{
    return m_sdlProcedure;
}

auto StatementTranslatorVisitor::StatementContext::intervalIndexVariableCreated() const -> bool
{
    return m_intervalIndexVariableCreated;
}

auto StatementTranslatorVisitor::StatementContext::setIntervalIndexVariableCreated(bool created) -> void
{
    m_intervalIndexVariableCreated = created;
}

auto StatementTranslatorVisitor::StatementContext::handleSplinePointCount(const std::size_t count) -> void
{
    m_masterContext.handleSplinePointCount(count);
}

auto StatementTranslatorVisitor::StatementContext::addActivityInfo(const QString &name, ActivityInfo info) -> void
{
    m_masterContext.addActivityInfo(name, std::move(info));
}

auto StatementTranslatorVisitor::StatementContext::getCommand(const QString &interface, const QString &name)
        -> const CommandInfo *
{
    return m_masterContext.getCommand(interface, name);
}

StatementTranslatorVisitor::StatementTranslatorVisitor(
        StatementContext &context, ::sdl::Transition *sdlTransition, const Options &options)
    : m_context(context)
    , m_sdlTransition(sdlTransition)
    , m_options(options)
{
}

auto StatementTranslatorVisitor::operator()(const ::seds::model::ActivityInvocation &invocation) -> void
{
    auto task = translateActivityCall(m_context.sdlProcess(), invocation);
    m_sdlTransition->addAction(std::move(task));
}

auto StatementTranslatorVisitor::operator()(const ::seds::model::Assignment &assignment) -> void
{
    const auto targetName = translateVariableReference(assignment.outputVariableRef().value().value());
    const auto &element = assignment.element();
    if (std::holds_alternative<::seds::model::VariableRef>(element)) {
        const auto &reference = std::get<::seds::model::VariableRef>(element);
        const auto referenceDefinition = reference.value().value();
        const auto action = QString("%1 := %2").arg(targetName, translateVariableReference(referenceDefinition));

        auto sdlTask = std::make_unique<::sdl::Task>("", action);
        DescriptionTranslator::translate(assignment, sdlTask.get());

        m_sdlTransition->addAction(std::move(sdlTask));

    } else if (std::holds_alternative<::seds::model::ValueOperand>(element)) {
        const auto &operand = std::get<::seds::model::ValueOperand>(element);
        const auto operandValue = operand.value().value();
        const auto action = QString("%1 := %2").arg(targetName, operandValue);

        auto sdlTask = std::make_unique<::sdl::Task>("", action);
        DescriptionTranslator::translate(assignment, sdlTask.get());

        m_sdlTransition->addAction(std::move(sdlTask));
    } else {
        throw TranslationException("Assignment not implemented");
    }
}

auto StatementTranslatorVisitor::operator()(const ::seds::model::Calibration &calibration) -> void
{
    const auto &calibrator = calibration.calibrator();
    if (std::holds_alternative<Polynomial>(calibrator)) {
        const auto targetName = translateVariableReference(calibration.outputVariableRef().value().value());
        const auto sourceName = translateVariableReference(calibration.inputVariableRef().value().value());

        const auto &polynomial = std::get<Polynomial>(calibrator);
        const auto action = QString("%1 := %2").arg(targetName, translatePolynomial(sourceName, polynomial));

        auto sdlTask = std::make_unique<::sdl::Task>("", action);
        DescriptionTranslator::translate(calibration, sdlTask.get());

        m_sdlTransition->addAction(std::move(sdlTask));
    } else if (std::holds_alternative<SplineCalibrator>(calibrator)) {
        const auto &splineCalibrator = std::get<SplineCalibrator>(calibrator);

        SplineCalibratorTranslator splineCalibratorTranslator(m_context, calibration, m_sdlTransition);
        splineCalibratorTranslator.translate(splineCalibrator);

        m_context.handleSplinePointCount(splineCalibrator.splinePoints().size());
    } else {
        throw TranslationException("Calibration activity not implemented");
    }
}

auto StatementTranslatorVisitor::operator()(const ::seds::model::Conditional &conditional) -> void
{
    auto decision =
            translateBooleanExpression(m_context.sdlProcess(), m_context.sdlProcedure(), conditional.condition());
    auto label = std::make_unique<::sdl::Label>(m_context.uniqueLabelName(CONDITION_END_LABEL_PREFIX));

    auto trueAnswer = translateAnswer(m_context, label.get(), TRUE_LITERAL, conditional.onConditionTrue(), m_options);
    auto falseAnswer =
            translateAnswer(m_context, label.get(), FALSE_LITERAL, conditional.onConditionFalse(), m_options);
    decision->addAnswer(std::move(trueAnswer));
    decision->addAnswer(std::move(falseAnswer));

    DescriptionTranslator::translate(conditional, decision.get());

    m_sdlTransition->addAction(std::move(decision));
    m_sdlTransition->addAction(std::move(label));
}

auto StatementTranslatorVisitor::operator()(const ::seds::model::Iteration &iteration) -> void
{
    auto startLabel = std::make_unique<::sdl::Label>(m_context.uniqueLabelName(LOOP_START_LABEL_PREFIX));
    auto startLabelPointer = startLabel.get();
    auto endLabel = std::make_unique<::sdl::Label>(m_context.uniqueLabelName(LOOP_END_LABEL_PREFIX));
    auto decision = std::make_unique<::sdl::Decision>();
    auto exitLoop = std::make_unique<::sdl::Answer>();
    auto continueLoop = std::make_unique<::sdl::Answer>();
    auto exitTransition = std::make_unique<::sdl::Transition>();
    auto continueTransition = std::make_unique<::sdl::Transition>();
    auto transitionPointer = continueTransition.get();
    auto exitJoin = std::make_unique<::sdl::Join>();
    exitJoin->setLabel(endLabel->name());

    // Loop condition not met - jump to exit
    exitTransition->addAction(std::move(exitJoin));
    exitLoop->setLiteral(::sdl::VariableLiteral(FALSE_LITERAL));
    exitLoop->setTransition(std::move(exitTransition));

    // Loop condition met
    continueLoop->setLiteral(::sdl::VariableLiteral(TRUE_LITERAL));
    continueLoop->setTransition(std::move(continueTransition));

    decision->addAnswer(std::move(exitLoop));
    decision->addAnswer(std::move(continueLoop));

    generateLoopStart(m_sdlTransition, iteration, decision.get());

    m_sdlTransition->addAction(std::move(startLabel));
    // This contains the actual body, which is generated a few lines below
    m_sdlTransition->addAction(std::move(decision));
    m_sdlTransition->addAction(std::move(endLabel));

    // Generate loop body and insert it into the True decision
    translateBody(m_context, transitionPointer, iteration.doBody(), m_options);

    generateLoopEnd(transitionPointer, iteration, startLabelPointer);
}

auto StatementTranslatorVisitor::operator()(const ::seds::model::MathOperation &operation) -> void
{
    if (operation.elements().empty()) {
        throw TranslationException("Empty MathOperation");
    }

    const auto &elements = operation.elements();
    const auto &firstElement = elements.front();

    const auto isSwap = isSwapOperator(firstElement);

    if (isSwap) {
        const auto swapOperations = MathOperationTranslator::translateSwapOperation(operation);

        bool first = true;

        for (const auto &swapOperation : swapOperations) {
            auto sdlTask = std::make_unique<::sdl::Task>("", swapOperation);

            if (first) {
                DescriptionTranslator::translate(operation, sdlTask.get());
                first = false;
            }

            m_sdlTransition->addAction(std::move(sdlTask));
        }
    } else {
        const auto targetName = translateVariableReference(operation.outputVariableRef().value().value());
        const auto value = MathOperationTranslator::translateOperation(operation.elements());
        const auto action = QString("%1 := %2").arg(targetName, value);

        auto sdlTask = std::make_unique<::sdl::Task>("", action);
        DescriptionTranslator::translate(operation, sdlTask.get());

        m_sdlTransition->addAction(std::move(sdlTask));
    }
}

auto StatementTranslatorVisitor::operator()(const ::seds::model::SendCommandPrimitive &sendCommand) -> void
{
    const auto commandName = sendCommand.command().value();
    const auto interfaceName = sendCommand.interface().value();

    const auto command = m_context.getCommand(interfaceName, commandName);
    if (command == nullptr) {
        auto errorMessage = QString("Command '%1' of interface '%2' not found").arg(commandName, interfaceName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto isReturnCall = command->interfaceType() == CommandInfo::HostInterfaceType::Provided;

    // Check, if this is a sync return call
    if (isReturnCall && command->definition()->mode() == ::seds::model::InterfaceCommandMode::Sync) {
        // Registered and provided sync command
        // SendCommandPrimitive is basically a return statement
        ActivityInfo info(m_context.sdlProcedure()->name());

        for (const auto &argument : sendCommand.argumentValues()) {
            const auto argumentValue = translateArgument(argument);
            if (std::holds_alternative<std::unique_ptr<::sdl::VariableReference>>(argumentValue)) {
                info.addAssignment(AssignmentInfo(translateVariableReference(argument.name().value()),
                        std::get<std::unique_ptr<::sdl::VariableReference>>(argumentValue)->variableName()));
            } else if (std::holds_alternative<std::unique_ptr<::sdl::VariableLiteral>>(argumentValue)) {
                info.addAssignment(AssignmentInfo(translateVariableReference(argument.name().value()),
                        std::get<std::unique_ptr<::sdl::VariableLiteral>>(argumentValue)->value()));
            }
        }

        if (m_options.isSet(SedsOptions::enableFailureReporting)) {
            const auto &failureParameterName = asn1::translator::seds::Constants::failureParamName;
            const auto &failureVariableName = asn1::translator::seds::Constants::failureReturnVariableName;
            const auto failureValue = sendCommand.isFailed() ? "true" : "false";

            const auto failureVariableAssignment = QString("%1 := %2").arg(failureVariableName).arg(failureValue);
            auto failureVariableAssignmentTask = std::make_unique<::sdl::Task>("", failureVariableAssignment);
            m_sdlTransition->addAction(std::move(failureVariableAssignmentTask));

            auto failureAssignmentInfo = AssignmentInfo(failureParameterName, failureVariableName);
            info.addAssignment(std::move(failureAssignmentInfo));
        }

        m_context.addActivityInfo(info.name(), info);

        return;
    }

    const auto callName = InterfaceTranslatorHelper::buildCommandInterfaceName(
            interfaceName, commandName, ivm::IVInterface::InterfaceType::Required, m_options);

    // Process name carries iv-escaped component name
    const auto ivInterface = findIvInterface(m_context.ivFunction(), callName);
    if (ivInterface->kind() == ivm::IVInterface::OperationKind::Sporadic) {
        auto outputActions = translateOutput(callName, sendCommand, ivInterface, isReturnCall, m_options);
        for (auto &action : outputActions) {
            m_sdlTransition->addAction(std::move(action));
        }
    } else if (ivInterface->kind() == ivm::IVInterface::OperationKind::Protected
            || ivInterface->kind() == ivm::IVInterface::OperationKind::Unprotected) {
        auto call = translateCall(m_context.sdlProcess(), callName, sendCommand);

        if (m_options.isSet(SedsOptions::enableFailureReporting)) {
            handleFailureReporting(call.get());
        }

        if (sendCommand.transaction()) {
            handleTransaction(*sendCommand.transaction(), call.get(), ivInterface);
        }

        m_sdlTransition->addAction(std::move(call));
    }
}

auto StatementTranslatorVisitor::operator()(const ::seds::model::SendParameterPrimitive &sendParameter) -> void
{
    const auto parameterType = sendParameter.operation() == ::seds::model::ParameterOperation::Get
            ? InterfaceTranslatorHelper::InterfaceParameterType::Getter
            : InterfaceTranslatorHelper::InterfaceParameterType::Setter;
    const auto parameterName = sendParameter.parameter().value();
    const auto interfaceName = sendParameter.interface().value();

    const auto callName = InterfaceTranslatorHelper::buildParameterInterfaceName(
            interfaceName, parameterName, parameterType, ivm::IVInterface::InterfaceType::Required);

    // Process name carries iv-escaped component name
    const auto ivInterface = findIvInterface(m_context.ivFunction(), callName);

    switch (ivInterface->kind()) {
    case ivm::IVInterface::OperationKind::Sporadic: {
        if (m_options.isSet(SedsOptions::enableFailureReporting)) {
            auto errorMessage = QString("Async parameters are not allowed when using SEDS failure reporting mechanism");
            throw TranslationException(std::move(errorMessage));
        }

        auto outputActions = translateOutput(callName, sendParameter);
        for (auto &action : outputActions) {
            m_sdlTransition->addAction(std::move(action));
        }
        return;
    }
    case ivm::IVInterface::OperationKind::Protected:
    case ivm::IVInterface::OperationKind::Unprotected: {
        auto call = translateCall(m_context.sdlProcess(), callName, sendParameter);

        if (m_options.isSet(SedsOptions::enableFailureReporting)) {
            handleFailureReporting(call.get());
        }

        if (sendParameter.transaction()) {
            handleTransaction(*sendParameter.transaction(), call.get(), ivInterface);
        }

        m_sdlTransition->addAction(std::move(call));
        return;
    }
    default:
        throw TranslationException(QString("Unsupported OperationKind for interface %1").arg(callName));
    }
}

auto StatementTranslatorVisitor::translateActivityCall(::sdl::Process *process,
        const ::seds::model::ActivityInvocation &invocation) -> std::unique_ptr<::sdl::ProcedureCall>
{
    auto call = std::make_unique<::sdl::ProcedureCall>();

    DescriptionTranslator::translate(invocation, call.get());

    const auto procedureName = Escaper::escapeSdlName(invocation.activity().value());
    const auto &procedure = std::find_if(process->procedures().begin(), process->procedures().end(),
            [procedureName](const auto &p) { return p->name() == procedureName; });

    if (procedure == process->procedures().end()) {
        throw TranslationException(QString("Procedure %1 not found").arg(procedureName));
    }
    call->setProcedure(procedure->get());

    for (const auto &argument : invocation.argumentValues()) {
        call->addArgument(translateArgument(argument));
    }
    return call;
}

auto StatementTranslatorVisitor::translateVariableReference(const QString &reference) -> QString
{
    std::regex pattern("[^\\[\\]\\.]+");
    const auto input = reference.toStdString();

    std::string escaped;
    auto i = input.cbegin();
    const auto end = input.cend();
    for (std::smatch match; std::regex_search(i, end, match, pattern); i = match[0].second) {
        escaped += match.prefix();
        const auto identifier = QString::fromStdString(match.str());
        bool isNumber;
        identifier.toDouble(&isNumber);
        if (isNumber) {
            escaped += identifier.toStdString();
        } else {
            escaped += Escaper::escapeSdlVariableName(identifier).toStdString();
        }
    }
    escaped.append(i, end);
    std::replace(escaped.begin(), escaped.end(), '[', '(');
    std::replace(escaped.begin(), escaped.end(), ']', ')');
    return QString::fromStdString(escaped);
}

auto StatementTranslatorVisitor::findIvInterface(ivm::IVFunction *function, const QString &interfaceName)
        -> ivm::IVInterface *
{
    // This form is shorter and easier to debug than find_if and for unknown reason works more reliably
    for (const auto &interface : function->interfaces()) {
        if (interface->ifaceLabel() == interfaceName) {
            return interface;
        }
    }

    throw TranslationException(QString("Interface %1 not found for function %2").arg(interfaceName, function->title()));
    return nullptr;
}

auto StatementTranslatorVisitor::translateArgument(const ::seds::model::NamedArgumentValue &argument)
        -> ::sdl::ProcedureCall::Argument
{
    if (std::holds_alternative<::seds::model::ValueOperand>(argument.value())) {
        const auto &value = std::get<::seds::model::ValueOperand>(argument.value());
        return std::make_unique<::sdl::VariableLiteral>(value.value().value());
    } else if (std::holds_alternative<::seds::model::VariableRefOperand>(argument.value())) {
        const auto &value = std::get<::seds::model::VariableRefOperand>(argument.value());
        const auto variableName = translateVariableReference(value.variableRef().value().value());
        return std::make_unique<::sdl::VariableReference>(variableName);
    } else {
        throw TranslationException("Argument not implemented");
        return std::unique_ptr<::sdl::VariableReference>();
    }
}

auto StatementTranslatorVisitor::translatePolynomial(
        const QString variable, const ::seds::model::Polynomial &polynomial) -> QString
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

auto StatementTranslatorVisitor::translateCall(::sdl::Process *hostProcess, const QString callName,
        const ::seds::model::SendCommandPrimitive &sendCommand) -> std::unique_ptr<::sdl::ProcedureCall>
{
    auto call = std::make_unique<::sdl::ProcedureCall>();

    DescriptionTranslator::translate(sendCommand, call.get());

    const auto &procedure = std::find_if(hostProcess->procedures().begin(), hostProcess->procedures().end(),
            [callName](const auto &p) { return p->name() == callName; });

    if (procedure == hostProcess->procedures().end()) {
        throw TranslationException(QString("Procedure %1 not found").arg(callName));
    }
    call->setProcedure(procedure->get());

    for (const auto &argument : sendCommand.argumentValues()) {
        call->addArgument(translateArgument(argument));
    }

    return call;
}

auto StatementTranslatorVisitor::translateCall(::sdl::Process *hostProcess, const QString callName,
        const ::seds::model::SendParameterPrimitive &sendParameter) -> std::unique_ptr<::sdl::ProcedureCall>
{
    auto call = std::make_unique<::sdl::ProcedureCall>();

    DescriptionTranslator::translate(sendParameter, call.get());

    const auto &procedure = std::find_if(hostProcess->procedures().begin(), hostProcess->procedures().end(),
            [&callName](const auto &p) { return p->name() == callName; });

    if (procedure == hostProcess->procedures().end()) {
        throw TranslationException(QString("Procedure %1 not found").arg(callName));
    }
    call->setProcedure(procedure->get());

    const auto &value = sendParameter.argumentValue().value().value();

    // clang-format off
    std::visit(overloaded {
        [&call](const ::seds::model::ValueOperand &operand) {
            call->addArgument(std::make_unique<::sdl::VariableLiteral>(operand.value().value()));
        },
        [&call](const ::seds::model::VariableRefOperand &operand) {
            const auto variableName = translateVariableReference(operand.variableRef().value().value());
            call->addArgument(std::make_unique<::sdl::VariableReference>(variableName));
        }
    }, value);
    // clang-format on

    return call;
}

auto StatementTranslatorVisitor::translateOutput(const QString &callName,
        const ::seds::model::SendCommandPrimitive &sendCommand, const ivm::IVInterface *ivInterface,
        const bool isReturnCall, const Options &options) -> std::vector<std::unique_ptr<::sdl::Action>>
{
    std::vector<std::unique_ptr<::sdl::Action>> result;

    auto output = std::make_unique<::sdl::Output>();
    DescriptionTranslator::translate(sendCommand, output.get());
    output->setName(callName);

    const auto paramsCount = ivInterface->params().size();

    if (paramsCount == 0) {
        result.push_back(std::move(output));
        return result;
    }

    // clang-format off
    auto operandValueVisitor = overloaded {
        [](const ::seds::model::ValueOperand &operand) {
            return operand.value().value(); },
        [](const ::seds::model::VariableRefOperand &operand) {
            return translateVariableReference(operand.variableRef().value().value());
        }
    };
    // clang-format on

    //--taste translation
    if (options.isSet(SedsOptions::tasteTranslation)) {
        if (paramsCount != 1) {
            auto errorMessage =
                    QString("More than one(%1) IV interface parameter is not allowed for --taste translation option")
                            .arg(paramsCount);
            throw TranslationException(std::move(errorMessage));
        }

        const auto &argument = sendCommand.argumentValues().front();

        const auto source = std::visit(operandValueVisitor, argument.value());
        output->setParameter(std::make_unique<::sdl::VariableReference>(source));

        result.push_back(std::move(output));
        return result;
    }

    const auto ioVariable = StateMachineTranslator::ioVariableName(callName);
    output->setParameter(std::make_unique<::sdl::VariableReference>(ioVariable));

    for (const auto &argument : sendCommand.argumentValues()) {
        const auto fieldName = translateVariableReference(argument.name().value());

        const auto source = std::visit(operandValueVisitor, argument.value());

        auto assignment = QString("%1.%2 := %3").arg(ioVariable, fieldName, source);
        auto assignmentTask = std::make_unique<::sdl::Task>("", assignment);

        result.push_back(std::move(assignmentTask));
    }

    if (isReturnCall && options.isSet(SedsOptions::enableFailureReporting)) {
        const auto failureFieldName = asn1::translator::seds::Constants::failureParamName;
        const auto failureValue = sendCommand.isFailed() ? "true" : "false";

        auto assignment = QString("%1.%2 := %3").arg(ioVariable, failureFieldName, failureValue);
        auto assignmentTask = std::make_unique<::sdl::Task>("", assignment);

        result.push_back(std::move(assignmentTask));
    }

    result.push_back(std::move(output));
    return result;
}

auto StatementTranslatorVisitor::translateOutput(const QString &callName,
        const ::seds::model::SendParameterPrimitive &sendParameter) -> std::vector<std::unique_ptr<::sdl::Action>>
{
    std::vector<std::unique_ptr<::sdl::Action>> result;

    if (!sendParameter.argumentValue().has_value()) {
        throw TranslationException(
                QString("No argument for sending a parameter primitive %1").arg(sendParameter.parameter().value()));
        return result;
    }

    auto output = std::make_unique<::sdl::Output>();
    output->setName(callName);
    const auto ioVariable = StateMachineTranslator::ioVariableName(callName);
    output->setParameter(std::make_unique<::sdl::VariableReference>(ioVariable));

    const auto &value = sendParameter.argumentValue().value().value();
    // clang-format off
    const auto &source = std::visit(overloaded {
            [](const ::seds::model::ValueOperand &operand) {
                return operand.value().value();
            },
            [](const ::seds::model::VariableRefOperand &operand) {
                return translateVariableReference(operand.variableRef().value().value());
            }
        }, value);
    // clang-format on
    auto assignment = std::make_unique<::sdl::Task>("", QString("%1 := %2").arg(ioVariable, source));
    result.emplace_back(std::move(assignment));

    result.emplace_back(std::move(output));

    return result;
}

class ExpressionTranslatorVisitor
{
public:
    ExpressionTranslatorVisitor(::sdl::Process *process, ::sdl::Procedure *procedure)
        : m_process(process)
        , m_procedure(procedure)
    {
    }

    auto operator()(const ::seds::model::Comparison &comparison) -> QString
    {
        return StatementTranslatorVisitor::translateComparison(comparison);
    }
    auto operator()(const std::unique_ptr<::seds::model::AndedConditions> &conditions) -> QString
    {
        return StatementTranslatorVisitor::translateAndedConditions(m_process, m_procedure, *conditions);
    }
    auto operator()(const std::unique_ptr<::seds::model::OredConditions> &conditions) -> QString
    {
        return StatementTranslatorVisitor::translateOredConditions(m_process, m_procedure, *conditions);
    }
    auto operator()(const ::seds::model::TypeCheck &check) -> QString
    {
        return StatementTranslatorVisitor::translateTypeCheck(m_process, m_procedure, check);
    }

private:
    ::sdl::Process *m_process;
    ::sdl::Procedure *m_procedure;
};

auto StatementTranslatorVisitor::translateBooleanExpression(::sdl::Process *hostProcess,
        ::sdl::Procedure *hostProcedure, const ::seds::model::BooleanExpression &expression)
        -> std::unique_ptr<::sdl::Decision>
{
    auto decision = std::make_unique<::sdl::Decision>();
    const auto &condition = expression.condition();
    const auto expressionText = std::visit(ExpressionTranslatorVisitor(hostProcess, hostProcedure), condition);
    auto expressionAction = std::make_unique<::sdl::Expression>(expressionText);
    decision->setExpression(std::move(expressionAction));
    return decision;
}

auto StatementTranslatorVisitor::translateComparison(const ::seds::model::Comparison &comparison) -> QString
{
    const auto left = translateVariableReference(comparison.firstOperand().variableRef().value().value());
    // clang-format off
    const auto &right = std::visit(
        overloaded {
            [](const ::seds::model::VariableRefOperand &reference) {
                return translateVariableReference(reference.variableRef().value().value());
            },
            [](const ::seds::model::ValueOperand &value) {
                return value.value().value();
            }
        }, comparison.secondOperand());
    // clang-format on
    const auto op = comparisonOperatorToString(comparison.comparisonOperator());
    return QString("%1 %2 %3").arg(left, op, right);
}

auto StatementTranslatorVisitor::translateAndedConditions(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
        const ::seds::model::AndedConditions &conditions) -> QString
{
    return std::accumulate(conditions.conditions().begin(), conditions.conditions().end(), QString(""),
            [&hostProcess, &hostProcedure](const auto &accumulator, const auto &element) {
                const auto expression = std::visit(ExpressionTranslatorVisitor(hostProcess, hostProcedure), element);
                return accumulator.size() == 0 ? QString("(%1)").arg(expression)
                                               : QString(accumulator + " and " + QString("(%1)").arg(expression));
            });
}

auto StatementTranslatorVisitor::translateOredConditions(::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure,
        const ::seds::model::OredConditions &conditions) -> QString
{
    return std::accumulate(conditions.conditions().begin(), conditions.conditions().end(), QString(""),
            [&hostProcess, &hostProcedure](const auto &accumulator, const auto &element) {
                const auto expression = std::visit(ExpressionTranslatorVisitor(hostProcess, hostProcedure), element);
                return accumulator.size() == 0 ? QString("(%1)").arg(expression)
                                               : QString(accumulator + " or " + QString("(%1)").arg(expression));
            });
}

auto StatementTranslatorVisitor::translateTypeCheck(
        ::sdl::Process *hostProcess, ::sdl::Procedure *hostProcedure, const ::seds::model::TypeCheck &check) -> QString
{
    Q_UNUSED(hostProcess);
    Q_UNUSED(hostProcedure);
    Q_UNUSED(check);
    throw TranslationException("Expression not implemented");
    return "";
}

auto StatementTranslatorVisitor::comparisonOperatorToString(const ::seds::model::ComparisonOperator op) -> QString
{
    switch (op) {
    case ::seds::model::ComparisonOperator::Equals:
        return "=";
    case ::seds::model::ComparisonOperator::NotEquals:
        return "/=";
    case ::seds::model::ComparisonOperator::LessThan:
        return "<";
    case ::seds::model::ComparisonOperator::LessThanEquals:
        return "<=";
    case ::seds::model::ComparisonOperator::GreaterThan:
        return ">";
    case ::seds::model::ComparisonOperator::GreaterThanEquals:
        return ">=";
    }
    throw TranslationException("Expression not implemented");
    return "";
}

auto StatementTranslatorVisitor::translateAnswer(StatementContext &context, ::sdl::Label *joinLabel,
        const QString &value, const ::seds::model::Body *body, const Options &options) -> std::unique_ptr<::sdl::Answer>
{
    auto answer = std::make_unique<::sdl::Answer>();
    answer->setLiteral(::sdl::VariableLiteral(value));
    auto transition = std::make_unique<::sdl::Transition>();
    if (body != nullptr) {
        StatementTranslatorVisitor nestedVisitor(context, transition.get(), options);
        for (const auto &statement : body->statements()) {
            std::visit(nestedVisitor, statement);
        }
    }
    auto join = std::make_unique<::sdl::Join>();
    join->setLabel(joinLabel->name());
    transition->addAction(std::move(join));
    answer->setTransition(std::move(transition));

    return answer;
}

auto StatementTranslatorVisitor::getOperandValue(const ::seds::model::Operand &operand) -> QString
{
    if (std::holds_alternative<::seds::model::ValueOperand>(operand.value())) {
        const auto &value = std::get<::seds::model::ValueOperand>(operand.value());
        return value.value().value();
    } else if (std::holds_alternative<::seds::model::VariableRefOperand>(operand.value())) {
        const auto &value = std::get<::seds::model::VariableRefOperand>(operand.value());
        return translateVariableReference(value.variableRef().value().value());
    }
    throw TranslationException("Operand not implemented");
    return "";
}

auto StatementTranslatorVisitor::translateBody(StatementContext &context, ::sdl::Transition *transition,
        const ::seds::model::Body *body, const Options &options) -> void
{
    if (body != nullptr) {
        StatementTranslatorVisitor nestedVisitor(context, transition, options);
        for (const auto &statement : body->statements()) {
            std::visit(nestedVisitor, statement);
        }
    }
}

auto StatementTranslatorVisitor::generateLoopStart(
        ::sdl::Transition *transition, const ::seds::model::Iteration &iteration, ::sdl::Decision *decision) -> void
{
    const auto variable = translateVariableReference(iteration.iteratorVariableRef().value().value());

    if (std::holds_alternative<::seds::model::Iteration::NumericRange>(iteration.range())) {
        const auto &range = std::get<::seds::model::Iteration::NumericRange>(iteration.range());
        const auto startValue = getOperandValue(range.startAt());
        const auto endValue = getOperandValue(range.endAt());
        transition->addAction(std::make_unique<::sdl::Task>("", QString("%1 := %2").arg(variable, startValue)));

        decision->setExpression(std::make_unique<::sdl::Expression>(QString("%1 <= %2").arg(variable, endValue)));
    } else {
        throw TranslationException("Variable range not implemented");
    }
}

auto StatementTranslatorVisitor::generateLoopEnd(
        ::sdl::Transition *transition, const ::seds::model::Iteration &iteration, ::sdl::Label *startLabel) -> void
{
    const auto variable = translateVariableReference(iteration.iteratorVariableRef().value().value());
    if (std::holds_alternative<::seds::model::Iteration::NumericRange>(iteration.range())) {
        const auto &range = std::get<::seds::model::Iteration::NumericRange>(iteration.range());
        const auto stepValue = range.step().has_value() ? getOperandValue(*range.step()) : "1";
        transition->addAction(std::make_unique<::sdl::Task>("", QString("%1 := %1 + %2").arg(variable, stepValue)));
    } else {
        throw TranslationException("Variable range not implemented");
    }
    auto loopBackJoin = std::make_unique<::sdl::Join>();
    loopBackJoin->setLabel(startLabel->name());
    transition->addAction(std::move(loopBackJoin));
}

auto StatementTranslatorVisitor::handleFailureReporting(::sdl::ProcedureCall *call) const -> void
{
    const auto &failureVariableName = asn1::translator::seds::Constants::failureVariableName;

    // Reset failure variable value
    const auto failureVariableAssignment = QString("%1 := false").arg(failureVariableName);
    auto failureVariableAssignmentTask = std::make_unique<::sdl::Task>("", failureVariableAssignment);
    m_sdlTransition->addAction(std::move(failureVariableAssignmentTask));

    // Add failure variable to the call
    auto failureLiteral = std::make_unique<::sdl::VariableLiteral>(failureVariableName);
    call->addArgument(std::move(failureLiteral));
}

auto StatementTranslatorVisitor::handleTransaction(
        const ::seds::model::Name &transaction, ::sdl::ProcedureCall *call, ivm::IVInterface *ivInterface) const -> void
{
    if (!m_options.isSet(SedsOptions::transactionNameType)) {
        throw TranslationException(
                "SEDS transaction feature was used but no ASN.1 type for transaction name was specified");
    }

    const auto &transactionName = QString("\"%1\"").arg(transaction.value());
    const auto transactionParamTypeName = m_options.value(SedsOptions::transactionNameType).value();

    auto transactionNameLiteral = std::make_unique<::sdl::VariableLiteral>(transactionName);
    call->addArgument(std::move(transactionNameLiteral));

    auto ivParameter = shared::InterfaceParameter(
            Constants::transactionParamName, shared::BasicParameter::Type::Other, transactionParamTypeName);
    ivInterface->addParam(ivParameter);
}

auto StatementTranslatorVisitor::isSwapOperator(const ::seds::model::MathOperation::Element &element) -> bool
{
    const auto op = std::get_if<::seds::model::Operator>(&element);
    if (op == nullptr) {
        return false;
    }

    const auto mathOp = std::get_if<::seds::model::CoreMathOperator>(&op->mathOperator());
    if (mathOp == nullptr) {
        return false;
    }

    return *mathOp == CoreMathOperator::Swap;
}

}
