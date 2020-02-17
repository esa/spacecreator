/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "aadlobjecttemplateproperty.h"

#include "../tab_aadl/aadlcommonprops.h"

namespace taste3 {
namespace templating {

/**
 * @brief generateTemplateProperties generates a variant list sorted by meta::Props::Token.
 * @param props can be hash of attributes or properties of AADLObject.
 * @return sorted QVariantList which can be used in string templates
 */
QVariantList generateTemplateProperties(const QHash<QString, QVariant> &props)
{
    QVariantList result;
    for (auto it = props.cbegin(); it != props.cend(); ++it)
        result << QVariant::fromValue(AADLObjectTemplateProperty(it.key(), it.value()));

    std::sort(result.begin(), result.end(), [] (const QVariant &left_val, const QVariant &right_val) {
        const AADLObjectTemplateProperty r = right_val.value<AADLObjectTemplateProperty>();
        const aadl::meta::Props::Token right_token = aadl::meta::Props::token(r.name());
        if (right_token == aadl::meta::Props::Token::Unknown)
            return true;

        const AADLObjectTemplateProperty l = left_val.value<AADLObjectTemplateProperty>();
        aadl::meta::Props::Token left_token = aadl::meta::Props::token(l.name());
        if (left_token == aadl::meta::Props::Token::Unknown)
            return false;

        return left_token < right_token;
    });

    return result;
}

} // ns templating
} // ns taste3
