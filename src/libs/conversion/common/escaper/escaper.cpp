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
#include <algorithm>
#include <utility>

namespace conversion {

QString Escaper::escapeSdlName(QString name)
{
    escapeName(name, '_');

    return name;
}

QString Escaper::escapeIvName(QString name)
{
    escapeName(name, '_');

    return name;
}

QString Escaper::escapeAsn1TypeName(QString name)
{
    escapeName(name, '-');

    name[0] = name[0].toUpper(); // type name starts with uppercase letter in ASN.1

    return name;
}

QString Escaper::escapeAsn1PackageName(QString name)
{
    replaceDelimeters(name,
            {
                    '/',
                    '\\',
            },
            '-');
    escapeName(name, '-');

    name = name.toUpper();

    return name;
}

QString Escaper::escapeAsn1FieldName(QString name)
{
    escapeName(name, '-');

    name[0] = name[0].toLower(); // sequence field name starts with lowercase letter in ASN.1

    return name;
}

void Escaper::escapeName(QString &name, const QChar &delimeter)
{
    name = name.trimmed();
    removeLeadingNonletters(name);
    replaceDelimeters(name, { '_', '-', ' ' }, delimeter);
    removeNonalphanumericCharacters(name, delimeter);

    if (name.isEmpty()) {
        throw EmptyNameException();
    }
}

bool Escaper::isCharInVector(const QChar &c, const std::vector<QChar> &delimeters)
{
    return std::count(delimeters.begin(), delimeters.end(), c) != 0;
}

void Escaper::removeLeadingNonletters(QString &name)
{
    name.remove(QRegExp("^[0-9 \\-_]*"));
}

void Escaper::removeNonalphanumericCharacters(QString &name, const QChar &delimeter)
{
    name.remove(QRegExp(QString("[^a-zA-Z0-9\\%1]").arg(delimeter)));
}

void Escaper::replaceDelimeters(QString &name, const std::vector<QChar> &srcDelimeters, const QChar &dstDelimeter)
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
