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

#include <QHash>
#include <QObject>
#include <QString>

namespace ivm {
namespace meta {

struct ArchetypeProps {
    Q_GADGET
public:
    enum class Token
    {
        Unknown = 0,

        // tags:
        ParameterArchetype,
        InterfaceArchetype,
        FunctionArchetype,
        ArchetypeLibrary,
        CommunicationLayerTypes,
        CommunicationLayerType,

        // attrs:
        name,
        value,
        type,
        interfaceType,
        kind,
        direction,
        layer
    };
    Q_ENUM(Token)

    static const QHash<QString, ArchetypeProps::Token> TokensByName;

    static ArchetypeProps::Token token(const QString &fromString);

    static QString token(ArchetypeProps::Token tag);
};

} // namespace meta
} // namespace ivm
