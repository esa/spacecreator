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
#include <sdl/SdlModel/variablereference.h>

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
    if (process.name() == "") {
        throw ExportException("Process shall have a name but it doesn't");
    }

    // write some dummy CIF
    m_stream << "/* CIF PROCESS (" << 250 << ", " << 150 << "), (" << 150 << ", " << 75 << ") */\n";
    m_stream << "process " << process.name() << ";\n";

    if (!process.variables().empty()) {
        m_stream << "    /* CIF TEXT (16, 317), (267, 140) */\n";
        exportCollection(process.variables());
        m_stream << "    /* CIF ENDTEXT */\n";
        m_stream << "\n";
    }

    // TODO: loop over procedures and export them

    if (process.startTransition() != nullptr) {
        m_stream << "    /* CIF START (9, 285), (70, 35) */\n"
                    "    START;\n";
        exportCollection(process.startTransition()->actions());
    } else {
        throw ExportException("START transition not specified but required");
    }
    m_stream << "\n";

    exportCollection(process.stateMachine()->states());

    m_stream << "endprocess " << process.name() << ";\n";
}

void SdlVisitor::visit(const State &state) const
{
    if (state.name() == "") {
        throw ExportException("State shall have a name but it doesn't");
    }

    // write some dummy CIF
    m_stream << "    /* CIF state (" << 250 << ", " << 150 << "), (" << 150 << ", " << 75 << ") */\n";
    m_stream << "    state " << state.name() << ";\n";
    exportCollection(state.inputs());
    m_stream << "    endstate;\n";
    m_stream << "\n";
}

void SdlVisitor::visit(const Input &input) const
{
    if (input.name() == "") {
        throw ExportException("Input shall have a name but it doesn't");
    }

    // write some dummy CIF
    m_stream << "        /* CIF input (" << 250 << "," << 150 << "), (" << 150 << ", " << 75 << ") */\n";
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
    QString outputParamStr;

    if (output.name() == "") {
        throw ExportException("Output shall have a name but it doesn't");
    }

    const auto outputParamRef = output.parameter();
    if (outputParamRef != nullptr) {
        outputParamStr = QString("(%1)").arg(outputParamRef->declaration()->name());
    }

    // write some dummy CIF
    m_stream << "            /* CIF output (" << 250 << "," << 150 << "), (" << 150 << ", " << 75 << ") */\n";
    m_stream << "            output " << output.name() << outputParamStr << ";\n";
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

    // write some dummy CIF
    m_stream << "            /* CIF NEXTSTATE (" << 250 << "," << 150 << "), (" << 150 << ", " << 75 << ") */\n";
    m_stream << "            NEXTSTATE " << nextStateName << ";\n";
}

void SdlVisitor::visit(const Task &task) const
{
    if (task.content() == "") {
        throw ExportException("Task shall have contents but it doesn't");
    }

    // write some dummy CIF
    m_stream << "            /* CIF task (" << 250 << "," << 150 << "), (" << 150 << ", " << 75 << ") */\n";
    m_stream << "            task " << task.content() << ";\n";
}

void SdlVisitor::visit(const VariableDeclaration &declaration) const
{
    if (declaration.name() == "") {
        throw ExportException("Variable declaration shall have a name but it doesn't");
    }
    if (declaration.type() == "") {
        throw ExportException("Variable declaration shall have a specified type but it doesn't");
    }

    m_stream << "    dcl " << declaration.name() << " " << declaration.type() << ";\n";
}

void SdlVisitor::visit(const Label &label) const
{
    // write some dummy CIF
    m_stream << "        /* CIF label (" << 250 << "," << 150 << "), (" << 150 << ", " << 75 << ") */\n";
    m_stream << "        " << label.name() << ":\n";
}

void SdlVisitor::visit(const Join &join) const
{
    m_stream << "            /* CIF join (" << 250 << "," << 150 << "), (" << 150 << ", " << 75 << ") */\n";
    m_stream << "            join ";
    if (join.label() != nullptr) {
        m_stream << join.label()->name();
    } else {
        throw ExportException("Label is not set in Join, but Join without specified Label is ill-formed");
    }
    m_stream << ";\n";
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
