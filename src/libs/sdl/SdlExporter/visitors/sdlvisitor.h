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
     * @brief   Simple layouter for ensuring that SLD entities do not overlap and therefore do not crash OpenGEODE
     */
    class Layouter
    {
    public:
        /**
         * @brief  Entity 2D position
         */
        using Position = std::pair<uint32_t, uint32_t>;

        /**
         * @brief   Entity 2D size
         */
        using Size = std::pair<uint32_t, uint32_t>;

        /**
         * @brief   SDL element type
         */
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

        /**
         * @brief   Constructor
         */
        Layouter();

        /**
         * @brief   Reset the current position and position watermarks
         */
        auto resetPosition() -> void;

        /**
         * @brief   Push current position onto the position stack
         */
        auto pushPosition() -> void;

        /**
         * @brief   Pop current position from the position stack
         */
        auto popPosition() -> void;

        /**
         * @brief   Move position right by the size of the element + proportional margin
         *
         * @param   element   element to use as size reference
         */
        auto moveRight(const ElementType element) -> void;

        /**
         * @brief   Move position down by the size of the element + proportional margin
         *
         * @param   element   element to use as size reference
         */
        auto moveDown(const ElementType element) -> void;

        /**
         * @brief   Get current position
         *
         * @return  current position
         */
        auto getPosition() -> const Position &;

        /**
         * @brief   Get CIF position string
         *
         * @param   element   element to source the name from
         *
         * @return  CIF position string
         */
        auto getPositionString(const ElementType element) -> QString;

        /**
         * @brief   Move position cursor to the rightmost encountered position
         */
        auto moveRightToHighWatermark() -> void;

    private:
        std::vector<Position> m_positions;
        uint32_t m_highWatermarkX;
    };

    /**
     * @brief   Stream writer which keeps track of intent
     */
    class IndentingStreamWriter
    {
    public:
        /**
         * @brief   Constructor
         *
         * @param   stream   stream to write to
         */
        IndentingStreamWriter(QTextStream &stream);

        /**
         * @brief   Start new line with indent and write text
         *
         * @param   line   text to write
         */
        auto beginLine(const QString &line) -> void;

        /**
         * @brief   Write text
         *
         * @param   line   text to write
         */
        auto write(const QString &line) -> void;

        /**
         * @brief   Write text and end line
         *
         * @param   line   text to write
         */
        auto endLine(const QString &line) -> void;

        /**
         * @brief   Write entire line - indent, text and newline
         *
         * @param   line   text to write
         */
        auto writeLine(const QString &line) -> void;

        /**
         * @brief   Push indent onto the top of the indent stack
         *
         * @param   indent   additional indent amount
         */
        auto pushIndent(const QString &indent) -> void;

        /**
         * @brief   Remove indent from the top of the indent stack
         */
        auto popIndent() -> void;

    private:
        QTextStream &m_stream;
        std::vector<QString> m_indent;
        auto getIndent() -> QString;
    };

public:
    /**
     * @brief   Constructor
     *
     * @param   writer   output stream writer
     * @param   layouter layouter for calculating element positions
     */
    SdlVisitor(IndentingStreamWriter &writer, Layouter &layouter);

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
    template<typename T>
    auto exportCollection(const T &collection) -> void;

    IndentingStreamWriter &m_writer;
    Layouter &m_layouter;
};

} // namespace conversion::Sdl
