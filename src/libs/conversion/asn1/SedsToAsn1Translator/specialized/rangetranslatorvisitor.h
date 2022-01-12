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

#include <conversion/common/translation/exceptions.h>
#include <optional>
#include <seds/SedsModel/types/ranges/floatprecisionrange.h>
#include <seds/SedsModel/types/ranges/minmaxrange.h>

namespace conversion::asn1::translator {

template<typename ValueType>
class RangeTranslatorVisitor final
{
    using RangeConstraint = Asn1Acn::Constraints::RangeConstraint<ValueType>;

public:
    RangeTranslatorVisitor(Asn1Acn::Constraints::ConstraintList<ValueType> &constraints,
            std::optional<typename ValueType::Type> smallestValue,
            std::optional<typename ValueType::Type> greatestValue);
    RangeTranslatorVisitor(const RangeTranslatorVisitor &) = delete;
    RangeTranslatorVisitor(RangeTranslatorVisitor &&) = delete;
    RangeTranslatorVisitor &operator=(const RangeTranslatorVisitor &) = delete;
    RangeTranslatorVisitor &operator=(RangeTranslatorVisitor &&) = delete;

    /**
     * @brief   Translate SEDS min-max range
     *
     * @param   range   Range to translate
     */
    auto operator()(const seds::model::MinMaxRange &range) -> void;
    /**
     * @brief   Translate SEDS float precision range
     *
     * @param   range   Range to translate
     */
    auto operator()(const seds::model::FloatPrecisionRange &range) -> void;

private:
    auto getMin(const seds::model::MinMaxRange &range) const -> typename ValueType::Type;
    auto getMax(const seds::model::MinMaxRange &range) const -> typename ValueType::Type;
    auto getSmallest() const -> typename ValueType::Type;
    auto getGreatest() const -> typename ValueType::Type;

    auto checkIfValid(typename ValueType::Type min, typename ValueType::Type max) const -> void;

private:
    Asn1Acn::Constraints::ConstraintList<ValueType> &m_constraints;
    std::optional<typename ValueType::Type> m_smallestValue;
    std::optional<typename ValueType::Type> m_greatestValue;

    /** @brief   Smallest range value for single precision */
    constexpr static double singleRangeMin = 1.17549E-38;
    /** @brief   Greatest range value for single precision */
    constexpr static double singleRangeMax = 3.40282e+38;
    /** @brief   Smallest range value for double precision */
    constexpr static double doubleRangeMin = 2.22507e-308;
    /** @brief   Greate range value for double precision */
    constexpr static double doubleRangeMax = 1.79769e+308;
};

template<typename ValueType>
RangeTranslatorVisitor<ValueType>::RangeTranslatorVisitor(Asn1Acn::Constraints::ConstraintList<ValueType> &constraints,
        std::optional<typename ValueType::Type> smallestValue, std::optional<typename ValueType::Type> greatestValue)
    : m_constraints(constraints)
    , m_smallestValue(std::move(smallestValue))
    , m_greatestValue(std::move(greatestValue))
{
}

template<>
void RangeTranslatorVisitor<Asn1Acn::RealValue>::operator()(const seds::model::MinMaxRange &range)
{
    switch (range.type()) {
    case seds::model::RangeType::ExclusiveMinExclusiveMax:
    case seds::model::RangeType::InclusiveMinExclusiveMax:
    case seds::model::RangeType::ExclusiveMinInclusiveMax:
    case seds::model::RangeType::GreaterThan:
    case seds::model::RangeType::LessThan:
        throw conversion::translator::TranslationException(
                "Exclusive min-max ranges are not supported for floating point values");
        break;
    case seds::model::RangeType::InclusiveMinInclusiveMax: {
        const auto min = getMin(range);
        const auto max = getMax(range);
        checkIfValid(min, max);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    case seds::model::RangeType::AtLeast: {
        const auto min = getMin(range);
        const auto max = getGreatest();
        checkIfValid(min, max);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    case seds::model::RangeType::AtMost: {
        const auto min = getSmallest();
        const auto max = getMax(range);
        checkIfValid(min, max);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    }
}

template<>
void RangeTranslatorVisitor<Asn1Acn::IntegerValue>::operator()(const seds::model::MinMaxRange &range)
{
    switch (range.type()) {
    case seds::model::RangeType::ExclusiveMinExclusiveMax: {
        const auto min = getMin(range) + 1;
        const auto max = getMax(range) - 1;
        checkIfValid(min, max);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    case seds::model::RangeType::InclusiveMinInclusiveMax: {
        const auto min = getMin(range);
        const auto max = getMax(range);
        checkIfValid(min, max);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    case seds::model::RangeType::InclusiveMinExclusiveMax: {
        const auto min = getMin(range);
        const auto max = getMax(range) - 1;
        checkIfValid(min, max);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    case seds::model::RangeType::ExclusiveMinInclusiveMax: {
        const auto min = getMin(range) + 1;
        const auto max = getMax(range);
        checkIfValid(min, max);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    case seds::model::RangeType::GreaterThan: {
        const auto min = getMin(range) + 1;
        const auto max = getGreatest();
        checkIfValid(min, max);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    case seds::model::RangeType::LessThan: {
        const auto min = getSmallest();
        const auto max = getMax(range) - 1;
        checkIfValid(min, max);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    case seds::model::RangeType::AtLeast: {
        const auto min = getMin(range);
        const auto max = getGreatest();
        checkIfValid(min, max);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    case seds::model::RangeType::AtMost: {
        const auto min = getSmallest();
        const auto max = getMax(range);
        checkIfValid(min, max);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    }
}

template<typename ValueType>
void RangeTranslatorVisitor<ValueType>::operator()(const seds::model::FloatPrecisionRange &range)
{
    switch (range) {
    case seds::model::FloatPrecisionRange::Single: {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<ValueType>::create({ singleRangeMin, singleRangeMax });
        m_constraints.append(std::move(constraint));
    } break;
    case seds::model::FloatPrecisionRange::Double: {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<ValueType>::create({ doubleRangeMin, doubleRangeMax });
        m_constraints.append(std::move(constraint));
    } break;
    case seds::model::FloatPrecisionRange::Quad:
        throw conversion::UnsupportedValueException("FloatPrecisionRange", "Quad");
    default:
        throw conversion::UnhandledValueException("FloatPrecisionRange");
    }
}

template<typename ValueType>
typename ValueType::Type RangeTranslatorVisitor<ValueType>::getMin(const seds::model::MinMaxRange &range) const
{
    const auto hasMin = range.min().has_value();

    if (!hasMin) {
        auto errorMessage = QString("Missing min value in %1 MinMaxRange").arg(stringFromEnum(range.type()));
        throw conversion::translator::TranslationException(std::move(errorMessage));
    }

    return ValueType::fromAstValue(range.min()->value());
}

template<typename ValueType>
typename ValueType::Type RangeTranslatorVisitor<ValueType>::getMax(const seds::model::MinMaxRange &range) const
{
    const auto hasMax = range.max().has_value();

    if (!hasMax) {
        auto errorMessage = QString("Missing max value in %1 MinMaxRange").arg(stringFromEnum(range.type()));
        throw conversion::translator::TranslationException(std::move(errorMessage));
    }

    return ValueType::fromAstValue(range.max()->value());
}

template<typename ValueType>
typename ValueType::Type RangeTranslatorVisitor<ValueType>::getSmallest() const
{
    if (m_smallestValue) {
        return m_smallestValue.value();
    } else {
        return std::numeric_limits<typename ValueType::Type>::min();
    }
}

template<typename ValueType>
typename ValueType::Type RangeTranslatorVisitor<ValueType>::getGreatest() const
{
    if (m_greatestValue) {
        return m_greatestValue.value();
    } else {
        return std::numeric_limits<typename ValueType::Type>::max();
    }
}

template<typename ValueType>
void RangeTranslatorVisitor<ValueType>::checkIfValid(typename ValueType::Type min, typename ValueType::Type max) const
{
    if (min > max) {
        throw conversion::translator::TranslationException(QString("Range (%1 .. %2) is invalid").arg(min).arg(max));
    }
}

} // namespace conversion::asn1::translator
