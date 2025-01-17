/****************************************************************************
**
** Copyright (C) 2017-2021 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a programme and funded by
** European Space Agency.
**
** This Library is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#pragma once

#include "constraint.h"
#include "constraintvisitor.h"

namespace Asn1Acn {
namespace Constraints {

template<typename ValueType>
class AndConstraint : public Constraint<ValueType>
{
public:
    AndConstraint(std::unique_ptr<Constraint<ValueType>> leftChild, std::unique_ptr<Constraint<ValueType>> rightChild)
        : m_leftChild(std::move(leftChild))
        , m_rightChild(std::move(rightChild))
    {
    }
    ~AndConstraint() override = default;

    const Constraint<ValueType> *leftChild() const { return m_leftChild.get(); }
    const Constraint<ValueType> *rightChild() const { return m_rightChild.get(); }

    void accept(ConstraintVisitor<ValueType> &visitor) const override { visitor.visit(*this); }

    std::unique_ptr<Constraint<ValueType>> clone() const override
    {
        return std::make_unique<AndConstraint>(m_leftChild->clone(), m_rightChild->clone());
    }

private:
    std::unique_ptr<Constraint<ValueType>> m_leftChild;
    std::unique_ptr<Constraint<ValueType>> m_rightChild;
};

template<typename ValueType>
class OrConstraint : public Constraint<ValueType>
{
public:
    OrConstraint(std::unique_ptr<Constraint<ValueType>> leftChild, std::unique_ptr<Constraint<ValueType>> rightChild)
        : m_leftChild(std::move(leftChild))
        , m_rightChild(std::move(rightChild))
    {
    }
    ~OrConstraint() override = default;

    const Constraint<ValueType> *leftChild() const { return m_leftChild.get(); }
    const Constraint<ValueType> *rightChild() const { return m_rightChild.get(); }

    void accept(ConstraintVisitor<ValueType> &visitor) const override { visitor.visit(*this); }

    std::unique_ptr<Constraint<ValueType>> clone() const override
    {
        return std::make_unique<OrConstraint>(m_leftChild->clone(), m_rightChild->clone());
    }

private:
    std::unique_ptr<Constraint<ValueType>> m_leftChild;
    std::unique_ptr<Constraint<ValueType>> m_rightChild;
};

}
}
