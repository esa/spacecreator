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
    class Layouter
    {
    public:
        using Position = std::pair<uint32_t, uint32_t>;
        using Size = std::pair<uint32_t, uint32_t>;
        enum class ElementType
        {
            Text,
            Start,
            Answer,
            Decision,
            Process,
            State,
            Input,
            Output,
            NextState,
            Task,
            Label,
            Join,
            Procedure,
            ProcedureCall,
        };

    private:
        std::vector<Position> m_positions;
        uint32_t m_highWatermarkX;

    public:
        Layouter();

        auto resetPosition() -> void;
        auto pushPosition() -> void;
        auto popPosition() -> void;
        auto moveRight(const ElementType element) -> void;
        auto moveDown(const ElementType element) -> void;
        auto getPosition() -> const Position &;
        auto getPositionString(const ElementType element) -> QString;
        auto moveRightToHighWatermark() -> void;
    };

public:
    /**
     * @brief   Constructor
     *
     * @param   stream   output stream (where the serialized values are put)
     * @param   layouter layouter for calculating element positions
     */
    SdlVisitor(QTextStream &stream, Layouter &layouter);

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
     * @brief   Process visitor
     *
     * @param   process   process to be serialized
     */
    auto visit(const Process &process) -> void override;

    /**
     * @brief   State visitor
     *
     * @param   state   state to be serialized
     */
    auto visit(const State &state) -> void override;

    /**
     * @brief   Input visitor
     *
     * @param   input   input to be serialized
     */
    auto visit(const Input &input) -> void override;

    /**
     * @brief   Output visitor
     *
     * @param   output  output to be serialized
     */
    auto visit(const Output &output) -> void override;

    /**
     * @brief   NEXTSTATE action visitor
     *
     * @param   nextstate   NEXTSTATE action to be serialized
     */
    auto visit(const NextState &nextstate) -> void override;

    /**
     * @brief   Task visitor
     *
     * @param   task   task to be serialized
     */
    auto visit(const Task &task) -> void override;

    /**
     * @brief   Variable declaration visitor
     *
     * @param   declaration   declaration to be serialized
     */
    auto visit(const VariableDeclaration &declaration) -> void override;

    /**
     * @brief   Label visitor
     *
     * @param   label   label to be serialized
     */
    auto visit(const Label &label) -> void override;

    /**
     * @brief   Join visitor
     *
     * @param   join   join to be serialized
     */
    virtual auto visit(const Join &join) -> void override;

    /**
     * @brief   Answer visitor
     *
     * @param   answer  answer to be serialized
     */
    auto visit(const Answer &answer) -> void override;

    /**
     * @brief   Decision visitor
     *
     * @param   decision  decision to be serialized
     */
    virtual auto visit(const Decision &decision) -> void override;

    /**
     * @brief   Procedure visitor
     *
     * @param   procedure   procedure to be serialized
     */
    auto visit(const Procedure &procedure) -> void override;

    /**
     * @brief   Procedure call visitor
     *
     * @param   procedureCall   procedure call to be serialized
     */
    auto visit(const ProcedureCall &procedureCall) -> void override;

private:
    auto dummyCif(const QString &cifType) -> QString;

    template<typename T>
    auto exportCollection(const T &collection) -> void;

    QTextStream &m_stream;
    Layouter &m_layouter;
};

} // namespace conversion::Sdl
