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

#include <conversion/common/options.h>
#include <memory>
#include <msccore/mscchart.h>
#include <sdl/SdlModel/rename.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <vector>

namespace conversion::sdl::translator {

/**
 * @brief   Translator for MSC "Never" charts
 */
class NeverObserverTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sdlModel    Parent SDL model
     * @param   options     Conversion options
     */
    NeverObserverTranslator(::sdl::SdlModel *sdlModel, const Options &options);

    /**
     * @brief   Deleted copy constuctor
     */
    NeverObserverTranslator(const NeverObserverTranslator &) = delete;
    /**
     * @brief   Deleted move constuctor
     */
    NeverObserverTranslator(NeverObserverTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    NeverObserverTranslator &operator=(const NeverObserverTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    NeverObserverTranslator &operator=(NeverObserverTranslator &&) = delete;

public:
    /**
     * @brief   Create observer from given MSC chart and add it to the SDL model
     *
     * @param   mscChart    Chart to translatr
     */
    auto createObserver(const msc::MscChart *mscChart) const -> void;

private:
    struct Context {
        ::sdl::Process process;
        ::sdl::StateMachine *stateMachine;
        ::sdl::State *startState;
        ::sdl::State *lastState;
        std::vector<std::unique_ptr<::sdl::Rename>> signalRenames;
        std::size_t stateCounter;
    };

private:
    auto handleEvent(NeverObserverTranslator::Context &context, const msc::MscInstanceEvent *mscEvent) const -> void;
    auto handleMessageEvent(NeverObserverTranslator::Context &context, const msc::MscMessage *mscMessage) const -> void;

    auto createSdlSkeleton(const msc::MscChart *mscChart) const -> Context;
    auto createSdlSystem(NeverObserverTranslator::Context &context) const -> ::sdl::System;

private:
    inline static const QString m_stateNameTemplate = "s%1";
    inline static const QString m_signalRenameNameTemplate = "sig%1";
    inline static const QString m_defaultChannelName = "c";
    inline static const QString m_defaultRouteName = "r";

    ::sdl::SdlModel *m_sdlModel;
    const Options &m_options;
};

} // namespace conversion::sdl::translator
