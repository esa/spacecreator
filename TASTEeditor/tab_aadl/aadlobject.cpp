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

AADLObject::AADLObject(const common::Id &id, const QString &title, QObject *parent)
    : QObject(parent)
    , d(new AADLObjectPrivate {
              id, QHash<QString, QVariant> { { meta::Props::token(meta::Props::Token::name), title } }, // attrs
              QHash<QString, QVariant> {}, // props
              nullptr })
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

void AADLObject::handleClonedAttr(taste3::aadl::meta::Props::Token attr) {}

void AADLObject::handleClonedProp(taste3::aadl::meta::Props::Token prop) {}

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

/**
 * @brief AADLObject::generateSortedList generates a variant list sorted by meta::Props::Token.
 * @param props can be internal hash of attributes or properties.
 * @return sorted QVariantList which can be used in string templates
 */
QVariantList AADLObject::generateSortedList(const QHash<QString, QVariant> &props)
{
    QVariantList result;
    for (auto it = props.cbegin(); it != props.cend(); ++it)
        result << QVariant::fromValue(AADLObjectProperty(it.key(), it.value()));

    std::sort(result.begin(), result.end(), [] (const QVariant &left_val, const QVariant &right_val) {
        const AADLObjectProperty r = right_val.value<AADLObjectProperty>();
        meta::Props::Token right_token = meta::Props::token(r.name());
        if (right_token == meta::Props::Token::Unknown)
            return true;

        const AADLObjectProperty l = left_val.value<AADLObjectProperty>();
        meta::Props::Token left_token = meta::Props::token(l.name());
        if (left_token == meta::Props::Token::Unknown)
            return false;

        return left_token < right_token;
    });

    return result;
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
    }

    setProp(meta::Props::token(token), coordinatesToString(coordinates));
    emit coordinatesChanged(coordinates);
}

AADLObject *AADLObject::parentObject() const
{
    return qobject_cast<AADLObject *>(parent());
}

QHash<QString, QVariant> AADLObject::attrs() const
{
    return d->m_attrs;
}

/**
 * @brief AADLObject::attrList returns attribute list which sorted by name
 * according to meta::Props::Token order.
 * @return sorted attribute list.
 */
QVariantList AADLObject::attrList() const
{
    return generateSortedList(d->m_attrs);
}

void AADLObject::setAttrs(const QHash<QString, QVariant> &attrs)
{
    if (d->m_attrs != attrs) {
        d->m_attrs = attrs;
        emit attributeChanged();
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

        const meta::Props::Token t = meta::Props::token(name);
        switch (t) {
        case meta::Props::Token::name: {
            emit titleChanged(val.toString());
            break;
        }
        default:
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
 * @brief AADLObject::propList returns property list which sorted by name
 * according to meta::Props::Token order.
 * @return sorted property list.
 */
QVariantList AADLObject::propList() const
{
    return generateSortedList(d->m_props);
}

void AADLObject::setProps(const QHash<QString, QVariant> &props)
{
    if (props != d->m_props) {
        d->m_props = props;
        emit propertyChanged();
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
