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

#include "specialized/sequencetranslator.h"

#include <msccore/mscchart.h>
#include <sdl/SdlModel/rename.h>

namespace conversion::sdl::translator {

/**
 * @brief   Translator for MSC "Never" charts
 */
class NeverSequenceTranslator final : public SequenceTranslator
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sdlModel    Parent SDL model
     * @param   options     Conversion options
     */
    NeverSequenceTranslator(::sdl::SdlModel *sdlModel, const Options &options);

    /**
     * @brief   Deleted copy constuctor
     */
    NeverSequenceTranslator(const NeverSequenceTranslator &) = delete;
    /**
     * @brief   Deleted move constuctor
     */
    NeverSequenceTranslator(NeverSequenceTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    NeverSequenceTranslator &operator=(const NeverSequenceTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    NeverSequenceTranslator &operator=(NeverSequenceTranslator &&) = delete;

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
        ::sdl::State *errorState;
        std::size_t signalCounter;
    };

private:
    auto collectData(const msc::MscChart *mscChart) const -> NeverSequenceTranslator::Context;

    auto handleEvent(Context &context, const msc::MscInstanceEvent *mscEvent) const -> void;
    auto handleMessageEvent(Context &context, const msc::MscMessage *mscMessage) const -> void;

    auto createStateMachine(Context &context) const -> std::unique_ptr<::sdl::StateMachine>;
};

} // namespace conversion::sdl::translator
