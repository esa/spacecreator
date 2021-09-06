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
#include "dvfunction.h"
#include "dvnode.h"
#include "dvobject.h"
#include "dvpartition.h"
#include "errorhub.h"

#include <QDebug>
#include <QFile>
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

QList<DVConnection *> DVModel::connections(DVNode *node) const
{
    QList<DVConnection *> c;
    const QHash<shared::Id, shared::VEObject *> &allObjects = objects();
    for (auto it = allObjects.begin(); it != allObjects.end(); ++it) {
        if (auto connection = qobject_cast<DVConnection *>(it.value())) {
            if (connection->sourceNode() == node || connection->targetNode() == node) {
                c.append(connection);
            }
        }
    }
    return c;
}

/*!
   Returns the connection with the given \p name.
   Return nullptr if no such connection is found.
 */
DVConnection *DVModel::connectionByName(const QString &name) const
{
    QVector<DVConnection *> connections = allObjectsByType<DVConnection>();
    for (DVConnection *connection : connections) {
        if (connection && connection->title() == name) {
            return connection;
        }
    }
    return nullptr;
}

/*!
   Returns a unique connection name not yet used in the model
 */
QString DVModel::newConnectionName() const
{
    QVector<DVConnection *> connections = allObjectsByType<DVConnection>();
    int i = connections.size() + 1;
    QString name = tr("Connection_%1").arg(i);
    while (connectionByName(name) != nullptr) {
        ++i;
        name = tr("Connection_%1").arg(i);
    }
    return name;
}

/*!
   Returns if the given \p device is used. Meaning has a connection bound to it.
 */
bool DVModel::isUsed(const DVDevice *device) const
{
    if (!device) {
        return false;
    }
    const QHash<shared::Id, shared::VEObject *> &allObjects = objects();
    for (auto it = allObjects.begin(); it != allObjects.end(); ++it) {
        if (auto connection = qobject_cast<DVConnection *>(it.value())) {
            if (connection->sourceDevice() == device || connection->targetDevice() == device) {
                return true;
            }
        }
    }
    return false;
}

QList<DVFunction *> DVModel::functions(DVNode *node) const
{
    if (!node) {
        return {};
    }

    QList<DVFunction *> result;
    for (DVPartition *partition : node->partitions()) {
        for (DVFunction *fn : partition->functions()) {
            result.append(fn);
        }
    }
    return result;
}

bool DVModel::addObjectImpl(shared::VEObject *obj)
{
    if (shared::VEModel::addObjectImpl(obj)) {
        if (obj->hasEntityAttribute(dvm::meta::Props::token(dvm::meta::Props::Token::asn1file))) {
            const QString asn1file =
                    obj->entityAttributeValue<QString>(dvm::meta::Props::token(dvm::meta::Props::Token::asn1file));
            if (!QFile::exists(asn1file)) {
                shared::ErrorHub::addError(shared::ErrorItem::Warning, tr("ASN1File doesn't exists: %1").arg(asn1file));
            }
        }
        return true;
    }
    return false;
}

DVObject *DVModel::getObject(const shared::Id &id) const
{
    return qobject_cast<DVObject *>(shared::VEModel::getObject(id));
}

} // namespace dvm
