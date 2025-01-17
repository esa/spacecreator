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

#include "abstractsystemchecks.h"
#include "common.h"
#include "dvbus.h"
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
    connect(this, &DVModel::objectsAdded, this, &DVModel::onObjectsAdded);
    connect(this, &DVModel::objectRemoved, this, &DVModel::onObjectRemoved);
}

DVModel::~DVModel() { }

DVObject *DVModel::getObject(const shared::Id &id) const
{
    return qobject_cast<DVObject *>(shared::VEModel::getObject(id));
}

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

void DVModel::clear()
{
    // Avoid calling resetBuses() again and again
    disconnect(this, &DVModel::objectRemoved, this, &DVModel::onObjectRemoved);
    VEModel::clear();
    connect(this, &DVModel::objectRemoved, this, &DVModel::onObjectRemoved);
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
   A cluster of connections is a group of connections, that a bound together being connected via devices
 */
QList<QList<DVConnection *>> DVModel::connectionClusters() const
{
    QVector<DVConnection *> connections = allObjectsByType<DVConnection>();
    QList<QList<DVConnection *>> clusters;

    auto isConnected = [](DVConnection *connection, const QList<DVConnection *> &cluster) {
        for (DVConnection *c : cluster) {
            if (connection->isConnected(c)) {
                return true;
            }
        }
        return false;
    };

    for (DVConnection *connection : connections) {
        QList<DVConnection *> cluster1;
        int idx = 0;
        for (QList<DVConnection *> &cluster : clusters) {
            if (isConnected(connection, cluster)) {
                if (cluster1.isEmpty()) {
                    cluster.append(connection);
                    cluster1 = cluster;
                } else {
                    cluster1.append(cluster);
                    clusters.removeAt(idx);
                    break;
                }
            }
            ++idx;
        }
        if (cluster1.isEmpty()) {
            cluster1.append(connection);
            clusters.append(cluster1);
        }
    }

    return clusters;
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
    try {
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
    } catch (InconsistentModelException &e) {
        qDebug() << e.what();
    }
    return false;
}

void DVModel::resetBuses()
{
    // Remove old buses
    disconnect(this, &DVModel::objectRemoved, this, &DVModel::onObjectRemoved);
    for (DVBus *bus : allObjectsByType<DVBus>()) {
        removeObject(bus);
    }

    // Add buses
    QList<QList<DVConnection *>> clusters = connectionClusters();
    int idx = 1;
    for (const QList<DVConnection *> &cluster : clusters) {
        auto bus = new DVBus();
        bus->setConnections(cluster);
        bus->setTitle(QString("bus_%1").arg(idx));
        addObject(bus);
        ++idx;
    }
    connect(this, &DVModel::objectRemoved, this, &DVModel::onObjectRemoved);
}

/*!
    Sets the class to query system/IV information
 */
void DVModel::setIVQueries(AbstractSystemChecks *ivQueries)
{
    m_ivQueries = ivQueries;
}

/*!
    Returns the class to query system/IV information
 */
AbstractSystemChecks *DVModel::ivQueries() const
{
    return m_ivQueries;
}

void DVModel::onObjectsAdded(const QVector<shared::Id> &objectsIds)
{
    for (const shared::Id &id : objectsIds) {
        DVObject *obj = getObject(id);
        if (obj && obj->type() == dvm::DVObject::Type::Connection) {
            resetBuses();
            return;
        }
    }
}

void DVModel::onObjectRemoved()
{
    resetBuses();
}

} // namespace dvm
