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

#pragma once

#include "node.h"
#include "statemachine.h"

#include <QString>
#include <memory>
#include <sdl/SdlExporter/visitors/visitor.h>
#include <vector>

namespace sdl {

class VariableDeclaration
{
    // TODO
};

class Procedure
{
    // TODO
};

class Process : public Node
{
public:
    Process(const Process &process) = delete; // TODO: there are probably some other cosntructors to delete
    Process(const QString &name, std::unique_ptr<StateMachine> &stateMachine);
    virtual ~Process();

    auto stateMachine() const -> const std::unique_ptr<StateMachine> &;
    auto variables() const -> const std::vector<VariableDeclaration> &;
    auto procedures() const -> const std::vector<Procedure> &;
    auto accept(Visitor &visitor) const -> void override;

private:
    std::unique_ptr<StateMachine> m_stateMachine;
    std::vector<VariableDeclaration> m_variables;
    std::vector<Procedure> m_procedures;
};
} // namespace sdl
