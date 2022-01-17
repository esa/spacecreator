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

#include "specialized/rangetranslatorvisitor.h"

#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <cmath>

namespace conversion::asn1::translator {

template<>
typename Asn1Acn::IntegerValue::Type
RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue>::getSmallest() const;
template<>
typename Asn1Acn::RealValue::Type RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue>::getSmallest() const;
template<>
typename Asn1Acn::IntegerValue::Type
RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue>::getGreatest() const;
template<>
typename Asn1Acn::RealValue::Type RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue>::getGreatest() const;

template<>
void RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue>::operator()(
        const seds::model::MinMaxRange &range)
{
    switch (range.type()) {
    case seds::model::RangeType::ExclusiveMinExclusiveMax: {
        const auto min = getMin(range) + 1;
        const auto max = getMax(range) - 1;

        addRangeConstraint(min, max);
    } break;
    case seds::model::RangeType::InclusiveMinInclusiveMax: {
        const auto min = getMin(range);
        const auto max = getMax(range);

        addRangeConstraint(min, max);
    } break;
    case seds::model::RangeType::InclusiveMinExclusiveMax: {
        const auto min = getMin(range);
        const auto max = getMax(range) - 1;

        addRangeConstraint(min, max);
    } break;
    case seds::model::RangeType::ExclusiveMinInclusiveMax: {
        const auto min = getMin(range) + 1;
        const auto max = getMax(range);

        addRangeConstraint(min, max);
    } break;
    case seds::model::RangeType::GreaterThan: {
        const auto min = getMin(range) + 1;
        const auto max = getGreatest();

        addRangeConstraint(min, max);
    } break;
    case seds::model::RangeType::LessThan: {
        const auto min = getSmallest();
        const auto max = getMax(range) - 1;

        addRangeConstraint(min, max);
    } break;
    case seds::model::RangeType::AtLeast: {
        const auto min = getMin(range);
        const auto max = getGreatest();

        addRangeConstraint(min, max);
    } break;
    case seds::model::RangeType::AtMost: {
        const auto min = getSmallest();
        const auto max = getMax(range);

        addRangeConstraint(min, max);
    } break;
    }
}

template<>
void RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue>::operator()(const seds::model::MinMaxRange &range)
{
    switch (range.type()) {
    case seds::model::RangeType::ExclusiveMinExclusiveMax:
    case seds::model::RangeType::InclusiveMinExclusiveMax:
    case seds::model::RangeType::ExclusiveMinInclusiveMax:
    case seds::model::RangeType::GreaterThan:
    case seds::model::RangeType::LessThan: {
        auto errorMessage =
                QString("Exclusive min-max ranges are not supported for floating point values (in type \"%1\")")
                        .arg(m_asn1Type->identifier());
        throw conversion::translator::TranslationException(std::move(errorMessage));
    } break;
    case seds::model::RangeType::InclusiveMinInclusiveMax: {
        const auto min = getMin(range);
        const auto max = getMax(range);

        addRangeConstraint(min, max);
    } break;
    case seds::model::RangeType::AtLeast: {
        const auto min = getMin(range);
        const auto max = getGreatest();

        addRangeConstraint(min, max);
    } break;
    case seds::model::RangeType::AtMost: {
        const auto min = getSmallest();
        const auto max = getMax(range);

        addRangeConstraint(min, max);
    } break;
    }
}

template<>
void RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue>::operator()(
        const seds::model::FloatPrecisionRange &range)
{
    switch (range) {
    case seds::model::FloatPrecisionRange::Single: {
        const auto min = static_cast<double>(std::numeric_limits<float>::min());
        const auto max = static_cast<double>(std::numeric_limits<float>::max());

        addRangeConstraint(min, max);
    } break;
    case seds::model::FloatPrecisionRange::Double: {
        const auto min = std::numeric_limits<double>::min();
        const auto max = std::numeric_limits<double>::max();

        addRangeConstraint(min, max);
    } break;
    case seds::model::FloatPrecisionRange::Quad:
        throw conversion::UnsupportedValueException("FloatPrecisionRange", "Quad");
    default:
        throw conversion::UnhandledValueException("FloatPrecisionRange");
    }
}

template<>
typename Asn1Acn::IntegerValue::Type
RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue>::getSmallest() const
{
    switch (m_asn1Type->encoding()) {
    case Asn1Acn::Types::IntegerEncoding::pos_int:
        return 0;
    case Asn1Acn::Types::IntegerEncoding::twos_complement:
        return -std::pow(2, m_asn1Type->size() - 1);
    case Asn1Acn::Types::IntegerEncoding::ASCII:
        return -std::pow(10, (m_asn1Type->size() / 8) - 1);
    case Asn1Acn::Types::IntegerEncoding::BCD:
        return 0;
    case Asn1Acn::Types::IntegerEncoding::unspecified:
        return std::numeric_limits<Asn1Acn::IntegerValue::Type>::min();
    default:
        throw conversion::translator::TranslationException("Unhandled IntegerEncoding for container constraint");
        break;
    }
}

template<>
typename Asn1Acn::RealValue::Type RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue>::getSmallest() const
{
    switch (m_asn1Type->encoding()) {
    case Asn1Acn::Types::RealEncoding::IEEE754_1985_32:
        return static_cast<double>(std::numeric_limits<float>::min());
    case Asn1Acn::Types::RealEncoding::IEEE754_1985_64:
        return std::numeric_limits<double>::min();
    case Asn1Acn::Types::RealEncoding::unspecified:
        return std::numeric_limits<Asn1Acn::RealValue::Type>::min();
    default:
        throw conversion::translator::TranslationException("Unhandled RealEncoding for container constraint");
        break;
    }
}

template<>
typename Asn1Acn::IntegerValue::Type
RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue>::getGreatest() const
{
    switch (m_asn1Type->encoding()) {
    case Asn1Acn::Types::IntegerEncoding::pos_int:
        return std::pow(2, m_asn1Type->size()) - 1;
    case Asn1Acn::Types::IntegerEncoding::twos_complement:
        return std::pow(2, m_asn1Type->size() - 1) - 1;
    case Asn1Acn::Types::IntegerEncoding::ASCII:
        return std::pow(10, (m_asn1Type->size() / 8)) - 1;
    case Asn1Acn::Types::IntegerEncoding::BCD:
        return std::pow(10, (m_asn1Type->size() / 4)) - 1;
    case Asn1Acn::Types::IntegerEncoding::unspecified:
        return std::numeric_limits<Asn1Acn::IntegerValue::Type>::max();
    default:
        throw conversion::translator::TranslationException("Unhandled IntegerEncoding for container constraint");
        break;
    }
}

template<>
typename Asn1Acn::RealValue::Type RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue>::getGreatest() const
{
    switch (m_asn1Type->encoding()) {
    case Asn1Acn::Types::RealEncoding::IEEE754_1985_32:
        return static_cast<double>(std::numeric_limits<float>::max());
    case Asn1Acn::Types::RealEncoding::IEEE754_1985_64:
        return std::numeric_limits<double>::max();
    case Asn1Acn::Types::RealEncoding::unspecified:
        return std::numeric_limits<Asn1Acn::RealValue::Type>::max();
    default:
        throw conversion::translator::TranslationException("Unhandled RealEncoding for container constraint");
        break;
    }
}

} // namespace conversion::asn1::translator
