/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include "ivconnection.h"
#include "ivinterface.h"

namespace ivm {
class IVInterfaceGroup;

class IVConnectionGroup : public IVConnection
{
    Q_OBJECT
public:
    explicit IVConnectionGroup(const QString &name, IVInterfaceGroup *ifaceSource, IVInterfaceGroup *ifaceTarget,
            const QList<QPointer<IVConnection>> &connections = {}, QObject *parent = nullptr);

    struct CreationInfo {
        QPointer<IVModel> model;
        QPointer<IVObject> parentObject;
        QPointer<IVObject> sourceObject;
        QPointer<IVObject> targetObject;

        QString name;
        QList<QPointer<IVConnection>> connections;
        QVector<QPointF> points;

        QVariantList toVarList() const;
    };

    bool postInit() override;
    bool aboutToBeRemoved() override;

    void addConnection(const QPointer<IVConnection> &connection);
    void removeConnection(const QPointer<IVConnection> &connection);

    void initConnections(const QList<shared::Id> &ids) { m_initConnections = ids; }

    QList<QPointer<IVConnection>> groupedConnections() const;
    QList<QPointer<IVInterface>> groupedSourceInterfaces() const;
    QList<QPointer<IVInterface>> groupedTargetInterfaces() const;

    QList<IVInterface *> sourceFunctionInterfaces() const;
    QList<IVInterface *> targetFunctionInterfaces() const;

    IVInterfaceGroup *sourceInterfaceGroup() const;
    IVInterfaceGroup *targetInterfaceGroup() const;

    void addgroupedConnection(const QPointer<IVConnection> &connection);

Q_SIGNALS:
    void connectionAdded(IVConnection *connection);
    void connectionRemoved(IVConnection *connection);

private:
    QList<QPointer<IVConnection>> m_connections;
    QList<shared::Id> m_initConnections;
};

} // namespace ivm

Q_DECLARE_METATYPE(ivm::IVConnectionGroup::CreationInfo);
