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

#include "promelamodel.h"

namespace promela::model {
conversion::ModelType PromelaModel::modelType() const
{
    return conversion::ModelProperties<PromelaModel>::type;
}

void PromelaModel::addInclude(const QString &file)
{
    m_includes.append(file);
}

const QList<QString> &PromelaModel::getIncludes() const noexcept
{
    return m_includes;
}

void PromelaModel::addMtype(const QString &value)
{
    m_mtypeValues.insert(value);
}

const QSet<QString> &PromelaModel::getMtypeValues() const noexcept
{
    return m_mtypeValues;
}

void PromelaModel::addNamedMtype(const NamedMtype &mtype)
{
    const QString &name = mtype.getName();
    m_namedMtypeValues.emplace(name, mtype);
}

const std::map<QString, NamedMtype> &PromelaModel::getNamedMtypeValues() const noexcept
{
    return m_namedMtypeValues;
}

void PromelaModel::addUtype(const Utype &utype)
{
    m_userTypes.append(utype);
}

const QList<Utype> &PromelaModel::getUtypes() const noexcept
{
    return m_userTypes;
}

void PromelaModel::addTypeAlias(const TypeAlias &alias)
{
    m_typeAliases.append(alias);
}

const QList<TypeAlias> &PromelaModel::getTypeAliases() const noexcept
{
    return m_typeAliases;
}

void PromelaModel::addValueDefinition(const ValueDefinition &value)
{
    m_valueDefinitions.append(value);
}

const QList<ValueDefinition> &PromelaModel::getValueDefinitions() const noexcept
{
    return m_valueDefinitions;
}

void PromelaModel::addDeclaration(const Declaration &declaration)
{
    m_declarations.append(declaration);
}

const QList<Declaration> &PromelaModel::getDeclarations() const noexcept
{
    return m_declarations;
}
}
