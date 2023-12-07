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

#include <QScopedValueRollback>
#include <QVector>

namespace shared {

struct VEObjectPrivate {
    VEObjectPrivate(const shared::Id &id)
        : m_id(id == shared::InvalidId ? shared::createId() : id)
        , m_attrs()
        , m_model(nullptr)
    {
    }

    shared::Id m_id;
    EntityAttributes m_attrs;
    VEModel *m_model;
    bool m_marked = false;
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

void VEObject::setEntityAttributes(const EntityAttributes &attributes)
{
    if (attributes == entityAttributes()) {
        return;
    }

    clearAttributes();
    const QList<EntityAttribute> sortedAttrs = sortedAttributesValues(attributes);
    for (const EntityAttribute &attribute : qAsConst(sortedAttrs)) {
        setEntityAttribute(attribute);
    }
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

const VEObject *VEObject::rootObject() const
{
    VEObject *p = parentObject();
    if (!p)
        return this;

    while (p->parentObject()) {
        p = p->parentObject();
    }
    return p;
}

bool VEObject::setParentObject(VEObject *parentObject)
{
    if (parent() == parentObject)
        return false;

    setParent(parentObject);
    return true;
}

QList<EntityAttribute> VEObject::sortedAttributesValues(const EntityAttributes &attrs)
{
    return attrs.values();
}

void VEObject::setAttributeImpl(const QString &name, const QVariant &value, EntityAttribute::Type type)
{
    EntityAttribute attr { name, value, type };
    auto it = d->m_attrs.find(name);
    if (it != d->m_attrs.end()) {
        attr.setExportable(it->isExportable());
    }
    d->m_attrs[name] = attr;
    Q_EMIT attributeChanged(name);
}

void VEObject::clearAttributes()
{
    d->m_attrs.clear();
}

QVector<qint32> VEObject::coordinatesFromString(const QString &strCoordinates)
{
    if (strCoordinates.isEmpty())
        return {};

    const QStringList &strCoords = strCoordinates.split(QLatin1Char(' '), Qt::SkipEmptyParts);
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

bool VEObject::isNullCoordinates(const QVariant &coordinates)
{
    if (!coordinates.isValid()) {
        return true;
    }

    QVector<qint32> points;
    if (coordinates.canConvert<QString>()) {
        points = coordinatesFromString(coordinates.toString());
    } else if (coordinates.canConvert<QVector<qint32>>()) {
        points = coordinates.value<QVector<qint32>>();
    }

    if (points.isEmpty()) {
        return true;
    }

    return std::all_of(points.cbegin(), points.cend(), [](const qint32 &coordinate) { return coordinate == 0; });
}

/*!
   Returns if the \p other is equal to this object.
   Two of these are considered euqal if they have the exact same attributes.
 */
bool VEObject::isEqual(const VEObject *other) const
{
    return other && d->m_attrs == other->d->m_attrs;
}

void VEObject::setAttributeExportable(const QString &attrName, bool isExportable)
{
    auto it = d->m_attrs.find(attrName);
    if (it != d->m_attrs.end()) {
        it->setExportable(isExportable);
    }
}

/*!
 * A marked object might be drawn differently
 */
void VEObject::setMarked(bool mark)
{
    if (mark == d->m_marked) {
        return;
    }

    d->m_marked = mark;
    Q_EMIT markerChanged();
}

/*!
 * Retruns if the object is set to be marked
 */
bool VEObject::isMarked() const
{
    return d->m_marked;
}

/*!
 * Be very very careful using this function. As other very likely rely on the ID.
 */
void VEObject::recreateId()
{
    const shared::Id oldId = d->m_id;
    d->m_id = shared::createId();
    Q_EMIT idChanged(this, oldId, d->m_id);
}

/*!
 * Updates the ID reference, if it has one
 * \param obj Pointer to the object that changed it's ID
 * \param oldId The old ID of the object be fore the change
 * \param newId The new ID of the object
 */
void VEObject::updateIdDependency(VEObject *obj, const shared::Id &oldId, const shared::Id &newId)
{
    Q_UNUSED(newId)
    Q_UNUSED(oldId)
}

VEObject *VEObject::parentObject() const
{
    return qobject_cast<VEObject *>(parent());
}

/*!
   Returns all children/grandchildren/etc.
 */
QVector<VEObject *> VEObject::descendants() const
{
    QVector<VEObject *> result;
    for (QObject *child : children()) {
        if (VEObject *veChild = qobject_cast<VEObject *>(child)) {
            result.append(veChild->descendants());
            result.append(veChild);
        }
    }
    return result;
}

void VEObject::setModel(VEModel *model)
{
    d->m_model = model;
}

VEModel *VEObject::model() const
{
    return d->m_model;
}

QString toString(VEObject *object)
{
    return object->titleUI();
}

QDebug operator<<(QDebug debug, VEObject *object)
{
    QDebugStateSaver saver(debug);
    if (!object) {
        debug << "shared::VEObject(nullptr)";
    } else {
        debug << *object;
    }
    return debug;
}

QDebug operator<<(QDebug debug, const VEObject &object)
{
    static bool recursing = false;

    QDebugStateSaver saver(debug);
    auto metaType = object.metaObject()->metaType();

    if (recursing || metaType == QMetaType::fromType<VEObject>()) {
        debug.nospace() << metaType.name() << "(" << static_cast<const void *>(&object)
                        << ", title=" << object.titleUI() << ")";
    } else if (metaType.hasRegisteredDebugStreamOperator()) {
        // Guard for recursion. If a type derived from VEObject doesn't have a
        // debug operator, the meta type will still report that there is one,
        // but it will be the one for VEObject, and we will recurse infinitely.
        QScopedValueRollback guard(recursing, true);
        metaType.debugStream(debug, &object);
    }
    return debug;
}

}
