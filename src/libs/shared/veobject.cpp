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

#include "veobject.h"

#include "vemodel.h"

#include <QVector>

namespace shared {

struct VEObjectPrivate {
    VEObjectPrivate(const shared::Id &id)
        : m_id(id == shared::InvalidId ? shared::createId() : id)
        , m_attrs()
        , m_model(nullptr)
    {
    }

    const shared::Id m_id;
    EntityAttributes m_attrs;
    VEModel *m_model;
};

VEObject::VEObject(const shared::Id &id, QObject *parent)
    : QObject(parent)
    , d(new VEObjectPrivate(id))
{
    if (const VEObject *parentObject = qobject_cast<const VEObject *>(parent))
        setModel(parentObject->model());
    else if (VEModel *model = qobject_cast<VEModel *>(parent))
        setModel(model);
}

VEObject::~VEObject() { }

shared::Id VEObject::id() const
{
    return d->m_id;
}

EntityAttributes VEObject::entityAttributes() const
{
    return d->m_attrs;
}

void VEObject::setEntityAttribute(const EntityAttribute &attribute)
{
    setAttributeImpl(attribute.name(), attribute.value(), attribute.type());
}

void VEObject::setEntityProperty(const QString &name, const QVariant &value)
{
    setAttributeImpl(name, value, EntityAttribute::Type::Property);
}

void VEObject::setEntityAttribute(const QString &name, const QVariant &value)
{
    setAttributeImpl(name, value, EntityAttribute::Type::Attribute);
}

EntityAttribute VEObject::entityAttribute(const QString &name) const
{
    return d->m_attrs.value(name);
}

QVariant VEObject::entityAttributeValue(const QString &name, const QVariant &defaultValue) const
{
    const EntityAttribute attr = entityAttribute(name);
    return attr.isNull() ? defaultValue : attr.value();
}

void VEObject::removeEntityAttribute(const QString &attributeName)
{
    if (!attributeName.isEmpty() && d->m_attrs.remove(attributeName))
        Q_EMIT attributeChanged(attributeName);
}

bool VEObject::hasEntityAttribute(const QString &attributeName, const QVariant &value) const
{
    const auto it = d->m_attrs.constFind(attributeName);
    if (it == d->m_attrs.cend()) {
        return false;
    }
    return !value.isValid() || it->value() == value;
}

/*!
   Returns true, when the object has an attribute wit the given \p key and the given \p value
 */
bool VEObject::hasEntityAttribute(const EntityAttribute &attribute) const
{
    const auto it = d->m_attrs.constFind(attribute.name());
    if (it == d->m_attrs.cend()) {
        return false;
    }
    return it.value().value() == attribute.value();
}

/*!
   Returns true, if the object contains all the given attributes with exactly the same value
 */
bool VEObject::hasEntityAttributes(const EntityAttributes &attrs) const
{
    return std::all_of(attrs.cbegin(), attrs.cend(),
            std::bind(qOverload<const EntityAttribute &>(&VEObject::hasEntityAttribute), this, std::placeholders::_1));
}

bool VEObject::setParentObject(VEObject *parentObject)
{
    if (parent() == parentObject)
        return false;

    setParent(parentObject);
    return true;
}

void VEObject::setAttributeImpl(const QString &name, const QVariant &value, EntityAttribute::Type type)
{
    d->m_attrs[name] = EntityAttribute { name, value, type };
    Q_EMIT attributeChanged(name);
}

void VEObject::clearAttributes()
{
    d->m_attrs.clear();
}

QVector<qint32> VEObject::coordinatesFromString(const QString &strCoordinates)
{
    const QStringList &strCoords = strCoordinates.split(QLatin1Char(' '), QString::SkipEmptyParts);
    const int coordsCount = strCoords.size();
    QVector<qint32> coords(coordsCount);
    for (int i = 0; i < coordsCount; ++i)
        coords[i] = strCoords[i].toLong() / 100;
    return coords;
}

QString VEObject::coordinatesToString(const QVector<qint32> &coordinates)
{
    QString coordString;
    for (auto coord : coordinates) {
        if (!coordString.isEmpty())
            coordString.append(QLatin1Char(' '));
        coordString.append(QString::number(coord * 100));
    }

    return coordString;
}

VEObject *VEObject::parentObject() const
{
    return qobject_cast<VEObject *>(parent());
}

void VEObject::setModel(VEModel *model)
{
    d->m_model = model;
}

VEModel *VEObject::model() const
{
    return d->m_model;
}

}
