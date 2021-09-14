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

#include "dvpseudointerface.h"

namespace dvm {

DVPseudoInterface::DVPseudoInterface(DVObject *parent)
    : DVObject(DVObject::Type::PseudoInterface, {}, parent)
{
}

void DVPseudoInterface::setInterfaceType(InterfaceType type)
{
    m_interfaceType = type;
}

DVPseudoInterface::InterfaceType DVPseudoInterface::interfaceType() const
{
    return m_interfaceType;
}

QVector<shared::InterfaceParameter> DVPseudoInterface::params() const
{
    return m_parameters;
}

shared::InterfaceParameter DVPseudoInterface::param(const QString &name) const
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

void DVPseudoInterface::setParams(const QVector<shared::InterfaceParameter> &params)
{
    if (m_parameters != params) {
        m_parameters = params;
        Q_EMIT paramsChanged();
    }
}

void DVPseudoInterface::addParam(const shared::InterfaceParameter &param)
{
    if (!m_parameters.contains(param)) {
        m_parameters.append(param);
        Q_EMIT paramsChanged();
    }
}

} // namespace dvm
