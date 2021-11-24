/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "interfaces/commandargumentmode.h"

namespace seds::model {

template<>
auto enumFromString(const QStringRef &enumStr) -> std::optional<CommandArgumentMode>
{
    if (enumStr.compare(QStringLiteral("in"), Qt::CaseInsensitive) == 0) {
        return CommandArgumentMode::In;
    } else if (enumStr.compare(QStringLiteral("out"), Qt::CaseInsensitive) == 0) {
        return CommandArgumentMode::Out;
    } else if (enumStr.compare(QStringLiteral("inout"), Qt::CaseInsensitive) == 0) {
        return CommandArgumentMode::InOut;
    } else if (enumStr.compare(QStringLiteral("notify"), Qt::CaseInsensitive) == 0) {
        return CommandArgumentMode::Notify;
    } else {
        return std::nullopt;
    }
}

template<>
auto stringFromEnum(CommandArgumentMode mode) -> const QString &
{
    switch (mode) {
    case CommandArgumentMode::In: {
        static const QString in = "in";
        return in;
    }
    case CommandArgumentMode::Out: {
        static const QString out = "out";
        return out;
    }
    case CommandArgumentMode::InOut: {
        static const QString inOut = "inOut";
        return inOut;
    }
    case CommandArgumentMode::Notify: {
        static const QString notify = "notify";
        return notify;
    }
    }
}

} // namespace seds::model
