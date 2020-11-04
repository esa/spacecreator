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
#include <QObject>
#include <QString>

namespace aadl {
namespace meta {

struct Props {
    Q_GADGET
public:
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
        ConnectionGroup,
        Comment,
        Source,
        Target,

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
        ri_name,
        pi_name,
        group_name,
        func_name,
        is_visible,

        // KNOWN TASTE props:
        coordinates,
        InnerCoordinates,
        RootCoordinates,
        InheritPI,
        Autonamed,
    };
    Q_ENUM(Token)

    static const QHash<QString, Props::Token> TokensByName;

    static Props::Token token(const QString &fromString);

    static QString token(Props::Token tag);
};

}
}
