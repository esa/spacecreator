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

#include "ivproject.h"

#include "interfacedocument.h"

#include <QDebug>
#include <QFileInfo>

namespace ive {

IvProject::IvProject(QObject *parent)
    : shared::AbstractProject(parent)
{
}

void IvProject::setModel(InterfaceDocument *model)
{
    m_model = model;
}

QStringList IvProject::allAsn1Files() const
{
    QStringList result;
    QFileInfo fi(m_model->asn1FilePath());
    if (m_model && fi.exists()) {
        result.append(fi.absoluteFilePath());
    }

    return result;
}

QStringList IvProject::allDVFiles() const
{
    return {};
}

QStringList IvProject::allIVFiles() const
{
    QStringList result;
    if (m_model && !m_model->path().isEmpty()) {
        result.append(m_model->path());
    }
    return result;
}

QStringList IvProject::allMscFiles() const
{
    QStringList result;
    QFileInfo fi(m_model->mscFilePath());
    if (m_model && fi.exists()) {
        result.append(fi.absoluteFilePath());
    }

    return result;
}

} // namespace msc
