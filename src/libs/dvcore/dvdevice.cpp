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

#include "dvdevice.h"

#include "dvcommonprops.h"
#include "dvnode.h"
#include "dvport.h"

namespace dvm {

DVDevice::DVDevice(const DVPort &port, DVObject *parent)
    : DVDevice(parent)
{
    EntityAttributes attrs = port.entityAttributes();
    EntityAttribute entityAttr = attrs.take(meta::Props::token(meta::Props::Token::name));
    QString attrName = dvm::meta::Props::token(meta::Props::Token::port);
    attrs[attrName] = EntityAttribute { attrName, entityAttr.value(), entityAttr.type() };

    entityAttr = attrs.take(meta::Props::token(meta::Props::Token::requiresBusAccess));
    attrName = dvm::meta::Props::token(meta::Props::Token::requires_bus_access);
    attrs[attrName] = EntityAttribute { attrName, entityAttr.value(), entityAttr.type() };

    setEntityAttributes(attrs);
    if (title().isEmpty()) {
        setTitle(portName());
    }
}

DVDevice::DVDevice(DVObject *parent)
    : DVObject(DVObject::Type::Device, {}, parent)
{
    connect(this, &shared::VEObject::attributeChanged, this, [this](const QString &name) {
        if (name == meta::Props::token(meta::Props::Token::port)) {
            Q_EMIT portChanged(this->portName());
        }
    });
    connect(this, &shared::VEObject::attributeChanged, this, [this](const QString &name) {
        if (name == meta::Props::token(meta::Props::Token::requires_bus_access)) {
            Q_EMIT qualifierChanged(this->qualifier());
        }
    });
}

DVNode *DVDevice::node() const
{
    return qobject_cast<DVNode *>(parent());
}

QString DVDevice::portName() const
{
    return entityAttributeValue(meta::Props::token(meta::Props::Token::port)).toString();
}

void DVDevice::setPortName(const QString &name)
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::port), name);
}

QString DVDevice::qualifier() const
{
    return entityAttributeValue(meta::Props::token(meta::Props::Token::requires_bus_access)).toString();
}

void DVDevice::setQualifier(const QString &name)
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::requires_bus_access), name);
}

} // namespace dvm
