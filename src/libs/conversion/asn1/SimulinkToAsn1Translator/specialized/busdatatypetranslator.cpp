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

#include "busdatatypetranslator.h"

#include "dimensiontranslator.h"

#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>

using Asn1Acn::AsnSequenceComponent;
using Asn1Acn::SourceLocation;
using Asn1Acn::Types::Sequence;
using Asn1Acn::Types::Type;
using Asn1Acn::Types::UserdefinedType;
using conversion::Escaper;
using conversion::translator::TranslationException;
using simulink::model::BusDataType;
using simulink::model::VectorDimensions;

namespace conversion::asn1::translator {

BusDataTypeTranslator::BusDataTypeTranslator(Context &context)
    : m_context(context)
    , m_dimensionTranslator(context)
{
}

void BusDataTypeTranslator::translate(const BusDataType &busDataType)
{
    const auto busAsn1TypeName = Escaper::escapeAsn1TypeName(busDataType.name());
    auto busAsn1SequenceType = std::make_unique<Sequence>(busAsn1TypeName);

    for (const auto &busMember : busDataType.busMembers()) {
        const auto *busMemberDataTypeAsn1Type = m_context.findAsn1Type(busMember.dataType());
        const auto busMemberAsn1TypeName = Escaper::escapeAsn1FieldName(busMember.name());
        const auto &busMemberDimensions = busMember.dimensions();

        if (const auto *scalarDimensionsPtr = std::get_if<int64_t>(&busMemberDimensions)) {
            const auto scalarDimensions = *scalarDimensionsPtr;

            if (scalarDimensions == 1) {
                auto busMemberAsn1UserdefinedType = std::make_unique<UserdefinedType>(
                        busMemberDataTypeAsn1Type->identifier(), m_context.simulinkModelDefinitionsName());

                appendAsn1TypeToSequence(
                        busAsn1SequenceType.get(), busMemberAsn1TypeName, std::move(busMemberAsn1UserdefinedType));
            } else {
                auto busMemberAsn1SequenceOfType = m_dimensionTranslator.translate(
                        busMemberAsn1TypeName, busMemberDataTypeAsn1Type, scalarDimensions);

                appendAsn1TypeToSequence(
                        busAsn1SequenceType.get(), busMemberAsn1TypeName, std::move(busMemberAsn1SequenceOfType));
            }
        } else if (const auto *vectorDimensionsPtr = std::get_if<VectorDimensions>(&busMemberDimensions)) {
            const auto &vectorDimensions = vectorDimensionsPtr->values();

            auto busMemberAsn1SequenceOfType =
                    m_dimensionTranslator.translate(busMemberDataTypeAsn1Type, vectorDimensions);

            appendAsn1TypeToSequence(
                    busAsn1SequenceType.get(), busMemberAsn1TypeName, std::move(busMemberAsn1SequenceOfType));
        }
    }

    m_context.addSimulinkModelAsn1Type(std::move(busAsn1SequenceType), busDataType.description());
}

void BusDataTypeTranslator::appendAsn1TypeToSequence(
        Sequence *sequence, const QString &sequenceComponentName, std::unique_ptr<Type> asn1Type)
{
    auto sequenceComponent = std::make_unique<AsnSequenceComponent>(sequenceComponentName, sequenceComponentName, false,
            std::nullopt, "", AsnSequenceComponent::Presence::NotSpecified, SourceLocation(), std::move(asn1Type));

    sequence->addComponent(std::move(sequenceComponent));
}

} // namespace conversion::asn1::translator
