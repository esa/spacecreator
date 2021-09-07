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

#pragma once

#include <QString>
#include <asn1library/asn1/types/typevisitorwithvalue.h>

namespace Asn1Acn {

class TypeConstraintsReconstructingVisitor : public Types::TypeVisitorWithValue<QString>
{
public:
    TypeConstraintsReconstructingVisitor() = default;
    ~TypeConstraintsReconstructingVisitor() override = default;

    QString valueFor(const Types::Boolean &type) const override;
    QString valueFor(const Types::Null &type) const override;
    QString valueFor(const Types::BitString &type) const override;
    QString valueFor(const Types::OctetString &type) const override;
    QString valueFor(const Types::IA5String &type) const override;
    QString valueFor(const Types::NumericString &type) const override;
    QString valueFor(const Types::Enumerated &type) const override;
    QString valueFor(const Types::Choice &type) const override;
    QString valueFor(const Types::Sequence &type) const override;
    QString valueFor(const Types::SequenceOf &type) const override;
    QString valueFor(const Types::Real &type) const override;
    QString valueFor(const Types::LabelType &type) const override;
    QString valueFor(const Types::Integer &type) const override;
    QString valueFor(const Types::UserdefinedType &type) const override;
};

} // namespace Asn1Acn
