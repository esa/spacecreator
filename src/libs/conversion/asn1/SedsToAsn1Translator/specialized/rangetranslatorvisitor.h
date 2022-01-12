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
#include <seds/SedsModel/types/ranges/enumerateddatatyperange.h>
#include <seds/SedsModel/types/ranges/floatprecisionrange.h>
#include <seds/SedsModel/types/ranges/minmaxrange.h>

namespace conversion::asn1::translator {

/**
 * @brief   Translator for all SEDS ranges
 */
template<typename ValueType>
class RangeTranslatorVisitor final
{
    using RangeConstraint = Asn1Acn::Constraints::RangeConstraint<ValueType>;

public:
    /**
     * @brief   Constructor
     *
     * @param   asn1Type        ASN.1 type that is currently translated
     * @param   constraints     Constraints of the ASN.1 type that is currently translated
     * @param   smallestValue   Optional smallest value possible to use in range
     * @param   greatestValue   Optional greatest value possible to use in range
     */
    RangeTranslatorVisitor(Asn1Acn::Types::Type *asn1Type, Asn1Acn::Constraints::ConstraintList<ValueType> &constraints,
            std::optional<typename ValueType::Type> smallestValue,
            std::optional<typename ValueType::Type> greatestValue);
    /**
     * @brief   Deleted copy constructor
     */
    RangeTranslatorVisitor(const RangeTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    RangeTranslatorVisitor(RangeTranslatorVisitor &&) = delete;
    /**
     * @brief   Deleted copy assisnment operator
     */
    RangeTranslatorVisitor &operator=(const RangeTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move assisnment operator
     */
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
    /**
     * @brief   Translate SEDS enumerated data type range
     *
     * @param   range   Range to translate
     */
    auto operator()(const seds::model::EnumeratedDataTypeRange &range) -> void;

private:
    /**
     * @brief   Get minimum value from given range
     *
     * @param   ragne       MinMaxRange
     *
     * @return  Minimum value
     */
    auto getMin(const seds::model::MinMaxRange &range) const -> typename ValueType::Type;
    /**
     * @brief   Get maximum value from given range
     *
     * @param   ragne       MinMaxRange
     *
     * @return  Maximum value
     */
    auto getMax(const seds::model::MinMaxRange &range) const -> typename ValueType::Type;
    /**
     * @brief   Get smallest value that could be used in the range
     *
     * @return  Smallest value
     */
    auto getSmallest() const -> typename ValueType::Type;
    /**
     * @brief   Get greatest value that could be used in the range
     *
     * @return  Greatest value
     */
    auto getGreatest() const -> typename ValueType::Type;

    /**
     * @brief   Check if range with given min and max would be values
     *
     * @throws  TranslationException    Thrown when range would be invalid
     */
    auto checkIfValid(typename ValueType::Type min, typename ValueType::Type max) const -> void;

private:
    Asn1Acn::Types::Type *m_asn1Type;
    Asn1Acn::Constraints::ConstraintList<ValueType> &m_constraints;
    std::optional<typename ValueType::Type> m_smallestValue;
    std::optional<typename ValueType::Type> m_greatestValue;
};

template<typename ValueType>
RangeTranslatorVisitor<ValueType>::RangeTranslatorVisitor(Asn1Acn::Types::Type *asn1Type,
        Asn1Acn::Constraints::ConstraintList<ValueType> &constraints,
        std::optional<typename ValueType::Type> smallestValue, std::optional<typename ValueType::Type> greatestValue)
    : m_asn1Type(asn1Type)
    , m_constraints(constraints)
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
    case seds::model::RangeType::LessThan: {
        auto errorMessage =
                QString("Exclusive min-max ranges are not supported for floating point values (in type \"%1\")")
                        .arg(m_asn1Type->identifier());
        throw conversion::translator::TranslationException(std::move(errorMessage));
    } break;
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
        const auto min = static_cast<double>(std::numeric_limits<float>::min());
        const auto max = static_cast<double>(std::numeric_limits<float>::max());

        auto constraint = Asn1Acn::Constraints::RangeConstraint<ValueType>::create({ min, max });
        m_constraints.append(std::move(constraint));
    } break;
    case seds::model::FloatPrecisionRange::Double: {
        const auto min = std::numeric_limits<double>::min();
        const auto max = std::numeric_limits<double>::max();

        auto constraint = Asn1Acn::Constraints::RangeConstraint<ValueType>::create({ min, max });
        m_constraints.append(std::move(constraint));
    } break;
    case seds::model::FloatPrecisionRange::Quad:
        throw conversion::UnsupportedValueException("FloatPrecisionRange", "Quad");
    default:
        throw conversion::UnhandledValueException("FloatPrecisionRange");
    }
}

template<typename ValueType>
void RangeTranslatorVisitor<ValueType>::operator()(const seds::model::EnumeratedDataTypeRange &range)
{
    Q_UNUSED(range);

    throw conversion::translator::TranslationException("EnumeratedDataTypeRange not yet supported");
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
