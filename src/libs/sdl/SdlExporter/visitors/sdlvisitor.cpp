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
    m_stream << "process " << process.name() << "\n";

    // TODO: loop over process variables and export them

    // TODO: loop over procedures and export them

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
    m_stream << "        input " << input.name() << "(" /* parametres of the input */ << ");\n";
}

void SdlVisitor::visit(const Signal &sig) const
{
    Q_UNUSED(sig);
}

template<typename T>
auto SdlVisitor::exportCollection(const T &collection) const -> void
{
    for (const auto &item : collection) {
        SdlVisitor visitor(m_stream);
        item->accept(visitor);
    }
}

} // namespace sdl
