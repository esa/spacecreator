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

#include "deploymentobject.h"

namespace deployment {

struct DeploymentObjectPrivate {
    DeploymentObjectPrivate(const shared::Id &id, const DeploymentObject::Type t)
        : m_id(id == shared::InvalidId ? shared::createId() : id)
        , m_attrs()
        , m_props()
        , m_model(nullptr)
        , m_type(t)
    {
    }

    const shared::Id m_id;
    QHash<QString, QVariant> m_attrs;
    QHash<QString, QVariant> m_props;
    DeploymentObjectsModel *m_model;
    const DeploymentObject::Type m_type;
};

DeploymentObject::DeploymentObject(
        const DeploymentObject::Type t, const QString &title, QObject *parent, const shared::Id &id)
    : QObject(parent)
    , d(new DeploymentObjectPrivate(id, t))
{
    connect(this, qOverload<const QString &>(&DeploymentObject::propertyChanged), this,
            [this](const QString &token) { Q_EMIT propertyChanged(meta::Props::token(token)); });

    connect(this, qOverload<const QString &>(&DeploymentObject::attributeChanged), this,
            [this](const QString &token) { Q_EMIT attributeChanged(meta::Props::token(token)); });

    setAttr(meta::Props::token(meta::Props::Token::name), title);
}

DeploymentObject::~DeploymentObject() {}

QString DeploymentObject::title() const
{
    return attr(meta::Props::token(meta::Props::Token::name)).toString();
}

shared::Id DeploymentObject::id() const
{
    return d->m_id;
}

DeploymentObject::Type DeploymentObject::deployType() const
{
    return d->m_type;
}

bool DeploymentObject::setTitle(const QString &title)
{
    if (!title.isEmpty() && title != this->title()) {
        setAttr(meta::Props::token(meta::Props::Token::name), title);
        return true;
    }
    return false;
}

bool DeploymentObject::setParentObject(DeploymentObject *parentObject)
{
    if (parent() == parentObject)
        return false;

    setParent(parentObject);
    return true;
}

QVector<qint32> DeploymentObject::coordinatesFromString(const QString &strCoordinates)
{
    const QStringList &strCoords = strCoordinates.split(' ', QString::SkipEmptyParts);
    const int coordsCount = strCoords.size();
    QVector<qint32> coords(coordsCount);
    for (int i = 0; i < coordsCount; ++i)
        coords[i] = strCoords[i].toLong() / 100;
    return coords;
}

QString DeploymentObject::coordinatesToString(const QVector<qint32> &coordinates)
{
    QString coordString;
    for (auto coord : coordinates) {
        if (!coordString.isEmpty())
            coordString.append(' ');
        coordString.append(QString::number(coord * 100));
    }

    return coordString;
}

QVector<qint32> DeploymentObject::coordinates() const
{
    const meta::Props::Token token = meta::Props::Token::coordinates;
    const QVariant varCoord = prop(meta::Props::token(token));
    return coordinatesFromString(varCoord.toString());
}

void DeploymentObject::setCoordinates(const QVector<qint32> &coordinates)
{
    if (this->coordinates() == coordinates)
        return;

    const meta::Props::Token token = meta::Props::Token::coordinates;
    setProp(meta::Props::token(token), coordinatesToString(coordinates));
    Q_EMIT coordinatesChanged(coordinates);
}

DeploymentObject *DeploymentObject::parentObject() const
{
    return qobject_cast<DeploymentObject *>(parent());
}

QHash<QString, QVariant> DeploymentObject::attrs() const
{
    return d->m_attrs;
}

void DeploymentObject::setAttrs(const QHash<QString, QVariant> &attrs)
{
    if (d->m_attrs != attrs) {
        d->m_attrs.clear();

        for (auto i = attrs.cbegin(); i != attrs.cend(); ++i)
            setAttr(i.key(), i.value());
    }
}

QVariant DeploymentObject::attr(const QString &name, const QVariant &defaultValue) const
{
    return d->m_attrs.value(name, defaultValue);
}

void DeploymentObject::setAttr(const QString &name, const QVariant &val)
{
    d->m_attrs[name] = val;
    Q_EMIT attributeChanged(name);
}

void DeploymentObject::removeAttr(const QString &name)
{
    if (!name.isEmpty() && d->m_attrs.remove(name))
        Q_EMIT attributeChanged(name);
}

/*!
   Returns true, when the object has an attribute wit the given \p key and the given \p value
 */
bool DeploymentObject::hasAttribute(const QString &attributeName, const QVariant &value) const
{
    if (!d->m_attrs.contains(attributeName)) {
        return false;
    }
    return d->m_attrs[attributeName] == value;
}

/*!
   Returns true, if the object contains all the given attributes with exactly the same value
 */
bool DeploymentObject::hasAttributes(const QHash<QString, QVariant> &attrs) const
{
    for (auto it = attrs.cbegin(); it != attrs.end(); ++it) {
        if (!hasAttribute(it.key(), it.value())) {
            return false;
        }
    }
    return true;
}

QHash<QString, QVariant> DeploymentObject::props() const
{
    return d->m_props;
}

void DeploymentObject::setProps(const QHash<QString, QVariant> &props)
{
    if (props != d->m_props) {
        d->m_props.clear();

        for (auto i = props.cbegin(); i != props.cend(); ++i)
            setProp(i.key(), i.value());
    }
}

QVariant DeploymentObject::prop(const QString &name, const QVariant &defaultValue) const
{
    return d->m_props.value(name, defaultValue);
}

void DeploymentObject::setProp(const QString &name, const QVariant &val)
{
    if (!name.isEmpty() && val != d->m_props[name]) {
        d->m_props[name] = val;
        Q_EMIT propertyChanged(name);
    }
}

void DeploymentObject::removeProp(const QString &name)
{
    if (!name.isEmpty() && d->m_props.remove(name)) {
        Q_EMIT propertyChanged(name);
    }
}

/*!
   Returns true, when the object has a property with the given \p key and the given \p value
 */
bool DeploymentObject::hasProperty(const QString &propertyName, const QVariant &value) const
{
    const auto it = d->m_props.constFind(propertyName);
    if (it == d->m_props.constEnd()) {
        return false;
    }
    if (value.isNull()) {
        return true;
    }
    return value == *it;
}

/*!
   Returns true, if the object contains all the given properties with exactly the same value
 */
bool DeploymentObject::hasProperties(const QHash<QString, QVariant> &props) const
{
    for (auto it = props.cbegin(); it != props.end(); ++it) {
        if (!hasProperty(it.key(), it.value())) {
            return false;
        }
    }
    return true;
}

void DeploymentObject::setObjectsModel(DeploymentObjectsModel *model)
{
    d->m_model = model;
}

DeploymentObjectsModel *DeploymentObject::objectsModel() const
{
    return d->m_model;
}

} // namespace deployment
