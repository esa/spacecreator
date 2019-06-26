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

#include "aadlobjectsmodel.h"

namespace taste3 {
namespace aadl {

struct AADLObjectsModelPrivate {
    AADLContainersVector m_containers;
    AADLFunctionsVector m_functions;
    AADLIfacesVector m_ifaces;
};

AADLObjectsModel::AADLObjectsModel(QObject *parent)
    : QObject(parent)
    , d(new AADLObjectsModelPrivate)
{
}

AADLObjectsModel::~AADLObjectsModel() {}

AADLContainersVector AADLObjectsModel::containers() const
{
    return d->m_containers;
}

AADLFunctionsVector AADLObjectsModel::functions() const
{
    return d->m_functions;
}

AADLIfacesVector AADLObjectsModel::ifaces() const
{
    return d->m_ifaces;
}

bool AADLObjectsModel::addFunctionType(AADLObjectContainer *container)
{
    if (container && !containers().contains(container)) {
        d->m_containers.append(container);
        notifyObjectAdded(container);
        return true;
    }
    return false;
}

bool AADLObjectsModel::removeFunctionType(AADLObjectContainer *container)
{
    const int id = containers().indexOf(container);
    if (id >= 0 && id < containers().size()) {
        d->m_containers.remove(id);
        notifyObjectRemoved(container);
        return true;
    }
    return false;
}

bool AADLObjectsModel::addFunction(AADLObjectFunction *function)
{
    if (function && !functions().contains(function)) {
        d->m_functions.append(function);
        notifyObjectAdded(function);
        return true;
    }
    return false;
}

bool AADLObjectsModel::removeFunction(AADLObjectFunction *function)
{
    const int id = functions().indexOf(function);
    if (id >= 0 && id < functions().size()) {
        d->m_functions.remove(id);
        notifyObjectRemoved(function);
        return true;
    }
    return false;
}

bool AADLObjectsModel::addIface(AADLObjectIface *iface)
{
    if (iface && !ifaces().contains(iface)) {
        d->m_ifaces.append(iface);
        notifyObjectAdded(iface);
        return true;
    }
    return false;
}

bool AADLObjectsModel::removeIface(AADLObjectIface *iface)
{
    const int id = ifaces().indexOf(iface);
    if (id >= 0 && id < ifaces().size()) {
        d->m_ifaces.remove(id);
        notifyObjectRemoved(iface);
        return true;
    }
    return false;
}

bool AADLObjectsModel::addObject(AADLObject *obj)
{
    if (obj) {
        switch (obj->aadlType()) {
        case AADLObject::AADLObjectType::AADLFunctionType:
            return addFunctionType(qobject_cast<AADLObjectContainer *>(obj));
        case AADLObject::AADLObjectType::AADLFunction:
            return addFunction(qobject_cast<AADLObjectFunction *>(obj));
        case AADLObject::AADLObjectType::AADLIface:
            return addIface(qobject_cast<AADLObjectIface *>(obj));
        default:
            break;
        }
    }
    return false;
}

bool AADLObjectsModel::removeObject(AADLObject *obj)
{
    if (obj) {
        switch (obj->aadlType()) {
        case AADLObject::AADLObjectType::AADLFunctionType:
            return removeFunctionType(qobject_cast<AADLObjectContainer *>(obj));
        case AADLObject::AADLObjectType::AADLFunction:
            return removeFunction(qobject_cast<AADLObjectFunction *>(obj));
        case AADLObject::AADLObjectType::AADLIface:
            return removeIface(qobject_cast<AADLObjectIface *>(obj));
        default:
            break;
        }
    }
    return false;
}

void AADLObjectsModel::notifyObjectAdded(AADLObject *obj)
{
    if (obj) {
        switch (obj->aadlType()) {
        case AADLObject::AADLObjectType::AADLFunctionType: {
            emit functionTypeAdded(qobject_cast<AADLObjectContainer *>(obj));
            break;
        }
        case AADLObject::AADLObjectType::AADLFunction: {
            emit functionAdded(qobject_cast<AADLObjectFunction *>(obj));
            break;
        }
        case AADLObject::AADLObjectType::AADLIface: {
            emit ifaceAdded(qobject_cast<AADLObjectIface *>(obj));
            break;
        }
        default:
            return;
        }
        emit aadlObjectAdded(obj);
    }
}

void AADLObjectsModel::notifyObjectRemoved(AADLObject *obj)
{
    if (obj) {
        switch (obj->aadlType()) {
        case AADLObject::AADLObjectType::AADLFunctionType: {
            emit functionTypeRemoved(qobject_cast<AADLObjectContainer *>(obj));
            break;
        }
        case AADLObject::AADLObjectType::AADLFunction: {
            emit functionRemoved(qobject_cast<AADLObjectFunction *>(obj));
            break;
        }
        case AADLObject::AADLObjectType::AADLIface: {
            emit ifaceRemoved(qobject_cast<AADLObjectIface *>(obj));
            break;
        }
        default:
            return;
        }
        emit aadlObjectRemoved(obj);
    }
}

} // ns aadl

} // ns taste3
