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
#include <msccore/mscchart.h>
#include <sdl/SdlModel/rename.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <vector>

namespace conversion::sdl::translator {

class WhenSequenceTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sdlModel    Parent SDL model
     * @param   options     Conversion options
     */
    WhenSequenceTranslator(::sdl::SdlModel *sdlModel, const Options &options);

    /**
     * @brief   Deleted copy constuctor
     */
    WhenSequenceTranslator(const WhenSequenceTranslator &) = delete;
    /**
     * @brief   Deleted move constuctor
     */
    WhenSequenceTranslator(WhenSequenceTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    WhenSequenceTranslator &operator=(const WhenSequenceTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    WhenSequenceTranslator &operator=(WhenSequenceTranslator &&) = delete;

public:
    /**
     * @brief   Create observer from given MSC chart and add it to the SDL model
     *
     * @param   mscChart    Chart to translatr
     */
    auto createObserver(const msc::MscChart *mscChart) -> void;

private:
    struct Context {
        QString chartName;
        std::vector<uint32_t> sequence;
        std::unordered_map<uint32_t, std::unique_ptr<::sdl::Rename>> signals;
        std::size_t signalCounter;
    };

private:
    using StateList = std::vector<std::unique_ptr<::sdl::State>>;
    using TransitionList = std::vector<std::unique_ptr<::sdl::Transition>>;

    auto collectData(const msc::MscChart *mscChart) const -> WhenSequenceTranslator::Context;

    auto handleEvent(Context &context, const msc::MscInstanceEvent *mscEvent) const -> void;
    auto handleMessageEvent(Context &context, const msc::MscMessage *mscMessage) const -> void;

    auto createSdlSystem(Context &context) -> void;
    auto createSdlProcess(const Context &context) -> ::sdl::Process;
    auto createStateMachine(const Context &context) const -> std::unique_ptr<::sdl::StateMachine>;

    auto createStates(const uint32_t stateCount) const -> StateList;
    auto createTransitions(const TFTable &table, StateList &states) const -> TransitionList;

private:
    inline static const QString m_stateNameTemplate = "s%1";
    inline static const QString m_signalRenameNameTemplate = "sig%1";
    inline static const QString m_defaultChannelName = "c";
    inline static const QString m_defaultRouteName = "r";

    ::sdl::SdlModel *m_sdlModel;
    const Options &m_options;
};

} // namespace conversion::sdl::translator
