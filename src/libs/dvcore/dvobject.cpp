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

#include "dvobject.h"

#include "dvmodel.h"
#include "dvnamevalidator.h"
#include "exportableproperty.h"

#include <QVector>

namespace dvm {

struct DVObjectPrivate {
    DVObjectPrivate(const DVObject::Type t)
        : m_type(t)
    {
    }

    const DVObject::Type m_type;
};

DVObject::DVObject(const DVObject::Type t, const QString &title, QObject *parent, const shared::Id &id)
    : shared::VEObject(id, parent)
    , d(new DVObjectPrivate(t))
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::name), title);
}

DVObject::~DVObject() {}

QString DVObject::title() const
{
    return entityAttributeValue<QString>(meta::Props::token(meta::Props::Token::name));
}

QString DVObject::titleUI() const
{
    QString text = DVNameValidator::decodeName(type(), title());
    static const QString urlAttrName { dvm::meta::Props::token(dvm::meta::Props::Token::url) };
    if (hasEntityAttribute(urlAttrName)) {
        const QString url = entityAttributeValue<QString>(urlAttrName);
        text = QStringLiteral("<a href=\"%1\">%2</a>").arg(url, text);
    }
    return text;
}

DVObject::Type DVObject::type() const
{
    return d->m_type;
}

bool DVObject::setTitle(const QString &title)
{
    if (!title.isEmpty() && title != this->title()) {
        setEntityAttribute(meta::Props::token(meta::Props::Token::name), title);
        return true;
    }
    return false;
}

bool DVObject::setParentObject(DVObject *parentObject)
{
    if (parent() == parentObject)
        return false;

    setParent(parentObject);
    return true;
}

void DVObject::setAttributeImpl(const QString &name, const QVariant &value, EntityAttribute::Type type)
{
    EntityAttribute attr = entityAttribute(name);
    if (attr.isValid() && attr.value() == value) {
        return;
    }
    const meta::Props::Token token = meta::Props::token(name);
    switch (token) {
    case meta::Props::Token::name: {
        attr = EntityAttribute { name, value, type };
        QString usedName = value.value<QString>();
        if (usedName.isEmpty()) {
            usedName = DVNameValidator::nameForObject(this);
            attr.setValue(usedName);
        }
        VEObject::setAttributeImpl(attr.name(), attr.value(), attr.type());
        Q_EMIT titleChanged(usedName);
        break;
    }
    default: {
        VEObject::setAttributeImpl(name, value, type);
        if (meta::Props::Token::coordinates == token) {
            Q_EMIT coordinatesChanged(value.value<QVector<qint32>>());
        } else if (meta::Props::Token::url == token) {
            Q_EMIT urlChanged(value.toString());
        }
        break;
    }
    }
}

QVector<qint32> DVObject::coordinates() const
{
    const meta::Props::Token token = meta::Props::Token::coordinates;
    const QVariant varCoord = entityAttributeValue(meta::Props::token(token));
    return coordinatesFromString(varCoord.toString());
}

void DVObject::setCoordinates(const QVector<qint32> &coordinates)
{
    if (this->coordinates() == coordinates)
        return;

    const meta::Props::Token token = meta::Props::Token::coordinates;
    setEntityProperty(meta::Props::token(token), coordinatesToString(coordinates));
}

DVObject *DVObject::parentObject() const
{
    return qobject_cast<DVObject *>(parent());
}

DVModel *DVObject::model() const
{
    return qobject_cast<DVModel *>(VEObject::model());
}

bool DVObject::postInit()
{
    return true;
}

bool DVObject::aboutToBeRemoved()
{
    return true;
}

bool DVObject::isEqual(DVObject *other) const
{
    return this->type() == other->type() && VEObject::isEqual(other);
}

/**
 * @brief Returns list of attributes for using in string templates.
 * @return list of attributes.
 */
QVariantList DVObject::attributes() const
{
    return generateProperties(false);
}

/**
 * @brief Returns list of properties for using in string templates.
 * @return list of properties.
 */
QVariantList DVObject::properties() const
{
    return generateProperties(true);
}

/**
 * @brief generateProperties generates a variant list sorted by meta::Props::Token.
 * @param props can be hash of attributes or properties of DVObject.
 * @return sorted QVariantList which can be used in string templates
 */
QVariantList DVObject::generateProperties(bool isProperty) const
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
        const dvm::meta::Props::Token right_token = dvm::meta::Props::token(r.name());
        if (right_token == dvm::meta::Props::Token::Unknown)
            return true;

        const auto &l = left_val.value<shared::ExportableProperty>();
        const dvm::meta::Props::Token left_token = dvm::meta::Props::token(l.name());
        if (left_token == dvm::meta::Props::Token::Unknown)
            return false;

        return left_token < right_token;
    });

    return result;
}

} // namespace dvm
