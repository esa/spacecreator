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

#pragma once

#include "context.h"
#include "specialized/entrytranslatorvisitor.h"

#include <asn1library/asn1/types/sequence.h>
#include <seds/SedsModel/types/containerdatatype.h>
#include <vector>

namespace conversion::asn1::translator {

class ContainerDataTypeTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context     Current translation context
     */
    explicit ContainerDataTypeTranslator(Context &context);
    /**
     * @brief   Deleted copy constructor
     */
    ContainerDataTypeTranslator(const ContainerDataTypeTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    ContainerDataTypeTranslator(ContainerDataTypeTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    ContainerDataTypeTranslator &operator=(const ContainerDataTypeTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    ContainerDataTypeTranslator &operator=(ContainerDataTypeTranslator &&) = delete;

public:
    auto translate(const seds::model::ContainerDataType &sedsType) -> void;

private:
    auto handleEntries(
            const std::vector<seds::model::EntryType> &entries, EntryTranslatorVisitor &entryTranslator) const -> void;
    auto handleTrailerEntries(
            const std::vector<seds::model::EntryType> &entries, EntryTranslatorVisitor &entryTranslator) const -> void;
    auto handleBaseTypeEntries(
            const seds::model::ContainerDataType &sedsType, EntryTranslatorVisitor &entryTranslator) const -> void;
    auto handleBaseTypeTrailerEntries(
            const seds::model::ContainerDataType &sedsType, EntryTranslatorVisitor &entryTranslator) const -> void;

    auto applyContainerConstraints(
            const seds::model::ContainerDataType &sedsType, Asn1Acn::Types::Sequence *asn1Type) const -> void;
    auto updateParentContainer(const seds::model::DataTypeRef &baseTypeRef, Asn1Acn::Types::Sequence *asn1Type) -> void;

    auto createRealizationContainerField(Asn1Acn::Types::Sequence *asn1Sequence) -> void;

private:
    Context &m_context;

    inline static const QString m_realizationComponentsName = "realization";
    inline static const QString m_realizationComponentsAlternativeNameTemplate = "realization%1";
};

} // namespace conversion::asn1::translator
