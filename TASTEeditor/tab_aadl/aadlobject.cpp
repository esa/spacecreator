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

#include "aadlcommonprops.h"
#include "aadlobjectsmodel.h"

#include <QPointer>
#include <QVector>

namespace taste3 {
namespace aadl {

struct AADLObjectPrivate {
    common::Id m_id;
    QHash<QString, QVariant> m_attrs;
    QHash<QString, QVariant> m_props;
    AADLObjectsModel *m_model;
};

AADLObject::AADLObject(const QString &title, QObject *parent)
    : QObject(parent)
    , d(new AADLObjectPrivate { common::createId(),
                                QHash<QString, QVariant> { { meta::token(meta::Token::name), title } }, // attrs
                                QHash<QString, QVariant> {}, // props
                                nullptr })
{
}

AADLObject::~AADLObject() {}

QString AADLObject::title() const
{
    return attr(meta::token(meta::Token::name)).toString();
}

common::Id AADLObject::id() const
{
    return d->m_id;
}

bool AADLObject::setTitle(const QString &title)
{
    if (title != this->title()) {
        setAttr(meta::token(meta::Token::name), title);
        emit titleChanged(title);
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

QVector<qint32> AADLObject::coordinatesFromString(const QString &strCoordinates) const
{
    const QStringList &strCoords = strCoordinates.split(' ');
    const int coordsCount = strCoords.size();
    QVector<qint32> coords(coordsCount);
    for (int i = 0; i < coordsCount; ++i)
        coords[i] = strCoords[i].toLong() / 100;
    return coords;
}

QVector<qint32> AADLObject::coordinates() const
{
    return coordinatesFromString(prop(meta::token(meta::Token::coordinates)).toString());
}

QString AADLObject::coordinatesToString(const QVector<qint32> &coordinates) const
{
    QString coordString;
    for (auto coord : coordinates) {
        if (!coordString.isEmpty())
            coordString.append(' ');
        coordString.append(QString::number(coord * 100));
    }

    return coordString;
}

void AADLObject::setCoordinates(const QVector<qint32> &coordinates)
{
    if (this->coordinates() != coordinates) {
        setProp(meta::token(meta::Token::coordinates), coordinatesToString(coordinates));
        emit coordinatesChanged(coordinates);
    }
}

AADLObject *AADLObject::parentObject() const
{
    return qobject_cast<AADLObject *>(parent());
}

QHash<QString, QVariant> AADLObject::attrs() const
{
    return d->m_attrs;
}

void AADLObject::setAttrs(const QHash<QString, QVariant> &attrs)
{
    if (d->m_attrs != attrs) {
        d->m_attrs = attrs;
        emit attributesChanged();
    }
}

QVariant AADLObject::attr(const QString &name, const QVariant &defaultValue) const
{
    return d->m_attrs.value(name, defaultValue);
}

void AADLObject::setAttr(const QString &name, const QVariant &val)
{
    if (!name.isEmpty() && val != d->m_attrs[name]) {
        d->m_attrs[name] = val;
        emit attributesChanged();
    }
}

void AADLObject::removeAttr(const QString &name)
{
    if (!name.isEmpty() && d->m_attrs.remove(name))
        emit attributesChanged();
}

QHash<QString, QVariant> AADLObject::props() const
{
    return d->m_props;
}

void AADLObject::setProps(const QHash<QString, QVariant> &props)
{
    if (props != d->m_props) {
        d->m_props = props;
        emit propertiesChanged();
    }
}

QVariant AADLObject::prop(const QString &name, const QVariant &defaultValue) const
{
    return d->m_props.value(name, defaultValue);
}

void AADLObject::setProp(const QString &name, const QVariant &val)
{
    if (!name.isEmpty() && val != d->m_props[name]) {
        d->m_props[name] = val;
        emit propertiesChanged();
    }
}

void AADLObject::removeProp(const QString &name)
{
    if (!name.isEmpty() && d->m_props.remove(name))
        emit propertiesChanged();
}

void AADLObject::setObjectsModel(AADLObjectsModel *model)
{
    d->m_model = model;
}

AADLObjectsModel *AADLObject::objectsModel() const
{
    return d->m_model;
}

} // ns aadl
} // ns taste3
