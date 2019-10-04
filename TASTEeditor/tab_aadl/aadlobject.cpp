/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlobject.h"

#include <QPointer>
#include <QVector>

namespace taste3 {
namespace aadl {

struct AADLObjectPrivate {
    QString m_title;
    common::Id m_id;
    QVector<qint32> m_coords;
};

AADLObject::AADLObject(const QString &title, QObject *parent)
    : QObject(parent)
    , d(new AADLObjectPrivate { title, common::createId(), QVector<qint32> {} })
{
}

AADLObject::~AADLObject() {}

QString AADLObject::title() const
{
    return d->m_title;
}

common::Id AADLObject::id() const
{
    return d->m_id;
}

bool AADLObject::setTitle(const QString &title)
{
    if (title != d->m_title) {
        d->m_title = title;
        emit titleChanged(this->title());
        return true;
    }
    return false;
}

bool AADLObject::setId(const common::Id &id)
{
    if (id != d->m_id) {
        d->m_id = id;
        emit idChanged(this->id());
        return true;
    }
    return false;
}

bool AADLObject::setParentObject(AADLObject *parentObject)
{
    if (parent() == parentObject)
        return false;

    setParent(parentObject);
    return true;
}

QVector<qint32> AADLObject::coordinates() const
{
    return d->m_coords;
}

void AADLObject::setCoordinates(const QVector<qint32> &coordinates)
{
    if (d->m_coords != coordinates) {
        d->m_coords = coordinates;
        emit coordinatesChanged(d->m_coords);
    }
}

AADLObject *AADLObject::parentObject() const
{
    return qobject_cast<AADLObject *>(parent());
}

} // ns aadl
} // ns taste3
