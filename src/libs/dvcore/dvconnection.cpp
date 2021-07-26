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

#include "dvdevice.h"
#include "dvmessage.h"
#include "dvmodel.h"
#include "dvnode.h"
#include "errorhub.h"

#include <QPointer>
#include <QtDebug>

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
    if (sourceDev && targetDev && sourceDev->busName() == targetDev->busName()) {
        setEntityAttribute(meta::Props::token(meta::Props::Token::to_bus), targetDev->busName());
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

    auto getDevice = [this](const QString &nodeName, const QString &portName, const QString &busName) -> DVDevice * {
        if (nodeName.isEmpty()) {
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Malformed data, empty source node"));
            return nullptr;
        }
        if (portName.isEmpty()) {
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Malformed data, empty source port"));
            return nullptr;
        }

        auto node =
                qobject_cast<DVNode *>(model()->getObjectByName(nodeName, DVObject::Type::Node, Qt::CaseInsensitive));
        if (!node) {
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Can't find node with name %1\n").arg(nodeName));
            return nullptr;
        }
        const QList<QPointer<DVDevice>> nodeDevices = node->devices();
        auto it = std::find_if(nodeDevices.cbegin(), nodeDevices.cend(), [portName, busName](DVDevice *dev) {
            return dev->portName() == portName && dev->busName() == busName;
        });
        if (it == nodeDevices.cend()) {
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Can't find device with name %1\n").arg(portName));
            return nullptr;
        }
        return *it;
    };

    const QString busName = entityAttributeValue(meta::Props::token(meta::Props::Token::to_bus)).toString();

    const QString sourceNodeName = entityAttributeValue(meta::Props::token(meta::Props::Token::from_node)).toString();
    const QString sourcePortName = entityAttributeValue(meta::Props::token(meta::Props::Token::from_port)).toString();
    d->sourceDevice = getDevice(sourceNodeName, sourcePortName, busName);

    const QString targetNodeName = entityAttributeValue(meta::Props::token(meta::Props::Token::to_node)).toString();
    const QString targetPortName = entityAttributeValue(meta::Props::token(meta::Props::Token::to_port)).toString();
    d->targetDevice = getDevice(targetNodeName, targetPortName, busName);

    return d->sourceDevice && d->targetDevice;
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

QList<dvm::DVMessage *> DVConnection::messages() const
{
    return findChildren<dvm::DVMessage *>();
}

} // namespace dvm
