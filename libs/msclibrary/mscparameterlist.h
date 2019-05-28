/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QMetaType>
#include <QString>
#include <QVector>

namespace msc {

class MscParameter
{
public:
    enum class Type
    {
        Expression,
        Pattern,
        Unknown // error state
    };

    MscParameter() {}
    explicit MscParameter(const QString &parameter);
    MscParameter(const QString &expression, const QString &pattern);

    bool operator==(const msc::MscParameter &other) const
    {
        return m_type == other.m_type && m_parameter == other.m_parameter;
    }
    bool operator!=(const msc::MscParameter &other) const { return !(*this == other); }

    Type type() const;
    const QString &parameter() const;

    QString expression() const;
    void setExpression(const QString &expression);

    QString pattern() const;
    void setPattern(const QString &pattern);

private:
    QString m_parameter;
    Type m_type = Type::Unknown;
};

class MscParameterList
{
public:
    static const QChar DefaultExtraBraceOpen;
    static const QChar DefaultExtraBraceClose;

    MscParameterList(const QString &braceOpen = QString(), const QString &braceClose = QString(),
                     const QVector<msc::MscParameter> &data = QVector<msc::MscParameter>());
    const QVector<msc::MscParameter> &data() const;
    QVector<msc::MscParameter> &dataRef();
    QString extraBraceOpen() const;
    QString extraBraceClose() const;

    void setExtraBraces(const QString &open, const QString &close);

    bool operator==(const MscParameterList &other) const;

private:
    QVector<msc::MscParameter> m_data;
    QString m_braceOpen, m_braceClose;
};

} // namespace msc

Q_DECLARE_METATYPE(msc::MscParameter);
Q_DECLARE_METATYPE(msc::MscParameterList);
