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

#include "aadlobjectsmodel.h"
#include "templating/aadlobjecttemplateproperty.h"

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

AADLObject::AADLObject(const common::Id &id, const QString &title, QObject *parent)
    : QObject(parent)
    , d(new AADLObjectPrivate {
              id,
              QHash<QString, QVariant> {
                      { meta::Props::token(meta::Props::Token::name), common::validatedName(title) } }, // attrs
              QHash<QString, QVariant> {}, // props
              nullptr, // model
      })
{
}

AADLObject::AADLObject(const QString &title, QObject *parent)
    : AADLObject(common::createId(), title, parent)
{
    if (!title.isEmpty())
        setAttr(meta::Props::token(meta::Props::Token::name), title);
}

AADLObject::~AADLObject() {}

QString AADLObject::title() const
{
    return attr(meta::Props::token(meta::Props::Token::name)).toString();
}

void AADLObject::postInit() {}

common::Id AADLObject::id() const
{
    return d->m_id;
}

bool AADLObject::setTitle(const QString &title)
{
    if (title != this->title()) {
        setAttr(meta::Props::token(meta::Props::Token::name), title);
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
    const QStringList &strCoords = strCoordinates.split(' ', QString::SkipEmptyParts);
    const int coordsCount = strCoords.size();
    QVector<qint32> coords(coordsCount);
    for (int i = 0; i < coordsCount; ++i)
        coords[i] = strCoords[i].toLong() / 100;
    return coords;
}

QVector<qint32> AADLObject::coordinates() const
{
    meta::Props::Token token = meta::Props::Token::coordinates;
    if (auto parentItem = parentObject()) {
        if (parentObject()->isRootObject()) {
            token = meta::Props::Token::InnerCoordinates;
        } else if (auto grandParent = parentItem->parentObject()) {
            if (aadlType() == AADLObject::AADLObjectType::AADLIface && grandParent->isRootObject())
                token = meta::Props::Token::InnerCoordinates;
        }
    } else if (isRootObject()) {
        token = meta::Props::Token::RootCoordinates;
    }

    return coordinatesFromString(prop(meta::Props::token(token)).toString());
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
    if (this->coordinates() == coordinates)
        return;

    meta::Props::Token token = meta::Props::Token::coordinates;
    if (auto parentItem = parentObject()) {
        if (parentObject()->isRootObject()) {
            token = meta::Props::Token::InnerCoordinates;
        } else if (auto grandParent = parentItem->parentObject()) {
            if (aadlType() == AADLObject::AADLObjectType::AADLIface && grandParent->isRootObject())
                token = meta::Props::Token::InnerCoordinates;
        }
    } else if (isRootObject()) {
        token = meta::Props::Token::RootCoordinates;
    }

    setProp(meta::Props::token(token), coordinatesToString(coordinates));
    emit coordinatesChanged(coordinates);
}

AADLObject *AADLObject::parentObject() const
{
    return qobject_cast<AADLObject *>(parent());
}

bool AADLObject::isFunction() const
{
    return aadlType() == AADLObjectType::AADLFunction;
}

bool AADLObject::isFunctionType() const
{
    return aadlType() == AADLObjectType::AADLFunctionType;
}

bool AADLObject::isNestedInFunction() const
{
    if (const AADLObject *parent = parentObject())
        return parent->isFunction();
    return false;
}

bool AADLObject::isNestedInFunctionType() const
{
    if (const AADLObject *parent = parentObject())
        return parent->isFunctionType();
    return false;
}

bool AADLObject::isNested() const
{
    return isNestedInFunction() || isNestedInFunctionType();
}

QHash<QString, QVariant> AADLObject::attrs() const
{
    return d->m_attrs;
}

/**
 * @brief AADLObject::templateAttributes returns list of attribues for using in string templates.
 * @return list of attribues.
 */
QVariantList AADLObject::templateAttributes() const
{
    return templating::generateTemplateProperties(d->m_attrs);
}

void AADLObject::setAttrs(const QHash<QString, QVariant> &attrs)
{
    if (d->m_attrs != attrs) {
        d->m_attrs.clear();

        for (auto i = attrs.cbegin(); i != attrs.cend(); ++i)
            setAttr(i.key(), i.value());
    }
}

QVariant AADLObject::attr(const QString &name, const QVariant &defaultValue) const
{
    return d->m_attrs.value(name, defaultValue);
}

void AADLObject::setAttr(const QString &name, const QVariant &val)
{
    if (!name.isEmpty() && val != d->m_attrs[name]) {
        const meta::Props::Token t = meta::Props::token(name);
        switch (t) {
        case meta::Props::Token::name: {
            const QString title = common::validatedName(val.toString());
            d->m_attrs[name] = title;
            emit titleChanged(title);
            break;
        }
        default:
            d->m_attrs[name] = val;
            break;
        }
        emit attributeChanged(meta::Props::token(name));
    }
}

void AADLObject::removeAttr(const QString &name)
{
    if (!name.isEmpty() && d->m_attrs.remove(name))
        emit attributeChanged(meta::Props::token(name));
}

QHash<QString, QVariant> AADLObject::props() const
{
    return d->m_props;
}

/**
 * @brief AADLObject::templateProperties returns list of properties for using in string templates.
 * @return list of properties.
 */
QVariantList AADLObject::templateProperties() const
{
    return templating::generateTemplateProperties(d->m_props);
}

void AADLObject::setProps(const QHash<QString, QVariant> &props)
{
    if (props != d->m_props) {
        d->m_props.clear();

        for (auto i = props.cbegin(); i != props.cend(); ++i)
            setProp(i.key(), i.value());
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
        emit propertyChanged(meta::Props::token(name));
    }
}

void AADLObject::removeProp(const QString &name)
{
    if (!name.isEmpty() && d->m_props.remove(name))
        emit propertyChanged(meta::Props::token(name));
}

void AADLObject::setObjectsModel(AADLObjectsModel *model)
{
    d->m_model = model;
}

AADLObjectsModel *AADLObject::objectsModel() const
{
    return d->m_model;
}

bool AADLObject::isRootObject() const
{
    return d->m_model ? d->m_model->rootObject() == this : false;
}

} // ns aadl
} // ns taste3
