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

#include "specialized/descriptiontranslator.h"

#include <seds/SedsModel/types/arraydatatype.h>
#include <seds/SedsModel/types/binarydatatype.h>
#include <seds/SedsModel/types/booleandatatype.h>
#include <seds/SedsModel/types/containerdatatype.h>
#include <seds/SedsModel/types/enumerateddatatype.h>
#include <seds/SedsModel/types/floatdatatype.h>
#include <seds/SedsModel/types/integerdatatype.h>
#include <seds/SedsModel/types/stringdatatype.h>
#include <seds/SedsModel/types/subrangedatatype.h>

namespace conversion::asn1::translator::seds {

void DescriptionTranslator::translate(const ::seds::model::Description *sedsDescription, Asn1Acn::Node *asn1Node)
{
    if (sedsDescription == nullptr) {
        return;
    }

    auto description = combineDescriptions(sedsDescription);
    asn1Node->setComment(std::move(description));
}

QString DescriptionTranslator::combineDescriptions(const ::seds::model::Description *sedsDescription)
{
    auto description = sedsDescription->shortDescription().value_or("");

    if (sedsDescription->longDescription()) {
        if (!description.isEmpty()) {
            description += "\n";
        }

        description += sedsDescription->longDescription().value();
    }

    return description;
}

} // namespace conversion::asn1::translator::seds
