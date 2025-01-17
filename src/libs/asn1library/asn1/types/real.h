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

#include "../constraints/withconstraints.h"
#include "type.h"

#include <QString>

namespace Asn1Acn {
namespace Types {

enum class RealEncoding
{
    IEEE754_1985_32,
    IEEE754_1985_64,
    unspecified
};

class Real : public Type, public Constraints::WithConstraints<RealValue>
{
public:
    Real(const QString &identifier = QString());
    Real(const Real &other) = default;

    QString typeName() const override { return QLatin1String("REAL"); }
    ASN1Type typeEnum() const override { return REAL; }

    void accept(TypeMutatingVisitor &visitor) override;
    void accept(TypeReadingVisitor &visitor) const override;

    std::unique_ptr<Type> clone() const override;

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/real.png"); }

    void setEncoding(const RealEncoding encoding) { m_encoding = encoding; }
    RealEncoding encoding() const { return m_encoding; }

    void setEndianness(const Endianness endianness) { m_endianness = endianness; }
    Endianness endianness() const { return m_endianness; }

    static RealEncoding mapEncoding(QStringView ref);
    static RealEncoding mapEncoding(const QString &in) { return mapEncoding(QStringView(in)); }
    static QString encodingToString(RealEncoding encoding);

private:
    RealEncoding m_encoding;
    Endianness m_endianness;
};

}
}
