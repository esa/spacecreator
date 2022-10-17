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

#include "transition.h"

#include <exception>
#include <memory>
#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

const std::vector<std::unique_ptr<Action>> &Transition::actions()
{
    return m_actions;
}

const Action *Transition::lastAction() const
{
    if (m_actions.empty()) {
        return nullptr;
    }

    return m_actions.back().get();
}

void Transition::addAction(std::unique_ptr<Action> action)
{
    if (!m_actions.empty()) {
        auto &lastAddedAction = m_actions.back();
        if (dynamic_cast<Join *>(lastAddedAction.get()) != nullptr) {
            throw std::logic_error("Adding action after Join is illegal");
        }
        if (dynamic_cast<NextState *>(lastAddedAction.get()) != nullptr) {
            throw std::logic_error("Adding action after NextState is illegal");
        }
    }

    m_actions.push_back(std::move(action));
}

bool Transition::removeLastAction()
{
    if (m_actions.empty()) {
        return false;
    }

    m_actions.pop_back();

    return true;
}

const QString &Transition::comment() const
{
    return m_comment;
}

void Transition::setComment(QString comment)
{
    m_comment = std::move(comment);
}

} // namespace sdl
