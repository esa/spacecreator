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

SdlVisitor::SdlVisitor(QTextStream &stream)
    : m_stream(stream)
{
}

void SdlVisitor::visit(const SdlModel &model) const
{
    visit(model.data());
}

void SdlVisitor::visit(const Process &process) const
{
    if (process.name().isEmpty()) {
        throw ExportException("Process shall have a name but it doesn't");
    }

    m_stream << dummyCif("PROCESS");
    m_stream << "process " << process.name() << ";\n";

    if (!process.variables().empty()) {
        m_stream << "    /* CIF TEXT (16, 317), (267, 140) */\n";
        exportCollection(process.variables());
        m_stream << "    /* CIF ENDTEXT */\n";
        m_stream << "\n";
    }

    if (!process.procedures().empty()) {
        exportCollection(process.procedures());
    }

    if (process.startTransition() != nullptr) {
        m_stream << "    /* CIF START (9, 285), (70, 35) */\n"
                    "    START;\n";
        exportCollection(process.startTransition()->actions());
    } else {
        throw ExportException("START transition not specified but required");
    }
    m_stream << "\n";

    if (process.stateMachine() != nullptr) {
        exportCollection(process.stateMachine()->states());
    } else {
        throw ExportException("Process does not contain a State Machine");
    }

    m_stream << "endprocess " << process.name() << ";\n";
}

void SdlVisitor::visit(const State &state) const
{
    if (state.name().isEmpty()) {
        throw ExportException("State shall have a name but it doesn't");
    }

    m_stream << "    " << dummyCif("state");
    m_stream << "    state " << state.name() << ";\n";
    exportCollection(state.inputs());
    m_stream << "    endstate;\n";
    m_stream << "\n";
}

void SdlVisitor::visit(const Input &input) const
{
    if (input.name().isEmpty()) {
        throw ExportException("Input shall have a name but it doesn't");
    }

    m_stream << "        " << dummyCif("input");
    m_stream << "        input " << input.name();

    const auto &inputParameters = input.parameters();
    const auto numOfInputParameters = inputParameters.size();

    if (numOfInputParameters > 0) {
        m_stream << "(";

        QString parameters = std::accumulate(std::next(inputParameters.begin()), inputParameters.end(),
                inputParameters[0]->declaration()->name(),
                [](auto &a, auto *b) { return std::move(a) + ", " + b->declaration()->name(); });
        m_stream << parameters;

        m_stream << ")";
    }
    m_stream << ";\n";

    if (input.transition() != nullptr) {
        exportCollection(input.transition()->actions());
    } else {
        throw ExportException("Transition in Input not specified but required");
    }
}

void SdlVisitor::visit(const Output &output) const
{
    if (output.name().isEmpty()) {
        throw ExportException("Output shall have a name but it doesn't");
    }

    m_stream << "            " << dummyCif("output");
    m_stream << "            output " << output.name();
    const auto outputParamRef = output.parameter();
    if (outputParamRef != nullptr) {
        m_stream << QString("(%1)").arg(outputParamRef->declaration()->name());
    }
    m_stream << ";\n";
}

void SdlVisitor::visit(const NextState &nextstate) const
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

    m_stream << "            " << dummyCif("NEXTSTATE");
    m_stream << "            NEXTSTATE " << nextStateName << ";\n";
}

void SdlVisitor::visit(const Task &task) const
{
    if (task.content().isEmpty()) {
        throw ExportException("Task shall have contents but it doesn't");
    }

    m_stream << "            " << dummyCif("task");
    m_stream << "            task " << task.content() << ";\n";
}

void SdlVisitor::visit(const VariableDeclaration &declaration) const
{
    if (declaration.name().isEmpty()) {
        throw ExportException("Variable declaration shall have a name but it doesn't");
    }
    if (declaration.type().isEmpty()) {
        throw ExportException("Variable declaration shall have a specified type but it doesn't");
    }

    m_stream << "    dcl " << declaration.name() << " " << declaration.type() << ";\n";
}

void SdlVisitor::visit(const Label &label) const
{
    if (label.name().isEmpty()) {
        throw ExportException("Label name cannot be empty");
    }

    m_stream << "        " << dummyCif("label");
    m_stream << "        " << label.name() << ":\n";
}

void SdlVisitor::visit(const Join &join) const
{
    m_stream << "            " << dummyCif("join");
    m_stream << "            join ";
    if (join.label() != nullptr) {
        m_stream << join.label()->name();
    } else {
        throw ExportException("Label is not set in Join, but Join without specified Label is ill-formed");
    }
    m_stream << ";\n";
}

void SdlVisitor::visit(const Answer &answer) const
{
    if (answer.transition() == nullptr) {
        throw ExportException("Required Transition is missing in Answer");
    }
    if (answer.literal().value().isEmpty()) {
        throw ExportException("Required Literal have a missing value in Answer");
    }

    m_stream << "                /* CIF ANSWER (585, 323), (77, 24) */\n";
    if (answer.literal().value() == "else") {
        m_stream << "                " << answer.literal().value() << ":\n";
    } else {
        m_stream << "                (" << answer.literal().value() << "):\n";
    }
    exportCollection(answer.transition()->actions());
}

void SdlVisitor::visit(const Decision &decision) const
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

    m_stream << "            /* CIF decision (388, 241), (115, 50) */\n";
    m_stream << "            decision " << decision.expression()->content() << ";\n";
    exportCollection(decision.answers());
    m_stream << "            enddecision;\n";
}

void SdlVisitor::visit(const Procedure &procedure) const
{
    m_stream << "    " << dummyCif("procedure");
    m_stream << "    procedure " << procedure.name() << ";\n";

    m_stream << "        " << dummyCif("TEXT");
    m_stream << "        fpar\n";

    auto &procedureParameters = procedure.parameters();
    if (!procedureParameters.empty()) {
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

    m_stream << "        returns ";
    if (procedure.returnVariableDeclaration() != nullptr) {
        m_stream << procedure.returnVariableDeclaration()->type();
    }
    m_stream << ";\n";
    if (procedure.returnVariableDeclaration() != nullptr) {
        visit(*procedure.returnVariableDeclaration());
    }
    m_stream << "        /* CIF ENDTEXT */\n";

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
}

void SdlVisitor::visit(const ProcedureCall &procedureCall) const
{
    if (procedureCall.procedure() == nullptr || procedureCall.procedure()->name().isEmpty()) {
        throw ExportException("Procedure to call not specified");
    }
    if (procedureCall.procedure()->returnVariableDeclaration() != nullptr) {
        throw ExportException("Procedure with a return variable cannot be called from a Procedure Call. "
                              "It must be called from a Task");
    }

    m_stream << "        " << dummyCif("PROCEDURECALL");
    m_stream << "        call " << procedureCall.procedure()->name();

    auto &procedureCallArgs = procedureCall.arguments();
    if (!procedureCallArgs.empty()) {
        auto getArgAsQString = [](const ProcedureCall::Argument &argument) -> QString {
            QString arg;
            if (std::holds_alternative<VariableLiteral>(argument)) {
                arg += std::get<VariableLiteral>(argument).value();
            } else if (std::holds_alternative<VariableReference *>(argument)) {
                arg += std::get<VariableReference *>(argument)->declaration()->name();
            } else {
                throw ExportException("Unknown Argument type");
            }
            return arg;
        };

        const auto args = std::accumulate(std::next(procedureCallArgs.begin()), procedureCallArgs.end(),
                getArgAsQString(procedureCallArgs[0]),
                [&](QString &accumulator, const ProcedureCall::Argument &argument) {
                    return accumulator + ", " + getArgAsQString(argument);
                });

        m_stream << "(" << args << ")";
    }
    m_stream << ";\n";
}

QString SdlVisitor::dummyCif(const QString &cifType) const
{
    const static QString dummyCifTemplate = "/* CIF %1 (250, 150), (150, 75) */\n";
    return dummyCifTemplate.arg(cifType);
}

template<typename T>
void SdlVisitor::exportCollection(const T &collection) const
{
    for (const auto &item : collection) {
        SdlVisitor visitor(m_stream);
        item->accept(visitor);
    }
}

} // namespace sdl
