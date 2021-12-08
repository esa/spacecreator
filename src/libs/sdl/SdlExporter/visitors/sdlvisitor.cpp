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

// clang-format off

// const does not work with std::map accesses
static std::map<SdlVisitor::Layouter::ElementType, SdlVisitor::SdlVisitor::Layouter::Size> CIF_SIZES = {
    { SdlVisitor::Layouter::ElementType::Text, std::make_pair<uint32_t, uint32_t>(400, 500) },
    { SdlVisitor::Layouter::ElementType::Start, std::make_pair<uint32_t, uint32_t>(200, 50) },
    { SdlVisitor::Layouter::ElementType::Answer, std::make_pair<uint32_t, uint32_t>(400, 50) },
    { SdlVisitor::Layouter::ElementType::Decision, std::make_pair<uint32_t, uint32_t>(400, 50) },
    { SdlVisitor::Layouter::ElementType::Process, std::make_pair<uint32_t, uint32_t>(800, 100) },
    { SdlVisitor::Layouter::ElementType::State, std::make_pair<uint32_t, uint32_t>(250, 50) },
    { SdlVisitor::Layouter::ElementType::Input, std::make_pair<uint32_t, uint32_t>(200, 50) },
    { SdlVisitor::Layouter::ElementType::Output, std::make_pair<uint32_t, uint32_t>(200, 50) },
    { SdlVisitor::Layouter::ElementType::NextState, std::make_pair<uint32_t, uint32_t>(200, 50) },
    { SdlVisitor::Layouter::ElementType::Task, std::make_pair<uint32_t, uint32_t>(100, 50) },
    { SdlVisitor::Layouter::ElementType::Label, std::make_pair<uint32_t, uint32_t>(100, 50) },
    { SdlVisitor::Layouter::ElementType::Join, std::make_pair<uint32_t, uint32_t>(100, 50) },
    { SdlVisitor::Layouter::ElementType::Procedure, std::make_pair<uint32_t, uint32_t>(800, 50) },
    { SdlVisitor::Layouter::ElementType::ProcedureCall, std::make_pair<uint32_t, uint32_t>(100, 50) }
};

static std::map<SdlVisitor::Layouter::ElementType, QString> CIF_NAMES = {
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
    m_xOffset = 100;
    m_yOffset = 50;
    m_positions.push_back(std::make_pair(0, 0));
}

auto SdlVisitor::Layouter::pushPosition() -> void
{
    auto current = getPosition();
    m_positions.push_back(current);
}

auto SdlVisitor::Layouter::popPosition() -> void
{
    m_positions.pop_back();
}

auto SdlVisitor::Layouter::moveRight(const ElementType element) -> void
{
    m_positions[m_positions.size() - 1].first += (CIF_SIZES[element].first * 3) / 2;
}
auto SdlVisitor::Layouter::moveDown(const ElementType element) -> void
{
    m_positions[m_positions.size() - 1].second += (CIF_SIZES[element].second * 3) / 2;
}
auto SdlVisitor::Layouter::getPosition() -> const Position &
{
    return m_positions.back();
}

auto SdlVisitor::Layouter::getPositionString(const SdlVisitor::Layouter::ElementType element) -> QString
{
    Q_UNUSED(element);
    const auto position = getPosition();
    return POSITION_STRING_PATTERN.arg(CIF_NAMES[element], QString::number(position.first),
            QString::number(position.second), QString::number(CIF_SIZES[element].first),
            QString::number(CIF_SIZES[element].second));
}

SdlVisitor::SdlVisitor(QTextStream &stream, Layouter &layouter)
    : m_stream(stream)
    , m_layouter(layouter)
{
}

void SdlVisitor::visit(const Process &process)
{
    if (process.name().isEmpty()) {
        throw ExportException("Process shall have a name but it doesn't");
    }

    m_layouter.pushPosition();
    m_stream << m_layouter.getPositionString(Layouter::ElementType::Process) << "\n";
    m_layouter.moveDown(Layouter::ElementType::Process);
    m_stream << "process " << process.name() << ";\n";

    if (!process.variables().empty() || !process.timerNames().empty()) {
        m_stream << m_layouter.getPositionString(Layouter::ElementType::Text) << "\n";
        m_layouter.moveDown(Layouter::ElementType::Text);
        // Timers are just names, a dedicated visitor does not add any benfits
        for (const auto &timer : process.timerNames()) {
            m_stream << "    Timer " << timer << ";\n";
        }
        exportCollection(process.variables());
        m_stream << "    /* CIF ENDTEXT */\n";
    }

    if (!process.procedures().empty()) {
        m_layouter.pushPosition();
        exportCollection(process.procedures());
        m_layouter.popPosition();
        m_layouter.moveRight(Layouter::ElementType::Procedure);
    }

    if (process.startTransition() != nullptr) {
        m_stream << m_layouter.getPositionString(Layouter::ElementType::Start)
                 << "\n"
                    "    START;\n";
        m_layouter.moveDown(Layouter::ElementType::Start);
        exportCollection(process.startTransition()->actions());
        m_layouter.popPosition();
        m_layouter.moveRight(Layouter::ElementType::Process);
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

    m_stream << "endprocess " << process.name() << ";";
}

void SdlVisitor::visit(const State &state)
{
    if (state.name().isEmpty()) {
        throw ExportException("State shall have a name but it doesn't");
    }

    m_layouter.pushPosition();
    m_stream << m_layouter.getPositionString(Layouter::ElementType::State) << "\n";
    m_stream << "    state " << state.name() << ";\n";
    exportCollection(state.inputs());
    m_stream << "    endstate;\n";
    m_layouter.popPosition();
    m_layouter.moveRight(Layouter::ElementType::State);
}

void SdlVisitor::visit(const Input &input)
{
    if (input.name().isEmpty()) {
        throw ExportException("Input shall have a name but it doesn't");
    }

    m_layouter.pushPosition();
    m_stream << m_layouter.getPositionString(Layouter::ElementType::Input) << "\n";
    m_stream << "        input " << input.name();

    const auto &inputParameters = input.parameters();
    const auto numOfInputParameters = inputParameters.size();

    if (numOfInputParameters > 0) {
        m_stream << "(";

        QString parameters = std::accumulate(std::next(inputParameters.begin()), inputParameters.end(),
                inputParameters[0]->declaration()->name(),
                [](const auto &a, const auto &b) { return std::move(a) + ", " + b->declaration()->name(); });
        m_stream << parameters;

        m_stream << ")";
    }
    m_stream << ";\n";

    if (input.transition() != nullptr) {
        exportCollection(input.transition()->actions());
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
    m_stream << m_layouter.getPositionString(Layouter::ElementType::Output) << "\n";
    m_stream << "            output " << output.name();
    const auto &outputParamRef = output.parameter();
    if (outputParamRef != nullptr) {
        m_stream << QString("(%1)").arg(outputParamRef->declaration()->name());
    }
    m_stream << ";\n";
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
    m_stream << m_layouter.getPositionString(Layouter::ElementType::NextState) << "\n";
    m_stream << "            NEXTSTATE " << nextStateName << ";\n";
}

void SdlVisitor::visit(const Task &task)
{
    if (task.content().isEmpty()) {
        throw ExportException("Task shall have contents but it doesn't");
    }

    m_layouter.moveDown(Layouter::ElementType::Task);
    m_stream << m_layouter.getPositionString(Layouter::ElementType::Task) << "\n";
    m_stream << "            task " << task.content() << ";\n";
}

void SdlVisitor::visit(const VariableDeclaration &declaration)
{
    if (declaration.name().isEmpty()) {
        throw ExportException("Variable declaration shall have a name but it doesn't");
    }
    if (declaration.type().isEmpty()) {
        throw ExportException("Variable declaration shall have a specified type but it doesn't");
    }

    m_stream << "    dcl " << declaration.name() << " " << declaration.type() << ";\n";
}

void SdlVisitor::visit(const Label &label)
{
    if (label.name().isEmpty()) {
        throw ExportException("Label name cannot be empty");
    }

    m_layouter.moveDown(Layouter::ElementType::Label);
    m_stream << m_layouter.getPositionString(Layouter::ElementType::Label) << "\n";
    m_stream << "        " << label.name() << ":\n";
}

void SdlVisitor::visit(const Join &join)
{
    m_layouter.moveDown(Layouter::ElementType::Join);
    m_stream << m_layouter.getPositionString(Layouter::ElementType::Join) << "\n";
    m_stream << "            join ";
    if (join.label() != nullptr) {
        m_stream << join.label()->name();
    } else {
        throw ExportException("Label is not set in Join, but Join without specified Label is ill-formed");
    }
    m_stream << ";\n";
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
    m_stream << m_layouter.getPositionString(Layouter::ElementType::Answer) << "\n";
    if (answer.literal().value() == "else") {
        m_stream << "                " << answer.literal().value() << ":\n";
    } else {
        m_stream << "                (" << answer.literal().value() << "):\n";
    }
    exportCollection(answer.transition()->actions());
    m_layouter.popPosition();
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

    m_stream << m_layouter.getPositionString(Layouter::ElementType::Decision) << "\n";
    m_stream << "            decision " << decision.expression()->content() << ";\n";
    m_layouter.moveDown(Layouter::ElementType::Decision);
    m_layouter.pushPosition();
    exportCollection(decision.answers());
    m_layouter.popPosition();
    m_stream << "            enddecision;\n";
}

void SdlVisitor::visit(const Procedure &procedure)
{
    if (procedure.transition() == nullptr) {
        // No implementation -> external procedure
        return;
    }
    m_layouter.pushPosition();
    m_stream << m_layouter.getPositionString(Layouter::ElementType::Procedure) << "\n";
    m_layouter.moveDown(Layouter::ElementType::Procedure);
    m_stream << "    procedure " << procedure.name() << ";\n";

    auto &procedureParameters = procedure.parameters();

    const bool parametersPresent = !procedureParameters.empty();
    const bool returnVarPresent = procedure.returnVariableDeclaration() != nullptr;

    if (parametersPresent || returnVarPresent) {
        m_stream << m_layouter.getPositionString(Layouter::ElementType::Text) << "\n";
        m_layouter.moveDown(Layouter::ElementType::Text);
    }

    if (parametersPresent) {
        m_stream << "        fpar\n";
        QString fpars = QString("            %1 %2 %3")
                                .arg(procedureParameters[0]->direction())
                                .arg(procedureParameters[0]->name())
                                .arg(procedureParameters[0]->type());
        for (auto it = std::next(procedureParameters.begin()); it != procedureParameters.end(); it++) {
            fpars = fpars
                    + QString(",\n            %1 %2 %3")
                              .arg(it->get()->direction())
                              .arg(it->get()->name())
                              .arg(it->get()->type());
        }
        m_stream << fpars;
        m_stream << ";\n";
    }

    if (returnVarPresent) {
        m_stream << "        returns ";
        m_stream << procedure.returnVariableDeclaration()->type();
        m_stream << ";\n";

        visit(*procedure.returnVariableDeclaration());
    }

    if (parametersPresent || returnVarPresent) {
        m_stream << "        /* CIF ENDTEXT */\n";
    }

    m_stream << m_layouter.getPositionString(Layouter::ElementType::Start) << "\n";
    m_layouter.moveDown(Layouter::ElementType::Start);
    m_stream << "        START;\n";
    if (procedure.transition()->actions().empty()) {
        qWarning("Procedure is empty");
    } else {
        exportCollection(procedure.transition()->actions());
    }
    m_stream << "        return ";
    if (procedure.returnVariableDeclaration() != nullptr) {
        m_stream << procedure.returnVariableDeclaration()->name();
    }
    m_stream << ";\n";
    m_stream << "    endprocedure;\n";
    m_layouter.popPosition();
    m_layouter.moveRight(Layouter::ElementType::Procedure);
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
    m_stream << m_layouter.getPositionString(Layouter::ElementType::ProcedureCall) << "\n";
    m_stream << "        call " << procedureCall.procedure()->name();

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

        m_stream << "(" << args << ")";
    }
    m_stream << ";\n";
}

QString SdlVisitor::dummyCif(const QString &cifType)
{
    const static QString dummyCifTemplate = "/* CIF %1 (250, 150), (150, 75) */\n";
    return dummyCifTemplate.arg(cifType);
}

template<typename T>
void SdlVisitor::exportCollection(const T &collection)
{
    for (const auto &item : collection) {
        SdlVisitor visitor(m_stream, m_layouter);
        item->accept(visitor);
    }
}

} // namespace sdl
