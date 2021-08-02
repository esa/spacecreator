/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Fuzzer - Tool for generating test cases
** based on ASN.1/ACN models and simulating malformed or malicious data.
**
** Tool was developed under a programme and funded by
** European Space Agency.
**
** This Tool is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Tool is distributed in the hope that it will be useful,
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

#include "../constraints/withconstraints.h"
#include "string.h"
#include "type.h"

namespace Asn1Acn {
namespace Types {

class BitString : public String, public Constraints::WithConstraints<BitStringValue>
{
public:
    BitString(const QString &identifier = QString())
        : String(identifier)
    { }
    BitString(const BitString &other) = default;

    QString typeName() const override { return QLatin1String("BIT STRING"); }
    ASN1Type typeEnum() const override { return BITSTRING; }

    void accept(TypeMutatingVisitor &visitor) override;
    void accept(TypeReadingVisitor &visitor) const override;

    std::unique_ptr<Type> clone() const override;

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/bitstring.png"); }
};

}
}
