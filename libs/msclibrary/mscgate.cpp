/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscgate.h"

namespace msc {

MscGate::MscGate(QObject *parent)
    : MscInstanceEvent(parent)
{
}

MscGate::MscGate(const QString &name, QObject *parent)
    : MscInstanceEvent(name, parent)
{
}

MscGate::Direction MscGate::direction() const
{
    return m_direction;
}

void MscGate::setDirection(MscGate::Direction dir)
{
    if (dir == m_direction)
        return;

    m_direction = dir;
}

MscInstance *MscGate::instance() const
{
    return m_instance;
}

QString MscGate::instanceName() const
{
    return m_instanceName;
}

void MscGate::setInstance(MscInstance *instance)
{
    if (instance == m_instance)
        return;

    m_instance = instance;
}

void MscGate::setInstanceName(const QString &instanceName)
{
    if (instanceName == m_instanceName)
        return;

    m_instanceName = instanceName;
}

QString MscGate::paramName() const
{
    return m_paramName;
}

void MscGate::setParamName(const QString &name)
{
    if (m_paramName == name)
        return;

    m_paramName = name;
}

QVariantList MscGate::params() const
{
    return m_params;
}

void MscGate::setParams(const QVariantList &params)
{
    if (params == m_params)
        return;

    m_params = params;
}

MscEntity::EntityType MscGate::entityType() const
{
    return MscEntity::EntityType::Gate;
}

} // ns msc
