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

#include "exportablebusaccess.h"

namespace dve {

const QString &ExportableBusAccess::name() const
{
    return m_name;
}

void ExportableBusAccess::setName(const QString &name)
{
    m_name = name;
}

const QString &ExportableBusAccess::busName() const
{
    return m_busName;
}

void ExportableBusAccess::setBusName(const QString &name)
{
    m_busName = name;
}

} // namespace dve
