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

#pragma once

#include "dvdevice.h"
#include "dvmessage.h"
#include "dvnode.h"
#include "dvobject.h"

#include <QList>
#include <QPointer>
#include <memory>

namespace dvm {
struct DVConnectionPrivate;

class DVConnection : public DVObject
{
    Q_OBJECT
    Q_PROPERTY(dvm::DVDevice *sourceDevice READ sourceDevice)
    Q_PROPERTY(dvm::DVDevice *targetDevice READ targetDevice)
    Q_PROPERTY(dvm::DVNode *sourceNode READ sourceNode)
    Q_PROPERTY(dvm::DVNode *targetNode READ targetNode)
    Q_PROPERTY(QList<dvm::DVMessage *> messages READ messages)

public:
    explicit DVConnection(DVObject *parent = nullptr, const shared::Id &id = shared::InvalidId);
    explicit DVConnection(DVDevice *sourceDev, DVDevice *targetDev, DVObject *parent = nullptr,
            const shared::Id &id = shared::InvalidId);
    ~DVConnection() override;

    bool postInit() override;
    QString titleUI() const override;

    DVDevice *sourceDevice() const;
    DVDevice *targetDevice() const;

    DVNode *sourceNode() const;
    DVNode *targetNode() const;

    bool isConnected(DVConnection *other) const;

    QList<DVMessage *> messages() const;

    bool hasMessage(const QString &fromFunction, const QString &fromInterface, const QString &toFunction,
            const QString &toInterface) const;
    DVMessage *message(const QString &fromFunction, const QString &fromInterface, const QString &toFunction,
            const QString &toInterface) const;

    DVDevice *getDevice(const QString &nodeName, const QString &portName, const QString &busName);
    void updateAttributeFromEntity(dvm::DVObject *object, const meta::Props::Token token);

private:
    std::unique_ptr<DVConnectionPrivate> d;
};

} // namespace dvm

Q_DECLARE_METATYPE(dvm::DVConnection *)
