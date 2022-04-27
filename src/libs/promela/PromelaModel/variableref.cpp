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

#include "variableref.h"

#include "expression.h"

namespace promela::model {
VariableRef::Element::Element(QString name, std::unique_ptr<Expression> index)
    : m_name(std::move(name))
    , m_index(std::move(index))
{
}

VariableRef::VariableRef(QString ref)
{
    m_elements.emplace_back(std::move(ref), std::unique_ptr<Expression>());
}

VariableRef::VariableRef(QString ref, std::unique_ptr<Expression> indexExpression)
{
    m_elements.emplace_back(std::move(ref), std::move(indexExpression));
}

VariableRef::VariableRef(const VariableRef &other)
{
    for (const auto &element : other.m_elements) {
        if (element.m_index) {
            m_elements.emplace_back(element.m_name, std::make_unique<Expression>(*element.m_index));
        } else {
            m_elements.emplace_back(element.m_name, std::unique_ptr<Expression>());
        }
    }
}

VariableRef::VariableRef(VariableRef &&other) noexcept
{
    for (auto &element : other.m_elements) {
        m_elements.emplace_back(std::move(element.m_name), std::move(element.m_index));
    }
}

const VariableRef &VariableRef::operator=(const VariableRef &rhs)
{
    m_elements.clear();

    for (const auto &element : rhs.m_elements) {
        if (element.m_index) {
            m_elements.emplace_back(element.m_name, std::make_unique<Expression>(*element.m_index));
        } else {
            m_elements.emplace_back(element.m_name, std::unique_ptr<Expression>());
        }
    }

    return *this;
}

const VariableRef &VariableRef::operator=(VariableRef &&rhs) noexcept
{
    m_elements.clear();

    for (auto &element : rhs.m_elements) {
        m_elements.emplace_back(std::move(element.m_name), std::move(element.m_index));
    }

    return *this;
}

void VariableRef::appendElement(QString ref)
{
    m_elements.emplace_back(std::move(ref), std::unique_ptr<Expression>());
}

void VariableRef::appendElement(QString ref, std::unique_ptr<Expression> indexExpression)
{
    m_elements.emplace_back(std::move(ref), std::move(indexExpression));
}

const std::list<VariableRef::Element> &VariableRef::getElements() const noexcept
{
    return m_elements;
}
}
