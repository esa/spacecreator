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

namespace taste3 {
namespace aadl {

struct BasicParameter {
    enum class Type
    {
        Other = 0,
        Timer,
        Directive
    };

    BasicParameter(const QString &name = QString(), Type t = BasicParameter::Type::Other,
                   const QString &paramTypeName = QString());
    virtual ~BasicParameter();

    QString name() const;
    bool setName(const QString &name);

    Type paramType() const;
    bool setParamType(const Type &type);

    QString paramTypeName() const;
    bool setParamTypeName(const QString &typeName);

    bool operator==(const BasicParameter &other) const;

protected:
    QString m_paramName = {};
    Type m_paramType = { Type::Other };
    QString m_typeName = {};
};

struct ContextParameter : public BasicParameter {
    ContextParameter(const QString &name = QString(), Type t = BasicParameter::Type::Other,
                     const QString &paramTypeName = QString(), const QVariant &val = QVariant());
    ~ContextParameter() override;

    QVariant defaultValue() const;
    bool setDefaultValue(const QVariant &value);

    bool operator==(const ContextParameter &other) const;

protected:
    QVariant m_defaultValue = {};
};

struct IfaceParameter : public BasicParameter {
    enum class Direction
    {
        In = 0,
        Out
    };

    IfaceParameter(const QString &name = QString(), Type t = BasicParameter::Type::Other,
                   const QString &paramTypeName = QString(), const QString &encoding = QString(),
                   Direction dir = IfaceParameter::Direction::In);
    ~IfaceParameter() override;

    QString encoding() const;
    bool setEncoding(const QString &encoding);

    Direction direction() const;
    bool setDirection(Direction dir);

    bool operator==(const IfaceParameter &other) const;

protected:
    QString m_encoding = {};
    Direction m_direction = { Direction::In };
};

} // ns aadl
} // ns taste3

Q_DECLARE_METATYPE(taste3::aadl::BasicParameter);
Q_DECLARE_METATYPE(taste3::aadl::ContextParameter);
Q_DECLARE_METATYPE(taste3::aadl::IfaceParameter);
