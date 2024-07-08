/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2024 N7 Space Sp. z o.o.
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

#include <QString>

namespace promela::model {
/**
 * @brief Representation of c_decl block in promela
 */
class CDecl final
{
public:
    /**
     * @brief Default Constructor
     * Constructs empty c_decl block
     */
    CDecl();
    /**
     * @brief Constructor
     * Constructs c_decl with content
     *
     * @param content content of c declaration
     */
    CDecl(QString content);

    /**
     * @brief Getter for content of c_decl block
     *
     * @returns the reference to content of c_decl
     */
    const QString &getContent() const noexcept;

    /**
     * @brief Check if the c_decl contains any content
     *
     * @returns true if the content is not empty, otherwise false
     */
    bool hasContent() const noexcept;

    /**
     * @brief Setter for content of c_decl block
     *
     * @param content content to set
     */
    void setContent(QString content);

private:
    QString m_content;
};
}
