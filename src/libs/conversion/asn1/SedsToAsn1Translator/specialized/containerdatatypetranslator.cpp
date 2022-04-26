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

#include "specialized/containerdatatypetranslator.h"

#include "specialized/containerconstrainttranslatorvisitor.h"

#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/types/choice.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/package/package.h>
#include <seds/SedsModel/types/datatype.h>

using conversion::translator::TranslationException;

namespace conversion::asn1::translator {

ContainerDataTypeTranslator::ContainerDataTypeTranslator(Context &context)
    : m_context(context)
{
}

void ContainerDataTypeTranslator::translate(const seds::model::ContainerDataType &sedsType)
{
    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::Sequence>(asn1TypeName);

    EntryTranslatorVisitor entryTranslator(m_context, sedsType, asn1Type.get());

    if (!sedsType.isAbstract()) {
        handleBaseTypeEntries(sedsType, entryTranslator);
        handleEntries(sedsType.entries(), entryTranslator);
        handleTrailerEntries(sedsType.trailerEntries(), entryTranslator);
        handleBaseTypeTrailerEntries(sedsType, entryTranslator);
    }

    if (sedsType.baseType()) {
        applyContainerConstraints(sedsType, asn1Type.get());
        updateParentContainer(*sedsType.baseType(), asn1Type.get());
    }

    m_context.addAsn1Type(std::move(asn1Type), &sedsType);
}

void ContainerDataTypeTranslator::handleEntries(
        const std::vector<seds::model::EntryType> &entries, EntryTranslatorVisitor &entryTranslator) const
{
    for (const auto &entry : entries) {
        std::visit(entryTranslator, entry);
    }
}

void ContainerDataTypeTranslator::handleTrailerEntries(
        const std::vector<seds::model::EntryType> &trailerEntries, EntryTranslatorVisitor &entryTranslator) const
{
    for (const auto &trailerEntry : trailerEntries) {
        std::visit(entryTranslator, trailerEntry);
    }
}

void ContainerDataTypeTranslator::handleBaseTypeEntries(
        const seds::model::ContainerDataType &sedsType, EntryTranslatorVisitor &entryTranslator) const
{
    if (!sedsType.baseType()) {
        return;
    }

    const auto baseType = m_context.findSedsType(*sedsType.baseType());
    const auto baseContainerType = std::get_if<seds::model::ContainerDataType>(baseType);

    if (baseContainerType == nullptr) {
        auto errorMessage = QString("Container '%1' has a base type '%2' that is not a container")
                                    .arg(sedsType.nameStr())
                                    .arg(dataTypeNameStr(*baseType));
        throw TranslationException(std::move(errorMessage));
    }

    handleBaseTypeEntries(*baseContainerType, entryTranslator);
    handleEntries(baseContainerType->entries(), entryTranslator);
}

void ContainerDataTypeTranslator::handleBaseTypeTrailerEntries(
        const seds::model::ContainerDataType &sedsType, EntryTranslatorVisitor &entryTranslator) const
{
    if (!sedsType.baseType()) {
        return;
    }

    const auto baseType = m_context.findSedsType(*sedsType.baseType());
    const auto baseContainerType = std::get_if<seds::model::ContainerDataType>(baseType);

    if (baseContainerType == nullptr) {
        auto errorMessage = QString("Container '%1' has a base type '%2' that is not a container")
                                    .arg(sedsType.nameStr())
                                    .arg(dataTypeNameStr(*baseType));
        throw TranslationException(std::move(errorMessage));
    }

    handleEntries(baseContainerType->trailerEntries(), entryTranslator);
    handleBaseTypeTrailerEntries(*baseContainerType, entryTranslator);
}

void ContainerDataTypeTranslator::applyContainerConstraints(
        const seds::model::ContainerDataType &sedsType, Asn1Acn::Types::Sequence *asn1Type) const
{
    ContainerConstraintTranslatorVisitor constraintTranslator(m_context, asn1Type);

    for (const auto &constraint : sedsType.constraints()) {
        std::visit(constraintTranslator, constraint);
    }
}

void ContainerDataTypeTranslator::updateParentContainer(
        const seds::model::DataTypeRef &baseTypeRef, Asn1Acn::Types::Sequence *asn1Type)
{
    auto asn1BaseType = m_context.findAsn1Type(baseTypeRef);
    auto asn1BaseSequence = dynamic_cast<Asn1Acn::Types::Sequence *>(asn1BaseType);

    auto *asn1RealizationComponent = asn1BaseSequence->component(m_realizationComponentsName);
    if (!asn1RealizationComponent) {
        createRealizationContainerField(asn1BaseSequence);
        asn1RealizationComponent = asn1BaseSequence->component(m_realizationComponentsName);
    }

    auto *asn1RealizationChoice = dynamic_cast<Asn1Acn::Types::Choice *>(asn1RealizationComponent->type());
    const auto asn1RealizationChoiceAlternativeName =
            m_realizationComponentsAlternativeNameTemplate.arg(asn1Type->identifier());

    auto asn1SequenceReference =
            std::make_unique<Asn1Acn::Types::UserdefinedType>(asn1Type->identifier(), m_context.definitionsName());
    auto asn1RealizationChoiceAlternative = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
            asn1RealizationChoiceAlternativeName, asn1RealizationChoiceAlternativeName,
            asn1RealizationChoiceAlternativeName, asn1RealizationChoiceAlternativeName, "", Asn1Acn::SourceLocation(),
            std::move(asn1SequenceReference));
    asn1RealizationChoice->addComponent(std::move(asn1RealizationChoiceAlternative));
}

void ContainerDataTypeTranslator::createRealizationContainerField(Asn1Acn::Types::Sequence *asn1Sequence)
{
    auto realizationChoice = std::make_unique<Asn1Acn::Types::Choice>();

    if (!asn1Sequence->components().empty()) {
        auto ownRealizationSequence = std::make_unique<Asn1Acn::Types::Sequence>();
        for (const auto &asn1SequenceComponent : asn1Sequence->components()) {
            ownRealizationSequence->addComponent(asn1SequenceComponent->clone());
        }
        asn1Sequence->components().clear();

        const auto ownRealizationChoiceAlternativeName =
                m_realizationComponentsAlternativeNameTemplate.arg(asn1Sequence->identifier());
        auto ownRealizationChoiceAlternative = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
                ownRealizationChoiceAlternativeName, ownRealizationChoiceAlternativeName,
                ownRealizationChoiceAlternativeName, ownRealizationChoiceAlternativeName, "", Asn1Acn::SourceLocation(),
                std::move(ownRealizationSequence));
        realizationChoice->addComponent(std::move(ownRealizationChoiceAlternative));
    }

    auto realizationComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(m_realizationComponentsName,
            m_realizationComponentsName, false, std::nullopt, "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified,
            Asn1Acn::SourceLocation(), std::move(realizationChoice));
    asn1Sequence->addComponent(std::move(realizationComponent));
}

} // namespace conversion::asn1::translator
