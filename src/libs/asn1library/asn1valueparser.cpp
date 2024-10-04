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

#include "asn1valueparser.h"

#include "constraints/logicoperators.h"
#include "constraints/rangeconstraint.h"
#include "constraints/sizeconstraint.h"
#include "typeassignment.h"
#include "types/bitstring.h"
#include "types/choice.h"
#include "types/enumerated.h"
#include "types/ia5string.h"
#include "types/integer.h"
#include "types/numericstring.h"
#include "types/octetstring.h"
#include "types/real.h"
#include "types/sequence.h"
#include "types/sequenceof.h"
#include "types/type.h"
#include "types/userdefinedtype.h"
#include "values.h"

#include <QRegularExpression>
#include <QVariant>
#include <type_traits>

namespace Asn1Acn {

/*!
 * \class Asn1ValueParser
 *
 * This class is used to parse an ASN.1 file.
 */

Asn1ValueParser::Asn1ValueParser(QObject *parent)
    : QObject(parent)
{
}

/*!
 * \brief Asn1ValueParser::parseAsn1Value Parse an ASN.1 value from a file
 * \param asn1Type
 * \param asn1Value
 * \param valueOk
 * \return
 */
QVariantMap Asn1ValueParser::parseAsn1Value(
        const Asn1Acn::TypeAssignment *typeAssignment, const QString &asn1Value, bool *valueOk) const
{
    return parseAsn1Value(typeAssignment->type(), asn1Value, valueOk);
}

QVariantMap Asn1ValueParser::parseAsn1Value(
        const Asn1Acn::Types::Type *type, const QString &asn1Value, bool *valueOk) const
{
    QVariantMap valueMap;
    bool ok = true;

    if (!type || asn1Value.isEmpty()) {
        return valueMap;
    }

    // In case of user types, use the referenced type
    if (type->typeEnum() == Asn1Acn::Types::Type::USERDEFINED) {
        auto userType = dynamic_cast<const Asn1Acn::Types::UserdefinedType *>(type);
        if (userType && userType->type()) {
            valueMap = parseAsn1Value(userType->type(), asn1Value, valueOk);
            valueMap["name"] = type->identifier();
            return valueMap;
        }
    }

    QString value = asn1Value.trimmed();

    valueMap["name"] = type->identifier();

    if (value == "*") {
        // We want to use empty and invalid QVariant as an information that value is present, but it can be any value
        QVariant empty;
        empty.clear();

        valueMap["value"] = empty;
    } else {
        switch (type->typeEnum()) {
        case Asn1Acn::Types::Type::INTEGER: {
            const auto *integerType = dynamic_cast<const Asn1Acn::Types::Integer *>(type);
            const auto integerValue = value.toInt(&ok);
            if (ok && (ok = checkRange<Asn1Acn::IntegerValue>(integerType, integerValue))) {
                valueMap["value"] = value;
            }
            break;
        }
        case Asn1Acn::Types::Type::REAL: {
            const auto *realType = dynamic_cast<const Asn1Acn::Types::Real *>(type);
            const auto realValue = value.toDouble(&ok);
            if (ok && (ok = checkRange<Asn1Acn::RealValue>(realType, realValue))) {
                valueMap["value"] = value;
            }
            break;
        }
        case Asn1Acn::Types::Type::BOOLEAN: {
            ok = (value == "TRUE" || value == "FALSE");
            if (ok) {
                valueMap["value"] = value == "TRUE";
            }
            break;
        }
        case Asn1Acn::Types::Type::SEQUENCE: {
            ok = parseSequenceValue(type, value, valueMap);
            break;
        }
        case Asn1Acn::Types::Type::SEQUENCEOF: {
            const auto *sequenceOfType = dynamic_cast<const Asn1Acn::Types::SequenceOf *>(type);
            ok = parseSequenceOfValue(type, value, valueMap)
                    && checkSize<IntegerValue>(sequenceOfType, valueMap["seqofvalue"].toList().count());
            break;
        }
        case Asn1Acn::Types::Type::ENUMERATED: {
            const auto *enumeratedType = dynamic_cast<const Asn1Acn::Types::Enumerated *>(type);
            const auto &enumeratedItems = enumeratedType->items();
            ok = enumeratedItems.contains(value);
            if (ok) {
                valueMap["value"] = value;
            }
            break;
        }
        case Asn1Acn::Types::Type::CHOICE: {
            ok = parseChoiceValue(type, value, valueMap);
            break;
        }
        case Asn1Acn::Types::Type::BITSTRING: {
            // bitstring is '..'H or '..'B
            if (!value.startsWith("'") || !(value.endsWith("'H") || value.endsWith("'B"))) {
                ok = false;
                break;
            }
            const auto *bitStringType = dynamic_cast<const Asn1Acn::Types::BitString *>(type);
            ok = checkBitStringCharacters(value);
            ok = ok && checkBitStringLength(bitStringType, value);
            if (ok) {
                valueMap["value"] = value;
            }
            break;
        }
        case Asn1Acn::Types::Type::IA5STRING: {
            const auto *ia5String = dynamic_cast<const Asn1Acn::Types::IA5String *>(type);

            if (value.startsWith("\"")) {
                value = value.remove(0, 1);
            }
            if (value.endsWith("\"")) {
                value.chop(1);
            }

            ok = checkStringLength(ia5String, value);
            if (ok) {
                valueMap["value"] = value;
            }
            break;
        }
        case Asn1Acn::Types::Type::NUMERICSTRING: {
            const auto *numericString = dynamic_cast<const Asn1Acn::Types::NumericString *>(type);

            if (value.startsWith("\"")) {
                value = value.remove(0, 1);
            }
            if (value.endsWith("\"")) {
                value.chop(1);
            }

            ok = checkStringLength(numericString, value);
            if (ok) {
                valueMap["value"] = value;
            }
            break;
        }
        case Asn1Acn::Types::Type::OCTETSTRING: {
            // bitstring is '..'H or '..'B
            if (!value.startsWith("'") || !(value.endsWith("'H") || value.endsWith("'B"))) {
                ok = false;
                break;
            }
            const auto *octetString = dynamic_cast<const Asn1Acn::Types::OctetString *>(type);
            ok = checkBitStringCharacters(value);
            ok = ok && checkBitStringLength(octetString, value);
            if (ok) {
                valueMap["value"] = value;
            }
            break;
        }
        default:
            break;
        }
    }

    if (!ok) {
        valueMap.clear();
        Q_EMIT parseError(QString(tr("Incorrect value for %1")).arg(type->identifier()));
    }

    if (valueOk != nullptr) {
        *valueOk = ok;
    }
    return valueMap;
}

bool Asn1ValueParser::isValueValid(const Types::Type *basicDataType, const QVariant &value)
{
    if (!basicDataType) {
        return value.isValid();
    }

    Asn1Acn::Asn1ValueParser valueParser;
    bool ok;
    valueParser.parseAsn1Value(basicDataType, value.toString(), &ok);
    return ok;
}

bool Asn1ValueParser::checkFormat(const QString &asn1Value) const
{
    return asn1Value.startsWith("{") && asn1Value.endsWith("}");
}

/*!
 * \brief Asn1ValueParser::parseSequenceValue Parse a sequence value
 * \param asn1Type
 * \param asn1Value
 * \param valueMap
 * \return
 */
bool Asn1ValueParser::parseSequenceValue(
        const Asn1Acn::Types::Type *asn1Type, const QString &asn1Value, QVariantMap &valueMap) const
{
    /*
{ intVal 5,
  realVal 42.0,
  intArray {},
  boolVal TRUE,
  enumVal enum2,
  enumArray {enum1, enum3},
  label "Hello World",
  choiceVal choiceReal : 10.5,
  seqVal { iVal 5 }
}
*/
    const auto *sequenceType = dynamic_cast<const Asn1Acn::Types::Sequence *>(asn1Type);
    if (!sequenceType) {
        Q_EMIT parseError(tr("Incorrect type while parsing sequence value"));
        return false;
    }

    QVariantList children;
    QVariantList parseList;
    QString value = asn1Value.trimmed();

    if (!checkFormat(value)) {
        Q_EMIT parseError(tr("Incorrect format for ASN.1 sequence value"));
        return false;
    }

    // delete '{' from begin and '}' from end
    value.remove(0, 1);
    value = value.remove(-1, 1).trimmed();

    int index;

    while ((index = value.indexOf(" ")) != -1) {
        QVariantMap fieldMap;

        fieldMap["name"] = value.mid(0, index);
        value = value.remove(0, index).trimmed();

        index = nextIndex(value);

        fieldMap["value"] = value.mid(0, index);
        parseList.append(fieldMap);

        if (index != -1) {
            value = value.remove(0, index).trimmed();
            value = value.remove(0, value.indexOf(",") + 1).trimmed();
        } else
            break;
    }

    for (const auto &field : parseList) {
        const auto *sequenceComponent = sequenceType->component(field.toMap()["name"].toString());

        if (sequenceComponent && sequenceComponent->type()) {
            bool convOk = false;
            auto childValue = parseAsn1Value(sequenceComponent->type(), field.toMap()["value"].toString(), &convOk);
            if (!convOk) {
                return false;
            }

            if (!childValue.isEmpty()) {
                children.append(childValue);
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    valueMap["children"] = children;

    return true;
}

bool Asn1ValueParser::parseSequenceOfValue(
        const Asn1Acn::Types::Type *asn1Type, const QString &asn1Value, QVariantMap &valueMap) const
{
    /*
{enum1, enum3}
{ { intVal 5, realVal 42.0 }, { intVal 6, realVal 13.9 } )

*/
    const auto *sequenceOfType = dynamic_cast<const Asn1Acn::Types::SequenceOf *>(asn1Type);
    if (!sequenceOfType) {
        Q_EMIT parseError(tr("Incorrect type while parsing sequence of value"));
        return false;
    }

    QVariantList seqofValues;
    QString value = asn1Value.trimmed();

    if (!checkFormat(value)) {
        Q_EMIT parseError(tr("Incorrect format for ASN.1 sequence value"));
        return false;
    }

    // delete '{' from begin and '}' from end
    value.remove(0, 1);
    value = value.remove(-1, 1).trimmed();

    int index;
    while ((index = nextIndex(value))) {
        auto item = value.mid(0, index);

        const auto *itemType = sequenceOfType->itemsType() ? sequenceOfType->itemsType() : asn1Type;
        bool convOk = false;
        auto itemValue = parseAsn1Value(itemType, item, &convOk);
        if (!convOk) {
            return false;
        }
        if (!itemValue.empty()) {
            seqofValues.append(itemValue);
        } else {
            return false;
        }

        if (index != -1) {
            value = value.remove(0, index).trimmed();
            value = value.remove(0, value.indexOf(",") + 1).trimmed();
        } else {
            break;
        }
    }

    valueMap["seqofvalue"] = seqofValues;

    return true;
}

/*!
   \brief Asn1ValueParser::parseChoiceValue
   \param asn1Type the choice paramater containing type and value. Example "choiceReal : 10.5"
   \param asn1Value
   \param valueMap
   \return
 */
bool Asn1ValueParser::parseChoiceValue(
        const Asn1Acn::Types::Type *asn1Type, const QString &asn1Value, QVariantMap &valueMap) const
{
    const auto *choiceType = dynamic_cast<const Asn1Acn::Types::Choice *>(asn1Type);
    if (!choiceType) {
        Q_EMIT parseError(tr("Incorrect type while parsing choice value"));
        return false;
    }

    const QString name = asn1Value.left(asn1Value.indexOf(":")).trimmed();

    const auto *choiceComponent = choiceType->component(name);
    if (choiceComponent && choiceComponent->type()) {
        const QString value = asn1Value.mid(asn1Value.indexOf(":") + 1).trimmed();
        valueMap["choice"] = parseAsn1Value(choiceComponent->type(), value);
        bool convOk = false;
        valueMap["choice"] = parseAsn1Value(choiceComponent->type(), value, &convOk);
        if (!convOk) {
            return false;
        }

        return true;
    }

    return false;
}

template<typename ValueType>
bool Asn1ValueParser::checkRange(
        const Constraints::WithConstraints<ValueType> *asn1Type, const typename ValueType::Type &value) const
{
    for (const auto &constraint : asn1Type->constraints().constraints()) {
        if (!checkRangeConstraint<ValueType>(constraint.get(), value)) {
            return false;
        }
    }

    return true;
}

template<typename ValueType>
bool Asn1ValueParser::checkSize(const Constraints::WithConstraints<ValueType> *asn1Type, const int32_t value) const
{
    for (const auto &constraint : asn1Type->constraints().constraints()) {
        if (!checkSizeConstraint<ValueType>(constraint.get(), value)) {
            return false;
        }
    }

    return true;
}

template<typename ValueType>
bool Asn1ValueParser::checkStringLength(const Constraints::WithConstraints<ValueType> *asn1Type, QString value) const
{
    // take string between " "
    if (value.startsWith("\"")) {
        value = value.remove(0, 1);
    }
    if (value.endsWith("\"")) {
        value.chop(1);
    }

    return checkSize(asn1Type, value.length());
}

template<typename ValueType>
bool Asn1ValueParser::checkBitStringLength(const Constraints::WithConstraints<ValueType> *asn1Type, QString value) const
{
    // take string between '...'H or '...'B
    if (value.startsWith("'")) {
        value = value.remove(0, 1);
    }

    int length = value.length();

    if (value.endsWith("'H")) {
        value.chop(2);

        length = value.length();
        if (std::is_same_v<ValueType, Asn1Acn::OctetStringValue>) {
            length /= 2; // One octet has 2 hex digits
        }
        if (std::is_same_v<ValueType, Asn1Acn::BitStringValue>) {
            length *= 4; // one hex digit is 4 bits
        }
    }
    if (value.endsWith("'B")) {
        value.chop(2);
        length = value.length();
        if (std::is_same_v<ValueType, Asn1Acn::OctetStringValue>) {
            length /= 8; // One octet has 8 binary digits
        }
    }

    return checkSize(asn1Type, length);
}

/**
 * Checks if a 'H string has only 0-9 and A-F characters
 * And  if a 'B string has only 0/1 characters
 */
bool Asn1ValueParser::checkBitStringCharacters(const QString &value) const
{
    if (value.endsWith("'H")) {
        static QRegularExpression regex("'([0-9A-F][0-9A-F])*'H");
        return regex.match(value).hasMatch();
    }

    if (value.endsWith("'B")) {
        static QRegularExpression regex("'(0|1)*'B");
        return regex.match(value).hasMatch();
    }

    return false;
}

template<typename ValueType>
bool Asn1ValueParser::checkRangeConstraint(
        const Constraints::Constraint<ValueType> *constraint, const typename ValueType::Type &value) const
{
    if (!constraint) {
        return false;
    }

    if (const auto *andConstraint = dynamic_cast<const Constraints::AndConstraint<ValueType> *>(constraint);
            andConstraint) {
        const auto leftConstraint = checkRangeConstraint(andConstraint->leftChild(), value);
        const auto rightConstraint = checkRangeConstraint(andConstraint->rightChild(), value);
        return leftConstraint && rightConstraint;
    } else if (const auto *orConstraint = dynamic_cast<const Constraints::OrConstraint<ValueType> *>(constraint);
               orConstraint) {
        const auto leftConstraint = checkRangeConstraint(andConstraint->leftChild(), value);
        const auto rightConstraint = checkRangeConstraint(andConstraint->rightChild(), value);
        return leftConstraint || rightConstraint;
    } else if (const auto *rangeConstraint = dynamic_cast<const Constraints::RangeConstraint<ValueType> *>(constraint);
               rangeConstraint) {
        const auto &range = rangeConstraint->range();
        return range.check(value);
    } else {
        return true;
    }
}

template<typename ValueType>
bool Asn1ValueParser::checkSizeConstraint(const Constraints::Constraint<ValueType> *constraint, int32_t value) const
{
    if (!constraint) {
        return false;
    }

    if (const auto *andConstraint = dynamic_cast<const Constraints::AndConstraint<ValueType> *>(constraint);
            andConstraint) {
        const auto leftConstraint = checkSizeConstraint<ValueType>(andConstraint->leftChild(), value);
        const auto rightConstraint = checkSizeConstraint<ValueType>(andConstraint->rightChild(), value);
        return leftConstraint && rightConstraint;
    } else if (const auto *orConstraint = dynamic_cast<const Constraints::OrConstraint<ValueType> *>(constraint);
               orConstraint) {
        const auto leftConstraint = checkSizeConstraint<ValueType>(andConstraint->leftChild(), value);
        const auto rightConstraint = checkSizeConstraint<ValueType>(andConstraint->rightChild(), value);
        return leftConstraint || rightConstraint;
    } else if (const auto *sizeConstraint = dynamic_cast<const Constraints::SizeConstraint<ValueType> *>(constraint);
               sizeConstraint) {
        return checkRangeConstraint<IntegerValue>(sizeConstraint->innerConstraints(), value);
    } else {
        return true;
    }
}

int Asn1ValueParser::nextIndex(const QString &value) const
{
    int index;

    if (value.startsWith("{")) {
        /* possible formats:
         *  { ... }
         *  { { ...  }, { ... }, ... }
         *  { { { ...  }, { ... } }, { { ...  }, { ... } }, ... }
         */
        int countBracket = 0;
        for (index = 0; index < value.size(); ++index) {
            if (value[index] == '{')
                ++countBracket;
            else if (value[index] == '}')
                --countBracket;

            if (countBracket == 0)
                break;
        }

        ++index;
    } else {
        index = value.indexOf(",");
        if (index == -1)
            index = value.size();
    }

    return index;
}

} // namespace asn1
