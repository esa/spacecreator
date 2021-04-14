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

#include "ivobject.h"

#include "ivmodel.h"
#include "ivnamevalidator.h"

#include <QPointer>
#include <QVector>

namespace ivm {

/*!
 * \namespace ivm
 * \brief The IV model code
 */

struct IVObjectPrivate {
    IVObjectPrivate(const shared::Id &id, const IVObject::Type t)
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
    IVModel *m_model;
    const IVObject::Type m_type;
};

IVObject::IVObject(const IVObject::Type t, const QString &title, QObject *parent, const shared::Id &id)
    : QObject(parent)
    , d(new IVObjectPrivate(id, t))
{
    connect(this, qOverload<const QString &>(&IVObject::propertyChanged), this,
            [this](const QString &token) { Q_EMIT propertyChanged(meta::Props::token(token)); });

    connect(this, qOverload<const QString &>(&IVObject::attributeChanged), this,
            [this](const QString &token) { Q_EMIT attributeChanged(meta::Props::token(token)); });

    if (const IVObject *parentObject = qobject_cast<const IVObject *>(parent))
        setObjectsModel(parentObject->model());
    else if (IVModel *model = qobject_cast<IVModel *>(parent))
        setObjectsModel(model);

    setAttr(meta::Props::token(meta::Props::Token::name), title);
}

IVObject::~IVObject() { }

QString IVObject::title() const
{
    return attr(meta::Props::token(meta::Props::Token::name)).toString();
}

QString IVObject::titleUI() const
{
    return IVNameValidator::decodeName(type(), title());
}

bool IVObject::postInit()
{
    return true;
}

bool IVObject::aboutToBeRemoved()
{
    return true;
}

//! This sorts the objects on type.
//! \sa ivm::IVObject::Type
void IVObject::sortObjectList(QList<IVObject *> &objects)
{
    std::stable_sort(objects.begin(), objects.end(),
            [](ivm::IVObject *obj1, ivm::IVObject *obj2) { return obj1->type() < obj2->type(); });
}

shared::Id IVObject::id() const
{
    return d->m_id;
}

IVObject::Type IVObject::type() const
{
    return d->m_type;
}

bool IVObject::setTitle(const QString &title)
{
    if (!title.isEmpty() && title != this->title()) {
        setAttr(meta::Props::token(meta::Props::Token::name), title);
        return true;
    }
    return false;
}

bool IVObject::setParentObject(IVObject *parentObject)
{
    if (parent() == parentObject)
        return false;

    setParent(parentObject);
    return true;
}

QVector<qint32> IVObject::coordinatesFromString(const QString &strCoordinates)
{
    const QStringList &strCoords = strCoordinates.split(' ', QString::SkipEmptyParts);
    const int coordsCount = strCoords.size();
    QVector<qint32> coords(coordsCount);
    for (int i = 0; i < coordsCount; ++i)
        coords[i] = strCoords[i].toLong() / 100;
    return coords;
}

QString IVObject::coordinatesToString(const QVector<qint32> &coordinates)
{
    QString coordString;
    for (auto coord : coordinates) {
        if (!coordString.isEmpty())
            coordString.append(' ');
        coordString.append(QString::number(coord * 100));
    }

    return coordString;
}

QVector<qint32> IVObject::coordinates() const
{
    const meta::Props::Token token = coordinatesType();
    const QVariant varCoord = prop(meta::Props::token(token));
    return coordinatesFromString(varCoord.toString());
}

void IVObject::setCoordinates(const QVector<qint32> &coordinates)
{
    if (this->coordinates() == coordinates)
        return;

    const meta::Props::Token token = coordinatesType();
    setProp(meta::Props::token(token), coordinatesToString(coordinates));
    Q_EMIT coordinatesChanged(coordinates);
}

meta::Props::Token IVObject::coordinatesType() const
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

QStringList IVObject::path() const
{
    return IVObject::path(this);
}

QStringList IVObject::path(const IVObject *obj)
{
    if (!obj) {
        return {};
    }
    QStringList list { obj->title() };
    IVObject *parent = obj->parentObject();
    while (parent) {
        if (parent->type() == ivm::IVObject::Type::Function
                || parent->type() == ivm::IVObject::Type::FunctionType) {
            list.prepend(parent->title());
        }
        parent = parent->parentObject();
    };
    return list;
}

IVObject *IVObject::parentObject() const
{
    return qobject_cast<IVObject *>(parent());
}

bool IVObject::isFunction() const
{
    return type() == Type::Function;
}

bool IVObject::isFunctionType() const
{
    return type() == Type::FunctionType;
}

bool IVObject::isInterfaceGroup() const
{
    return type() == Type::InterfaceGroup;
}

bool IVObject::isRequiredInterface() const
{
    return type() == Type::RequiredInterface;
}

bool IVObject::isProvidedInterface() const
{
    return type() == Type::ProvidedInterface;
}

bool IVObject::isInterface() const
{
    return isRequiredInterface() || isProvidedInterface() || isInterfaceGroup();
}

bool IVObject::isComment() const
{
    return type() == Type::Comment;
}

bool IVObject::isConnection() const
{
    return type() == Type::Connection;
}

bool IVObject::isConnectionGroup() const
{
    return type() == Type::ConnectionGroup;
}

bool IVObject::isNestedInFunction() const
{
    if (const IVObject *parent = parentObject())
        return parent->isFunction();
    return false;
}

bool IVObject::isNestedInFunctionType() const
{
    if (const IVObject *parent = parentObject())
        return parent->isFunctionType();
    return false;
}

bool IVObject::isNested() const
{
    return isNestedInFunction() || isNestedInFunctionType();
}

QString IVObject::groupName() const
{
    return attr(meta::Props::token(meta::Props::Token::group_name)).toString();
}

void IVObject::setGroupName(const QString &groupName)
{
    if (groupName.isEmpty()) {
        removeAttr(meta::Props::token(meta::Props::Token::group_name));
    } else {
        setAttr(meta::Props::token(meta::Props::Token::group_name), groupName);
    }
}

QHash<QString, QVariant> IVObject::attrs() const
{
    return d->m_attrs;
}

void IVObject::setAttrs(const QHash<QString, QVariant> &attrs)
{
    if (d->m_attrs != attrs) {
        d->m_attrs.clear();

        for (auto i = attrs.cbegin(); i != attrs.cend(); ++i)
            setAttr(i.key(), i.value());
    }
}

QVariant IVObject::attr(const QString &name, const QVariant &defaultValue) const
{
    return d->m_attrs.value(name, defaultValue);
}

void IVObject::setAttr(const QString &name, const QVariant &val)
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
                usedName = IVNameValidator::nextNameFor(this);

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

void IVObject::removeAttr(const QString &name)
{
    if (!name.isEmpty() && d->m_attrs.remove(name))
        Q_EMIT attributeChanged(name);
}

/*!
   Returns true, when the object has an attribute wit the given \p key and the given \p value
 */
bool IVObject::hasAttribute(const QString &attributeName, const QVariant &value) const
{
    const auto it = d->m_attrs.constFind(attributeName);
    if (it == d->m_attrs.constEnd()) {
        return false;
    }
    if (value.isNull()) {
        return true;
    }
    return value == *it;
}

/*!
   Returns true, if the object contains all the given attributes with exactly the same value
 */
bool IVObject::hasAttributes(const QHash<QString, QVariant> &attrs) const
{
    for (auto it = attrs.cbegin(); it != attrs.end(); ++it) {
        if (!hasAttribute(it.key(), it.value())) {
            return false;
        }
    }
    return true;
}

QHash<QString, QVariant> IVObject::props() const
{
    return d->m_props;
}

void IVObject::setProps(const QHash<QString, QVariant> &props)
{
    if (props != d->m_props) {
        d->m_props.clear();

        for (auto i = props.cbegin(); i != props.cend(); ++i)
            setProp(i.key(), i.value());
    }
}

QVariant IVObject::prop(const QString &name, const QVariant &defaultValue) const
{
    return d->m_props.value(name, defaultValue);
}

void IVObject::setProp(const QString &name, const QVariant &val)
{
    if (!name.isEmpty() && val != d->m_props[name]) {
        d->m_props[name] = val;
        Q_EMIT propertyChanged(name);
    }
}

void IVObject::removeProp(const QString &name)
{
    if (!name.isEmpty() && d->m_props.remove(name)) {
        Q_EMIT propertyChanged(name);
    }
}

/*!
   Returns true, when the object has a property with the given \p key and the given \p value
 */
bool IVObject::hasProperty(const QString &propertyName, const QVariant &value) const
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
bool IVObject::hasProperties(const QHash<QString, QVariant> &props) const
{
    for (auto it = props.cbegin(); it != props.end(); ++it) {
        if (!hasProperty(it.key(), it.value())) {
            return false;
        }
    }
    return true;
}

void IVObject::setObjectsModel(IVModel *model)
{
    d->m_model = model;
}

IVModel *IVObject::model() const
{
    return d->m_model;
}

bool IVObject::isRootObject() const
{
    return d->m_model ? d->m_model->rootObject() == this : false;
}

bool IVObject::isGrouped() const
{
    return !groupName().isEmpty();
}

void IVObject::setVisible(bool isVisible)
{
    setAttr(meta::Props::token(meta::Props::Token::is_visible), isVisible);
}

/*!
   Returns false, if the "is_visible" is set to false.
   Returns true, if set to true or is not set at all. Also does not take the parent's visibility into account.
 */
bool IVObject::isVisible() const
{
    return d->m_attrs.value(meta::Props::token(meta::Props::Token::is_visible), true).toBool();
}

}
