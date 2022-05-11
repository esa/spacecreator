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

#include "context.h"
#include "generictypemapper.h"

#include <seds/SedsModel/generics/generictype.h>
#include <seds/SedsModel/generics/generictypemapset.h>

namespace conversion::asn1::translator {

class GenericTypeCreator final
{
public:
    GenericTypeCreator(Context &context, QString parentName, const GenericTypeMapper &typeMapper);

public:
    auto createTypeForGeneric(const TypeMapping *mapping, const QString &genericName) -> QString;

private:
    auto createConcreteTypeAlias(
        const QString &genericName, const TypeMapping::Concrete &concreteMapping) -> QString;

    auto createConcreteChoice(
        const QString &genericName, const TypeMapping *mapping) -> QString;
    auto createConcreteChoiceAlternative(
        const TypeMapping::Concrete &concreteMapping, Asn1Acn::Types::Choice *choice, const std::size_t count) -> void;

    auto handleFixedValue(Asn1Acn::Types::Type *type, const QString &fixedValue) -> void;

private:
    Context &m_context;
    QString m_parentName;
    const GenericTypeMapper &m_typeMapper;
};

} // namespace conversion::asn1::translator
