/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "common.h"

#include <asn1library/asn1/asn1model.h>
#include <ivcore/ivmodel.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <seds/SedsModel/sedsmodel.h>

namespace conversion {
class Options;
} // namespace conversion

namespace conversion::sdl::translator {

/**
 *  @brief Translator of SEDS state machines into SDL state machines
 */
class ActivityTranslator final
{
public:
    /**
     * @brief   Translate the given SEDS activity into SDL procedure and add it to the target process
     *
     * @param context           Translation context
     * @param sedsStateMachine  Source SEDS activity
     * @param sedsActivity      Source SEDS activity
     * @param sdlProcess        Target SDL process
     */
    static auto translateActivity(Context &context, const seds::model::Activity &sedsActivity,
            ::sdl::Process *sdlProcess, const Options &options) -> void;

private:
    static auto translateBody(Context &context, const seds::model::Activity &sedsActivity, ::sdl::Process *sdlProcess,
            ::sdl::Procedure *procedure, const Options &options) -> void;
};

}
