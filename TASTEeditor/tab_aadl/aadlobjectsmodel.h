/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlobjectcontainer.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"

#include <QObject>
#include <QVector>
#include <memory>

namespace taste3 {
namespace aadl {

struct AADLObjectsModelPrivate;
class AADLObjectsModel : public QObject
{
    Q_OBJECT
public:
    explicit AADLObjectsModel(QObject *parent = nullptr);
    ~AADLObjectsModel() override;

    AADLContainersVector containers() const;
    AADLFunctionsVector functions() const;
    AADLIfacesVector ifaces() const;

    bool addFunctionType(AADLObjectContainer *container);
    bool removeFunctionType(AADLObjectContainer *container);

    bool addFunction(AADLObjectFunction *function);
    bool removeFunction(AADLObjectFunction *function);

    bool addIface(AADLObjectIface *iface);
    bool removeIface(AADLObjectIface *iface);

    bool addObject(AADLObject *obj);
    bool removeObject(AADLObject *obj);

signals:
    void functionTypeAdded(AADLObjectContainer *container);
    void functionTypeRemoved(AADLObjectContainer *container);

    void functionAdded(AADLObjectFunction *function);
    void functionRemoved(AADLObjectFunction *function);

    void ifaceAdded(AADLObjectIface *iface);
    void ifaceRemoved(AADLObjectIface *iface);

    void aadlObjectAdded(AADLObject *object);
    void aadlObjectRemoved(AADLObject *object);

private:
    const std::unique_ptr<AADLObjectsModelPrivate> d;
    void notifyObjectAdded(AADLObject *object);
    void notifyObjectRemoved(AADLObject *object);
};

} // ns aadl
} // ns taste3
