/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "specialized/arraydatatypetranslator.h"

#include "datatypetranslationhelper.h"

#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>

using conversion::translator::TranslationException;

namespace conversion::asn1::translator::seds {

ArrayDataTypeTranslator::ArrayDataTypeTranslator(Context &context)
    : m_context(context)
    , m_dimensionTranslator(m_context)
{
}

void ArrayDataTypeTranslator::translate(const ::seds::model::ArrayDataType &sedsType)
{
    const auto &dimensions = sedsType.dimensions();

    if (dimensions.empty()) {
        throw TranslationException("Encountered ArrayDataType without dimensions");
    }

    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());

    if (dimensions.size() == 1) { // Sequence of type with one dimension
        auto asn1Type = std::make_unique<Asn1Acn::Types::SequenceOf>(asn1TypeName);

        translateArrayType(sedsType.type(), asn1Type.get());

        m_dimensionTranslator.translate(dimensions[0], asn1Type.get());

        m_context.addAsn1Type(std::move(asn1Type), &sedsType);
    } else { // Sequence of with many dimensions
        // The outermost 'sequence of' element
        auto rootType = std::make_unique<Asn1Acn::Types::SequenceOf>(asn1TypeName);

        m_dimensionTranslator.translate(dimensions[0], rootType.get());

        // Create 'sequence of' chain
        auto *lastType = rootType.get();
        std::for_each(std::next(dimensions.begin()), dimensions.end(), [&](const auto &dimension) {
            auto subType = std::make_unique<Asn1Acn::Types::SequenceOf>();
            m_dimensionTranslator.translate(dimension, subType.get());
            lastType->setItemsType(std::move(subType));

            lastType = dynamic_cast<Asn1Acn::Types::SequenceOf *>(lastType->itemsType());
        });

        // Add item type to the last element
        translateArrayType(sedsType.type(), lastType);

        m_context.addAsn1Type(std::move(rootType), &sedsType);
    }
}

void ArrayDataTypeTranslator::translateArrayType(
        const ::seds::model::DataTypeRef &sedsTypeRef, Asn1Acn::Types::SequenceOf *asn1Type) const
{
    auto asn1ReferencedType = m_context.findAsn1Type(sedsTypeRef)->clone();

    DataTypeTranslationHelper::removeConstraints(asn1ReferencedType.get());

    auto asn1ItemType = std::make_unique<Asn1Acn::Types::UserdefinedType>(
            asn1ReferencedType->identifier(), m_context.definitionsName());
    asn1ItemType->setType(std::move(asn1ReferencedType));

    asn1Type->setItemsType(std::move(asn1ItemType));
}

} // namespace conversion::asn1::translator::seds
