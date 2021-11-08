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

#include "escaper.h"

#include "../exceptions.h"

#include <QRegExp>
#include <utility>

namespace conversion {

QString Escaper::escapeIvName(QString name)
{
    name = name.trimmed();
    name.remove(QRegExp("^[0-9 \\-_]*"));

    name.replace(name.indexOf("-"), 1U, '_');

    if (!name.contains("_")) {
        name.replace(name.indexOf(" "), 1U, '_');
    }
    name.remove(" ");
    name.remove(QRegExp("[^a-zA-Z_0-9]"));

    if (name.isEmpty()) {
        throw EscaperException();
    }

    return name;
}

QString Escaper::escapeAsn1TypeName(QString name)
{
    name = name.trimmed();
    name.remove(QRegExp("^[0-9 \\-_]*"));

    name.replace(name.indexOf("_"), 1U, '-');

    if (!name.contains("-")) {
        name.replace(name.indexOf(" "), 1U, '-');
    }
    name.remove(" ");
    name.remove(QRegExp("[^a-zA-Z\\-0-9]"));

    if (name.isEmpty()) {
        throw EscaperException();
    }

    name[0] = name[0].toUpper();

    return name;
}

QString Escaper::escapeAsn1FieldName(QString name)
{
    name = name.trimmed();
    name.remove(QRegExp("^[0-9 \\-_]*"));

    name.replace(name.indexOf("_"), 1U, '-');

    if (!name.contains("-")) {
        name.replace(name.indexOf(" "), 1U, '-');
    }
    name.remove(" ");
    name.remove(QRegExp("[^a-zA-Z\\-0-9]"));

    if (name.isEmpty()) {
        throw EscaperException();
    }

    name[0] = name[0].toLower();

    return name;
}

} // namespace conversion
