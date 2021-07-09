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

#include "mscproject.h"

#include "mainmodel.h"

#include <QDebug>
#include <QFileInfo>

namespace msc {

MscProject::MscProject(QObject *parent)
    : shared::AbstractProject(parent)
{
}

void MscProject::setModel(MainModel *model)
{
    m_model = model;
}

QStringList MscProject::allAsn1Files() const
{
    QStringList result;
    QFileInfo fi = m_model->asn1File();
    if (m_model && fi.exists()) {
        result.append(fi.absoluteFilePath());
    }

    return result;
}

QStringList MscProject::allDVFiles() const
{
    return {};
}

QStringList MscProject::allIVFiles() const
{
    return {};
}

QStringList MscProject::allMscFiles() const
{
    QStringList result;
    if (m_model && !m_model->currentFilePath().isEmpty()) {
        result.append(m_model->currentFilePath());
    }
    return result;
}

} // namespace msc
