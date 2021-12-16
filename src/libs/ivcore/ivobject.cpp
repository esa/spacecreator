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

#include "exportableproperty.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"

#include <QPointer>
#include <QVector>
#include <QtDebug>

namespace ivm {

/*!
 * \namespace ivm
 * \brief The IV model code
 */

struct IVObjectPrivate {
    IVObjectPrivate(const IVObject::Type t)
        : m_type(t)
    {
    }

    const IVObject::Type m_type;
};

IVObject::IVObject(const IVObject::Type t, QObject *parent, const shared::Id &id)
    : shared::VEObject(id, parent)
    , d(new IVObjectPrivate(t))
{
    if (const IVObject *parentObject = qobject_cast<const IVObject *>(parent))
        setModel(parentObject->model());
    else if (IVModel *model = qobject_cast<IVModel *>(parent))
        setModel(model);
}

IVObject::~IVObject() {}

QString IVObject::title() const
{
    return entityAttributeValue<QString>(meta::Props::token(meta::Props::Token::name));
}

QString IVObject::titleUI() const
{
    QString text = IVNameValidator::decodeName(type(), title());
    static const QString urlAttrName { ivm::meta::Props::token(ivm::meta::Props::Token::url) };
    if (hasEntityAttribute(urlAttrName)) {
        const QString url = entityAttributeValue<QString>(urlAttrName);
        text = QStringLiteral("<a href=\"%1\">%2</a>").arg(url, text);
    }
    return text;
}

bool IVObject::postInit()
{
    if (title().isEmpty()) {
        resetTitle();
    }
    return true;
}

bool IVObject::aboutToBeRemoved()
{
    return true;
}

void IVObject::resetTitle()
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::name), {});
}

//! This sorts the objects on type.
//! \sa ivm::IVObject::Type
void IVObject::sortObjectList(QList<IVObject *> &objects)
{
    std::stable_sort(objects.begin(), objects.end(),
            [](ivm::IVObject *obj1, ivm::IVObject *obj2) { return obj1->type() < obj2->type(); });
}

QString IVObject::typeToString(Type t)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Type>();
    return metaEnum.valueToKey(int(t));
}

/**
 * @brief Returns list of attributes for using in string templates.
 * @return list of attributes.
 */
QVariantList IVObject::attributes() const
{
    return generateProperties(false);
}

/**
 * @brief Returns list of properties for using in string templates.
 * @return list of properties.
 */
QVariantList IVObject::properties() const
{
    return generateProperties(true);
}

/**
 * @brief generateProperties generates a variant list sorted by meta::Props::Token.
 * @param props can be hash of attributes or properties of IVObject.
 * @return sorted QVariantList which can be used in string templates
 */
QVariantList IVObject::generateProperties(bool isProperty) const
{
    QVariantList result;
    EntityAttributes attributes = entityAttributes();
    for (auto it = attributes.cbegin(); it != attributes.cend(); ++it) {
        if (it.value().isExportable() && it.value().isProperty() == isProperty) {
            result << QVariant::fromValue(shared::ExportableProperty(it.key(), it.value().value()));
        }
    }

    std::sort(result.begin(), result.end(), [](const QVariant &left_val, const QVariant &right_val) {
        const auto &r = right_val.value<shared::ExportableProperty>();
        const ivm::meta::Props::Token right_token = ivm::meta::Props::token(r.name());
        if (right_token == ivm::meta::Props::Token::Unknown)
            return true;

        const auto &l = left_val.value<shared::ExportableProperty>();
        const ivm::meta::Props::Token left_token = ivm::meta::Props::token(l.name());
        if (left_token == ivm::meta::Props::Token::Unknown)
            return false;

        return left_token < right_token;
    });

    return result;
}

IVObject::Type IVObject::type() const
{
    return d->m_type;
}

bool IVObject::setTitle(const QString &title)
{
    if (!title.isEmpty() && title != this->title()) {
        setEntityAttribute(meta::Props::token(meta::Props::Token::name), title);
        return true;
    }
    return false;
}

QVector<qint32> IVObject::coordinates() const
{
    const meta::Props::Token token = coordinatesType();
    const QVariant varCoord = entityAttributeValue(meta::Props::token(token));
    return coordinatesFromString(varCoord.toString());
}

void IVObject::setCoordinates(const QVector<qint32> &coordinates)
{
    if (this->coordinates() == coordinates)
        return;

    const meta::Props::Token token = coordinatesType();
    setEntityProperty(meta::Props::token(token), coordinatesToString(coordinates));
}

meta::Props::Token IVObject::coordinatesType() const
{
    bool isRootObj = isRootObject() || (isInterface() && parentObject() && parentObject()->isRootObject());
    return isRootObj ? meta::Props::Token::RootCoordinates : meta::Props::Token::coordinates;
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
        if (parent->type() == ivm::IVObject::Type::Function || parent->type() == ivm::IVObject::Type::FunctionType) {
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

bool IVObject::isConnectionLayer() const
{
    return type() == Type::ConnectionLayer;
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
    return entityAttributeValue<QString>(meta::Props::token(meta::Props::Token::group_name));
}

void IVObject::setGroupName(const QString &groupName)
{
    if (groupName.isEmpty()) {
        removeEntityAttribute(meta::Props::token(meta::Props::Token::group_name));
    } else {
        setEntityAttribute(meta::Props::token(meta::Props::Token::group_name), groupName);
    }
}

void IVObject::setAttributeImpl(const QString &attributeName, const QVariant &value, EntityAttribute::Type type)
{
    if (!attributeName.isEmpty()) {
        EntityAttribute attr = entityAttribute(attributeName);
        if (attr.isValid() && attr.value() == value) {
            return;
        }
        attr = EntityAttribute { attributeName, value, type };
        const meta::Props::Token t = meta::Props::token(attributeName);
        switch (t) {
        case meta::Props::Token::is_visible: {
            VEObject::setAttributeImpl(attr.name(), attr.value(), attr.type());
            Q_EMIT visibilityChanged(value.value<bool>());
            break;
        }
        case meta::Props::Token::url: {
            VEObject::setAttributeImpl(attr.name(), attr.value(), attr.type());
            Q_EMIT urlChanged(value.value<QString>());
            break;
        }
        case meta::Props::Token::group_name: {
            VEObject::setAttributeImpl(attr.name(), attr.value(), attr.type());
            Q_EMIT groupChanged(value.value<QString>());
            break;
        }
        case meta::Props::Token::RootCoordinates:
        case meta::Props::Token::coordinates: {
            if (isNullCoordinates(attr.value())) {
                VEObject::removeEntityAttribute(attr.name());
            } else {
                VEObject::setAttributeImpl(attr.name(), attr.value(), attr.type());
            }
            Q_EMIT coordinatesChanged(value.value<QVector<qint32>>());
            break;
        }
        case meta::Props::Token::name: {
            QString usedName = value.value<QString>();
            if (usedName.isEmpty()) {
                usedName = IVNameValidator::nextNameFor(this);
                attr.setValue(usedName);
            }
            VEObject::setAttributeImpl(attr.name(), attr.value(), attr.type());
            Q_EMIT titleChanged(usedName);
            break;
        }
        default:
            VEObject::setAttributeImpl(attr.name(), attr.value(), attr.type());
            break;
        }
    }
}

QList<EntityAttribute> IVObject::sortedAttributesValues(const EntityAttributes &attributes)
{
    QList<EntityAttribute> attrs = attributes.values();
    std::sort(attrs.begin(), attrs.end(), [](const EntityAttribute &a1, const EntityAttribute &a2) {
        return meta::Props::token(a1.name()) > meta::Props::token(a2.name());
    });
    return attrs;
}

IVModel *IVObject::model() const
{
    return qobject_cast<IVModel *>(shared::VEObject::model());
}

bool IVObject::isRootObject() const
{
    return model() ? model()->rootObject() == this : false;
}

bool IVObject::isGrouped() const
{
    return !groupName().isEmpty();
}

void IVObject::setVisible(bool isVisible)
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::is_visible), isVisible);
}

/*!
   Returns false, if the "is_visible" is set to false.
   Returns true, if set to true or is not set at all. Also does not take the parent's visibility into account.
 */
bool IVObject::isVisible() const
{
    return entityAttributeValue(meta::Props::token(meta::Props::Token::is_visible), true);
}

}
