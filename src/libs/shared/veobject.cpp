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

#include "errorhub.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"
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
    if (!attr.isNull())
        return attr.value();

    if (!defaultValue.isNull())
        return defaultValue;

    return defaultEntityAttributeValue(name);
}

QVariant VEObject::defaultEntityAttributeValue(const QString &name) const
{
    return {};
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

bool VEObject::postInit()
{
    if (auto dynPropConfig = propertyTemplaceConfig()) {
        for (const auto attr : dynPropConfig->propertyTemplatesForObject(this)) {
            if (attr->validate(this) && !attr->isOptional() && !hasEntityAttribute(attr->name())) {
                const QVariant &defaultValue = attr->defaultValue();
                if (!defaultValue.isNull()) {
                    if (attr->info() == PropertyTemplate::Info::Attribute) {
                        setEntityAttribute(attr->name(), defaultValue);
                    } else if (attr->info() == PropertyTemplate::Info::Property) {
                        setEntityProperty(attr->name(), defaultValue);
                    } else {
                        QMetaEnum metaEnum = QMetaEnum::fromType<shared::PropertyTemplate::Info>();
                        ErrorHub::addError(ErrorItem::Warning,
                                tr("Unknown dynamic property info: %1").arg(metaEnum.valueToKey(int(attr->info()))));
                    }
                }
            }
        }
    }
    return true;
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
