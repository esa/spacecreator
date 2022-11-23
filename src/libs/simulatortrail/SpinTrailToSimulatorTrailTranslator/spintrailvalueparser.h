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

#pragma once

#include <QStringList>
#include <asn1library/asn1/types/type.h>
#include <asn1library/asn1/value.h>

namespace simulatortrail::translator {
/**
 * @brief Parse value from Spin trail to value of Simulator Trail
 */
class SpinTrailValueParser final
{
public:
    /**
     * @brief Default constructor.
     */
    SpinTrailValueParser();

    /**
     * @brief Parse value of spin trail to ASN.1 value with given type.
     *
     * @param spinTrailValue list of values from Spin Trail
     * @param asn1Type desired ASN.1 type
     * @return ASN.1 value of asn1Type constructed from spinTrailValue
     * @throw TranslationException if value cannot be constructed
     */
    Asn1Acn::ValuePtr parseValue(const QStringList &spinTrailValue, const Asn1Acn::Types::Type *asn1Type);

private:
    Asn1Acn::ValuePtr parseValue(QStringList &spinTrailValue, const Asn1Acn::Types::Type *asn1Type);
    Asn1Acn::ValuePtr parseInteger(QStringList &spinTrailValue);
    Asn1Acn::ValuePtr parseReal(QStringList &spinTrailValue);
    Asn1Acn::ValuePtr parseBoolean(QStringList &spinTrailValue);
    Asn1Acn::ValuePtr parseSequence(QStringList &spinTrailValue, const Asn1Acn::Types::Sequence *sequence);
    Asn1Acn::ValuePtr parseSequenceOf(QStringList &spinTrailValue, const Asn1Acn::Types::SequenceOf *sequenceOf);
    Asn1Acn::ValuePtr parseEnumerated(QStringList &spinTrailValue, const Asn1Acn::Types::Enumerated *enumerated);
    Asn1Acn::ValuePtr parseChoice(QStringList &spinTrailValue, const Asn1Acn::Types::Choice *choice);
    Asn1Acn::ValuePtr parseOctetString(QStringList &spinTrailValue, const Asn1Acn::Types::OctetString *octetString);
    Asn1Acn::ValuePtr parseIA5String(QStringList &spinTrailValue, const Asn1Acn::Types::IA5String *ia5string);
};
}
