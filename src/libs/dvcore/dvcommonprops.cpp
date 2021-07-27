/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvcommonprops.h"

namespace dvm {
namespace meta {

const QHash<QString, Props::Token> Props::TokensByName = {
    { "DeploymentView", Token::DeploymentView },

    // tags:
    { "Property", Token::Property },

    { "Node", Token::Node },
    { "Partition", Token::Partition },
    { "Device", Token::Device },
    { "Connection", Token::Connection },
    { "function", Token::Function },
    { "Message", Token::Message },

    // attrs:
    { "name", Token::name },
    { "value", Token::value },
    { "port", Token::port },
    { "bus", Token::bus },
    { "asn1file", Token::asn1file },
    { "asn1type", Token::asn1type },
    { "asn1module", Token::asn1module },

    { "from_node", Token::from_node },
    { "from_port", Token::from_port },
    { "to_bus", Token::to_bus },
    { "to_node", Token::to_node },
    { "to_port", Token::to_port },
    { "requires_bus_access", Token::requires_bus_access },
    { "requiresBusAccess", Token::requiresBusAccess },
    { "config", Token::config },
    { "from_function", Token::from_function },
    { "to_function", Token::to_function },

    // TASTE props:
    { "Taste::coordinates", Token::coordinates },

    // Boards listing
    { "Boards", Token::Boards },
    { "Board", Token::Board },
    { "Port", Token::Port },
};

Props::Token Props::token(const QString &fromString)
{
    return TokensByName.value(fromString, Token::Unknown);
}

QString Props::token(Props::Token tag)
{
    return TokensByName.key(tag);
}

}
}
