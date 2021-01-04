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

#include "aadlnamevalidator.h"
#include "aadlobjectsmodel.h"

#include <QPointer>
#include <QVector>

namespace aadl {

/*!
 * \namespace aadl
 * \brief The AADL model code
 */

struct AADLObjectPrivate {
    AADLObjectPrivate(const shared::Id &id, const AADLObject::Type t)
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
    AADLObjectsModel *m_model;
    const AADLObject::Type m_type;
};

AADLObject::AADLObject(const AADLObject::Type t, const QString &title, QObject *parent, const shared::Id &id)
    : QObject(parent)
    , d(new AADLObjectPrivate(id, t))
{
    connect(this, qOverload<const QString &>(&AADLObject::propertyChanged), this,
            [this](const QString &token) { Q_EMIT propertyChanged(meta::Props::token(token)); });

    connect(this, qOverload<const QString &>(&AADLObject::attributeChanged), this,
            [this](const QString &token) { Q_EMIT attributeChanged(meta::Props::token(token)); });

    if (const AADLObject *parentObject = qobject_cast<const AADLObject *>(parent))
        setObjectsModel(parentObject->objectsModel());
    else if (AADLObjectsModel *model = qobject_cast<AADLObjectsModel *>(parent))
        setObjectsModel(model);

    setAttr(meta::Props::token(meta::Props::Token::name), title);
}

AADLObject::~AADLObject() { }

QString AADLObject::title() const
{
    return attr(meta::Props::token(meta::Props::Token::name)).toString();
}

QString AADLObject::titleUI() const
{
    return AADLNameValidator::decodeName(aadlType(), title());
}

bool AADLObject::postInit()
{
    return true;
}

bool AADLObject::aboutToBeRemoved()
{
    return true;
}

//! This sorts the objects on type.
//! \sa aadlType
void AADLObject::sortObjectList(QList<AADLObject *> &objects)
{
    std::stable_sort(objects.begin(), objects.end(),
            [](aadl::AADLObject *obj1, aadl::AADLObject *obj2) { return obj1->aadlType() < obj2->aadlType(); });
}

shared::Id AADLObject::id() const
{
    return d->m_id;
}

AADLObject::Type AADLObject::aadlType() const
{
    return d->m_type;
}

bool AADLObject::setTitle(const QString &title)
{
    if (!title.isEmpty() && title != this->title()) {
        setAttr(meta::Props::token(meta::Props::Token::name), title);
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

QVector<qint32> AADLObject::coordinatesFromString(const QString &strCoordinates)
{
    const QStringList &strCoords = strCoordinates.split(' ', QString::SkipEmptyParts);
    const int coordsCount = strCoords.size();
    QVector<qint32> coords(coordsCount);
    for (int i = 0; i < coordsCount; ++i)
        coords[i] = strCoords[i].toLong() / 100;
    return coords;
}

QString AADLObject::coordinatesToString(const QVector<qint32> &coordinates)
{
    QString coordString;
    for (auto coord : coordinates) {
        if (!coordString.isEmpty())
            coordString.append(' ');
        coordString.append(QString::number(coord * 100));
    }

    return coordString;
}

QVector<qint32> AADLObject::coordinates() const
{
    const meta::Props::Token token = coordinatesType();
    const QVariant varCoord = prop(meta::Props::token(token));
    return coordinatesFromString(varCoord.toString());
}

void AADLObject::setCoordinates(const QVector<qint32> &coordinates)
{
    if (this->coordinates() == coordinates)
        return;

    const meta::Props::Token token = coordinatesType();
    setProp(meta::Props::token(token), coordinatesToString(coordinates));
    Q_EMIT coordinatesChanged(coordinates);
}

meta::Props::Token AADLObject::coordinatesType() const
{
    meta::Props::Token token = meta::Props::Token::coordinates;
    if (auto parentItem = parentObject()) {
        if (parentObject()->isRootObject()) {
            if (isInterface()) {
                token = meta::Props::Token::RootCoordinates;
            } else {
                token = meta::Props::Token::InnerCoordinates;
            }
        } else if (auto grandParent = parentItem->parentObject()) {
            if (isInterface() && grandParent->isRootObject()) {
                token = meta::Props::Token::InnerCoordinates;
            }
        }
    }
    if (isRootObject()) {
        token = meta::Props::Token::RootCoordinates;
    }
    return token;
}

QStringList AADLObject::path() const
{
    return AADLObject::path(this);
}

QStringList AADLObject::path(const AADLObject *obj)
{
    if (!obj) {
        return {};
    }
    QStringList list { obj->title() };
    AADLObject *parent = obj->parentObject();
    while (parent) {
        if (parent->aadlType() == aadl::AADLObject::Type::Function
                || parent->aadlType() == aadl::AADLObject::Type::FunctionType) {
            list.prepend(parent->title());
        }
        parent = parent->parentObject();
    };
    return list;
}

AADLObject *AADLObject::parentObject() const
{
    return qobject_cast<AADLObject *>(parent());
}

bool AADLObject::isFunction() const
{
    return aadlType() == Type::Function;
}

bool AADLObject::isFunctionType() const
{
    return aadlType() == Type::FunctionType;
}

bool AADLObject::isInterfaceGroup() const
{
    return aadlType() == Type::InterfaceGroup;
}

bool AADLObject::isRequiredInterface() const
{
    return aadlType() == Type::RequiredInterface;
}

bool AADLObject::isProvidedInterface() const
{
    return aadlType() == Type::ProvidedInterface;
}

bool AADLObject::isInterface() const
{
    return isRequiredInterface() || isProvidedInterface() || isInterfaceGroup();
}

bool AADLObject::isComment() const
{
    return aadlType() == Type::Comment;
}

bool AADLObject::isConnection() const
{
    return aadlType() == Type::Connection;
}

bool AADLObject::isConnectionGroup() const
{
    return aadlType() == Type::ConnectionGroup;
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

QString AADLObject::groupName() const
{
    return attr(meta::Props::token(meta::Props::Token::group_name)).toString();
}

void AADLObject::setGroupName(const QString &groupName)
{
    if (groupName.isEmpty()) {
        removeAttr(meta::Props::token(meta::Props::Token::group_name));
    } else {
        setAttr(meta::Props::token(meta::Props::Token::group_name), groupName);
    }
}

QHash<QString, QVariant> AADLObject::attrs() const
{
    return d->m_attrs;
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
        case meta::Props::Token::is_visible: {
            d->m_attrs[name] = val;
            Q_EMIT visibilityChanged(val.toBool());
            break;
        }
        case meta::Props::Token::group_name: {
            d->m_attrs[name] = val;
            Q_EMIT groupChanged(val.toString());
            break;
        }
        case meta::Props::Token::name: {
            QString usedName = val.toString();
            if (usedName.isEmpty())
                usedName = AADLNameValidator::nextNameFor(this);

            d->m_attrs[name] = usedName;
            Q_EMIT titleChanged(usedName);
            break;
        }
        default:
            d->m_attrs[name] = val;
            break;
        }
        Q_EMIT attributeChanged(name);
    }
}

void AADLObject::removeAttr(const QString &name)
{
    if (!name.isEmpty() && d->m_attrs.remove(name))
        Q_EMIT attributeChanged(name);
}

/*!
   Returns true, when the object has an attribute wit the given \p key and the given \p value
 */
bool AADLObject::hasAttribute(const QString &attributeName, const QVariant &value) const
{
    if (!d->m_attrs.contains(attributeName)) {
        return false;
    }
    return d->m_attrs[attributeName] == value;
}

/*!
   Returns true, if the object contains all the given attributes with exactly the same value
 */
bool AADLObject::hasAttributes(const QHash<QString, QVariant> &attrs) const
{
    for (auto it = attrs.cbegin(); it != attrs.end(); ++it) {
        if (!hasAttribute(it.key(), it.value())) {
            return false;
        }
    }
    return true;
}

QHash<QString, QVariant> AADLObject::props() const
{
    return d->m_props;
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
        Q_EMIT propertyChanged(name);
    }
}

void AADLObject::removeProp(const QString &name)
{
    if (!name.isEmpty() && d->m_props.remove(name)) {
        Q_EMIT propertyChanged(name);
    }
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

bool AADLObject::isGrouped() const
{
    return !groupName().isEmpty();
}

void AADLObject::setVisible(bool isVisible)
{
    setAttr(meta::Props::token(meta::Props::Token::is_visible), isVisible);
}

/*!
   Returns false, if the "is_visible" is set to false.
   Returns true, if set to true or is not set at all. Also does not take the parent's visibility into account.
 */
bool AADLObject::isVisible() const
{
    return d->m_attrs.value(meta::Props::token(meta::Props::Token::is_visible), true).toBool();
}

}
