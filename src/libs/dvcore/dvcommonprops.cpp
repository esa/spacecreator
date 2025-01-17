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
    { "Function", Token::Function },
    { "Message", Token::Message },

    // attrs:
    { "name", Token::name },
    { "value", Token::value },
    { "url", Token::url },
    { "type", Token::type },
    { "encoding", Token::encoding },
    { "path", Token::path },
    { "requirement_ids", Token::requirement_ids },
    { "review_ids", Token::review_ids },

    { "port", Token::port },
    { "bus", Token::bus },
    { "asn1file", Token::asn1file },
    { "asn1type", Token::asn1type },
    { "asn1module", Token::asn1module },
    { "node_label", Token::node_label },

    { "from_node", Token::from_node },
    { "from_port", Token::from_port },
    { "to_bus", Token::to_bus },
    { "to_node", Token::to_node },
    { "to_port", Token::to_port },
    { "requires_bus_access", Token::requires_bus_access },
    { "packetizer", Token::packetizer },
    { "requiresBusAccess", Token::requiresBusAccess },
    { "config", Token::config },
    { "from_function", Token::from_function },
    { "from_interface", Token::from_interface },
    { "to_function", Token::to_function },
    { "to_interface", Token::to_interface },
    { "selected_implementation", Token::selected_implementation },

    // TASTE props:
    { "Taste::coordinates", Token::coordinates },

    // Boards listing
    { "Boards", Token::Boards },
    { "Board", Token::Board },
    { "Port", Token::Port },
    { "System_Function", Token::System_Function },
    { "Provided_Interface", Token::Provided_Interface },
    { "Required_Interface", Token::Required_Interface },
    { "Input_Parameter", Token::Input_Parameter },
    { "Output_Parameter", Token::Output_Parameter },
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
