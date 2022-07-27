/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "observertype.h"

#include <msccore/mscchart.h>
#include <sdl/SdlModel/sdlmodel.h>

namespace conversion::sdl::translator {

class NeverObserverTranslator final
{
public:
    NeverObserverTranslator(::sdl::SdlModel *sdlModel);

    NeverObserverTranslator(const NeverObserverTranslator &) = delete;
    NeverObserverTranslator(NeverObserverTranslator &&) = delete;

    NeverObserverTranslator &operator=(const NeverObserverTranslator &) = delete;
    NeverObserverTranslator &operator=(NeverObserverTranslator &&) = delete;

public:
    auto createObserver(const msc::MscChart *mscChart) const -> void;

private:
    struct Context {
        QString processName;
        ::sdl::StateMachine *stateMachine;
        ::sdl::State *startState;
        ::sdl::State *lastState;
        std::size_t stateCounter;
    };

private:
    auto createSdlSkeleton(const msc::MscChart *mscChart) const -> Context;

    auto handleEvent(NeverObserverTranslator::Context &context, const msc::MscInstanceEvent *mscEvent) const -> void;
    auto handleMessageEvent(NeverObserverTranslator::Context &context, const msc::MscMessage *mscEvent) const -> void;

private:
    inline static const QString m_stateNameTemplate = "s%1";

    ::sdl::SdlModel *m_sdlModel;
};

} // namespace conversion::sdl::translator
