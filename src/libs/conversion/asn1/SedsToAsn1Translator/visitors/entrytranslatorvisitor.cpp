/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "visitors/entrytranslatorvisitor.h"

#include "visitors/datatypetranslatorvisitor.h"

#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/package/package.h>

using conversion::translator::TranslationException;
using conversion::translator::UndeclaredDataTypeException;

namespace conversion::asn1::translator {

void EntryTranslatorVisitor::operator()(const seds::model::Entry &sedsEntry)
{
    auto asn1EntryType = translateEntryType(sedsEntry.type().nameStr());

    m_asn1SequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            sedsEntry.nameStr(), sedsEntry.nameStr(), false, "", Asn1Acn::SourceLocation(), std::move(asn1EntryType));
}

void EntryTranslatorVisitor::operator()(const seds::model::ErrorControlEntry &sedsEntry)
{
    Q_UNUSED(sedsEntry);
    throw TranslationException("ErrorControlEntry translation no implemented");
}

void EntryTranslatorVisitor::operator()(const seds::model::FixedValueEntry &sedsEntry)
{
    Q_UNUSED(sedsEntry);
    throw TranslationException("FixedValueEntry translation no implemented");
}

void EntryTranslatorVisitor::operator()(const seds::model::LengthEntry &sedsEntry)
{
    Q_UNUSED(sedsEntry);
    throw TranslationException("LengthEntry translation no implemented");
}

void EntryTranslatorVisitor::operator()(const seds::model::ListEntry &sedsEntry)
{
    Q_UNUSED(sedsEntry);
    throw TranslationException("ListEntry translation no implemented");
}

void EntryTranslatorVisitor::operator()(const seds::model::PaddingEntry &sedsEntry)
{
    Q_UNUSED(sedsEntry);
    throw TranslationException("PaddingEntry translation no implemented");
}

std::unique_ptr<Asn1Acn::Types::UserdefinedType> EntryTranslatorVisitor::translateEntryType(
        const QString &sedsTypeName) const
{
    const auto *asn1ReferencedTypeAssignment = m_asn1Definitions->type(sedsTypeName);
    const auto *asn1ReferencedType = asn1ReferencedTypeAssignment->type();

    auto asn1EntryType = std::make_unique<Asn1Acn::Types::UserdefinedType>(
            asn1ReferencedType->identifier(), m_asn1Definitions->name(), asn1ReferencedTypeAssignment);
    asn1EntryType->setType(asn1ReferencedType->clone());

    return asn1EntryType;
}

} // namespace conversion::asn1::translator
