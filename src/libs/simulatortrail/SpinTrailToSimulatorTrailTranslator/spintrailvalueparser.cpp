/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "spintrailvalueparser.h"

#include "ia5stringprinter.h"
#include "octetstringprinter.h"

#include <QDebug>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/choicevalue.h>
#include <asn1library/asn1/multiplevalue.h>
#include <asn1library/asn1/namedvalue.h>
#include <asn1library/asn1/singlevalue.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/translation/exceptions.h>
#include <cstdint>
#include <map>
#include <promela/Asn1ToPromelaTranslator/enumeratedgenerator.h>
#include <promela/Asn1ToPromelaTranslator/visitors/sizeconstraintvisitor.h>
#include <vector>

using Asn1Acn::ChoiceValue;
using Asn1Acn::MultipleValue;
using Asn1Acn::NamedValue;
using Asn1Acn::SingleValue;
using Asn1Acn::ValuePtr;
using conversion::translator::TranslationException;
using promela::translator::EnumeratedGenerator;
using promela::translator::SizeConstraintVisitor;

namespace simulatortrail::translator {
SpinTrailValueParser::SpinTrailValueParser() {}

Asn1Acn::ValuePtr SpinTrailValueParser::parseValue(
        const QStringList &spinTrailValue, const Asn1Acn::Types::Type *asn1Type)
{
    QStringList values = spinTrailValue;
    Asn1Acn::ValuePtr result = parseValue(values, asn1Type);

    if (!values.isEmpty()) {
        QString message =
                QString("Cannot parse Spin Trail value: %1 parameters cannot be parsed.").arg(values.length());
        throw TranslationException(std::move(message));
    }

    return result;
}

Asn1Acn::ValuePtr SpinTrailValueParser::parseValue(QStringList &spinTrailValue, const Asn1Acn::Types::Type *asn1Type)
{
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::USERDEFINED) {
        const Asn1Acn::Types::UserdefinedType *userType =
                dynamic_cast<const Asn1Acn::Types::UserdefinedType *>(asn1Type);
        return parseValue(spinTrailValue, userType->type());
    }
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::INTEGER) {
        return parseInteger(spinTrailValue);
    }
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::BOOLEAN) {
        return parseBoolean(spinTrailValue);
    }
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::REAL) {
        return parseReal(spinTrailValue);
    }
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::SEQUENCE) {
        const Asn1Acn::Types::Sequence *sequence = dynamic_cast<const Asn1Acn::Types::Sequence *>(asn1Type);
        return parseSequence(spinTrailValue, sequence);
    }
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::SEQUENCEOF) {
        const Asn1Acn::Types::SequenceOf *sequenceOf = dynamic_cast<const Asn1Acn::Types::SequenceOf *>(asn1Type);
        return parseSequenceOf(spinTrailValue, sequenceOf);
    }
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::ENUMERATED) {
        const Asn1Acn::Types::Enumerated *enumerated = dynamic_cast<const Asn1Acn::Types::Enumerated *>(asn1Type);
        return parseEnumerated(spinTrailValue, enumerated);
    }
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::CHOICE) {
        const Asn1Acn::Types::Choice *choice = dynamic_cast<const Asn1Acn::Types::Choice *>(asn1Type);
        return parseChoice(spinTrailValue, choice);
    }
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::BITSTRING) {
        throw TranslationException("Translation from Spin Trail to Simulator Trail for BITSTRING is not implemented.");
    }
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::OCTETSTRING) {
        const Asn1Acn::Types::OctetString *octetString = dynamic_cast<const Asn1Acn::Types::OctetString *>(asn1Type);
        return parseOctetString(spinTrailValue, octetString);
    }
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::IA5STRING) {
        const Asn1Acn::Types::IA5String *ia5string = dynamic_cast<const Asn1Acn::Types::IA5String *>(asn1Type);
        return parseIA5String(spinTrailValue, ia5string);
    }
    if (asn1Type->typeEnum() == Asn1Acn::Types::Type::NUMERICSTRING) {
        throw TranslationException(
                "Translation from Spin Trail to Simulator Trail for NUMERICSTRING is not implemented.");
    }

    return std::make_unique<SingleValue>("0");
}

Asn1Acn::ValuePtr SpinTrailValueParser::parseInteger(QStringList &spinTrailValue)
{
    if (spinTrailValue.isEmpty()) {
        throw TranslationException("Invalid value for INTEGER type");
    }
    QString value = spinTrailValue.first();
    spinTrailValue.pop_front();
    return std::make_unique<SingleValue>(value);
}

Asn1Acn::ValuePtr SpinTrailValueParser::parseReal(QStringList &spinTrailValue)
{
    if (spinTrailValue.isEmpty()) {
        throw TranslationException("Invalid value for REAL type");
    }
    QString value = spinTrailValue.first();
    spinTrailValue.pop_front();
    return std::make_unique<SingleValue>(value);
}

Asn1Acn::ValuePtr SpinTrailValueParser::parseBoolean(QStringList &spinTrailValue)
{
    if (spinTrailValue.isEmpty()) {
        throw TranslationException("Invalid value for BOOLEAN type");
    }
    QString value = spinTrailValue.first().compare("0") == 0 ? "FALSE" : "TRUE";
    spinTrailValue.pop_front();
    return std::make_unique<SingleValue>(value);
}

Asn1Acn::ValuePtr SpinTrailValueParser::parseSequence(
        QStringList &spinTrailValue, const Asn1Acn::Types::Sequence *sequence)
{
    std::list<QString> componentNames;
    std::map<QString, ValuePtr> values;
    std::list<QString> optionalComponents;
    // iterate over all components
    for (const std::unique_ptr<Asn1Acn::SequenceComponent> &component : sequence->components()) {
        const Asn1Acn::AsnSequenceComponent *asnSequenceComponent =
                dynamic_cast<const Asn1Acn::AsnSequenceComponent *>(component.get());
        if (asnSequenceComponent == nullptr) {
            continue;
        }
        componentNames.push_back(component->name());
        ValuePtr componentValue = parseValue(spinTrailValue, component->type());
        values.emplace(component->name(), std::move(componentValue));
        if (asnSequenceComponent->isOptional()) {
            optionalComponents.push_back(component->name());
        }
    }
    // iterate over 'exist' structure
    // remove not existing values
    for (const QString &component : optionalComponents) {
        if (spinTrailValue.isEmpty()) {
            throw TranslationException("Invalid value for SEQUENCE type");
        }
        int exists = spinTrailValue.front().toInt();
        spinTrailValue.pop_front();
        if (exists == 0) {
            values.erase(component);
        }
    }

    // construct final value
    std::unique_ptr<NamedValue> result = std::make_unique<NamedValue>();
    for (const QString &component : componentNames) {
        auto iter = values.find(component);
        if (iter != values.end()) {
            result->addValue(iter->first, std::move(iter->second));
        }
    }

    return result;
}

Asn1Acn::ValuePtr SpinTrailValueParser::parseSequenceOf(
        QStringList &spinTrailValue, const Asn1Acn::Types::SequenceOf *sequenceOf)
{
    SizeConstraintVisitor<Asn1Acn::IntegerValue> constraintVisitor;
    sequenceOf->constraints().accept(constraintVisitor);
    const bool hasFixedSize = constraintVisitor.getMinSize() == constraintVisitor.getMaxSize();
    const size_t size = constraintVisitor.getMaxSize();

    std::list<ValuePtr> values;

    for (size_t index = 0; index < size; ++index) {
        values.push_back(parseValue(spinTrailValue, sequenceOf->itemsType()));
    }

    size_t actualSize = size;
    if (!hasFixedSize) {
        if (spinTrailValue.isEmpty()) {
            throw TranslationException("Invalid value for SEQUENCE OF type");
        }
        actualSize = spinTrailValue.first().toUInt();
        spinTrailValue.pop_front();
    }
    std::unique_ptr<MultipleValue> result = std::make_unique<MultipleValue>();
    for (size_t index = 0; index < actualSize; ++index) {
        result->addValue(std::move(values.front()));
        values.pop_front();
    }
    return result;
}

Asn1Acn::ValuePtr SpinTrailValueParser::parseEnumerated(
        QStringList &spinTrailValue, const Asn1Acn::Types::Enumerated *enumerated)
{
    if (spinTrailValue.isEmpty()) {
        throw TranslationException("Invalid value for ENUMERATED type");
    }

    EnumeratedGenerator enumeratedGenerator(enumerated->typeName(), *enumerated);

    const int32_t integerValue = spinTrailValue.first().toInt();
    spinTrailValue.pop_front();
    QString value = enumeratedGenerator.getNameForValue(integerValue);
    if (value.isEmpty()) {
        throw TranslationException("Invalid value for ENUMERATED type");
    }
    return std::make_unique<SingleValue>(value);
}

Asn1Acn::ValuePtr SpinTrailValueParser::parseChoice(QStringList &spinTrailValue, const Asn1Acn::Types::Choice *choice)
{
    // TODO: this does not support enhanced spin (support for unions)

    // parse all values
    std::map<int32_t, std::pair<QString, ValuePtr>> values;
    int32_t index = 1;
    for (const std::unique_ptr<Asn1Acn::Types::ChoiceAlternative> &component : choice->components()) {
        ValuePtr componentValue = parseValue(spinTrailValue, component->type());
        values.emplace(index, std::make_pair(component->name(), std::move(componentValue)));
        ++index;
    }

    if (spinTrailValue.isEmpty()) {
        throw TranslationException("Invalid value for CHOICE type");
    }
    const int32_t choiceSelection = spinTrailValue.first().toInt();
    spinTrailValue.pop_front();
    auto presentValue = values.find(choiceSelection);
    if (presentValue == values.end()) {
        QString message = QString("Invalid value for CHOICE type: selection value (%1)is invalid").arg(choiceSelection);
        throw TranslationException(std::move(message));
    }

    return std::make_unique<ChoiceValue>(presentValue->second.first, std::move(presentValue->second.second));
}

Asn1Acn::ValuePtr SpinTrailValueParser::parseOctetString(
        QStringList &spinTrailValue, const Asn1Acn::Types::OctetString *octetString)
{
    SizeConstraintVisitor<Asn1Acn::OctetStringValue> constraintVisitor;
    octetString->constraints().accept(constraintVisitor);
    const bool hasFixedSize = constraintVisitor.getMinSize() == constraintVisitor.getMaxSize();
    const size_t size = constraintVisitor.getMaxSize();

    std::vector<uint8_t> values;
    values.reserve(size);

    for (size_t index = 0; index < size; ++index) {
        if (spinTrailValue.isEmpty()) {
            throw TranslationException("Invalid value for OCTET STRING type");
        }
        int integerValue = spinTrailValue.first().toInt();
        spinTrailValue.pop_front();
        uint8_t octet = static_cast<uint8_t>(integerValue);
        values.push_back(octet);
    }

    size_t actualSize = size;
    if (!hasFixedSize) {
        if (spinTrailValue.isEmpty()) {
            throw TranslationException("Invalid value for OCTET STRING type");
        }
        actualSize = spinTrailValue.first().toUInt();
        spinTrailValue.pop_front();
    }

    QString value = "";

    for (size_t index = 0; index < actualSize; ++index) {
        value.append(QString("%1").arg(values[index], 0, 16));
    }

    return std::make_unique<SingleValue>(value, OctetStringPrinter());
}

Asn1Acn::ValuePtr SpinTrailValueParser::parseIA5String(
        QStringList &spinTrailValue, const Asn1Acn::Types::IA5String *ia5string)
{
    SizeConstraintVisitor<Asn1Acn::StringValue> constraintVisitor;
    ia5string->constraints().accept(constraintVisitor);
    const bool hasFixedSize = constraintVisitor.getMinSize() == constraintVisitor.getMaxSize();
    const size_t size = constraintVisitor.getMaxSize();

    std::vector<uint8_t> values;
    values.reserve(size);

    for (size_t index = 0; index < size; ++index) {
        if (spinTrailValue.isEmpty()) {
            throw TranslationException("Invalid value for OCTET STRING type");
        }
        int integerValue = spinTrailValue.first().toInt();
        spinTrailValue.pop_front();

        uint8_t octet = static_cast<uint8_t>(integerValue);
        values.push_back(octet);
    }

    size_t actualSize = size;
    if (!hasFixedSize) {
        if (spinTrailValue.isEmpty()) {
            throw TranslationException("Invalid value for OCTET STRING type");
        }
        actualSize = spinTrailValue.first().toUInt();
        spinTrailValue.pop_front();
    }

    const bool hasSpecialCharacters =
            std::any_of(values.begin(), std::next(values.begin(), static_cast<long int>(actualSize)),
                    [](const uint8_t c) { return !QChar::isPrint(c); });

    if (hasSpecialCharacters) {
        QString value;
        for (size_t index = 0; index < actualSize; ++index) {
            value.append(QString("%1").arg(values[index], 0, 16));
        }

        return std::make_unique<SingleValue>(value, OctetStringPrinter());
    } else {
        QString value;
        for (size_t index = 0; index < actualSize; ++index) {
            value.append(QChar(values[index]));
        }

        return std::make_unique<SingleValue>(value, IA5StringPrinter());
    }
}
}
