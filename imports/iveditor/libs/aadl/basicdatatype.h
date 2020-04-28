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

#pragma once

#include <QString>
#include <QVariant>
#include <QVector>

namespace aadl {

enum class DataTypeName
{
    Integer = 0,
    Real,
    Enumerated,
    Boolean,
    String
};

class BasicDataType
{
public:
    explicit BasicDataType(const QString &name, DataTypeName type);
    virtual ~BasicDataType();

    QString name() const;
    DataTypeName dataType() const;

private:
    QString m_name;
    DataTypeName m_dataType;
};

template<class T>
class RangedDataType : public BasicDataType
{
public:
    explicit RangedDataType(const QString &name, DataTypeName type)
        : BasicDataType(name, type)
    {
    }

    QVector<QVariant> range() const { return m_range; }
    void setRange(const QVector<QVariant> &r) { m_range = r; }

    T min() const
    {
        if (m_range.size() < 2)
            return std::numeric_limits<T>::min();

        return m_range.first().template value<T>();
    }
    void setMin(const T &t)
    {
        if (m_range.size() < 2)
            return;

        m_range.replace(0, t);
    }

    T max() const
    {
        if (m_range.size() < 2)
            return std::numeric_limits<T>::max();

        return m_range.last().template value<T>();
    }
    void setMax(const T &t)
    {
        const int size(m_range.size());
        if (size < 2)
            return;

        return m_range.replace(size - 1, t);
    }

protected:
    QVector<QVariant> m_range;
};

class SignedIntegerDataType : public RangedDataType<qint64>
{
public:
    explicit SignedIntegerDataType(const QString &name);
};

class UnsignedIntegerDataType : public RangedDataType<quint64>
{
public:
    explicit UnsignedIntegerDataType(const QString &name);
};

class RealDataType : public RangedDataType<qreal>
{
public:
    explicit RealDataType(const QString &name);
};

class BoolDataType : public RangedDataType<bool>
{
public:
    explicit BoolDataType(const QString &name);
};

class StringDataType : public RangedDataType<QString>
{
public:
    explicit StringDataType(const QString &name);

    QVector<QVariant> range() const;
    void setRange(const QVector<QVariant> &r);

    QString min() const;
    void setMin(const QString &t);

    QString max() const;
    void setMax(const QString &t);
};

class EnumDataType : public RangedDataType<QString>
{
public:
    explicit EnumDataType(const QString &name, const QVector<QString> &enumeration);

    QString min() const;
    void setMin(const QString &t);

    QString max() const;
    void setMax(const QString &t);
};

}
