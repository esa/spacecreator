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

#include "sdlvisitor.h"

#include <conversion/common/export/exceptions.h>
#include <iostream>
#include <map>
#include <qglobal.h>
#include <sdl/SdlModel/variableliteral.h>
#include <sdl/SdlModel/variablereference.h>
#include <variant>

using conversion::exporter::ExportException;

namespace sdl {

static const QString POSITION_STRING_PATTERN = "/* CIF %1 (%2, %3), (%4, %5) */";
static const QString INDENT = "    ";

// clang-format off

static const std::map<SdlVisitor::Layouter::ElementType, SdlVisitor::SdlVisitor::Layouter::Size> CIF_SIZES = {
    { SdlVisitor::Layouter::ElementType::Text, std::make_pair<uint32_t, uint32_t>(400, 500) },
    { SdlVisitor::Layouter::ElementType::Start, std::make_pair<uint32_t, uint32_t>(200, 75) },
    { SdlVisitor::Layouter::ElementType::Answer, std::make_pair<uint32_t, uint32_t>(400, 50) },
    { SdlVisitor::Layouter::ElementType::Decision, std::make_pair<uint32_t, uint32_t>(400, 50) },
    { SdlVisitor::Layouter::ElementType::Process, std::make_pair<uint32_t, uint32_t>(800, 100) },
    { SdlVisitor::Layouter::ElementType::State, std::make_pair<uint32_t, uint32_t>(200, 50) },
    { SdlVisitor::Layouter::ElementType::Input, std::make_pair<uint32_t, uint32_t>(200, 50) },
    { SdlVisitor::Layouter::ElementType::Output, std::make_pair<uint32_t, uint32_t>(200, 50) },
    { SdlVisitor::Layouter::ElementType::NextState, std::make_pair<uint32_t, uint32_t>(200, 50) },
    { SdlVisitor::Layouter::ElementType::Task, std::make_pair<uint32_t, uint32_t>(100, 50) },
    { SdlVisitor::Layouter::ElementType::Label, std::make_pair<uint32_t, uint32_t>(100, 50) },
    { SdlVisitor::Layouter::ElementType::Join, std::make_pair<uint32_t, uint32_t>(100, 50) },
    { SdlVisitor::Layouter::ElementType::Procedure, std::make_pair<uint32_t, uint32_t>(800, 50) },
    { SdlVisitor::Layouter::ElementType::ProcedureCall, std::make_pair<uint32_t, uint32_t>(100, 50) }
};

static const std::map<SdlVisitor::Layouter::ElementType, QString> CIF_NAMES = {
    { SdlVisitor::Layouter::ElementType::Text, "Text" },
    { SdlVisitor::Layouter::ElementType::Start, "Start" },
    { SdlVisitor::Layouter::ElementType::Answer, "Answer" },
    { SdlVisitor::Layouter::ElementType::Decision, "Decision" },
    { SdlVisitor::Layouter::ElementType::Process, "Process" },
    { SdlVisitor::Layouter::ElementType::State, "State" },
    { SdlVisitor::Layouter::ElementType::Input, "Input" },
    { SdlVisitor::Layouter::ElementType::Output, "Output" },
    { SdlVisitor::Layouter::ElementType::NextState, "NextState" },
    { SdlVisitor::Layouter::ElementType::Task, "Task" },
    { SdlVisitor::Layouter::ElementType::Label, "Label" },
    { SdlVisitor::Layouter::ElementType::Join, "Join" },
    { SdlVisitor::Layouter::ElementType::Procedure,"Procedure" },
    { SdlVisitor::Layouter::ElementType::ProcedureCall, "ProcedureCall" }
};

// clang-format on

SdlVisitor::Layouter::Layouter()
{
    m_positions.emplace_back(0, 0);
    m_highWatermarkX = 0;
}

auto SdlVisitor::Layouter::resetPosition() -> void
{
    m_positions.back().first = 0;
    m_positions.back().second = 0;
    m_highWatermarkX = 0;
}

auto SdlVisitor::Layouter::pushPosition() -> void
{
    const auto &current = getPosition();
    m_positions.push_back(current);
}

auto SdlVisitor::Layouter::popPosition() -> void
{
    m_positions.pop_back();
}

auto SdlVisitor::Layouter::moveRight(const ElementType element) -> void
{
    m_positions.back().first += (CIF_SIZES.at(element).first * 3) / 2;
    m_highWatermarkX = std::max(m_highWatermarkX, m_positions.back().first);
}

auto SdlVisitor::Layouter::moveDown(const ElementType element) -> void
{
    m_positions.back().second += (CIF_SIZES.at(element).second * 3) / 2;
}

auto SdlVisitor::Layouter::getPosition() -> const Position &
{
    return m_positions.back();
}

auto SdlVisitor::Layouter::moveRightToHighWatermark() -> void
{
    m_positions.back().first = m_highWatermarkX;
}

auto SdlVisitor::Layouter::getPositionString(const SdlVisitor::Layouter::ElementType element) -> QString
{
    const auto &position = getPosition();
    return POSITION_STRING_PATTERN.arg(CIF_NAMES.at(element), QString::number(position.first),
            QString::number(position.second), QString::number(CIF_SIZES.at(element).first),
            QString::number(CIF_SIZES.at(element).second));
}

auto SdlVisitor::IndentingStreamWriter::getIndent() -> QString
{
    return std::accumulate(m_indent.begin(), m_indent.end(), QString(""),
            [](const QString &accumulator, const QString &indent) { return accumulator + indent; });
}

SdlVisitor::IndentingStreamWriter::IndentingStreamWriter(QTextStream &stream)
    : m_stream(stream)
{
}

auto SdlVisitor::IndentingStreamWriter::beginLine(const QString &line) -> void
{
    m_stream << getIndent() << line;
}

auto SdlVisitor::IndentingStreamWriter::endLine(const QString &line) -> void
{
    m_stream << line << "\n";
}

auto SdlVisitor::IndentingStreamWriter::write(const QString &line) -> void
{
    m_stream << line;
}

auto SdlVisitor::IndentingStreamWriter::writeLine(const QString &line) -> void
{
    m_stream << getIndent() << line << "\n";
}

auto SdlVisitor::IndentingStreamWriter::writeComment(const QString &comment) -> void
{
    if (comment.isEmpty()) {
        return;
    }

    for (const auto &commentPart : comment.split("\n")) {
        m_stream << getIndent() << "-- " << commentPart << "\n";
    }
}

auto SdlVisitor::IndentingStreamWriter::pushIndent(const QString &indent) -> void
{
    m_indent.push_back(indent);
}

auto SdlVisitor::IndentingStreamWriter::popIndent() -> void
{
    m_indent.pop_back();
}

SdlVisitor::SdlVisitor(IndentingStreamWriter &writer, Layouter &layouter)
    : m_writer(writer)
    , m_layouter(layouter)
{
}

void SdlVisitor::visit(const System &system)
{
    if (system.name().isEmpty()) {
        throw ExportException("System shall have a name but it doesn't");
    }

    m_writer.writeLine(QString("system %1;").arg(system.name()));

    m_writer.pushIndent(INDENT);

    const auto hasFreeformTexts = !system.freeformTexts().empty();
    const auto hasSignals = !system.getSignals().empty();

    if (hasFreeformTexts || hasSignals) {
        m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Text));
        m_layouter.moveDown(Layouter::ElementType::Text);

        for (const auto &text : system.freeformTexts()) {
            m_writer.beginLine(text);
            m_writer.endLine(";");
        }

        exportCollection(system.getSignals());

        m_writer.writeLine("/* CIF ENDTEXT */");
    }

    for (const auto &channel : system.channels()) {
        channel.accept(*this);
    }

    system.block().accept(*this);

    m_writer.popIndent();

    m_writer.writeLine("endsystem;");
}

void SdlVisitor::visit(const Channel &channel)
{
    if (channel.name().isEmpty()) {
        throw ExportException("Channel shall have a name but it doesn't");
    }
    if (channel.routes().empty()) {
        throw ExportException("Channel shall have at least one route");
    }

    m_writer.writeLine(QString("channel %1").arg(channel.name()));

    m_writer.pushIndent(INDENT);

    for (const auto &route : channel.routes()) {
        route.accept(*this);
    }

    m_writer.popIndent();
    m_writer.writeLine("endchannel;");
}

void SdlVisitor::visit(const Block &block)
{
    if (block.name().isEmpty()) {
        throw ExportException("Block shall have a name but it doesn't");
    }

    m_writer.writeLine(QString("block %1;").arg(block.name()));

    m_writer.pushIndent(INDENT);

    for (const auto &signalRoute : block.signalRoutes()) {
        signalRoute.accept(*this);
    }

    for (const auto &connection : block.connections()) {
        connection.accept(*this);
    }

    block.process().accept(*this);

    m_writer.popIndent();
    m_writer.writeLine("endblock;");
}

void SdlVisitor::visit(const SignalRoute &signalRoute)
{
    if (signalRoute.name().isEmpty()) {
        throw ExportException("Signal route shall have a name but it doesn't");
    }
    if (signalRoute.routes().empty()) {
        throw ExportException("Signal route shall have at least one route");
    }

    m_writer.writeLine(QString("signalroute %1").arg(signalRoute.name()));

    m_writer.pushIndent(INDENT);

    for (const auto &route : signalRoute.routes()) {
        route.accept(*this);
    }

    m_writer.popIndent();
}

void SdlVisitor::visit(const Connection &connection)
{
    const auto &channelName = connection.channelName();
    const auto &signalRouteName = connection.signalRouteName();

    if (channelName.isEmpty()) {
        throw ExportException("Connection shall have a channel name but it doesn't");
    }
    if (signalRouteName.isEmpty()) {
        throw ExportException("Connection shall have a signal route name but it doesn't");
    }

    m_writer.writeLine(QString("connect %1 and %2;").arg(channelName).arg(signalRouteName));
}

void SdlVisitor::visit(const Route &route)
{
    if (route.from().isEmpty()) {
        throw ExportException("Route shall have a from name but it doesn't");
    }
    if (route.to().isEmpty()) {
        throw ExportException("Route shall have a to name but it doesn't");
    }
    if (route.with().isEmpty()) {
        throw ExportException("Route shall have at least one with name but it doesn't");
    }

    m_writer.beginLine(QString("from %1").arg(route.from()));
    m_writer.write(QString(" to %1").arg(route.to()));
    m_writer.endLine(QString(" with %1;").arg(route.with().join(", ")));
}

void SdlVisitor::visit(const Process &process)
{
    if (process.name().isEmpty()) {
        throw ExportException("Process shall have a name but it doesn't");
    }

    m_layouter.pushPosition();
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Process));
    m_layouter.moveDown(Layouter::ElementType::Process);
    m_writer.writeLine("process " + process.name() + ";");
    m_writer.pushIndent(INDENT);

    const auto hasVariables = !process.variables().empty();
    const auto hasNewtypes = !process.newtypes().empty();
    const auto hasTimers = !process.timerNames().empty();
    const auto hasSuccessStates = !process.successStates().empty();
    const auto hasErrorStates = !process.errorStates().empty();
    const auto hasProcedures = !process.procedures().empty();

    if (hasVariables || hasNewtypes || hasTimers || hasSuccessStates || hasErrorStates) {
        m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Text));
        m_layouter.moveDown(Layouter::ElementType::Text);

        // Timers are just names, a dedicated visitor does not add any benfits
        for (const auto &timer : process.timerNames()) {
            m_writer.writeLine("Timer " + timer + ";");
        }

        for (const auto &newtype : process.newtypes()) {
            newtype.accept(*this);
        }

        exportCollection(process.variables());

        if (hasSuccessStates) {
            m_writer.writeLine(QString("successstates %1;").arg(process.successStates().join(", ")));
        }

        if (hasErrorStates) {
            m_writer.writeLine(QString("errorstates %1;").arg(process.errorStates().join(", ")));
        }

        m_writer.writeLine("/* CIF ENDTEXT */");
    }

    if (hasProcedures) {
        exportCollection(process.procedures());
    }

    if (process.startTransition() != nullptr) {
        m_writer.writeComment(process.startTransition()->comment());
        m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Start));
        m_writer.writeLine("START;");
        m_writer.pushIndent(INDENT);
        m_layouter.moveDown(Layouter::ElementType::Start);
        exportCollection(process.startTransition()->actions());
        m_layouter.moveDown(Layouter::ElementType::State);
        m_writer.popIndent();
    } else {
        throw ExportException("START transition not specified but required");
    }

    if (process.stateMachine() != nullptr) {
        m_layouter.pushPosition();
        exportCollection(process.stateMachine()->states());
        m_layouter.popPosition();
    } else {
        throw ExportException("Process does not contain a State Machine");
    }

    m_writer.popIndent();
    m_writer.writeLine("endprocess " + process.name() + ";");
    m_layouter.popPosition();
}

void SdlVisitor::visit(const State &state)
{
    if (state.name().isEmpty()) {
        throw ExportException("State shall have a name but it doesn't");
    }

    m_layouter.pushPosition();
    m_writer.writeComment(state.comment());
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::State));
    m_writer.writeLine("state " + state.name() + ";");
    m_writer.pushIndent(INDENT);
    exportCollection(state.inputs());
    m_writer.popIndent();
    m_writer.writeLine("endstate;");
    m_layouter.popPosition();
    m_layouter.moveRightToHighWatermark();
    m_layouter.moveRight(Layouter::ElementType::State);
}

void SdlVisitor::visit(const Signal &signal)
{
    if (signal.name().isEmpty()) {
        throw ExportException("Signal shall have a name but it doesn't");
    }

    m_writer.writeLine(QString("signal %1;").arg(signal.name()));
}

void SdlVisitor::visit(const Rename &rename)
{
    if (rename.name().isEmpty()) {
        throw ExportException("Signal rename shall have a name but it doesn't");
    }

    m_writer.beginLine(QString("signal %1").arg(rename.name()));

    const auto parametersTypes = rename.parametersTypes();
    if (!parametersTypes.empty()) {
        m_writer.write("(");
        m_writer.write(parametersTypes.join(", "));
        m_writer.write(")");
    }

    m_writer.write(" renames");

    if (rename.referencedName().isEmpty()) {
        return;
    }

    switch (rename.direction()) {
    case Rename::Direction::Input:
        m_writer.write(" input ");
        break;
    case Rename::Direction::Output:
        m_writer.write(" output ");
        break;
    }

    m_writer.write(rename.referencedName());

    if (rename.referencedFunctionName().isEmpty()) {
        return;
    }

    switch (rename.direction()) {
    case Rename::Direction::Input:
        m_writer.write(" to ");
        break;
    case Rename::Direction::Output:
        m_writer.write(" from ");
        break;
    }

    m_writer.write(rename.referencedFunctionName());
    m_writer.endLine(";");
}

void SdlVisitor::visit(const Input &input)
{
    if (input.name().isEmpty()) {
        throw ExportException("Input shall have a name but it doesn't");
    }

    m_layouter.pushPosition();
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Input));
    m_writer.beginLine("input " + input.name());

    const auto &inputParameters = input.parameters();
    const auto numOfInputParameters = inputParameters.size();

    if (numOfInputParameters > 0) {
        m_writer.write("(");

        QString parameters = std::accumulate(std::next(inputParameters.begin()), inputParameters.end(),
                inputParameters[0]->variableName(),
                [](const auto &a, const auto &b) { return std::move(a) + ", " + b->variableName(); });
        m_writer.write(parameters);
        m_writer.write(")");
    }
    m_writer.endLine(";");

    if (input.transition() != nullptr) {
        m_writer.pushIndent(INDENT);
        m_writer.writeComment(input.transition()->comment());
        exportCollection(input.transition()->actions());
        m_writer.popIndent();
    } else {
        throw ExportException("Transition in Input not specified but required");
    }
    m_layouter.popPosition();
    m_layouter.moveRight(Layouter::ElementType::Input);
}

void SdlVisitor::visit(const Output &output)
{
    if (output.name().isEmpty()) {
        throw ExportException("Output shall have a name but it doesn't");
    }

    m_layouter.moveDown(Layouter::ElementType::Output);
    m_writer.writeComment(output.comment());
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Output));
    m_writer.beginLine("output " + output.name());
    const auto &outputParamRef = output.parameter();
    if (outputParamRef != nullptr) {
        m_writer.write(QString("(%1)").arg(outputParamRef->variableName()));
    }
    m_writer.endLine(";");
}

void SdlVisitor::visit(const NextState &nextstate)
{
    QString nextStateName;

    if (nextstate.state() == nullptr) {
        nextStateName = "-";
    } else {
        if (nextstate.state()->name() != nullptr) {
            nextStateName = nextstate.state()->name();
        } else {
            throw ExportException("Next state not specified");
        }
    }

    m_layouter.moveDown(Layouter::ElementType::State);
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::NextState));
    m_writer.writeLine("NEXTSTATE " + nextStateName + ";");
}

void SdlVisitor::visit(const Task &task)
{
    if (task.content().isEmpty()) {
        throw ExportException("Task shall have contents but it doesn't");
    }

    m_layouter.moveDown(Layouter::ElementType::Task);
    m_writer.writeComment(task.comment());
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Task));
    m_writer.writeLine("task " + task.content() + ";");
}

void SdlVisitor::visit(const VariableDeclaration &declaration)
{
    if (declaration.name().isEmpty()) {
        throw ExportException("Variable declaration shall have a name but it doesn't");
    }
    if (declaration.type().isEmpty()) {
        throw ExportException("Variable declaration shall have a specified type but it doesn't");
    }

    m_writer.writeComment(declaration.comment());

    if (declaration.isMonitor()) {
        m_writer.beginLine("monitor ");
    } else {
        m_writer.beginLine("dcl ");
    }

    m_writer.write(QString("%1 %2").arg(declaration.name()).arg(declaration.type()));
    m_writer.endLine(";");
}

void SdlVisitor::visit(const Newtype &newtype)
{
    if (newtype.name().isEmpty()) {
        throw ExportException("Newtype shall have a name but it doesn't");
    }

    if (newtype.indexingTypeName().isEmpty()) {
        throw ExportException("Newtype shall have an indexing type but it doesn't");
    }

    if (newtype.elementTypeName().isEmpty()) {
        throw ExportException("Newtype shall have an element type but it doesn't");
    }

    m_writer.writeComment(newtype.comment());

    m_writer.writeLine(QString("newtype %1 Array(%2, %3) endnewtype;")
                               .arg(newtype.name(), newtype.indexingTypeName(), newtype.elementTypeName()));
}

void SdlVisitor::visit(const Label &label)
{
    if (label.name().isEmpty()) {
        throw ExportException("Label name cannot be empty");
    }

    m_layouter.moveDown(Layouter::ElementType::Label);
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Label));
    m_writer.writeLine(label.name() + ":");
}

void SdlVisitor::visit(const Join &join)
{
    m_layouter.moveDown(Layouter::ElementType::Join);
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Join));
    m_writer.beginLine("join ");
    if (!join.label().isEmpty()) {
        m_writer.write(join.label());
    } else {
        throw ExportException("Label is not set in Join, but Join without specified Label is ill-formed");
    }
    m_writer.endLine(";");
}

void SdlVisitor::visit(const Return &ret)
{
    m_writer.beginLine("return ");
    if (!ret.content().isEmpty()) {
        m_writer.write(ret.content());
    }
    m_writer.endLine(";");
}

void SdlVisitor::visit(const Answer &answer)
{
    if (answer.transition() == nullptr) {
        throw ExportException("Required Transition is missing in Answer");
    }
    if (answer.literal().value().isEmpty()) {
        throw ExportException("Required Literal have a missing value in Answer");
    }

    m_layouter.pushPosition();
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Answer));
    if (answer.literal().value().toUpper() == ::sdl::Answer::ElseLiteral) {
        m_writer.writeLine(answer.literal().value() + ":");
    } else {
        m_writer.writeLine("(" + answer.literal().value() + "):");
    }
    m_writer.pushIndent(INDENT);
    exportCollection(answer.transition()->actions());
    m_writer.popIndent();
    m_layouter.popPosition();
    m_layouter.moveRightToHighWatermark();
    m_layouter.moveRight(Layouter::ElementType::Answer);
}

void SdlVisitor::visit(const Decision &decision)
{
    if (decision.expression() == nullptr) {
        throw ExportException("Required Expression is missing in Decision");
    }
    if (decision.expression()->content().isEmpty()) {
        throw ExportException("Content in Decision is empty");
    }
    if (decision.answers().empty()) {
        throw ExportException("No Answers in Decision");
    }

    m_writer.writeComment(decision.comment());
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Decision));
    m_writer.writeLine("decision " + decision.expression()->content() + ";");
    m_layouter.moveDown(Layouter::ElementType::Decision);
    m_layouter.pushPosition();
    m_writer.pushIndent(INDENT);
    exportCollection(decision.answers());
    m_writer.popIndent();
    m_layouter.popPosition();
    m_layouter.moveRightToHighWatermark();
    m_writer.writeLine("enddecision;");
}

void SdlVisitor::visit(const Procedure &procedure)
{
    if (procedure.transition() == nullptr) {
        // No implementation -> external procedure
        return;
    }
    m_layouter.pushPosition();
    m_writer.writeComment(procedure.comment());
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Procedure));
    m_layouter.moveDown(Layouter::ElementType::Procedure);
    m_writer.writeLine("procedure " + procedure.name() + ";");
    m_writer.pushIndent(INDENT);
    m_layouter.resetPosition();

    const auto &procedureParameters = procedure.parameters();
    const auto &procedureVariables = procedure.variables();

    const bool variablesPresent = !procedureVariables.empty();
    const bool parametersPresent = !procedureParameters.empty();
    const bool returnTypePresent = procedure.returnType().has_value();
    const bool generateVarCif = variablesPresent || parametersPresent || returnTypePresent;

    if (generateVarCif) {
        m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Text));
        m_layouter.moveDown(Layouter::ElementType::Text);
    }

    if (variablesPresent) {
        exportCollection(procedureVariables);
    }

    if (parametersPresent) {
        m_writer.writeLine("fpar");
        m_writer.pushIndent(INDENT);
        m_writer.writeComment(procedureParameters[0]->comment());
        m_writer.beginLine(QString("%1 %2 %3")
                                   .arg(procedureParameters[0]->direction())
                                   .arg(procedureParameters[0]->name())
                                   .arg(procedureParameters[0]->type()));
        for (auto it = std::next(procedureParameters.begin()); it != procedureParameters.end(); it++) {
            m_writer.endLine(",");
            m_writer.writeComment(it->get()->comment());
            m_writer.beginLine(
                    QString("%1 %2 %3").arg(it->get()->direction()).arg(it->get()->name()).arg(it->get()->type()));
        }
        m_writer.endLine(";");
        m_writer.popIndent();
    }

    if (returnTypePresent) {
        m_writer.writeLine("returns " + *procedure.returnType() + ";");
    }
    if (generateVarCif) {
        m_writer.writeLine("/* CIF ENDTEXT */");
    }

    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Start));
    m_layouter.moveDown(Layouter::ElementType::Start);
    m_writer.writeLine("START;");
    if (procedure.transition()->actions().empty()) {
        qWarning("Procedure is empty");
    } else {
        m_writer.pushIndent(INDENT);
        exportCollection(procedure.transition()->actions());
        m_writer.popIndent();
    }
    m_writer.popIndent();

    if (!returnTypePresent) {
        m_writer.writeLine("return;");
    }

    m_writer.writeLine("endprocedure;");
    m_layouter.popPosition();
    m_layouter.moveDown(Layouter::ElementType::Procedure);
}

void SdlVisitor::visit(const ProcedureCall &procedureCall)
{
    if (procedureCall.procedure() == nullptr || procedureCall.procedure()->name().isEmpty()) {
        throw ExportException("Procedure to call not specified");
    }
    if (procedureCall.procedure()->returnType()) {
        throw ExportException("Procedure with a return variable cannot be called from a Procedure Call. "
                              "It must be called from a Task");
    }

    m_layouter.moveDown(Layouter::ElementType::ProcedureCall);
    m_writer.writeComment(procedureCall.comment());
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::ProcedureCall));
    m_writer.beginLine("call " + procedureCall.procedure()->name());

    const auto &procedureCallArgs = procedureCall.arguments();
    if (!procedureCallArgs.empty()) {
        auto getArgAsQString = [](const ProcedureCall::Argument &argument) -> QString {
            QString arg;
            if (std::holds_alternative<std::unique_ptr<VariableLiteral>>(argument)) {
                arg += std::get<std::unique_ptr<VariableLiteral>>(argument)->value();
            } else if (std::holds_alternative<std::unique_ptr<VariableReference>>(argument)) {
                arg += std::get<std::unique_ptr<VariableReference>>(argument)->variableName();
            } else {
                throw ExportException("Unknown Argument type");
            }
            return arg;
        };

        const auto args = std::accumulate(std::next(procedureCallArgs.begin()), procedureCallArgs.end(),
                getArgAsQString(procedureCallArgs[0]), [&](QString &accumulator, const auto &argument) {
                    return accumulator + ", " + getArgAsQString(argument);
                });

        m_writer.write("(" + args + ")");
    }
    m_writer.endLine(";");
}

template<typename T>
void SdlVisitor::exportCollection(const T &collection)
{
    for (const auto &item : collection) {
        SdlVisitor visitor(m_writer, m_layouter);
        item->accept(visitor);
    }
}

} // namespace sdl
