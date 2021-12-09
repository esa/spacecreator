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
            [](QString &accumulator, const QString &indent) { return std::move(accumulator) + indent; });
}

SdlVisitor::IndentingStreamWriter::IndentingStreamWriter(QTextStream &stream)
    : m_stream(stream)
{
}

auto SdlVisitor::IndentingStreamWriter::beginLine(const QString &line) -> void
{
    m_stream << getIndent() << line;
}

auto SdlVisitor::IndentingStreamWriter::write(const QString &line) -> void
{
    m_stream << line;
}

auto SdlVisitor::IndentingStreamWriter::endLine(const QString &line) -> void
{
    m_stream << line << "\n";
}

auto SdlVisitor::IndentingStreamWriter::writeLine(const QString &line) -> void
{
    m_stream << getIndent() << line << "\n";
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

    if (!process.variables().empty() || !process.timerNames().empty()) {
        m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Text));
        m_layouter.moveDown(Layouter::ElementType::Text);
        // Timers are just names, a dedicated visitor does not add any benfits
        for (const auto &timer : process.timerNames()) {
            m_writer.writeLine("Timer " + timer + ";");
        }
        exportCollection(process.variables());
        m_writer.writeLine("/* CIF ENDTEXT */");
    }

    if (!process.procedures().empty()) {
        exportCollection(process.procedures());
    }

    if (process.startTransition() != nullptr) {
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
                inputParameters[0]->declaration()->name(),
                [](const auto &a, const auto &b) { return std::move(a) + ", " + b->declaration()->name(); });
        m_writer.write(parameters);
        m_writer.write(")");
    }
    m_writer.endLine(";");

    if (input.transition() != nullptr) {
        m_writer.pushIndent(INDENT);
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
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Output));
    m_writer.beginLine("output " + output.name());
    const auto &outputParamRef = output.parameter();
    if (outputParamRef != nullptr) {
        m_writer.write(QString("(%1)").arg(outputParamRef->declaration()->name()));
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

    m_writer.writeLine("dcl " + declaration.name() + " " + declaration.type() + ";");
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
    if (join.label() != nullptr) {
        m_writer.write(join.label()->name());
    } else {
        throw ExportException("Label is not set in Join, but Join without specified Label is ill-formed");
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
    if (answer.literal().value() == "else") {
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
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Procedure));
    m_layouter.moveDown(Layouter::ElementType::Procedure);
    m_writer.writeLine("procedure " + procedure.name() + ";");
    m_writer.pushIndent(INDENT);
    m_layouter.resetPosition();

    auto &procedureParameters = procedure.parameters();

    const bool parametersPresent = !procedureParameters.empty();
    const bool returnVarPresent = procedure.returnVariableDeclaration() != nullptr;

    if (parametersPresent || returnVarPresent) {
        m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::Text));
        m_layouter.moveDown(Layouter::ElementType::Text);
    }

    if (parametersPresent) {
        m_writer.writeLine("fpar");
        m_writer.pushIndent(INDENT);
        m_writer.beginLine(QString("%1 %2 %3")
                                   .arg(procedureParameters[0]->direction())
                                   .arg(procedureParameters[0]->name())
                                   .arg(procedureParameters[0]->type()));
        for (auto it = std::next(procedureParameters.begin()); it != procedureParameters.end(); it++) {
            m_writer.endLine(",");
            m_writer.beginLine(
                    QString("%1 %2 %3").arg(it->get()->direction()).arg(it->get()->name()).arg(it->get()->type()));
        }
        m_writer.endLine(";");
        m_writer.popIndent();
    }

    if (returnVarPresent) {
        m_writer.writeLine("returns " + procedure.returnVariableDeclaration()->type() + ";");

        visit(*procedure.returnVariableDeclaration());
    }

    if (parametersPresent || returnVarPresent) {
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
    m_writer.beginLine("return ");
    if (procedure.returnVariableDeclaration() != nullptr) {
        m_writer.write(procedure.returnVariableDeclaration()->name());
    }
    m_writer.endLine(";");
    m_writer.popIndent();
    m_writer.writeLine("endprocedure;");
    m_layouter.popPosition();
    m_layouter.moveDown(Layouter::ElementType::Procedure);
}

void SdlVisitor::visit(const ProcedureCall &procedureCall)
{
    if (procedureCall.procedure() == nullptr || procedureCall.procedure()->name().isEmpty()) {
        throw ExportException("Procedure to call not specified");
    }
    if (procedureCall.procedure()->returnVariableDeclaration() != nullptr) {
        throw ExportException("Procedure with a return variable cannot be called from a Procedure Call. "
                              "It must be called from a Task");
    }

    m_layouter.moveDown(Layouter::ElementType::ProcedureCall);
    m_writer.writeLine(m_layouter.getPositionString(Layouter::ElementType::ProcedureCall));
    m_writer.beginLine("call " + procedureCall.procedure()->name());

    const auto &procedureCallArgs = procedureCall.arguments();
    if (!procedureCallArgs.empty()) {
        auto getArgAsQString = [](const ProcedureCall::Argument &argument) -> QString {
            QString arg;
            if (std::holds_alternative<std::unique_ptr<VariableLiteral>>(argument)) {
                arg += std::get<std::unique_ptr<VariableLiteral>>(argument)->value();
            } else if (std::holds_alternative<std::unique_ptr<VariableReference>>(argument)) {
                arg += std::get<std::unique_ptr<VariableReference>>(argument)->declaration()->name();
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
