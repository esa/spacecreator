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

namespace tmc::promelamodel {

conversion::ModelType PromelaModel::modelType() const
{
    return conversion::ModelType::Promela;
}

void PromelaModel::addMtype(const QString &value)
{
    m_mtype_values.insert(value);
}

const QSet<QString> &PromelaModel::getMtypeValues() const
{
    return m_mtype_values;
}

void PromelaModel::addNamedMtype(const NamedMtype &mtype)
{
    const QString &name = mtype.getName();
    m_named_m_type_values.emplace(name, mtype);
}

const std::map<QString, NamedMtype> &PromelaModel::getNamedMtypeValues() const
{
    return m_named_m_type_values;
}

void PromelaModel::addUtype(const Utype &utype)
{
    m_user_types.append(utype);
}

const QList<Utype> &PromelaModel::getUtypes() const
{
    return m_user_types;
}
}
