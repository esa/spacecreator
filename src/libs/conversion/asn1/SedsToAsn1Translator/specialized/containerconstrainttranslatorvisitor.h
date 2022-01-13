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

#include <cstdint>

namespace Asn1Acn::Types {
class Integer;
class Real;
class Sequence;
class Type;
} // namespace Asn1Acn::Types

namespace seds::model {
class EntryRef;
class ContainerRangeConstraint;
class ContainerTypeConstraint;
class ContainerValueConstraint;
} // namespace seds::model

namespace conversion::asn1::translator {

class ContainerConstraintTranslatorVisitor final
{
public:
    explicit ContainerConstraintTranslatorVisitor(Asn1Acn::Types::Sequence *asn1Sequence);
    ContainerConstraintTranslatorVisitor(const ContainerConstraintTranslatorVisitor &) = delete;
    ContainerConstraintTranslatorVisitor(ContainerConstraintTranslatorVisitor &&) = delete;
    ContainerConstraintTranslatorVisitor &operator=(const ContainerConstraintTranslatorVisitor &) = delete;
    ContainerConstraintTranslatorVisitor &operator=(ContainerConstraintTranslatorVisitor &&) = delete;

public:
    auto operator()(const seds::model::ContainerRangeConstraint &rangeConstraint) -> void;
    auto operator()(const seds::model::ContainerTypeConstraint &typeConstraint) -> void;
    auto operator()(const seds::model::ContainerValueConstraint &valueConstraint) -> void;

private:
    auto applyContainerRangeConstraint(
            const seds::model::ContainerRangeConstraint &rangeConstraint, Asn1Acn::Types::Type *asn1Type) const -> void;
    auto applyContainerValueConstraint(
            const seds::model::ContainerValueConstraint &valueConstraint, Asn1Acn::Types::Type *asn1Type) const -> void;

    auto getConstrainedType(const seds::model::EntryRef &entry) const -> Asn1Acn::Types::Type *;

    auto getSmallestValue(Asn1Acn::Types::Integer *type) const -> std::int64_t;
    auto getSmallestValue(Asn1Acn::Types::Real *type) const -> double;
    auto getGreatestValue(Asn1Acn::Types::Integer *type) const -> std::int64_t;
    auto getGreatestValue(Asn1Acn::Types::Real *type) const -> double;

private:
    Asn1Acn::Types::Sequence *m_asn1Sequence;
};

} // namespace conversion::asn1::translator
