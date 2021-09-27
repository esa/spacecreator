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
#include <sdl/SdlModel/sdlmodel.h>

namespace sdl {

class SdlVisitor : public Visitor
{
public:
    SdlVisitor(QTextStream &stream);
    SdlVisitor(const QTextStream &&) = delete;
    auto visit(const SdlModel *model) const -> void override;
    auto visit(const Process &process) const -> void override;
    auto visit(const State &state) const -> void override;
    auto visit(const Input &input) const -> void override;

private:
    auto visit(const Process &process) const -> void;
    auto visit(const State &state) const -> void;
    auto visit(const Input &input) const -> void;

    template<typename T>
    auto exportCollection(const T &collection) const -> void;

    QTextStream &m_stream;
};

} // namespace conversion::Sdl
