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

#include "mscentity.h"

namespace msc {

/*!
   \class msc::MscEntity
   \brief base class for all MSC entities
 */

const QString MscEntity::DefaultName = QObject::tr("Untitled");

MscEntity::MscEntity(QObject *parent) : QObject(parent), m_id(QUuid::createUuid()) {}

MscEntity::MscEntity(const QString &name, QObject *parent) : QObject(parent), m_name(name), m_id(QUuid::createUuid()) {}

const QString &MscEntity::name() const
{
    return m_name;
}

void MscEntity::setName(const QString &name)
{
    if (name == m_name) {
        return;
    }

    m_name = name;
    Q_EMIT nameChanged(m_name);
}

QUuid MscEntity::internalId() const
{
    return m_id;
}

const QString &MscEntity::comment() const
{
    return m_comment;
}

void MscEntity::setComment(const QString &comment)
{
    if (comment == m_comment) {
        return;
    }

    m_comment = comment;
    Q_EMIT commentChanged(m_comment);
}

} // namespace msc
