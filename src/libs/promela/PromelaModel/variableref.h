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
    /**
     * @brief Variable reference element.
     *
     * Element contains a name and an optional index expression.
     */
    struct Element {
        /**
         * @brief Constructor.
         *
         * @param name Name of variable reference element.
         * @param index Index expression.
         */
        Element(QString name, std::unique_ptr<Expression> index);

        /**
         * @brief Move constructor.
         *
         * @param other The other object.
         */
        Element(Element &&other);

        /**
         * @brief Name of variable reference element.
         */
        QString m_name;
        /**
         * @brief Optional index expression of variable reference element.
         */
        std::unique_ptr<Expression> m_index;
    };

    /**
     * @brief Default constructor.
     * Constructs empty object.
     */
    VariableRef();

    /**
     * @brief Constructor.
     *
     * Construct Variable reference with single element.
     *
     * @param ref name of the variable.
     */
    VariableRef(QString ref);
    /**
     * @brief Constructor.
     *
     * Construct Variable reference with single element with index expression.
     *
     * @param ref name of the variable (array).
     * @param indexExpression index expression
     */
    VariableRef(QString ref, std::unique_ptr<Expression> indexExpression);
    /**
     * @brief Copy constructor.
     *
     * @param other Other instance of VariableRef.
     */
    VariableRef(const VariableRef &other);
    /**
     * @brief Move constructor.
     *
     * @param other Other instance of VariableRef.
     */
    VariableRef(VariableRef &&other) noexcept;

    /**
     * @brief Copy assignment operator.
     *
     * @param rhs right hand side instance of VariableRef.
     * @return reference to self.
     */
    const VariableRef &operator=(const VariableRef &rhs);
    /**
     * @brief Move assignment operator.
     *
     * @param rhs right hand side instance of VariableRef.
     * @return reference to self.
     */
    const VariableRef &operator=(VariableRef &&rhs) noexcept;

    /**
     * @brief Append single name element to the variable reference.
     *
     * @param ref Name of element to append.
     */
    void appendElement(QString ref);

    /**
     * @brief Append single name element to the variable reference.
     *
     * @param ref Name of element to append.
     * @param indexExpression index expression of element to append.
     */
    void appendElement(QString ref, std::unique_ptr<Expression> indexExpression);

    /**
     * @brief Append single element to the variable reference.
     *
     * @param element element to append.
     */
    void appendElement(VariableRef::Element element);

    /**
     * @brief Prepend single name element to the variable reference.
     *
     * @param ref Name of element to prepend.
     */
    void prependElement(QString ref);

    /**
     * @brief Prepend single name element to the variable reference.
     *
     * @param ref Name of element to prepend.
     * @param indexExpression index expression of element to prepend.
     */
    void prependElement(QString ref, std::unique_ptr<Expression> indexExpression);

    /**
     * @brief Modify the name of first element.
     *
     * @param ref Name of element to set.
     */
    void changeFirstRef(QString ref);

    /**
     * @brief Modify the index expression of first element.
     *
     * @param indexExpression expression to set.
     */
    void addIndexExpression(std::unique_ptr<Expression> indexExpression);

    /**
     * @brief Getter for elements to variable reference.
     *
     * @return List of variable reference elements.
     */
    const std::list<Element> &getElements() const noexcept;

    /**
     * @brief Take elements of variable reference.
     * After this, the variable reference will be empty, caller is new owner of elements.
     *
     * @returns a list of elements.
     */
    std::list<Element> takeElements();

    /**
     * @brief Check if variable reference is empty.
     * The empty variable reference does not have any elements.
     *
     * @return True if object is empty, otherwise false.
     */
    bool isEmpty() const noexcept;

private:
    // QString m_ref;
    std::list<Element> m_elements;
};
}
