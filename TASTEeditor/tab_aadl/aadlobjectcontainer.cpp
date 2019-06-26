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

#include "aadlobjectcontainer.h"

namespace taste3 {
namespace aadl {

struct AADLObjectContainerPrivate {
    QVector<AADLObject *> m_children {};
    QVector<AADLObjectIface *> m_ris {};
    QVector<AADLObjectIface *> m_pis {};
};

AADLObjectContainer::AADLObjectContainer(const QString &title, QObject *parent)
    : AADLObject(title, parent)
    , d(new AADLObjectContainerPrivate)
{
}

AADLObjectContainer::~AADLObjectContainer() {}

AADLObject::AADLObjectType AADLObjectContainer::aadlType() const
{
    return AADLObject::AADLObjectType::AADLFunctionContainer;
}

QVector<AADLObject *> AADLObjectContainer::children() const
{
    return d->m_children;
}

bool AADLObjectContainer::addChild(AADLObject *child)
{
    if (child && !children().contains(child)) {
        d->m_children.append(child);
        return true;
    }

    return false;
}

bool AADLObjectContainer::removeChild(AADLObject *child)
{
    int id = children().indexOf(child);
    if (id >= 0 && id < children().size()) {
        d->m_children.remove(id);
        return true;
    }

    return false;
}

QVector<AADLObjectIface *> AADLObjectContainer::ris() const
{
    return d->m_ris;
}

bool AADLObjectContainer::addRI(AADLObjectIface *ri)
{
    if (ri && !ris().contains(ri)) {
        d->m_ris.append(ri);
        return true;
    }

    return false;
}

bool AADLObjectContainer::removeRI(AADLObjectIface *ri)
{
    int id = ris().indexOf(ri);
    if (id >= 0 && id < ris().size()) {
        d->m_ris.remove(id);
        return true;
    }

    return false;
}

QVector<AADLObjectIface *> AADLObjectContainer::pis() const
{
    return d->m_pis;
}

bool AADLObjectContainer::addPI(AADLObjectIface *pi)
{
    if (pi && !pis().contains(pi)) {
        d->m_pis.append(pi);
        return true;
    }

    return false;
}

bool AADLObjectContainer::removePI(AADLObjectIface *pi)
{
    int id = pis().indexOf(pi);
    if (id >= 0 && id < pis().size()) {
        d->m_pis.remove(id);
        return true;
    }

    return false;
}

bool AADLObjectContainer::addInterface(AADLObjectIface *iface)
{
    return iface ? iface->isProvided() ? addPI(iface) : addRI(iface) : false;
}

bool AADLObjectContainer::removeInterface(AADLObjectIface *iface)
{
    return iface ? iface->isProvided() ? removePI(iface) : removeRI(iface) : false;
}

} // ns aadl
} // ns taste3
