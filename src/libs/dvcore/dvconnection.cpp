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
#include "dvmodel.h"

#include <QPointer>

namespace dvm {
struct DVConnectionPrivate {
    QPointer<DVDevice> sourceDevice;
    QPointer<DVDevice> targetDevice;
};

DVConnection::DVConnection(DVObject *parent)
    : DVObject(DVObject::Type::Connection, {}, parent)
    , d(std::make_unique<DVConnectionPrivate>())
{
}

DVConnection::~DVConnection() { }

bool DVConnection::postInit(QString *warning)
{
    if (!model()) {
        if (warning) {
            *warning = tr("Connection can't init without model set");
        }
        return false;
    }

    const QString sourceDeviceName = entityAttributeValue(meta::Props::token(meta::Props::Token::from_port)).toString();
    if (sourceDeviceName.isEmpty()) {
        if (warning) {
            *warning = tr("Malformed data, empty source port");
        }
        return false;
    }
    d->sourceDevice = qobject_cast<DVDevice *>(
            model()->getObjectByName(sourceDeviceName, DVObject::Type::Device, Qt::CaseInsensitive));
    if (!d->sourceDevice && warning) {
        *warning = tr("Can't find device with name %1\n").arg(sourceDeviceName);
    }

    const QString targetDeviceName = entityAttributeValue(meta::Props::token(meta::Props::Token::to_port)).toString();
    if (targetDeviceName.isEmpty()) {
        if (warning) {
            *warning = tr("Malformed data, empty target port");
        }
        return false;
    }
    d->targetDevice = qobject_cast<DVDevice *>(
            model()->getObjectByName(targetDeviceName, DVObject::Type::Device, Qt::CaseInsensitive));
    if (!d->targetDevice && warning) {
        *warning = tr("Can't find device with name %1\n").arg(targetDeviceName);
    }

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

} // namespace deploy
