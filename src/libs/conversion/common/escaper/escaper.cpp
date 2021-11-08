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

#include "exceptions.h"

#include <QChar>
#include <QRegExp>
#include <utility>

namespace conversion {

QString Escaper::escapeIvName(QString name)
{
    name = name.trimmed();
    removeLeadingNonletters(name);
    replaceDelimetersWithOne(name, '_', { '_', '-', ' ' });

    name.remove(QRegExp("[^a-zA-Z0-9_]"));

    if (name.isEmpty()) {
        throw EscaperException();
    }

    return name;
}

QString Escaper::escapeAsn1TypeName(QString name)
{
    name = name.trimmed();
    removeLeadingNonletters(name);
    replaceDelimetersWithOne(name, '-', { '_', '-', ' ' });

    name.remove(QRegExp("[^a-zA-Z0-9\\-]"));

    if (name.isEmpty()) {
        throw EscaperException();
    }

    name[0] = name[0].toUpper();

    return name;
}

QString Escaper::escapeAsn1FieldName(QString name)
{
    name = name.trimmed();
    removeLeadingNonletters(name);
    replaceDelimetersWithOne(name, '-', { '_', '-', ' ' });

    name.remove(QRegExp("[^a-zA-Z0-9\\-]"));

    if (name.isEmpty()) {
        throw EscaperException();
    }

    name[0] = name[0].toLower();

    return name;
}

void Escaper::replaceFirstOccurence(QString &name, QChar before, QChar after)
{
    name.replace(name.indexOf(before), 1U, after);
}

void Escaper::removeLeadingNonletters(QString &name)
{
    name.remove(QRegExp("^[0-9 \\-_]*"));
}

bool Escaper::isCharInVector(const QChar &c, std::vector<QChar> delimeters)
{
    return std::any_of(delimeters.begin(), delimeters.end(), [&](auto del) -> bool { return (del == c); });
}

void Escaper::replaceDelimetersWithOne(QString &name, const QChar dstDelimeter, const std::vector<QChar> &srcDelimeters)
{
    int howManyDelimeters = 0;
    for (int i = 0; i < name.size(); i++) {
        if (isCharInVector(name.at(i), srcDelimeters)) {
            howManyDelimeters++;
        } else if (howManyDelimeters > 0) {
            name.replace(i - howManyDelimeters, howManyDelimeters, dstDelimeter);
            howManyDelimeters = 0;
        }
    }
}

} // namespace conversion
