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

#include "common.h"

#include <QString>
#include <QVariant>

namespace Asn1Acn {
namespace Types {
class Type;
}
}

namespace ivm {

class BasicParameter
{
    Q_GADGET
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString underscoredName READ underscoredName)
    Q_PROPERTY(QString type READ paramTypeName WRITE setParamTypeName)
    Q_PROPERTY(QString underscoredType READ underscoredType)

public:
    enum class Type
    {
        Other = 0,
        Timer,
        Directive
    };
    Q_ENUM(Type)

    BasicParameter(const QString &name = QString(), Type t = BasicParameter::Type::Other,
            const QString &paramTypeName = QString());
    virtual ~BasicParameter();

    QString name() const;
    bool setName(const QString &name);

    QString underscoredName() const;

    Type paramType() const;
    virtual bool setParamType(const Type &type);

    QString paramTypeName() const;
    virtual bool setParamTypeName(const QString &typeName);

    QString underscoredType() const;

    bool operator==(const BasicParameter &other) const;

    virtual QString toString() const;
    virtual bool isNull() const;

protected:
    QString m_paramName = {};
    Type m_paramType = { Type::Other };
    QString m_typeName = {};

    bool isValidValue(const Asn1Acn::Types::Type *basicDataType, const QVariant &value) const;
};

class ContextParameter : public BasicParameter
{
    Q_GADGET
    Q_PROPERTY(QVariant defaultValue READ defaultValue)
public:
    ContextParameter(const QString &name = QString(), Type t = BasicParameter::Type::Timer,
            const QString &paramTypeName = shared::typeName(BasicParameter::Type::Timer),
            const QVariant &val = QVariant());
    ~ContextParameter() override;

    QVariant defaultValue() const;
    bool setDefaultValue(const Asn1Acn::Types::Type *basicDataType, const QVariant &value);

    bool operator==(const ContextParameter &other) const;

    bool setParamType(const Type &type) override;
    bool setParamTypeName(const QString &typeName) override;

    QString toString() const override;
    bool isNull() const override;

protected:
    QVariant m_defaultValue = {};
};

class InterfaceParameter : public BasicParameter
{
    Q_GADGET
    Q_PROPERTY(bool isIn READ isInDirection)
    Q_PROPERTY(bool isOut READ isOutDirection)
    Q_PROPERTY(QString encoding READ encoding)
public:
    enum class Direction
    {
        IN = 0,
        OUT
    };

    Q_ENUM(Direction)

    InterfaceParameter(const QString &name = QObject::tr("IfaceParam"), Type t = BasicParameter::Type::Other,
            const QString &paramTypeName = {}, const QString &encoding = QObject::tr("NATIVE"),
            Direction dir = InterfaceParameter::Direction::IN);
    ~InterfaceParameter() override;

    QString encoding() const;
    bool setEncoding(const QString &encoding);

    Direction direction() const;
    bool setDirection(Direction dir);

    inline bool isInDirection() const { return direction() == Direction::IN; }
    inline bool isOutDirection() const { return direction() == Direction::OUT; }

    bool operator==(const InterfaceParameter &other) const;

    QString toString() const override;
    bool isNull() const override;

protected:
    QString m_encoding = {};
    Direction m_direction = { Direction::IN };
};

}

Q_DECLARE_METATYPE(ivm::BasicParameter)
Q_DECLARE_METATYPE(ivm::ContextParameter)
Q_DECLARE_METATYPE(ivm::InterfaceParameter)
