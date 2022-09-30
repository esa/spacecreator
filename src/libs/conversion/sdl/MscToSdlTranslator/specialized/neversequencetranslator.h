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
 * @brief   Translator for MSC "Never" charts
 */
class NeverSequenceTranslator final : public SequenceTranslator
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sdlModel    Parent SDL model
     * @param   asn1File    ASN.1 file
     * @param   ivModel     IV model
     * @param   options     Conversion options
     */
    NeverSequenceTranslator(::sdl::SdlModel *sdlModel, const Asn1Acn::File *asn1File, const ivm::IVModel *ivModel,
            const Options &options);

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
    struct NeverContext : public Context {
        std::vector<uint32_t> sequence;
        ::sdl::State *errorState;
    };

private:
    auto collectData(const msc::MscChart *mscChart) const -> NeverSequenceTranslator::NeverContext;

    auto handleEvent(NeverContext &context, const msc::MscInstanceEvent *mscEvent) const -> void;
    auto handleMessageEvent(NeverContext &context, const msc::MscMessage *mscMessage) const -> void;

    auto createStateMachine(NeverContext &context) const -> std::unique_ptr<::sdl::StateMachine>;
};

} // namespace conversion::sdl::translator
