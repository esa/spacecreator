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

#include "visitor.h"

#include <QTextStream>
#include <sdl/SdlModel/answer.h>
#include <sdl/SdlModel/input.h>
#include <sdl/SdlModel/label.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/output.h>
#include <sdl/SdlModel/procedure.h>
#include <sdl/SdlModel/procedurecall.h>
#include <sdl/SdlModel/process.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <sdl/SdlModel/signal.h>
#include <sdl/SdlModel/state.h>
#include <sdl/SdlModel/task.h>
#include <sdl/SdlModel/transition.h>

namespace sdl {

/**
 * @brief   Visitor serializing components of the SDL model
 */
class SdlVisitor final : public Visitor
{
public:
    /**
     * @brief   Constructor
     *
     * @param   stream   output stream (where the serialized values are put)
     */
    SdlVisitor(QTextStream &stream);

    /**
     * @brief   Deleted copy constructor
     */
    SdlVisitor(const SdlVisitor &) = delete;

    /**
     * @brief   Default move constructor
     */
    SdlVisitor(SdlVisitor &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    SdlVisitor &operator=(const SdlVisitor &) = delete;

    /**
     * @brief   Deleted move assignment operator
     */
    SdlVisitor &operator=(SdlVisitor &&) = delete; // instances of this class cannot be moved

    /**
     * @brief   Model visitor
     *
     * @param   model   model to be serialized
     */
    auto visit(const SdlModel &model) const -> void override;

    /**
     * @brief   Process visitor
     *
     * @param   process   process to be serialized
     */
    auto visit(const Process &process) const -> void override;

    /**
     * @brief   State visitor
     *
     * @param   state   state to be serialized
     */
    auto visit(const State &state) const -> void override;

    /**
     * @brief   Input visitor
     *
     * @param   input   input to be serialized
     */
    auto visit(const Input &input) const -> void override;

    /**
     * @brief   Output visitor
     *
     * @param   output  output to be serialized
     */
    auto visit(const Output &output) const -> void override;

    /**
     * @brief   NEXTSTATE action visitor
     *
     * @param   nextstate   NEXTSTATE action to be serialized
     */
    auto visit(const NextState &nextstate) const -> void override;

    /**
     * @brief   Task visitor
     *
     * @param   task   task to be serialized
     */
    auto visit(const Task &task) const -> void override;

    /**
     * @brief   Variable declaration visitor
     *
     * @param   declaration   declaration to be serialized
     */
    auto visit(const VariableDeclaration &declaration) const -> void override;

    /**
     * @brief   Label visitor
     *
     * @param   label   label to be serialized
     */
    auto visit(const Label &label) const -> void override;

    /**
     * @brief   Join visitor
     *
     * @param   join   join to be serialized
     */
    virtual auto visit(const Join &join) const -> void override;

    /**
     * @brief   Answer visitor
     *
     * @param   answer  answer to be serialized
     */
    auto visit(const Answer &answer) const -> void override;

    /**
     * @brief   Decision visitor
     *
     * @param   decision  decision to be serialized
     */
    virtual auto visit(const Decision &decision) const -> void override;

    /**
     * @brief   Procedure visitor
     *
     * @param   procedure   procedure to be serialized
     */
    auto visit(const Procedure &procedure) const -> void override;

    /**
     * @brief   Procedure call visitor
     *
     * @param   procedureCall   procedure call to be serialized
     */
    auto visit(const ProcedureCall &procedureCall) const -> void override;

private:
    template<typename T>
    auto exportCollection(const T &collection) const -> void;

    QTextStream &m_stream;
};

} // namespace conversion::Sdl
