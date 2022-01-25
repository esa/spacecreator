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
     * @brief   Constructor
     *
     * @param   asn1Type        ASN.1 type that is currently translated
     */
    explicit SizeTranslatorVisitor(Asn1Acn::Types::Type *asn1Type);
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

public:
    /**
     * @brief   Translate SEDS min-max range
     *
     * @param   range   Range to translate
     */
    auto operator()(const seds::model::MinMaxRange &range) -> void;

public:
    /**
     * @brief   Create size constraint from given range
     *
     * @param   range   Range
     *
     * @return  ASN.1 size constraint
     */
    auto addSizeConstraint(const Asn1Acn::Range<typename Asn1Acn::IntegerValue::Type> &range) -> void;
    /**
     * @brief   Create size constraint from given min and max values
     *
     * @param   range   Range
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
};

template<typename Type, typename ValueType>
SizeTranslatorVisitor<Type, ValueType>::SizeTranslatorVisitor(Asn1Acn::Types::Type *asn1Type)
    : m_asn1Type(dynamic_cast<Type *>(asn1Type))
{
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
void SizeTranslatorVisitor<Type, ValueType>::addSizeConstraint(
        const Asn1Acn::Range<typename Asn1Acn::IntegerValue::Type> &range)
{
    auto rangeConstraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(range);

    auto sizeConstraint = std::make_unique<SizeConstraint>();
    sizeConstraint->setInnerConstraints(std::move(rangeConstraint));

    m_asn1Type->constraints().append(std::move(sizeConstraint));
}

template<typename Type, typename ValueType>
void SizeTranslatorVisitor<Type, ValueType>::addSizeConstraint(
        const typename Asn1Acn::IntegerValue::Type &min, const typename Asn1Acn::IntegerValue::Type &max)
{
    auto rangeConstraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create({ min, max });

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
