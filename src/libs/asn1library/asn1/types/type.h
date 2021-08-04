/****************************************************************************
**
** Copyright (C) 2017-2021 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a program and funded by
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

#include <QString>
#include <QVariantMap>
#include <memory>
#include <vector>

#include "typereadingvisitor.h"

namespace Asn1Acn {

class AstXmlParser;
class TypeAssignment;

namespace Types {

class TypeMutatingVisitor;

enum class AlignToNext { byte, word, dword, unspecified };
enum class Endianness { big, little, unspecified };

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
    Type(const Type &);
    virtual ~Type() = default;

    Type &operator=(const Type &) = delete;

    /*!
       Type as string
     */
    virtual QString typeName() const = 0;
    virtual QString label() const;
    virtual ASN1Type typeEnum() const = 0;

    virtual void accept(TypeMutatingVisitor &visitor) = 0;
    virtual void accept(TypeReadingVisitor &visitor) const = 0;

    virtual std::unique_ptr<Type> clone() const = 0;

    const QString &identifier() const;

    virtual QString baseIconFile() const = 0;

    const QVariantMap &parameters() const;
    void setParameters(const QVariantMap &parameters);

    const std::vector<std::unique_ptr<Type>> &children() const;
    void addChild(std::unique_ptr<Type> child);

    void setAlignToNext(const AlignToNext alignToNext) { m_alignment = alignToNext; }
    AlignToNext alignToNext() const { return m_alignment; }

    static AlignToNext mapAlignToNext(QStringRef in);
    static AlignToNext mapAlignToNext(const QString &in) { return mapAlignToNext(&in); }

    static Endianness mapEndianess(QStringRef in);
    static Endianness mapEndianess(const QString &in) { return mapEndianess(&in); }

    static QString alignToNextToString(AlignToNext param);
    static QString endiannessToString(Endianness param);

protected:
    void setIdentifier(const QString &name);

    QString m_identifier;
    QVariantMap m_parameters;
    std::vector<std::unique_ptr<Type>> m_children;

    AlignToNext m_alignment;

    friend class Asn1Acn::AstXmlParser;
    friend class Asn1Acn::TypeAssignment;
};

}
}

Q_DECLARE_METATYPE(Asn1Acn::Types::Type::ASN1Type)
