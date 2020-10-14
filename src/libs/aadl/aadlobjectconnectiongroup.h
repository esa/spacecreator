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

#include "aadlobjectconnection.h"
#include "aadlobjectiface.h"

namespace aadl {
class AADLObjectIfaceGroup;

class AADLObjectConnectionGroup : public AADLObjectConnection
{
    Q_OBJECT
public:
    explicit AADLObjectConnectionGroup(const QString &name, AADLObjectIfaceGroup *ifaceSource,
            AADLObjectIfaceGroup *ifaceTarget, const QList<QPointer<AADLObjectConnection>> &connections = {},
            QObject *parent = nullptr);

    struct CreationInfo {
        QPointer<AADLObjectsModel> model;
        QPointer<AADLObject> parentObject;
        QPointer<AADLObject> sourceObject;
        QPointer<AADLObject> targetObject;

        QString name;
        QList<QPointer<AADLObjectConnection>> connections;
        QList<QPointF> points;

        QVariantList toVarList() const;
    };

    bool postInit() override;

    void addConnection(const QPointer<AADLObjectConnection> &connection);
    void removeConnection(const QPointer<AADLObjectConnection> &connection);

    void initConnections(const QList<shared::Id> &ids) { m_initConnections = ids; }

    QList<QPointer<AADLObjectConnection>> groupedConnections() const;
    QList<QPointer<AADLObjectIface>> groupedSourceInterfaces() const;
    QList<QPointer<AADLObjectIface>> groupedTargetInterfaces() const;

    QList<AADLObjectIface *> sourceFunctionInterfaces() const;
    QList<AADLObjectIface *> targetFunctionInterfaces() const;

    AADLObjectIfaceGroup *sourceInterfaceGroup() const;
    AADLObjectIfaceGroup *targetInterfaceGroup() const;

Q_SIGNALS:
    void connectionAdded(AADLObjectConnection *connection);
    void connectionRemoved(AADLObjectConnection *connection);

private:
    QList<QPointer<AADLObjectConnection>> m_connections;
    QList<shared::Id> m_initConnections;
};

} // namespace aadl

Q_DECLARE_METATYPE(aadl::AADLObjectConnectionGroup::CreationInfo);
