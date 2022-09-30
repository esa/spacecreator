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

#include "rename.h"

#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

Rename::Rename(QString name, Direction direction, QString referencedName, QString referencedFunctionName)
    : Signal(std::move(name))
    , m_direction(direction)
    , m_referencedName(std::move(referencedName))
    , m_referencedFunctionName(std::move(referencedFunctionName))
{
}

Rename::Direction Rename::direction() const
{
    return m_direction;
}

void Rename::setDirection(Direction direction)
{
    m_direction = direction;
}

const QString &Rename::referencedName() const
{
    return m_referencedName;
}

void Rename::setReferencedName(QString name)
{
    m_referencedName = std::move(name);
}

const QString &Rename::referencedFunctionName() const
{
    return m_referencedFunctionName;
}

void Rename::setReferencedFunctionName(QString name)
{
    m_referencedFunctionName = std::move(name);
}

const QStringList &Rename::parametersTypes() const
{
    return m_parametersTypes;
}

void Rename::setParametersTypes(QStringList parametersTypes)
{
    m_parametersTypes = std::move(parametersTypes);
}

void Rename::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
