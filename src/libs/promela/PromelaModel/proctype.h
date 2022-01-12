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
/**
 * @brief Representation of proctype in promela
 */
class Proctype final
{
public:
    /**
     * @brief Constructor
     *
     * @param name name of proctype
     * @param sequence content of proctype
     */
    Proctype(QString name, Sequence sequence);

    /**
     * @brief Getter for name of proctype
     *
     * @return name of proctype
     */
    const QString &getName() const noexcept;
    /**
     * @brief Setter for name of proctype
     *
     * @param name name of proctype
     */
    void setName(QString name);

    /**
     * @brief Check if proctye is active proctype
     *
     * @return true if proctype is active proctype, otherwise false
     */
    bool isActive() const noexcept;
    /**
     * @brief Set proctype as active proctype.
     *
     * @param active true if proctype is active proctype, otherwise false
     */
    void setActive(bool active);
    /**
     * @brief Getter for instance count of proctype
     *
     * Active proctype has number of instances, default 1
     *
     * @return number of instances of active proctype
     */
    size_t getInstancesCount() const noexcept;
    /**
     * @brief Set number of active instances.
     *
     * @param instances number of active instances
     */
    void setInstancesCount(size_t instances);

    /**
     * @brief Check if proctype has assigned priority
     *
     * @return true if proctype has assigned priority, otherwise false
     */
    bool hasPriority() const noexcept;
    /**
     * @brief Getter for priority of proctype
     *
     * @return Priority value
     * @throws std::bad_optional_access if proctype has no prority
     */
    size_t getPriority() const;
    /**
     * @brief Setter for priority of proctype
     *
     * @param priority priority of proctype
     */
    void setPriority(size_t priority);

    /**
     * @brief Getter for content of proctype
     *
     * @return content of proctype
     */
    const Sequence &getSequence() const noexcept;
    /**
     * @brief Setter for content of proctype
     *
     * @param sequence content of proctype
     */
    void setSequence(Sequence sequence);

private:
    QString m_name;
    bool m_active;
    size_t m_instances;
    std::optional<size_t> m_priority;
    Sequence m_sequence;
};
}
