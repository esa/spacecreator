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

#include <QSet>
#include <QString>

namespace tmc::promelamodel {

/**
 * @brief Representation of named mtype form promela.
 */
class NamedMtype final
{
public:
    /**
     * @brief Constructor.
     *
     * Construct NamedMtype with given name.
     *
     * @param name name of mtype
     */
    NamedMtype(QString name);

    /**
     * @brief Getter for name of mtype.
     */
    const QString &getName() const noexcept;

    /**
     * @brief Add value to named mtype.
     *
     * @param value the string value to add
     */
    void addValue(const QString &value);

    /**
     * @brief Remove value from named mtype.
     *
     * @param value the string value to remove from named mtype
     */
    void removeValue(const QString &value);
    /**
     * @brief Check if named mtype contains given value.
     *
     * @param value the string value to check
     * @return true if value is included in named mtype, otherwise false
     */
    bool hasValue(const QString &value) const;

    /**
     * @brief Getter for all values of named mtype
     *
     * @return the reference to set with all mtype values
     */
    const QSet<QString> &values() const noexcept;

    /**
     * @brief begin iterator
     *
     * @return begin iterator for all values of named mtype
     */
    QSet<QString>::const_iterator values_begin() const;
    /**
     * @brief end iterator
     *
     * @return end iterator for all values of named mtype
     */
    QSet<QString>::const_iterator values_end() const;

private:
    QString m_name;
    QSet<QString> m_values;
};

}
