/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvsysteminterface.h"

namespace dvm {

DVSystemInterface::DVSystemInterface(DVObject *parent)
    : DVObject(DVObject::Type::SystemInterface, {}, parent)
{
}

void DVSystemInterface::setInterfaceType(InterfaceType type)
{
    m_interfaceType = type;
}

DVSystemInterface::InterfaceType DVSystemInterface::interfaceType() const
{
    return m_interfaceType;
}

bool DVSystemInterface::isProvided() const
{
    return m_interfaceType == InterfaceType::Provided;
}

bool DVSystemInterface::isRequired() const
{
    return m_interfaceType == InterfaceType::Required;
}

QVector<shared::InterfaceParameter> DVSystemInterface::params() const
{
    return m_parameters;
}

shared::InterfaceParameter DVSystemInterface::param(const QString &name) const
{
    if (!name.isEmpty()) {
        for (const shared::InterfaceParameter &param : m_parameters) {
            if (param.name() == name) {
                return param;
            }
        }
    }
    return {};
}

void DVSystemInterface::setParams(const QVector<shared::InterfaceParameter> &params)
{
    if (m_parameters != params) {
        m_parameters = params;
        Q_EMIT paramsChanged();
    }
}

void DVSystemInterface::addParam(const shared::InterfaceParameter &param)
{
    if (!m_parameters.contains(param)) {
        m_parameters.append(param);
        Q_EMIT paramsChanged();
    }
}

} // namespace dvm
