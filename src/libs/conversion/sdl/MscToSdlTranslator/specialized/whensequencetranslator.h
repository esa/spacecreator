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

namespace conversion::sdl::translator {

/**
 * @brief   Translator for MSC "When-Then" and 'When-ThenNot" charts
 */
class WhenSequenceTranslator final : public SequenceTranslator
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sdlModel            Parent SDL model
     * @param   observerAsn1File    ASN.1 with observer dataview
     * @param   ivModel             IV model
     * @param   options             Conversion options
     */
    WhenSequenceTranslator(::sdl::SdlModel *sdlModel, const Asn1Acn::File *observerAsn1File, const ivm::IVModel *ivModel, const Options &options);

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
    enum class Mode
    {
        When,
        Then,
        ThenNot
    };

    struct Context {
        QString chartName;
        std::vector<uint32_t> whenSequence;
        std::vector<uint32_t> thenSequence;
        std::unordered_map<uint32_t, SignalInfo> signals;
        ::sdl::State *endState;
        std::size_t signalCounter;
        Mode mode;
    };

private:
    auto collectData(const msc::MscChart *mscChart) const -> WhenSequenceTranslator::Context;

    auto handleEvent(Context &context, const msc::MscInstanceEvent *mscEvent) const -> void;
    auto handleConditionEvent(Context &context, const msc::MscCondition *mscCondition) const -> void;
    auto handleMessageEvent(Context &context, const msc::MscMessage *mscMessage) const -> void;

    auto createStateMachine(Context &context) const -> std::unique_ptr<::sdl::StateMachine>;
    auto createThenTransitions(Context &context, StateList &states, const uint32_t startStateId) const
            -> TransitionList;
    auto createThenNotTransitions(Context &context, StateList &states, const uint32_t startStateId) const
            -> TransitionList;
};

} // namespace conversion::sdl::translator
