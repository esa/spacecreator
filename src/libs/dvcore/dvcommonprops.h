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

#pragma once

#include <QHash>
#include <QObject>
#include <QString>

namespace dvm {
namespace meta {

struct Props {
    Q_GADGET
public:
    enum class Token
    {
        Unknown = 0,
        DeploymentView,

        // tags:
        Property,

        Node,
        Partition,
        Device,
        Connection,
        Function,

        Boards,
        Board,
        Port,

        // attrs:
        name,
        value,

        from_node,
        from_port,
        to_bus,
        to_node,
        to_port,
        requires_bus_access,

        requiresBusAccess,

        coverage,
        package,
        cpu,
        cou_platform,
        cpu_classifier,
        vp_name,
        vp_platform,
        vp_classifier,
        ada_runtime,

        pkg,
        classifier,
        proc,
        config,
        bus,
        port,
        asn1file,
        asn1type,
        asn1module,

        // TASTE props:
        coordinates,
    };
    Q_ENUM(Token)

    static const QHash<QString, Props::Token> TokensByName;

    static Props::Token token(const QString &fromString);

    static QString token(Props::Token tag);
};

} // namespace meta
} // namespace dvm
