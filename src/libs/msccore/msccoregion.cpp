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

#include "msccoregion.h"

#include "mscinstance.h"

namespace msc {

MscCoregion::MscCoregion(QObject *parent)
    : MscCoregion(Type::Begin, parent)
{
}

MscCoregion::MscCoregion(Type type, QObject *parent)
    : MscInstanceEvent(parent)
    , m_type(type)
{
}

MscCoregion::MscCoregion(MscInstance *instance, MscCoregion::Type type, QObject *parent)
    : MscCoregion(type, parent)
{
    setInstance(instance);
}

void MscCoregion::setType(MscCoregion::Type type)
{
    if (type == m_type)
        return;

    m_type = type;
    Q_EMIT typeChanged();
    Q_EMIT dataChanged();
}

MscInstance *MscCoregion::instance() const
{
    return m_instance;
}

void MscCoregion::setInstance(MscInstance *instance)
{
    if (instance == m_instance)
        return;

    MscInstance *oldInstance = m_instance;
    m_instance = instance;
    Q_EMIT instanceChanged();
    Q_EMIT instanceRelationChanged(m_instance, oldInstance);
    Q_EMIT dataChanged();
}

MscEntity *MscCoregion::instanceObj() const
{
    return m_instance;
}

void MscCoregion::setInstanceObj(MscEntity *instance)
{
    if (auto inst = qobject_cast<MscInstance *>(instance)) {
        setInstance(inst);
    }
}

bool MscCoregion::relatesTo(const MscInstance *instance) const
{
    return m_instance == instance;
}

} // namespace msc
