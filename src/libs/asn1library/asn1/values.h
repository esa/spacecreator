/****************************************************************************
**
** Copyright (C) 2017-2021 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a programme and funded by
** European Space Agency.
**
** This Library is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#pragma once

#include <cstdint>

#include <QString>

namespace Asn1Acn {

struct IntegerValue
{
    using Type = std::int64_t;
    static QString astNodeName() { return QStringLiteral("IntegerValue"); }
    static Type fromAstValue(const QString &value) { return value.toLongLong(); }
    static QString asString(Type t) { return QString::number(t); }
};

struct RealValue
{
    using Type = double;
    static QString astNodeName() { return QStringLiteral("RealValue"); }
    static Type fromAstValue(const QString &value) { return value.toDouble(); }
    static QString asString(Type t) { return QString::number(t); }
};

struct EnumValue
{
    using Type = QString;
    static QString astNodeName() { return QStringLiteral("EnumValue"); }
    static Type fromAstValue(const QString &value) { return value; }
    static QString asString(Type t) { return t; }
};

struct BooleanValue
{
    using Type = bool;
    static QString astNodeName() { return QStringLiteral("BooleanValue"); }
    static Type fromAstValue(const QString &value) { return value.toUpper() == "TRUE"; }
    static QString asString(Type t) { return t ? "TRUE" : "FALSE"; }
    static QString reformatString(const QString &value) { return asString(fromAstValue(value)); }
};

struct StringValue
{
    using Type = QString;
    static QString astNodeName() { return QStringLiteral("StringValue"); }
    static Type fromAstValue(const QString &value) { return value; }
    static QString asString(Type t) { return '"' + t + '"'; }
};

struct BitStringValue
{
    using Type = QString;
    static QString astNodeName() { return QStringLiteral("BitStringValue"); }
    static Type fromAstValue(const QString &value) { return value; }
    static QString asString(Type t) { return "'" + t + "'B"; }
};

struct OctetStringValue
{
    using Type = QString;
    static QString astNodeName() { return QStringLiteral("OctetStringValue"); }
    static Type fromAstValue(const QString &value) { return value; }
    static QString asString(Type t) { return "'" + t + "'H"; }
};

}
