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

#include "mscentity.h"

#include <QDebug>
#include <QMetaEnum>

namespace msc {

/*!
   \class msc::MscEntity
   \brief base class for all MSC entities
 */

const QString MscEntity::DefaultName = QObject::tr("Untitled");

MscEntity::MscEntity(QObject *parent)
    : QObject(parent)
    , m_id(QUuid::createUuid())
{
}

MscEntity::MscEntity(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_id(QUuid::createUuid())
{
}

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
    Q_EMIT dataChanged();
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
    Q_EMIT dataChanged();
}

QVector<cif::CifBlockShared> MscEntity::cifs() const
{
    return m_cifs;
}

void MscEntity::setCifs(const QVector<cif::CifBlockShared> &cifs)
{
    if (m_cifs != cifs) {
        m_cifs = cifs;
        dbgShowCifs();
    }
}

void MscEntity::addCif(const cif::CifBlockShared &cif)
{
    if (!m_cifs.contains(cif)) {
        m_cifs.append(cif);
        dbgShowCifs();
    }
}

void MscEntity::dbgShowCifs() const
{
#ifdef QT_DEBUG
    static const QString info("TODO: %1(%2)->setCif(%3)");
    const QMetaEnum &e = QMetaEnum::fromType<MscEntity::EntityType>();
    const QString currTypeName(e.valueToKey(int(entityType())));
    qDebug() << QString("%1[%2]->CIFs %3:").arg(name(), currTypeName).arg(m_cifs.size());
    for (int i = 0; i < m_cifs.size(); ++i) {
        qDebug() << QString("\t CIF # %1/%2").arg(i).arg(m_cifs.size());
        const cif::CifBlockShared &cifBlock = m_cifs.at(i);
        for (int j = 0; j < cifBlock->lines().size(); ++j)
            qDebug() << QString("\t\t CIF Line #%1/%2: %3")
                                .arg(j)
                                .arg(cifBlock->lines().size())
                                .arg(cifBlock->lines().at(j)->sourceLine());
    }
#endif
}

} // namespace msc
