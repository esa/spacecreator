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

#include <asn1library/asn1/constraints/constraintlist.h>
#include <asn1library/asn1/constraints/logicoperators.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/types/type.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/translation/exceptions.h>
#include <optional>
#include <seds/SedsModel/types/ranges/enumerateddatatyperange.h>
#include <seds/SedsModel/types/ranges/floatprecisionrange.h>
#include <seds/SedsModel/types/ranges/minmaxrange.h>

namespace conversion::asn1::translator {

/**
 * @brief   Translator for all SEDS ranges
 */
template<typename Type, typename ValueType>
class RangeTranslatorVisitor final
{
    using Constraint = Asn1Acn::Constraints::Constraint<ValueType>;
    using RangeConstraint = Asn1Acn::Constraints::RangeConstraint<ValueType>;
    using OrConstraint = Asn1Acn::Constraints::OrConstraint<ValueType>;

public:
    /**
     * @brief   Constructor
     *
     * @param   asn1Type        ASN.1 type that is currently translated
     */
    explicit RangeTranslatorVisitor(Asn1Acn::Types::Type *asn1Type);
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

public:
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

public:
    /**
     * @brief   Create range constraint from given value
     *
     * @param   value   Range value
     *
     * @return  ASN.1 range constraint
     */
    auto addValueConstraint(const typename ValueType::Type &value) -> void;
    /**
     * @brief   Create range constraint from given range
     *
     * @param   range   Range
     *
     * @return  ASN.1 range constraint
     */
    auto addRangeConstraint(const Asn1Acn::Range<typename ValueType::Type> &range) -> void;
    /**
     * @brief   Create range constraint from given min and max values
     *
     * @param   min     Min range value
     * @param   max     Max range value
     *
     * @return  ASN.1 range constraint
     */
    auto addRangeConstraint(const typename ValueType::Type &min, const typename ValueType::Type &max) -> void;

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
     * @brief   Get smallest value that is valid for the ASN.1 type encoding
     *
     * @return  Smallest value
     */
    auto getSmallest() const -> typename ValueType::Type;
    /**
     * @brief   Get greatest value that is valid for the ASN.1 type encoding
     *
     * @return  Greatest value
     */
    auto getGreatest() const -> typename ValueType::Type;

    /**
     * @brief   Check if range with given min and max would be values
     *
     * @throws  TranslationException    Thrown when range would be invalid
     */
    static auto checkIfValid(typename ValueType::Type min, typename ValueType::Type max) -> void;

private:
    Type *m_asn1Type;
};

template<>
void RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue>::operator()(
        const seds::model::MinMaxRange &range);
template<>
void RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue>::operator()(
        const seds::model::MinMaxRange &range);
template<>
void RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue>::operator()(
        const seds::model::FloatPrecisionRange &range);
template<>
void RangeTranslatorVisitor<Asn1Acn::Types::Enumerated, Asn1Acn::EnumValue>::operator()(
        const seds::model::EnumeratedDataTypeRange &range);

template<typename Type, typename ValueType>
RangeTranslatorVisitor<Type, ValueType>::RangeTranslatorVisitor(Asn1Acn::Types::Type *asn1Type)
    : m_asn1Type(dynamic_cast<Type *>(asn1Type))
{
}

template<typename Type, typename ValueType>
void RangeTranslatorVisitor<Type, ValueType>::operator()(const seds::model::MinMaxRange &range)
{
    Q_UNUSED(range);
    throw ::conversion::translator::TranslationException("Applying MinMaxRange on non-numeric data type is invalid");
}

template<typename Type, typename ValueType>
void RangeTranslatorVisitor<Type, ValueType>::operator()(const seds::model::FloatPrecisionRange &range)
{
    Q_UNUSED(range);
    throw ::conversion::translator::TranslationException(
            "Applying FloatPrecisionRange on non-float data type is invalid");
}

template<typename Type, typename ValueType>
void RangeTranslatorVisitor<Type, ValueType>::operator()(const seds::model::EnumeratedDataTypeRange &range)
{
    Q_UNUSED(range);
    throw ::conversion::translator::TranslationException(
            "Applying EnumeratedDataTypeRange on non-enum data type is invalid");
}

template<typename Type, typename ValueType>
void RangeTranslatorVisitor<Type, ValueType>::addValueConstraint(const typename ValueType::Type &value)
{
    auto constraint = RangeConstraint::create({ value });

    m_asn1Type->constraints().append(std::move(constraint));
}

template<typename Type, typename ValueType>
void RangeTranslatorVisitor<Type, ValueType>::addRangeConstraint(const Asn1Acn::Range<typename ValueType::Type> &range)
{
    auto constraint = RangeConstraint::create(range);

    m_asn1Type->constraints().append(std::move(constraint));
}

template<typename Type, typename ValueType>
void RangeTranslatorVisitor<Type, ValueType>::addRangeConstraint(
        const typename ValueType::Type &min, const typename ValueType::Type &max)
{
    checkIfValid(min, max);
    auto constraint = RangeConstraint::create({ min, max });

    m_asn1Type->constraints().append(std::move(constraint));
}

template<typename Type, typename ValueType>
typename ValueType::Type RangeTranslatorVisitor<Type, ValueType>::getMin(const seds::model::MinMaxRange &range) const
{
    const auto hasMin = range.min().has_value();

    if (!hasMin) {
        auto errorMessage = QString("Missing min value in %1 MinMaxRange").arg(stringFromEnum(range.type()));
        throw conversion::translator::TranslationException(std::move(errorMessage));
    }

    return ValueType::fromAstValue(range.min()->value());
}

template<typename Type, typename ValueType>
typename ValueType::Type RangeTranslatorVisitor<Type, ValueType>::getMax(const seds::model::MinMaxRange &range) const
{
    const auto hasMax = range.max().has_value();

    if (!hasMax) {
        auto errorMessage = QString("Missing max value in %1 MinMaxRange").arg(stringFromEnum(range.type()));
        throw conversion::translator::TranslationException(std::move(errorMessage));
    }

    return ValueType::fromAstValue(range.max()->value());
}

template<typename Type, typename ValueType>
void RangeTranslatorVisitor<Type, ValueType>::checkIfValid(typename ValueType::Type min, typename ValueType::Type max)
{
    if (min > max) {
        throw conversion::translator::TranslationException(QString("Range (%1 .. %2) is invalid").arg(min).arg(max));
    }
}

} // namespace conversion::asn1::translator
