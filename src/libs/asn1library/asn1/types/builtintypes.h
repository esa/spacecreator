/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a program and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#pragma once

#include "type.h"

#include <QString>
#include <QVariantList>

namespace Asn1Acn {
namespace Types {

class BuiltinType : public Type
{
public:
    BuiltinType(const QString &identifier = QString())
        : Type(identifier)
    {
    }

    static std::unique_ptr<Type> createBuiltinType(const QString &typeName);

    QString label() const override { return QLatin1String(": ") + typeName(); }
};

class Boolean : public BuiltinType
{
public:
    Boolean(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("BOOLEAN"); }

    ASN1Type typeEnum() const override { return BOOLEAN; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/boolean.png"); }
};

class Null : public BuiltinType
{
public:
    Null(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("NULL"); }

    ASN1Type typeEnum() const override { return NULLTYPE; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/null.png"); }
};

class Integer : public BuiltinType
{
public:
    Integer(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("INTEGER"); }

    ASN1Type typeEnum() const override { return INTEGER; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/integer.png"); }
};

class Real : public BuiltinType
{
public:
    Real(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("REAL"); }

    ASN1Type typeEnum() const override { return REAL; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/real.png"); }
};

class BitString : public BuiltinType
{
public:
    BitString(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("BIT STRING"); }

    ASN1Type typeEnum() const override { return BITSTRING; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/bitstring.png"); }
};

class OctetString : public BuiltinType
{
public:
    OctetString(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("OCTET STRING"); }

    ASN1Type typeEnum() const override { return OCTETSTRING; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/octetstring.png"); }
};

class IA5String : public BuiltinType
{
public:
    IA5String(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("IA5String"); }

    ASN1Type typeEnum() const override { return IA5STRING; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/ia5string.png"); }
};

class NumericString : public BuiltinType
{
public:
    NumericString(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("NumericString"); }

    ASN1Type typeEnum() const override { return NUMERICSTRING; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/numericstring.png"); }
};

class Enumerated : public BuiltinType
{
public:
    Enumerated(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("ENUMERATED"); }

    ASN1Type typeEnum() const override { return ENUMERATED; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/enumerated.png"); }

    QVariantList enumValues() const
    {
        if (m_parameters.contains("values")) {
            return m_parameters["values"].toList();
        }
        return {};
    }
};

class Choice : public BuiltinType
{
public:
    Choice(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("CHOICE"); }

    ASN1Type typeEnum() const override { return CHOICE; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/choice.png"); }
};

class Sequence : public BuiltinType
{
public:
    Sequence(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("SEQUENCE"); }

    ASN1Type typeEnum() const override { return SEQUENCE; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/sequence.png"); }
};

class SequenceOf : public BuiltinType
{
public:
    SequenceOf(const QString &identifier = QString())
        : BuiltinType(identifier)
    {
    }

    QString typeName() const override { return QLatin1String("SEQUENCE OF"); }

    ASN1Type typeEnum() const override { return SEQUENCEOF; }

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/sequenceof.png"); }
};

}
}
