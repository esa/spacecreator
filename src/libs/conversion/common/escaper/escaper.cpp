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

#include <QRegExp>
#include <utility>

namespace conversion {

QString Escaper::escapeIvName(QString name)
{
    QString newName = std::move(name);

    // remove leading & trailing spaces
    newName = newName.trimmed();

    // remove leading numbers
    newName.remove(QRegExp("^[0-9]*"));

    // all non-ascii to ascii
    // todo

    // replace '-' with '_'
    newName.replace(newName.indexOf("-"), 1U, '_');

    if (!newName.contains("_")) {
        // replace first encountered space with '_'
        newName.replace(newName.indexOf(" "), 1U, '_');
    }

    // remove remaining spaces
    newName.remove(" ");

    // remove non-[letters | numbers | _]
    newName.remove(QRegExp("[^a-zA-Z_0-9]"));

    return newName;
}

} // namespace conversion
