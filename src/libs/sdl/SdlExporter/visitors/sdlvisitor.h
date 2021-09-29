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

#include "visitor.h"

#include <QTextStream>
#include <sdl/SdlModel/input.h>
#include <sdl/SdlModel/process.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <sdl/SdlModel/signal.h>
#include <sdl/SdlModel/state.h>
#include <sdl/SdlModel/transition.h>

namespace sdl {

/**
 * @brief   Visitor serializing components of the SDL model
 */
class SdlVisitor : public Visitor
{
public:
    /**
     * @brief   Constructor
     *
     * @param   stream   output stream (where the serialized values are put)
     */
    SdlVisitor(QTextStream &stream);
    SdlVisitor(const QTextStream &&) = delete;
    auto visit(const SdlModel &model) const -> void override;
    auto visit(const Process &process) const -> void override;
    auto visit(const State &state) const -> void override;
    auto visit(const Input &input) const -> void override;
    auto visit(const Signal &sig) const -> void override;
    auto visit(const Transition &transition) const -> void override;
    auto visit(const Action &action) const -> void override;

private:
    template<typename T>
    auto exportCollection(const T &collection) const -> void;

    QTextStream &m_stream;
};

} // namespace conversion::Sdl
