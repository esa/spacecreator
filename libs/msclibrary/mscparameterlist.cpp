/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "mscparameterlist.h"

namespace msc {

MscParameter::MscParameter(const QString &expression, const QString &pattern)
    : m_parameter(pattern.isEmpty() ? expression : pattern)
    , m_type(pattern.isEmpty() ? Type::Expression : Type::Pattern)
{
    Q_ASSERT(pattern.isEmpty() != expression.isEmpty()); // one and only one has content
}

MscParameter::Type MscParameter::type() const
{
    return m_type;
}

const QString &MscParameter::parameter() const
{
    return m_parameter;
}

QString MscParameter::expression() const
{
    return m_type == Type::Expression ? m_parameter : QString();
}

void MscParameter::setExpression(const QString &expression)
{
    m_parameter = expression;
    m_type = Type::Expression;
}

QString MscParameter::pattern() const
{
    return m_type == Type::Pattern ? m_parameter : QString();
}

void MscParameter::setPattern(const QString &pattern)
{
    m_parameter = pattern;
    m_type = Type::Pattern;
}

} // namespace msc
