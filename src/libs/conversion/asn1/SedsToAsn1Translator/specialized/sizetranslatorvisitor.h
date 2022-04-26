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

#include "specialized/rangetranslatorvisitor.h"

#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/translation/exceptions.h>
#include <optional>
#include <seds/SedsModel/types/ranges/minmaxrange.h>

namespace conversion::asn1::translator {

/**
 * @brief   SEDS ranges to ASN.1 size constraint translator
 */
template<typename Type, typename ValueType>
class SizeTranslatorVisitor final
{
    using SizeConstraint = Asn1Acn::Constraints::SizeConstraint<ValueType>;

public:
    /**
     * @brief Length type
     */
    enum class LengthType
    {
        /**
         * @brief Fixed length (e.g. for SEDS Arrays)
         */
        FixedLength,
        /**
         * @brief Variable length (e.g. for SEDS List Entries)
         */
        VariableLength
    };

    /**
     * @brief Type of the source for size derivation
     */
    enum class SourceType
    {
        /**
         * @brief Length is determined by an index
         */
        Index,
        /**
         * @brief Length is determined by a field with a range
         */
        Determinant
    };

    /**
     * @brief   Constructor
     *
     * @param   asn1Type        ASN.1 type that is currently translated
     * @param   lengthType      Type of the desired constraint
     * @param   sourceType      Type of the source to derive the size from
     * @param   threshold       Optional ASN.1 sequence size threshold
     */
    SizeTranslatorVisitor(Asn1Acn::Types::Type *asn1Type, const LengthType lengthType, const SourceType sourceType,
            const std::optional<uint64_t> &threshold);
    /**
     * @brief   Deleted copy constructor
     */
    SizeTranslatorVisitor(const SizeTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    SizeTranslatorVisitor(SizeTranslatorVisitor &&) = delete;
    /**
     * @brief   Deleted copy assisnment operator
     */
    SizeTranslatorVisitor &operator=(const SizeTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move assisnment operator
     */
    SizeTranslatorVisitor &operator=(SizeTranslatorVisitor &&) = delete;

    /**
     * @brief   Returns last set minimum
     *
     * @returns Last set minimum
     */
    auto getLastSetMin() const -> typename Asn1Acn::IntegerValue::Type;

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

public:
    /**
     * @brief   Create size constraint from given maximum value
     *
     * @param   max   Maximum value
     *
     * @return  ASN.1 size constraint
     */
    auto addSizeConstraint(const typename Asn1Acn::IntegerValue::Type max) -> void;
    /**
     * @brief   Create size constraint from given min and max values
     *
     * @param   min   Minimum value
     * @param   max   Maximum value
     *
     * @return  ASN.1 size constraint
     */
    auto addSizeConstraint(
            const typename Asn1Acn::IntegerValue::Type &min, const typename Asn1Acn::IntegerValue::Type &max) -> void;

private:
    /**
     * @brief   Get minimum value from given range
     *
     * @param   ragne       MinMaxRange
     *
     * @return  Minimum value
     */
    auto getMin(const seds::model::MinMaxRange &range) const -> typename Asn1Acn::IntegerValue::Type;
    /**
     * @brief   Get maximum value from given range
     *
     * @param   ragne       MinMaxRange
     *
     * @return  Maximum value
     */
    auto getMax(const seds::model::MinMaxRange &range) const -> typename Asn1Acn::IntegerValue::Type;

private:
    Type *m_asn1Type;
    typename Asn1Acn::IntegerValue::Type m_lastSetMin;
    LengthType m_lengthType;
    SourceType m_sourceType;
    const std::optional<uint64_t> &m_threshold;
};

template<typename Type, typename ValueType>
SizeTranslatorVisitor<Type, ValueType>::SizeTranslatorVisitor(Asn1Acn::Types::Type *asn1Type,
        const LengthType lengthType, const SourceType sourceType, const std::optional<uint64_t> &threshold)
    : m_asn1Type(dynamic_cast<Type *>(asn1Type))
    , m_lengthType(lengthType)
    , m_sourceType(sourceType)
    , m_threshold(threshold)
{
}

template<typename Type, typename ValueType>
auto SizeTranslatorVisitor<Type, ValueType>::getLastSetMin() const -> typename Asn1Acn::IntegerValue::Type
{
    return m_lastSetMin;
}

template<typename Type, typename ValueType>
void SizeTranslatorVisitor<Type, ValueType>::operator()(const seds::model::MinMaxRange &range)
{
    switch (range.type()) {
    case seds::model::RangeType::ExclusiveMinExclusiveMax: {
        const auto min = getMin(range) + 1;
        const auto max = getMax(range) - 1;

        addSizeConstraint(min, max);
    } break;
    case seds::model::RangeType::InclusiveMinInclusiveMax: {
        const auto min = getMin(range);
        const auto max = getMax(range);

        addSizeConstraint(min, max);
    } break;
    case seds::model::RangeType::InclusiveMinExclusiveMax: {
        const auto min = getMin(range);
        const auto max = getMax(range) - 1;

        addSizeConstraint(min, max);
    } break;
    case seds::model::RangeType::ExclusiveMinInclusiveMax: {
        const auto min = getMin(range) + 1;
        const auto max = getMax(range);

        addSizeConstraint(min, max);
    } break;
    case seds::model::RangeType::GreaterThan: {
        const auto min = getMin(range) + 1;
        const auto max = std::numeric_limits<Asn1Acn::IntegerValue::Type>::max();

        addSizeConstraint(min, max);
    } break;
    case seds::model::RangeType::LessThan: {
        const auto min = std::numeric_limits<Asn1Acn::IntegerValue::Type>::min();
        const auto max = getMax(range) - 1;

        addSizeConstraint(min, max);
    } break;
    case seds::model::RangeType::AtLeast: {
        const auto min = getMin(range);
        const auto max = std::numeric_limits<Asn1Acn::IntegerValue::Type>::max();

        addSizeConstraint(min, max);
    } break;
    case seds::model::RangeType::AtMost: {
        const auto min = std::numeric_limits<Asn1Acn::IntegerValue::Type>::min();
        const auto max = getMax(range);

        addSizeConstraint(min, max);
    } break;
    }
}

template<typename Type, typename ValueType>
void SizeTranslatorVisitor<Type, ValueType>::addSizeConstraint(const typename Asn1Acn::IntegerValue::Type max)
{
    addSizeConstraint(Asn1Acn::IntegerValue::Type(), max);
}

template<typename Type, typename ValueType>
void SizeTranslatorVisitor<Type, ValueType>::addSizeConstraint(
        const typename Asn1Acn::IntegerValue::Type &min, const typename Asn1Acn::IntegerValue::Type &max)
{
    // This is currently a NOP, but in theory, there should be a difference whether the size
    // is derived from index o a determinant field. Let's consider the following examples:
    // length field with range 0..255 -> implies size 0..255
    // index field with range 0..255 -> implies size 0..256 (so that 255 is the last valid index)
    // However, CCSDS 876.0-B-1 explicitly states that:
    // "The maximum length of an array with a specified index type can be found by looking
    // at the maximum value of the index range (e.g., an array with index type ‘8 bit
    // unsigned’ has a maximum size of 255)."
    // Index type "8-bit" has valid values of 0..255, and if the maximum size of an array is 255,
    // then index 255 is invalid. Therefore it is assumed that the last index value is treated
    // as an invalid/length identifier, similarly in concept to end().
    auto actualMax = max;

    if (m_threshold.has_value() && actualMax > static_cast<long>(*m_threshold)) {
        actualMax = static_cast<long>(*m_threshold);
    }

    m_lastSetMin = min;
    auto rangeConstraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
            { m_lengthType == LengthType::FixedLength ? actualMax : min, actualMax });

    auto sizeConstraint = std::make_unique<SizeConstraint>();
    sizeConstraint->setInnerConstraints(std::move(rangeConstraint));

    m_asn1Type->constraints().append(std::move(sizeConstraint));
}

template<typename Type, typename ValueType>
typename Asn1Acn::IntegerValue::Type SizeTranslatorVisitor<Type, ValueType>::getMin(
        const seds::model::MinMaxRange &range) const
{
    const auto hasMin = range.min().has_value();

    if (!hasMin) {
        auto errorMessage = QString("Missing min value in %1 MinMaxRange").arg(stringFromEnum(range.type()));
        throw conversion::translator::TranslationException(std::move(errorMessage));
    }

    return Asn1Acn::IntegerValue::fromAstValue(range.min()->value());
}

template<typename Type, typename ValueType>
typename Asn1Acn::IntegerValue::Type SizeTranslatorVisitor<Type, ValueType>::getMax(
        const seds::model::MinMaxRange &range) const
{
    const auto hasMax = range.max().has_value();

    if (!hasMax) {
        auto errorMessage = QString("Missing max value in %1 MinMaxRange").arg(stringFromEnum(range.type()));
        throw conversion::translator::TranslationException(std::move(errorMessage));
    }

    return Asn1Acn::IntegerValue::fromAstValue(range.max()->value());
}

} // namespace conversion::asn1::translator
