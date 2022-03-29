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

class Boolean : public Type, public Constraints::WithConstraints<BooleanValue>
{
public:
    Boolean(const QString &identifier = QString())
        : Type(identifier)
        , m_acnSize(8)
    {
    }
    Boolean(const Boolean &other) = default;

    QString typeName() const override { return QLatin1String("BOOLEAN"); }
    ASN1Type typeEnum() const override { return BOOLEAN; }

    void accept(TypeMutatingVisitor &visitor) override;
    void accept(TypeReadingVisitor &visitor) const override;

    std::unique_ptr<Type> clone() const override;

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/boolean.png"); }

    void setAcnSize(const int acnSize) { m_acnSize = acnSize; }
    int acnSize() const { return m_acnSize; }

    void setTrueValue(const QString &trueValue) { m_trueValue = trueValue; }
    const QString &trueValue() const { return m_trueValue; }

    void setFalseValue(const QString &falseValue) { m_falseValue = falseValue; }
    const QString &falseValue() const { return m_falseValue; }

private:
    int m_acnSize;
    QString m_trueValue;
    QString m_falseValue;
};

}
}
