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
#include "../range.h"
#include "../sourcelocation.h"
#include "integeracnparams.h"
#include "type.h"

#include <QMap>
#include <QString>

namespace Asn1Acn {
namespace Types {

class EnumeratedItem
{
public:
    EnumeratedItem() = default;

    EnumeratedItem(std::size_t index, const QString &name, long value, const SourceLocation &location = {})
        : m_index(index)
        , m_name(name)
        , m_value(value)
        , m_location(location)
    {
    }

    const QString &name() const { return m_name; }
    long value() const { return m_value; }
    std::size_t index() const { return m_index; }
    const SourceLocation &location() const { return m_location; }

private:
    std::size_t m_index;
    QString m_name;
    long m_value;
    SourceLocation m_location;
};

class Enumerated : public Type, public IntegerAcnParameters, public Constraints::WithConstraints<EnumValue>
{
public:
    Enumerated(const QString &identifier = QString());
    Enumerated(const Enumerated &other) = default;

    QString typeName() const override;
    ASN1Type typeEnum() const override { return ENUMERATED; }

    void accept(TypeMutatingVisitor &visitor) override;
    void accept(TypeReadingVisitor &visitor) const override;

    std::unique_ptr<Type> clone() const override;

    QString baseIconFile() const override { return QStringLiteral(":/asn1acn/images/outline/enumerated.png"); }

    using Items = QMap<QString, EnumeratedItem>;

    Items &items() { return m_items; }
    const Items &items() const { return m_items; }
    void addItem(const EnumeratedItem &item);

    void setEncodeValues(bool encodeValues) { m_encodeValues = encodeValues; }
    bool encodeValues() const { return m_encodeValues; }

    int mapToValue(const QString &key) const;
    Range<int64_t> mapToValue(const Range<QString> &r) const;

private:
    Items m_items;
    bool m_encodeValues;
};

}
}
