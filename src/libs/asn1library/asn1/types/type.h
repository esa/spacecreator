/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a programme and funded by
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

#include <QString>
#include <QVariantMap>

namespace Asn1Acn {

class AstXmlParser;

namespace Types {

class Type
{
public:
    enum ASN1Type
    {
        INTEGER,
        REAL,
        BOOLEAN,
        SEQUENCE,
        SEQUENCEOF,
        ENUMERATED,
        CHOICE,
        STRING,
        NULLTYPE,
        BITSTRING,
        IA5STRING,
        NUMERICSTRING,
        OCTETSTRING,
        LABELTYPE,
        USERDEFINED
    };

    Type(const QString &identifier = QString());
    virtual ~Type() = default;

    /*!
       Type as string
     */
    virtual QString typeName() const = 0;
    virtual ASN1Type typeEnum() const = 0;

    virtual QString label() const = 0;

    const QString &identifier() const;

    virtual QString baseIconFile() const = 0;

    /// Data values of this ASN1 type (like min/max for numbers)
    QVariantMap m_values;

private:
    void setIdentifier(const QString &name);

    QString m_identifier;

    friend class Asn1Acn::AstXmlParser;
};

}
}

Q_DECLARE_METATYPE(Asn1Acn::Types::Type::ASN1Type)
