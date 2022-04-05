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

#include <sdl/SdlModel/answer.h>
#include <sdl/SdlModel/decision.h>
#include <sdl/SdlModel/input.h>
#include <sdl/SdlModel/join.h>
#include <sdl/SdlModel/label.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/output.h>
#include <sdl/SdlModel/procedure.h>
#include <sdl/SdlModel/procedurecall.h>
#include <sdl/SdlModel/process.h>
#include <sdl/SdlModel/return.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <sdl/SdlModel/signal.h>
#include <sdl/SdlModel/state.h>
#include <sdl/SdlModel/task.h>
#include <sdl/SdlModel/transition.h>
#include <sdl/SdlModel/variabledeclaration.h>

namespace sdl {

/**
 * @brief   Interface for SDL model visitors
 */
class Visitor
{
public:
    /**
     * @brief   Default constructor
     */
    Visitor() = default;

    /**
     * @brief   Deleted copy constructor
     */
    Visitor(const Visitor &) = delete;

    /**
     * @brief   Default move constructor
     */
    Visitor(Visitor &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Visitor &operator=(const Visitor &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    Visitor &operator=(Visitor &&) = default;

    /**
     * @brief   Virtual destructor
     */
    virtual ~Visitor() = default;

    /**
     * @brief   Process visitor
     *
     * @param   process   process to be visited
     */
    virtual auto visit(const Process &process) -> void = 0;

    /**
     * @brief   State visitor
     *
     * @param   state   state to be visited
     */
    virtual auto visit(const State &state) -> void = 0;

    /**
     * @brief   Input visitor
     *
     * @param   input   input to be visited
     */
    virtual auto visit(const Input &input) -> void = 0;

    /**
     * @brief   Output visitor
     *
     * @param   output  output to be visited
     */
    virtual auto visit(const Output &output) -> void = 0;

    /**
     * @brief   NEXTSTATE action visitor
     *
     * @param   nextstate   NEXTSTATE action to be visited
     */
    virtual auto visit(const NextState &nextstate) -> void = 0;

    /**
     * @brief   Task visitor
     *
     * @param   task   task to be visited
     */
    virtual auto visit(const Task &task) -> void = 0;

    /**
     * @brief   Variable declaration visitor
     *
     * @param   declaration   declaration to be visited
     */
    virtual auto visit(const VariableDeclaration &declaration) -> void = 0;

    /**
     * @brief   Label visitor
     *
     * @param   label   label to be visited
     */
    virtual auto visit(const Label &label) -> void = 0;

    /**
     * @brief   Join visitor
     *
     * @param   join   join to be visited
     */
    virtual auto visit(const Join &join) -> void = 0;

    /**
     * @brief   Return visitor
     *
     * @param   ret   return to be visited
     */
    virtual auto visit(const Return &ret) -> void = 0;

    /**
     * @brief   Answer visitor
     *
     * @param   answer  answer to be visited
     */
    virtual auto visit(const Answer &answer) -> void = 0;

    /**
     * @brief   Decision visitor
     *
     * @param   decision  decision to be visited
     */
    virtual auto visit(const Decision &decision) -> void = 0;

    /**
     * @brief   Procedure visitor
     *
     * @param   procedure   procedure to be visited
     */
    virtual auto visit(const Procedure &procedure) -> void = 0;

    /**
     * @brief   Procedure call visitor
     *
     * @param   procedureCall   procedure call to be visited
     */
    virtual auto visit(const ProcedureCall &procedureCall) -> void = 0;
};

} // namespace sdl
