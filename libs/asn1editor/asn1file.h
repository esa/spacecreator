/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#ifndef ASN1FILE_H
#define ASN1ILE_H

#include <QString>

namespace antlr4 {
class ANTLRInputStream;
}

namespace msc {
class Asn1ItemModel;

class Asn1File
{
public:
    Asn1File();

    Asn1ItemModel *parseFile(const QString &filename, QStringList *errorMessages = nullptr);
    Asn1ItemModel *parseText(const QString &text, QStringList *errorMessages = nullptr);

    QStringList getErrorMessages() const;

private:
    Asn1ItemModel *parse(antlr4::ANTLRInputStream &input, QStringList *errorMessages = nullptr);
};

} // namespace msc

#endif // ASN1FILE_H
