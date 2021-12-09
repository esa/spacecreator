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

#pragma once

#include "proctypeelement.h"
#include "sequence.h"

#include <QString>
#include <cstddef>
#include <list>
#include <memory>
#include <optional>

namespace promela::model {
class Proctype final
{
public:
    Proctype(QString name, Sequence sequence);

    const QString &getName() const noexcept;
    void setName(QString name);

    bool isActive() const noexcept;
    size_t getInstancesCount() const;
    void setActive(size_t instances);

    bool hasPriority() const noexcept;
    size_t getPriority() const;
    void setPriority(size_t priority);

    const Sequence &getSequence() const noexcept;
    void setSequence(Sequence sequence);

private:
    QString m_name;
    bool m_active;
    std::optional<size_t> m_instances;
    std::optional<size_t> m_priority;
    Sequence m_sequence;
};
}
