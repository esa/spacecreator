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

#include "archetypecommonprops.h"

namespace ivm {
namespace meta {

const QHash<QString, ArchetypeProps::Token> ArchetypeProps::TokensByName = {
    { "ArchetypeLibrary", Token::ArchetypeLibrary },

    // tags:
    { "ParameterArchetype", Token::ParameterArchetype },
    { "InterfaceArchetype", Token::InterfaceArchetype },
    { "FunctionArchetype", Token::FunctionArchetype },

    // attrs:
    { "name", Token::name },
    { "value", Token::value },
    { "type", Token::type },
    { "interfaceType", Token::interfaceType },
    { "kind", Token::kind },
    { "direction", Token::direction },
    { "layer", Token::layer },
};

ArchetypeProps::Token ArchetypeProps::token(const QString &fromString)
{
    return TokensByName.value(fromString, Token::Unknown);
}

QString ArchetypeProps::token(ArchetypeProps::Token tag)
{
    return TokensByName.key(tag);
}

}
}
