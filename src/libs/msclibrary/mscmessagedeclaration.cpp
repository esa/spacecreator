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

#include "mscmessagedeclaration.h"

namespace msc {

/*!
  \class msc::MscMessageDeclaration
  \brief Defines one message declaration. Those are in the "document head" part of an MSC file.
*/

MscMessageDeclaration::MscMessageDeclaration(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<msc::MscMessageDeclaration *>();
    qRegisterMetaType<QList<msc::MscMessageDeclaration *>>();
}

const QStringList &MscMessageDeclaration::names() const
{
    return m_names;
}

/**
   Returns all the names as one string, connected by a ", "
 */
QString MscMessageDeclaration::joinedNames() const
{
    return m_names.join(", ");
}

const QStringList &MscMessageDeclaration::typeRefList() const
{
    return m_typeRefList;
}

/**
   Returns all the type references as one string, connected by a ", "
 */
QString MscMessageDeclaration::joinedTypeRefList() const
{
    return m_typeRefList.join(", ");
}

void MscMessageDeclaration::setNames(const QStringList &names)
{
    Q_ASSERT(!names.isEmpty());
    if (m_names == names || names.isEmpty())
        return;

    m_names = names;
    Q_EMIT namesChanged();
    Q_EMIT dataChanged();
}

void MscMessageDeclaration::setTypeRefList(const QStringList &typeRefList)
{
    if (m_typeRefList == typeRefList)
        return;

    m_typeRefList = typeRefList;
    Q_EMIT typeRefListChanged();
    Q_EMIT dataChanged();
}

} // namespace msc
