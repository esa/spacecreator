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

#include "dvmodel.h"

#include "common.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvobject.h"

#include <QDebug>
#include <QHash>

namespace dvm {

DVModel::DVModel(QObject *parent)
    : shared::VEModel(parent)
{
}

DVModel::~DVModel() { }

DVObject *DVModel::getObjectByName(const QString &name, DVObject::Type type, Qt::CaseSensitivity caseSensitivity) const
{
    if (name.isEmpty())
        return nullptr;

    for (auto object : objects()) {
        if (auto obj = qobject_cast<dvm::DVObject *>(object)) {
            if ((type == DVObject::Type::Unknown || type == obj->type())
                    && obj->title().compare(name, caseSensitivity) == 0)
                return obj;
        }
    }
    return nullptr;
}

/*!
   Returns all connections that are connected to teh given \p device
 */
QList<DVConnection *> DVModel::connections(DVDevice *device) const
{
    QList<DVConnection *> c;
    const QHash<shared::Id, shared::VEObject *> &allObjects = objects();
    for (auto it = allObjects.begin(); it != allObjects.end(); ++it) {
        if (auto connection = qobject_cast<DVConnection *>(it.value())) {
            if (connection->sourceDevice() == device || connection->targetDevice() == device) {
                c.append(connection);
            }
        }
    }
    return c;
}

DVObject *DVModel::getObject(const shared::Id &id) const
{
    return qobject_cast<DVObject *>(shared::VEModel::getObject(id));
}

} // namespace dvm
