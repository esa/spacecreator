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

#include "dvobject.h"

#include "dvmodel.h"

#include <QVector>

namespace dvm {

struct DVObjectPrivate {
    DVObjectPrivate(const DVObject::Type t)
        : m_type(t)
    {
    }

    const DVObject::Type m_type;
};

DVObject::DVObject(const DVObject::Type t, const QString &title, QObject *parent, const shared::Id &id)
    : shared::VEObject(id, parent)
    , d(new DVObjectPrivate(t))
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::name), title);
}

DVObject::~DVObject() { }

QString DVObject::title() const
{
    return entityAttributeValue<QString>(meta::Props::token(meta::Props::Token::name));
}

DVObject::Type DVObject::type() const
{
    return d->m_type;
}

bool DVObject::setTitle(const QString &title)
{
    if (!title.isEmpty() && title != this->title()) {
        setEntityAttribute(meta::Props::token(meta::Props::Token::name), title);
        return true;
    }
    return false;
}

bool DVObject::setParentObject(DVObject *parentObject)
{
    if (parent() == parentObject)
        return false;

    setParent(parentObject);
    return true;
}

QVector<qint32> DVObject::coordinates() const
{
    const meta::Props::Token token = meta::Props::Token::coordinates;
    const QVariant varCoord = entityAttributeValue(meta::Props::token(token));
    return coordinatesFromString(varCoord.toString());
}

void DVObject::setCoordinates(const QVector<qint32> &coordinates)
{
    if (this->coordinates() == coordinates)
        return;

    const meta::Props::Token token = meta::Props::Token::coordinates;
    setEntityProperty(meta::Props::token(token), coordinatesToString(coordinates));
    Q_EMIT coordinatesChanged(coordinates);
}

DVObject *DVObject::parentObject() const
{
    return qobject_cast<DVObject *>(parent());
}

DVModel *DVObject::model() const
{
    return qobject_cast<DVModel *>(VEObject::model());
}

bool DVObject::postInit()
{
    return true;
}

bool DVObject::aboutToBeRemoved()
{
    return true;
}

} // namespace deploy
