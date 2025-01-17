/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "typeconstraintsreconstructingvisitor.h"

#include <asn1library/asn1/constraints/printingvisitor.h>
#include <asn1library/asn1/types/acnparameterizablecomposite.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/integeracnparams.h>
#include <asn1library/asn1/types/null.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>

using namespace Asn1Acn;

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::Boolean &type) const
{
    Q_UNUSED(type);
    return {};
}
QString TypeConstraintsReconstructingVisitor::valueFor(const Types::Null &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::BitString &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::OctetString &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::IA5String &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::NumericString &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::Enumerated &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::Choice &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::Sequence &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::SequenceOf &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::Real &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::LabelType &type) const
{
    Q_UNUSED(type);
    return {};
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::Integer &type) const
{
    return toString(type.constraints());
}

QString TypeConstraintsReconstructingVisitor::valueFor(const Types::UserdefinedType &type) const
{
    if (type.type()) {
        TypeConstraintsReconstructingVisitor visitor;
        type.type()->accept(visitor);
        return visitor.value();
    } else {
        return {};
    }
}
