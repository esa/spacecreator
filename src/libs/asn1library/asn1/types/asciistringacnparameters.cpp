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
#include "asciistringacnparameters.h"

using namespace Asn1Acn::Types;

AsciiStringAcnParameters::AsciiStringAcnParameters()
    : m_encoding(AsciiStringEncoding::unspecified)
{
}

AsciiStringEncoding AsciiStringAcnParameters::mapEncoding(QStringView in)
{
    if (in == QLatin1String("ASCII"))
        return AsciiStringEncoding::ASCII;
    return AsciiStringEncoding::unspecified;
}

QString AsciiStringAcnParameters::encodingToString(AsciiStringEncoding encoding)
{
    if (encoding == AsciiStringEncoding::ASCII)
        return QStringLiteral("ASCII");
    return {};
}
