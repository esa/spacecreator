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

#include "containerentriesscope.h"

#include "specialized/entrytranslatorvisitor.h"

#include <conversion/common/translation/exceptions.h>

using conversion::translator::TranslationException;
using seds::model::ContainerDataType;

namespace conversion::asn1::translator {

ContainerEntriesScope::ContainerEntriesScope(Asn1Acn::Definitions *asn1Definitions,
        const seds::model::Package *sedsPackage, const Asn1Acn::Asn1Model::Data &asn1Files,
        const std::vector<seds::model::Package> &sedsPackages, const std::optional<uint64_t> &sequenceSizeThreshold)
    : m_asn1Definitions(asn1Definitions)
    , m_sedsPackage(sedsPackage)
    , m_asn1Files(asn1Files)
    , m_sedsPackages(sedsPackages)
    , m_sequenceSizeThreshold(sequenceSizeThreshold)
{
}

void ContainerEntriesScope::addContainer(const ContainerDataType &sedsType)
{
    auto asn1SequenceComponents = std::make_unique<Asn1Acn::Types::Sequence>();

    // Get parent entries
    if (sedsType.baseType()) {
        const auto &sedsBaseTypeName = sedsType.baseType()->nameStr();
        const auto &asn1ParentComponents = fetchComponents(sedsBaseTypeName);

        for (const auto &asn1Component : asn1ParentComponents) {
            asn1SequenceComponents->addComponent(asn1Component->clone());
        }
    }

    // Translate own entries
    EntryTranslatorVisitor entriesTranslator(asn1SequenceComponents.get(), m_asn1Definitions, &sedsType, m_sedsPackage,
            m_asn1Files, m_sedsPackages, m_sequenceSizeThreshold);
    for (const auto &sedsEntry : sedsType.entries()) {
        std::visit(entriesTranslator, sedsEntry);
    }

    auto asn1SequenceTrailerComponents = std::make_unique<Asn1Acn::Types::Sequence>();

    if (sedsType.isAbstract()) {
        // Translate own trailer entries
        EntryTranslatorVisitor trailerEntriesTranslator(asn1SequenceTrailerComponents.get(), m_asn1Definitions,
                &sedsType, m_sedsPackage, m_asn1Files, m_sedsPackages, m_sequenceSizeThreshold);
        for (const auto &sedsTrailerEntry : sedsType.trailerEntries()) {
            std::visit(trailerEntriesTranslator, sedsTrailerEntry);
        }
    }

    // Get parent trailer entries
    if (sedsType.baseType()) {
        const auto &sedsBaseTypeName = sedsType.baseType()->nameStr();
        const auto &asn1ParentTrailerComponents = fetchTrailerComponents(sedsBaseTypeName);

        for (const auto &asn1TrailerComponent : asn1ParentTrailerComponents) {
            asn1SequenceTrailerComponents->addComponent(asn1TrailerComponent->clone());
        }
    }

    const auto patcherFunctions = hasPatcherFunctions(sedsType);

    // Save this type
    ScopeEntry entry { std::move(asn1SequenceComponents), std::move(asn1SequenceTrailerComponents), patcherFunctions };
    m_scope.insert({ sedsType.nameStr(), std::move(entry) });
}

const Asn1Acn::Types::Sequence::Components &ContainerEntriesScope::fetchComponents(const QString &sedsTypeName) const
{
    assertPresent(sedsTypeName);
    return m_scope.at(sedsTypeName).entries->components();
}

const Asn1Acn::Types::Sequence::Components &ContainerEntriesScope::fetchTrailerComponents(
        const QString &sedsTypeName) const
{
    assertPresent(sedsTypeName);
    return m_scope.at(sedsTypeName).trailerEntries->components();
}

bool ContainerEntriesScope::hasPatcherFunctions(const QString &sedsTypeName) const
{
    assertPresent(sedsTypeName);
    return m_scope.at(sedsTypeName).hasPatcherFunctions;
}

bool ContainerEntriesScope::hasPatcherFunctions(const ContainerDataType &sedsType) const
{
    if (sedsType.baseType()) {
        const auto &sedsBaseTypeName = sedsType.baseType()->nameStr();
        const auto basePatcherFunctions = hasPatcherFunctions(sedsBaseTypeName);

        if (basePatcherFunctions) {
            return true;
        }
    }

    for (const auto &sedsEntry : sedsType.entries()) {
        if (std::holds_alternative<seds::model::ErrorControlEntry>(sedsEntry)
                || std::holds_alternative<seds::model::LengthEntry>(sedsEntry)) {
            return true;
        }
    }

    if (sedsType.isAbstract()) {
        for (const auto &sedsEntry : sedsType.trailerEntries()) {
            if (std::holds_alternative<seds::model::ErrorControlEntry>(sedsEntry)
                    || std::holds_alternative<seds::model::LengthEntry>(sedsEntry)) {
                return true;
            }
        }
    }

    return false;
}

void ContainerEntriesScope::assertPresent(const QString &sedsTypeName) const
{
    if (m_scope.count(sedsTypeName) == 0) {
        auto errorMessage = QString("Unable to find type \"%1\" in the current scope.").arg(sedsTypeName);
        throw TranslationException(std::move(errorMessage));
    }
}

} // namespace conversion::asn1::translator
