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

#include <iostream>
#include <qglobal.h>

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
    // write some dummy CIF
    m_stream << "/* CIF PROCESS (" << 250 << ", " << 150 << "), (" << 150 << ", " << 75 << ") */\n";
    m_stream << "process " << process.name() << ";\n";

    // TODO: loop over process variables and export them

    // TODO: loop over procedures and export them

    // TODO: initial transition (START) must be explicitly stated
    m_stream << "    /* CIF START (9, 285), (70, 35) */\n"
                "    START;\n"
                "        /* CIF NEXTSTATE (9, 335), (70, 35) */\n"
                "        NEXTSTATE ";
    if (process.stateMachine() && //
            !process.stateMachine()->states().empty() && //
            process.stateMachine()->states()[0]->name() != nullptr) {
        m_stream << process.stateMachine()->states()[0]->name();
    }
    m_stream << ";\n";

    exportCollection(process.stateMachine()->states());

    m_stream << "endprocess " << process.name() << ";\n";
}

void SdlVisitor::visit(const State &state) const
{
    // write some dummy CIF
    m_stream << "    /* CIF state (" << 250 << ", " << 150 << "), (" << 150 << ", " << 75 << ") */\n";
    m_stream << "    state " << state.name() << ";\n";

    exportCollection(state.inputs());

    m_stream << "    endstate;\n";
}

void SdlVisitor::visit(const Input &input) const
{
    // write some dummy CIF
    m_stream << "        /* CIF input (" << 250 << "," << 150 << "), (" << 150 << ", " << 75 << ") */\n";
    m_stream << "        input " << input.name() << ";\n";

    if (input.transition() != nullptr) {
        exportCollection(input.transition()->actions());
    }
}

void SdlVisitor::visit(const Transition &transition) const
{
    Q_UNUSED(transition);
}

void SdlVisitor::visit(const NextState &nextstate) const
{
    // write some dummy CIF
    m_stream << "            /* CIF NEXTSTATE (" << 250 << "," << 150 << "), (" << 150 << ", " << 75 << ") */\n";
    m_stream << "            NEXTSTATE ";
    if (nextstate.state() == nullptr) {
        m_stream << "-";
    } else {
        if (nextstate.state()->name() != nullptr) {
            m_stream << nextstate.state()->name();
        }
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
