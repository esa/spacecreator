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

#pragma once

#include <QHash>
#include <QString>

namespace taste3 {
namespace aadl {
namespace meta {

enum class Token
{
    Unknown = 0,
    InterfaceView,

    // tags:
    Function,
    Property,
    Provided_Interface,
    Input_Parameter,
    Output_Parameter,
    Required_Interface,
    Connection,

    // attrs:
    name,
    language,
    is_type,
    instance_of,
    value,
    kind,
    period,
    wcet,
    queue_size,
    type,
    encoding,
    from,
    ri_name,
    to,
    pi_name,

    // TASTE props:
    Active_Interfaces,
    coordinates,
    Deadline,
    InterfaceName,
    RCMoperationKind,
    RCMperiod,
    labelInheritance
};

static const QHash<QString, Token> TokensByName = { { "InterfaceView", Token::InterfaceView },

                                                    // tags:
                                                    { "Function", Token::Function },
                                                    { "Property", Token::Property },
                                                    { "Provided_Interface", Token::Provided_Interface },
                                                    { "Input_Parameter", Token::Input_Parameter },
                                                    { "Output_Parameter", Token::Output_Parameter },
                                                    { "Required_Interface", Token::Required_Interface },
                                                    { "Connection", Token::Connection },

                                                    // attrs:
                                                    { "name", Token::name },
                                                    { "language", Token::language },
                                                    { "is_type", Token::is_type },
                                                    { "instance_of", Token::instance_of },
                                                    { "value", Token::value },
                                                    { "kind", Token::kind },
                                                    { "period", Token::period },
                                                    { "wcet", Token::wcet },
                                                    { "queue_size", Token::queue_size },
                                                    { "type", Token::type },
                                                    { "encoding", Token::encoding },
                                                    { "from", Token::from },
                                                    { "ri_name", Token::ri_name },
                                                    { "to", Token::to },
                                                    { "pi_name", Token::pi_name },

                                                    // TASTE props:
                                                    { "Taste::Active_Interfaces", Token::Active_Interfaces },
                                                    { "Taste::coordinates", Token::coordinates },
                                                    { "Taste::Deadline", Token::Deadline },
                                                    { "Taste::InterfaceName", Token::InterfaceName },
                                                    { "Taste::RCMoperationKind", Token::RCMoperationKind },
                                                    { "Taste::RCMperiod", Token::RCMperiod },
                                                    { "Taste::labelInheritance", Token::labelInheritance } };

static Token token(const QString &fromString)
{
    return TokensByName.value(fromString, Token::Unknown);
}

static QString token(Token tag)
{
    return TokensByName.key(tag);
}

} // meta
} // ns aadl
} // ns taste3
