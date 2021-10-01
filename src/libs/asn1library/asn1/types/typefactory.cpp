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
#include "typefactory.h"

#include "bitstring.h"
#include "boolean.h"
#include "choice.h"
#include "enumerated.h"
#include "ia5string.h"
#include "integer.h"
#include "null.h"
#include "numericstring.h"
#include "octetstring.h"
#include "real.h"
#include "sequence.h"
#include "sequenceof.h"

using namespace Asn1Acn::Types;

std::unique_ptr<Type> TypeFactory::createBuiltinType(const QString &name)
{
    if (name == QStringLiteral("BOOLEAN")) {
        return std::make_unique<Boolean>();
    } else if (name == QStringLiteral("NULL")) {
        return std::make_unique<Null>();
    } else if (name == QStringLiteral("INTEGER")) {
        return std::make_unique<Integer>();
    } else if (name == QStringLiteral("REAL")) {
        return std::make_unique<Real>();
    } else if (name == QStringLiteral("BIT_STRING")) {
        return std::make_unique<BitString>();
    } else if (name == QStringLiteral("OCTET_STRING")) {
        return std::make_unique<OctetString>();
    } else if (name == QStringLiteral("IA5String")) {
        return std::make_unique<IA5String>();
    } else if (name == QStringLiteral("NumericString")) {
        return std::make_unique<NumericString>();
    } else if (name == QStringLiteral("ENUMERATED")) {
        return std::make_unique<Enumerated>();
    } else if (name == QStringLiteral("CHOICE")) {
        return std::make_unique<Choice>();
    } else if (name == QStringLiteral("SEQUENCE")) {
        return std::make_unique<Sequence>();
    } else if (name == QStringLiteral("SEQUENCE_OF")) {
        return std::make_unique<SequenceOf>();
    } else {
        return nullptr;
    }
}
