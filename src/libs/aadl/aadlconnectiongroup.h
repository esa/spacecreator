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

#include "aadlconnection.h"
#include "aadliface.h"

namespace ivm {
class AADLIfaceGroup;

class AADLConnectionGroup : public AADLConnection
{
    Q_OBJECT
public:
    explicit AADLConnectionGroup(const QString &name, AADLIfaceGroup *ifaceSource,
            AADLIfaceGroup *ifaceTarget, const QList<QPointer<AADLConnection>> &connections = {},
            QObject *parent = nullptr);

    struct CreationInfo {
        QPointer<AADLModel> model;
        QPointer<AADLObject> parentObject;
        QPointer<AADLObject> sourceObject;
        QPointer<AADLObject> targetObject;

        QString name;
        QList<QPointer<AADLConnection>> connections;
        QVector<QPointF> points;

        QVariantList toVarList() const;
    };

    bool postInit() override;
    bool aboutToBeRemoved() override;

    void addConnection(const QPointer<AADLConnection> &connection);
    void removeConnection(const QPointer<AADLConnection> &connection);

    void initConnections(const QList<shared::Id> &ids) { m_initConnections = ids; }

    QList<QPointer<AADLConnection>> groupedConnections() const;
    QList<QPointer<AADLIface>> groupedSourceInterfaces() const;
    QList<QPointer<AADLIface>> groupedTargetInterfaces() const;

    QList<AADLIface *> sourceFunctionInterfaces() const;
    QList<AADLIface *> targetFunctionInterfaces() const;

    AADLIfaceGroup *sourceInterfaceGroup() const;
    AADLIfaceGroup *targetInterfaceGroup() const;

Q_SIGNALS:
    void connectionAdded(AADLConnection *connection);
    void connectionRemoved(AADLConnection *connection);

private:
    QList<QPointer<AADLConnection>> m_connections;
    QList<shared::Id> m_initConnections;
};

} // namespace ivm

Q_DECLARE_METATYPE(ivm::AADLConnectionGroup::CreationInfo);
