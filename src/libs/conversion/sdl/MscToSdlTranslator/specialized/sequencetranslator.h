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

#include "tftable.h"

#include <conversion/common/options.h>
#include <memory>
#include <sdl/SdlModel/sdlmodel.h>
#include <vector>

namespace conversion::sdl::translator {

class SequenceTranslator
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sdlModel    Parent SDL model
     * @param   options     Conversion options
     */
    SequenceTranslator(::sdl::SdlModel *sdlModel, const Options &options);

    /**
     * @brief   Deleted copy constuctor
     */
    SequenceTranslator(const SequenceTranslator &) = delete;
    /**
     * @brief   Deleted move constuctor
     */
    SequenceTranslator(SequenceTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    SequenceTranslator &operator=(const SequenceTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    SequenceTranslator &operator=(SequenceTranslator &&) = delete;

protected:
    using StateList = std::vector<std::unique_ptr<::sdl::State>>;
    using TransitionList = std::vector<std::unique_ptr<::sdl::Transition>>;

    auto createSdlProcess(const QString &chartName, std::unique_ptr<::sdl::StateMachine> stateMachine)
            -> ::sdl::Process;
    auto createSdlSystem(const QString &chartName, ::sdl::Process process) -> ::sdl::System;

    auto createStates(const uint32_t stateCount) const -> StateList;
    auto createTransitions(const TFTable &table, StateList &states, const uint32_t startStateId) const
            -> TransitionList;
    auto createTransitionOnInput(const QString &signalName, ::sdl::State *sourceState,
            const ::sdl::State *targetState) const -> std::unique_ptr<::sdl::Transition>;

protected:
    inline static const QString m_observerNameThen = "then";
    inline static const QString m_observerNameThenNot = "then not";
    inline static const QString m_stateNameTemplate = "s%1";
    inline static const QString m_signalRenameNameTemplate = "sig%1";
    inline static const QString m_errorStateName = "sErr";
    inline static const QString m_anySignalName = "*";
    inline static const QString m_defaultChannelName = "c";
    inline static const QString m_defaultRouteName = "r";

    ::sdl::SdlModel *m_sdlModel;
    const Options &m_options;
};

} // namespace conversion::sdl::translator
