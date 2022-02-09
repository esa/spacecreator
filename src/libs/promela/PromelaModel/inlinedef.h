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

#include <QList>
#include <QString>

namespace promela::model {
/**
 * @brief Representation of inline definition in promela
 */
class InlineDef final
{
public:
    /**
     * @brief Constructor
     *
     * @param name name of inline definition
     * @param arguments list of arguments
     * @param sequence content of inline definition
     */
    InlineDef(QString name, const QList<QString> &arguments, Sequence sequence);

    /**
     * @brief Getter for name of inline definition
     *
     * @return name of inline definition
     */
    const QString &getName() const noexcept;
    /**
     * @brief Setter for name of inline definition
     *
     * @param name name to set
     */
    void setName(QString name);

    /**
     * @brief Getter for content of inline definition
     *
     * @return content of inline definition
     */
    const Sequence &getSequence() const noexcept;
    /**
     * @brief Setter for content of inline definition
     *
     * @param sequence content of inline definition
     */
    void setSequence(Sequence sequence);

    /**
     * @brief Getter for arguments of inline definition
     *
     * @return arguments of inline definition
     */
    const QList<QString> &getArguments() const noexcept;
    /**
     * @brief Setter for arguments of inline definition
     *
     * @param arguments arguments of inline definition
     */
    void setArguments(const QList<QString> &arguments);

private:
    QString m_name;
    QList<QString> m_arguments;
    Sequence m_sequence;
};
}
