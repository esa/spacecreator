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

#include "proctype.h"

namespace promela::model {
Proctype::Proctype(QString name, Sequence sequence)
    : m_name(std::move(name))
    , m_active(false)
    , m_instances(1)
    , m_sequence(std::move(sequence))
{
}

const QString &Proctype::getName() const noexcept
{
    return m_name;
}

void Proctype::setName(QString name)
{
    m_name = std::move(name);
}

bool Proctype::isActive() const noexcept
{
    return m_active;
}

void Proctype::setActive(bool active)
{
    m_active = active;
}

size_t Proctype::getInstancesCount() const noexcept
{
    return m_instances;
}

void Proctype::setInstancesCount(size_t instances)
{
    m_instances = instances;
}

bool Proctype::hasPriority() const noexcept
{
    return m_priority.has_value();
}

void Proctype::setPriority(size_t priority)
{
    m_priority = priority;
}

size_t Proctype::getPriority() const
{
    return m_priority.value();
}

const Sequence &Proctype::getSequence() const noexcept
{
    return m_sequence;
}

void Proctype::setSequence(Sequence sequence)
{
    m_sequence = std::move(sequence);
}
}
