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

#include "declaration.h"

#include <QList>
#include <QString>

namespace promela::model {
/**
 * @brief Representation of user defined type in promela - utype.
 *
 * User defined type is a structure with contains ordered list of fields.
 **/
class Utype final
{
public:
    /**
     * @brief Constructor.
     *
     * @param name the name of the user defined type.
     * @param unionType true if type is union
     */
    explicit Utype(QString name, bool unionType = false);

    /**
     * @brief Getter for name of user defined type
     *
     * @return name of the user defined type
     */
    const QString &getName() const noexcept;

    /**
     * @brief Getter for unionType
     *
     * @return true if utype is union type, otherwise false
     */
    bool isUnionType() const noexcept;

    /**
     * @brief Add a field to the user defined type
     *
     * @param field a field to add
     */
    void addField(const Declaration &field);

    /**
     * @brief Getter for all fields from user defined type
     *
     * @return the reference to the list with all fields from user defined type
     */
    const QList<Declaration> &getFields() const noexcept;

private:
    QString m_name;
    bool m_unionType;
    QList<Declaration> m_fields;
};
}
