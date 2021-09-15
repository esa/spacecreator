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

#include "dvconnection.h"

#include "connectionvalidator.h"
#include "dvdevice.h"
#include "dvmessage.h"
#include "dvmodel.h"
#include "dvnode.h"
#include "errorhub.h"

#include <QDebug>
#include <QPointer>

namespace dvm {
struct DVConnectionPrivate {
    QPointer<DVDevice> sourceDevice;
    QPointer<DVDevice> targetDevice;
};

DVConnection::DVConnection(DVDevice *sourceDev, DVDevice *targetDev, DVObject *parent)
    : DVConnection(parent)
{
    if (sourceDev && sourceDev->node()) {
        setEntityAttribute(meta::Props::token(meta::Props::Token::from_node), sourceDev->node()->title());
        setEntityAttribute(meta::Props::token(meta::Props::Token::from_port), sourceDev->portName());
    }
    if (targetDev && targetDev->node()) {
        setEntityAttribute(meta::Props::token(meta::Props::Token::to_node), targetDev->node()->title());
        setEntityAttribute(meta::Props::token(meta::Props::Token::to_port), targetDev->portName());
    }
    if (sourceDev && targetDev && sourceDev->qualifier() == targetDev->qualifier()) {
        setEntityAttribute(meta::Props::token(meta::Props::Token::to_bus), targetDev->qualifier());
    }
}

DVConnection::DVConnection(DVObject *parent)
    : DVObject(DVObject::Type::Connection, {}, parent)
    , d(std::make_unique<DVConnectionPrivate>())
{
}

DVConnection::~DVConnection() { }

bool DVConnection::postInit()
{
    if (!model()) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Connection can't init without model set"));
        return false;
    }
    const QString busName = entityAttributeValue(meta::Props::token(meta::Props::Token::to_bus)).toString();

    const QString sourceNodeName = entityAttributeValue(meta::Props::token(meta::Props::Token::from_node)).toString();
    const QString sourcePortName = entityAttributeValue(meta::Props::token(meta::Props::Token::from_port)).toString();
    d->sourceDevice = getDevice(sourceNodeName, sourcePortName, busName);

    const QString targetNodeName = entityAttributeValue(meta::Props::token(meta::Props::Token::to_node)).toString();
    const QString targetPortName = entityAttributeValue(meta::Props::token(meta::Props::Token::to_port)).toString();
    d->targetDevice = getDevice(targetNodeName, targetPortName, busName);

    if (!d->sourceDevice || !d->targetDevice) {
        return false;
    }

    connect(d->sourceDevice->node(), &DVNode::titleChanged, this,
            [this]() { updateAttributeFromEntity(qobject_cast<DVObject *>(sender()), meta::Props::Token::from_node); });
    connect(d->sourceDevice, &DVNode::titleChanged, this,
            [this]() { updateAttributeFromEntity(qobject_cast<DVObject *>(sender()), meta::Props::Token::from_port); });
    connect(d->targetDevice->node(), &DVNode::titleChanged, this,
            [this]() { updateAttributeFromEntity(qobject_cast<DVObject *>(sender()), meta::Props::Token::to_node); });
    connect(d->targetDevice, &DVNode::titleChanged, this,
            [this]() { updateAttributeFromEntity(qobject_cast<DVObject *>(sender()), meta::Props::Token::to_port); });

    ConnectionValidator::FailReason valid = ConnectionValidator::check(this);
    if (valid != ConnectionValidator::FailReason::NotFail) {
        switch (valid) {
        case ConnectionValidator::FailReason::ParamsDiffer:
            shared::ErrorHub::addError(shared::ErrorItem::Error,
                    tr("%1 parameter differ for %2 and %3")
                            .arg(title(), d->sourceDevice->title(), d->targetDevice->title()));
            break;
        default:
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Connection %1 can't init").arg(title()));
        }
        return false;
    }
    return true;
}

QString DVConnection::titleUI() const
{
    QStringList parts {
        entityAttributeValue(meta::Props::token(meta::Props::Token::from_node)).toString(),
        entityAttributeValue(meta::Props::token(meta::Props::Token::from_port)).toString(),
        entityAttributeValue(meta::Props::token(meta::Props::Token::to_node)).toString(),
        entityAttributeValue(meta::Props::token(meta::Props::Token::to_port)).toString(),
    };
    return parts.join(QLatin1Char('_'));
}

DVDevice *DVConnection::sourceDevice() const
{
    return d->sourceDevice;
}

DVDevice *DVConnection::targetDevice() const
{
    return d->targetDevice;
}

DVNode *DVConnection::sourceNode() const
{
    return d->sourceDevice ? qobject_cast<DVNode *>(d->sourceDevice->parent()) : nullptr;
}

DVNode *DVConnection::targetNode() const
{
    return d->targetDevice ? qobject_cast<DVNode *>(d->targetDevice->parent()) : nullptr;
}

/*!
   Returns true if the given \p other connection shares at least one device with this connection.
 */
bool DVConnection::isConnected(DVConnection *other) const
{
    return d->sourceDevice == other->sourceDevice() || d->sourceDevice == other->targetDevice()
            || d->targetDevice == other->sourceDevice() || d->targetDevice == other->targetDevice();
}

QList<dvm::DVMessage *> DVConnection::messages() const
{
    return findChildren<dvm::DVMessage *>();
}

bool DVConnection::hasMessage(const QString &fromFunction, const QString &fromInterface, const QString &toFunction,
        const QString &toInterface) const
{
    return message(fromFunction, fromInterface, toFunction, toInterface) != nullptr;
}

DVMessage *DVConnection::message(const QString &fromFunction, const QString &fromInterface, const QString &toFunction,
        const QString &toInterface) const
{
    for (dvm::DVMessage *message : messages()) {
        if (message->fromFunction() == fromFunction && message->fromInterface() == fromInterface
                && message->toFunction() == toFunction && message->toInterface() == toInterface) {
            return message;
        }
    }
    return {};
}

DVDevice *DVConnection::getDevice(const QString &nodeName, const QString &portName, const QString &busName)
{
    if (nodeName.isEmpty()) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Malformed data, empty source node"));
        return nullptr;
    }
    if (portName.isEmpty()) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Malformed data, empty source port"));
        return nullptr;
    }

    auto node = qobject_cast<DVNode *>(model()->getObjectByName(nodeName, DVObject::Type::Node, Qt::CaseInsensitive));
    if (!node) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Can't find node with name %1\n").arg(nodeName));
        return nullptr;
    }
    const QList<DVDevice *> nodeDevices = node->devices();
    auto it = std::find_if(nodeDevices.cbegin(), nodeDevices.cend(),
            [portName, busName](DVDevice *dev) { return dev->portName() == portName && dev->qualifier() == busName; });
    if (it == nodeDevices.cend()) {
        shared::ErrorHub::addError(shared::ErrorItem::Error,
                tr("Can't find device with name %1 and bus access %2\n").arg(portName, busName));
        return nullptr;
    }
    return *it;
}

void DVConnection::updateAttributeFromEntity(DVObject *object, const meta::Props::Token token)
{
    if (!object)
        return;

    EntityAttribute attr = entityAttribute(meta::Props::token(token));
    attr.setValue(object->title());
    setEntityAttribute(attr);
}

} // namespace dvm
