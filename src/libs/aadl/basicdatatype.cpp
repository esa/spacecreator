/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "basicdatatype.h"

#include <limits>

namespace aadl {

BasicDataType::BasicDataType(const QString &name, DataTypeName type)
    : m_name(name)
    , m_dataType(type)
{
}

BasicDataType::~BasicDataType() { }

QString BasicDataType::name() const
{
    return m_name;
}

DataTypeName BasicDataType::dataType() const
{
    return m_dataType;
}

SignedIntegerDataType::SignedIntegerDataType(const QString &name)
    : RangedDataType<qint64>(name, DataTypeName::Integer)
{
    setRange({ std::numeric_limits<qint64>::min(), std::numeric_limits<qint64>::max() });
}

UnsignedIntegerDataType::UnsignedIntegerDataType(const QString &name)
    : RangedDataType<quint64>(name, DataTypeName::Integer)
{
    setRange({ std::numeric_limits<quint64>::min(), std::numeric_limits<quint64>::max() });
}

RealDataType::RealDataType(const QString &name)
    : RangedDataType<qreal>(name, DataTypeName::Real)
{
    setRange({ std::numeric_limits<qreal>::min(), std::numeric_limits<qreal>::max() });
}

BoolDataType::BoolDataType(const QString &name)
    : RangedDataType<bool>(name, DataTypeName::Boolean)
{
    setRange({ false, true });
}

StringDataType::StringDataType(const QString &name)
    : RangedDataType<QString>(name, DataTypeName::String)
{
}

QVector<QVariant> StringDataType::range() const
{
    return {};
}

void StringDataType::setRange(const QVector<QVariant> &)
{
    return;
}

QString StringDataType::min() const
{
    return QString();
}

void StringDataType::setMin(const QString &)
{
    return;
}

QString StringDataType::max() const
{
    return QString();
}

void StringDataType::setMax(const QString &)
{
    return;
}

EnumDataType::EnumDataType(const QString &name, const QVector<QString> &enumeration)
    : RangedDataType<QString>(name, DataTypeName::Enumerated)
{
    QVector<QVariant> varVec(enumeration.size());
    for (int i = 0; i < enumeration.size(); ++i)
        varVec[i] = enumeration.at(i);
    setRange(varVec);
}

}
