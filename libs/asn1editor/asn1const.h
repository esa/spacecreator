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

#ifndef ASN1CONST_H
#define ASN1CONST_H

#include <QString>
#include <Qt>

namespace asn1 {

const int ASN1TYPE_ROLE = Qt::UserRole;
const int MIN_RANGE_ROLE = Qt::UserRole + 1;
const int MAX_RANGE_ROLE = Qt::UserRole + 2;
const int CHOICE_LIST_ROLE = Qt::UserRole + 3;
const int OPTIONAL_ROLE = Qt::UserRole + 4;

const int MODEL_NAME_INDEX = 0;
const int MODEL_TYPE_INDEX = 1;
const int MODEL_VALUE_INDEX = 2;
const int MODEL_IS_OPTIONAL_INDEX = 3;

const QString ASN1_NAME = "name";
const QString ASN1_IS_OPTIONAL = "isOptional";
const QString ASN1_TYPE = "type";
const QString ASN1_MIN = "min";
const QString ASN1_MAX = "max";

enum ASN1Type
{
    INTEGER,
    DOUBLE,
    BOOL,
    SEQUENCE,
    SEQUENCEOF,
    ENUMERATED,
    CHOICE,
    STRING
};

const QString ASN1_VALUES = "values";
const QString ASN1_CHOICES = "choices";
const QString ASN1_CHILDREN = "children";
const QString ASN1_SEQOFTYPE = "seqoftype";
}
#endif // ASN1CONST_H
