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

#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <seds/SedsModel/base/description.h>

namespace conversion::iv::translator::seds {

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
     * @brief   Translate SEDS description to IV function comment
     *
     * @param   sedsDescription     Description to translate
     * @param   ivFunction          Function that should contain the comment
     */
    static auto translate(const ::seds::model::Description &sedsDescription, ivm::IVFunction *ivFunction) -> void;
    /**
     * @brief   Translate SEDS description to IV interface comment
     *
     * @param   sedsDescription     Description to translate
     * @param   ivFunction          Interface that should contain the comment
     */
    static auto translate(const ::seds::model::Description &sedsDescription, ivm::IVInterface *ivInterface) -> void;

private:
    static auto combineDescriptions(const ::seds::model::Description &sedsDescription) -> QString;
};

} // namespace conversion::iv::translator::seds
