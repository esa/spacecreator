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

namespace sdl {

/**
 * @brief   Visitor serializing components of the SDL model
 */
class SdlVisitor final : public Visitor
{
public:
    /**
     * @brief   Simple layouter for ensuring that SDL entities do not overlap and therefore do not crash OpenGEODE
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

    public:
        /**
         * @brief   Constructor
         */
        Layouter();

    public:
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

    public:
        /**
         * @brief   Start new line with indent and write text
         *
         * @param   line   text to write
         */
        auto beginLine(const QString &line) -> void;
        /**
         * @brief   Write text and end line
         *
         * @param   line   text to write
         */
        auto endLine(const QString &line) -> void;

        /**
         * @brief   Write text
         *
         * @param   line   text to write
         */
        auto write(const QString &line) -> void;
        /**
         * @brief   Write entire line - indent, text and newline
         *
         * @param   line   text to write
         */
        auto writeLine(const QString &line) -> void;
        /**
         * @brief   Write a comment line - indent, comment and newline
         *
         * @param   comment   comment to write
         */
        auto writeComment(const QString &line) -> void;

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

public:
    /**
     * @brief   System visitor
     *
     * @param   system     System to be serialized
     */
    virtual auto visit(const System &system) -> void override;
    /**
     * @brief   Channel visitor
     *
     * @param   channel   Channel to be visited
     */
    virtual auto visit(const Channel &channel) -> void override;
    /**
     * @brief   Block visitor
     *
     * @param   block   Block to be serialized
     */
    virtual auto visit(const Block &block) -> void override;
    /**
     * @brief   SignalRoute visitor
     *
     * @param   signalRoute     Signal route to be serialized
     */
    virtual auto visit(const SignalRoute &signalRoute) -> void override;
    /**
     * @brief   Connection visitor
     *
     * @param   connection      Connection to be serialized
     */
    virtual auto visit(const Connection &connection) -> void override;
    /**
     * @brief   Route visitor
     *
     * @param   route   Route to be visited
     */
    virtual auto visit(const Route &route) -> void override;
    /**
     * @brief   Process visitor
     *
     * @param   process   process to be serialized
     */
    virtual auto visit(const Process &process) -> void override;
    /**
     * @brief   State visitor
     *
     * @param   state   state to be serialized
     */
    virtual auto visit(const State &state) -> void override;
    /**
     * @brief   Signal visitor
     *
     * @param   signal   Signal to be visited
     */
    virtual auto visit(const Signal &input) -> void override;
    /**
     * @brief   Rename visitor
     *
     * @param   rename      Rename to be serialized
     */
    virtual auto visit(const Rename &rename) -> void override;
    /**
     * @brief   Input visitor
     *
     * @param   input   input to be serialized
     */
    virtual auto visit(const Input &input) -> void override;
    /**
     * @brief   Output visitor
     *
     * @param   output  output to be serialized
     */
    virtual auto visit(const Output &output) -> void override;
    /**
     * @brief   NEXTSTATE action visitor
     *
     * @param   nextstate   NEXTSTATE action to be serialized
     */
    virtual auto visit(const NextState &nextstate) -> void override;
    /**
     * @brief   Task visitor
     *
     * @param   task   task to be serialized
     */
    virtual auto visit(const Task &task) -> void override;
    /**
     * @brief   Variable declaration visitor
     *
     * @param   declaration   declaration to be serialized
     */
    virtual auto visit(const VariableDeclaration &declaration) -> void override;
    /**
     * @brief   Syntype visitor
     *
     * @param   syntype     Syntype to be serialized
     */
    virtual auto visit(const Syntype &syntype) -> void override;
    /**
     * @brief   Newtype visitor
     *
     * @param   newtype     Newtype to be serialized
     */
    virtual auto visit(const Newtype &netype) -> void override;
    /**
     * @brief   Label visitor
     *
     * @param   label   label to be serialized
     */
    virtual auto visit(const Label &label) -> void override;
    /**
     * @brief   Join visitor
     *
     * @param   join   join to be serialized
     */
    virtual auto visit(const Join &join) -> void override;
    /**
     * @brief   Return visitor
     *
     * @param   ret   return to be serialized
     */
    virtual auto visit(const Return &ret) -> void override;
    /**
     * @brief   Answer visitor
     *
     * @param   answer  answer to be serialized
     */
    virtual auto visit(const Answer &answer) -> void override;
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
    virtual auto visit(const Procedure &procedure) -> void override;
    /**
     * @brief   Procedure call visitor
     *
     * @param   procedureCall   procedure call to be serialized
     */
    virtual auto visit(const ProcedureCall &procedureCall) -> void override;

private:
    template<typename T>
    auto exportCollection(const T &collection) -> void;

    IndentingStreamWriter &m_writer;
    Layouter &m_layouter;
};

} // namespace conversion::Sdl
