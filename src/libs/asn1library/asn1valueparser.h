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

#include <QObject>

namespace Asn1Acn {
class TypeAssignment;
class IntegerValue;
namespace Constraints {
template<typename ValueType>
class Constraint;
template<typename ValueType>
class WithConstraints;
}
namespace Types {
class Type;
}

class Asn1ValueParser : public QObject
{
    Q_OBJECT
public:
    Asn1ValueParser(QObject *parent = nullptr);

    QVariantMap parseAsn1Value(
            const Asn1Acn::TypeAssignment *typeAssignemt, const QString &asn1Value, bool *valueOk = nullptr) const;
    QVariantMap parseAsn1Value(
            const Asn1Acn::Types::Type *type, const QString &asn1Value, bool *valueOk = nullptr) const;

    static bool isValueValid(const Asn1Acn::Types::Type *basicDataType, const QVariant &value);

Q_SIGNALS:
    void parseError(const QString &error) const;

private:
    bool checkFormat(const QString &asn1Value) const;

    bool parseSequenceValue(
            const Asn1Acn::Types::Type *asn1Type, const QString &asn1Value, QVariantMap &valueMap) const;
    bool parseSequenceOfValue(
            const Asn1Acn::Types::Type *asn1Type, const QString &asn1Value, QVariantMap &valueMap) const;
    bool parseChoiceValue(const Asn1Acn::Types::Type *asn1Type, const QString &asn1Value, QVariantMap &valueMap) const;

    template<typename ValueType>
    bool checkRange(
            const Constraints::WithConstraints<ValueType> *asn1Type, const typename ValueType::Type &value) const;
    template<typename ValueType>
    bool checkSize(const Constraints::WithConstraints<ValueType> *asn1Type, const int32_t value) const;
    template<typename ValueType>
    bool checkStringLength(const Constraints::WithConstraints<ValueType> *asn1Type, QString value) const;

    template<typename ValueType>
    bool checkRangeConstraint(
            const Constraints::Constraint<ValueType> *constraint, const typename ValueType::Type &value) const;
    template<typename ValueType>
    bool checkSizeConstraint(const Constraints::Constraint<ValueType> *constraint, const int32_t value) const;

    int nextIndex(const QString &value) const;
};

} // namespace asn1
