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

#include "ivcommonprops.h"

namespace ivm {
namespace meta {

const QHash<QString, Props::Token> Props::TokensByName = {
    { "InterfaceView", Token::InterfaceView },

    // tags:
    { "Function", Token::Function },
    { "Property", Token::Property },
    { "ContextParameter", Token::ContextParameter },
    { "Provided_Interface", Token::Provided_Interface },
    { "Input_Parameter", Token::Input_Parameter },
    { "Output_Parameter", Token::Output_Parameter },
    { "Required_Interface", Token::Required_Interface },
    { "Connection", Token::Connection },
    { "ConnectionGroup", Token::ConnectionGroup },
    { "Comment", Token::Comment },
    { "Source", Token::Source },
    { "Target", Token::Target },
    { "Implementations", Token::Implementations },
    { "Implementation", Token::Implementation },
    { "Layer", Token::Layer },
    { "ArchetypeReference", Token::ArchetypeReference },
    { "ArchetypeLibraryReference", Token::ArchetypeLibraryReference },

    // properties values
    { "QGenC", Token::QGenC },
    { "Full", Token::Full },

    // attrs:
    { "name", Token::name },
    { "comment", Token::comment },
    { "url", Token::url },
    { "is_type", Token::is_type },
    { "instance_of", Token::instance_of },
    { "value", Token::value },
    { "kind", Token::kind },
    { "type", Token::type },
    { "encoding", Token::encoding },
    { "ri_name", Token::ri_name },
    { "pi_name", Token::pi_name },
    { "group_name", Token::group_name },
    { "func_name", Token::func_name },
    { "is_visible", Token::is_visible },
    { "language", Token::language },
    { "type_language", Token::type_language },
    { "default_implementation", Token::default_implementation },
    { "enable_multicast", Token::enable_multicast },
    { "layer", Token::layer },
    { "archetype_library", Token::archetype_library },
    { "archetype_function", Token::archetype_function },
    { "path", Token::path },
    { "fixed_system_element", Token::fixed_system_element },
    { "required_system_element", Token::required_system_element },
    { "is_simulink_interface", Token::is_simulink_interface },
    { "simulink_interface_type", Token::simulink_interface_type },
    { "simulink_full_interface_ref", Token::simulink_full_interface_ref },
    { "iface_id", Token::iface_id },
    { "reference", Token::reference },
    { "requirement_ids", Token::requirement_ids },
    { "review_ids", Token::review_ids },

    // TASTE props:
    { "Taste::coordinates", Token::coordinates },
    { "Taste::RootCoordinates", Token::RootCoordinates },
    { "Taste::InheritPI", Token::InheritPI },
    { "Taste::Autonamed", Token::Autonamed },
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
