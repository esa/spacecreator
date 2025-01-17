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

#include <sdl/SdlModel/node.h>
#include <sdl/SdlModel/transition.h>
#include <seds/SedsModel/base/description.h>
#include <seds/SedsModel/types/datatype.h>

namespace conversion::sdl::translator {

/**
 * @brief   Translator for SEDS short and long descriptions
 */
class DescriptionTranslator final
{
public:
    /**
     * @brief   Deleted constructor
     */
    DescriptionTranslator() = delete;

public:
    /**
     * @brief   Translate SEDS description to SDL comment
     *
     * @param   sedsDescription     Description to translate
     * @param   sdlNode             Node that should contain the comment
     */
    static auto translate(const seds::model::Description &sedsDescription, ::sdl::Node *sdlNode) -> void;
    /**
     * @brief   Translate SEDS description to SDL comment
     *
     * @param   sedsDescription     Data type containing description
     * @param   sdlTransition       Transition that should contain the comment
     */
    static auto translate(const seds::model::Description &sedsDescription, ::sdl::Transition *sdlTransition) -> void;

private:
    static auto combineDescriptions(const seds::model::Description &sedsDescription) -> QString;
};

} // namespace conversion::asn1::translator
