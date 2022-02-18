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

#include <QString>
#include <list>
#include <memory>

namespace promela::model {
class Expression;
/**
 * @brief Representation of variable reference in promela
 */
class VariableRef
{
public:
    struct Element {
        Element(QString name, std::unique_ptr<Expression> index);
        QString m_name;
        std::unique_ptr<Expression> m_index;
    };

    /**
     * @brief Constructor
     *
     * @param ref name of the variable
     */
    VariableRef(QString ref);

    VariableRef(QString ref, std::unique_ptr<Expression> indexExpression);

    VariableRef(const VariableRef &other);

    VariableRef(VariableRef &&other);

    const VariableRef &operator=(const VariableRef &rhs);

    const VariableRef &operator=(VariableRef &&rhs);

    void appendElement(QString ref);

    void appendElement(QString ref, std::unique_ptr<Expression> indexExpression);

    /**
     * @brief Getter for reference to variable
     *
     * @retun Name of the variable
     */

    const std::list<Element> &getElements() const noexcept;

private:
    // QString m_ref;
    std::list<Element> m_elements;
};
}
