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

#include "parameterarchetype.h"

namespace ivm {

ParameterArchetype::ParameterArchetype(const QString &title, QObject *parent, const QString &type,
        const ParameterArchetype::ParameterDirection &direction)
    : ArchetypeObject(title, ArchetypeObject::Type::ParameterArchetype, parent)
    , m_direction(direction)
{
    setEntityAttribute(meta::ArchetypeProps::token(meta::ArchetypeProps::Token::type), type);
}

ParameterArchetype::~ParameterArchetype() = default;

QString ParameterArchetype::getType() const
{
    return entityAttributeValue(meta::ArchetypeProps::token(meta::ArchetypeProps::Token::type)).toString();
}

void ParameterArchetype::setType(const QString &type)
{
    setEntityAttribute(meta::ArchetypeProps::token(meta::ArchetypeProps::Token::type), type);
}

ParameterArchetype::ParameterDirection ParameterArchetype::getDirection() const
{
    return m_direction;
}

void ParameterArchetype::setDirection(const ParameterDirection &direction)
{
    m_direction = direction;
}

QString ParameterArchetype::directionToString(const ParameterArchetype::ParameterDirection &direction)
{
    const QMetaEnum &qMetaEnum = QMetaEnum::fromType<ivm::ParameterArchetype::ParameterDirection>();
    return QString::fromLatin1(qMetaEnum.valueToKey(static_cast<int>(direction)));
}

ParameterArchetype::ParameterDirection ParameterArchetype::directionFromString(
        const QString &directionString, const ParameterArchetype::ParameterDirection &defaultKind)
{
    const QMetaEnum &qMetaEnum = QMetaEnum::fromType<ivm::ParameterArchetype::ParameterDirection>();
    bool ok = false;
    const auto kind = static_cast<ParameterArchetype::ParameterDirection>(
            qMetaEnum.keyToValue(directionString.toLatin1().data(), &ok));
    return ok ? kind : defaultKind;
}
}
