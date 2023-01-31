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

#include "interfaces/interfacecommandmode.h"

namespace seds::model {

template<>
auto enumFromString(const QStringView &enumStr) -> std::optional<InterfaceCommandMode>
{
    if (enumStr.compare(QStringLiteral("sync"), Qt::CaseInsensitive) == 0) {
        return InterfaceCommandMode::Sync;
    } else if (enumStr.compare(QStringLiteral("async"), Qt::CaseInsensitive) == 0) {
        return InterfaceCommandMode::Async;
    } else {
        return std::nullopt;
    }
}

template<>
auto stringFromEnum(InterfaceCommandMode enumType) -> const QString &
{
    switch (enumType) {
    case InterfaceCommandMode::Sync: {
        static const QString sync = "sync";
        return sync;
    }
    case InterfaceCommandMode::Async: {
        static const QString async = "async";
        return async;
    }
    default: {
        static const QString unhandled = "unhandledCommandMode";
        return unhandled;
    }
    }
}

} // namespace seds::model
