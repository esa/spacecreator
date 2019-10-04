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
#include "aadlcommonprops.h"

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
    if (child && !d->m_children.contains(child)) {
        child->setParentObject(this);
        d->m_children.append(child);
        return true;
    }

    return false;
}

bool AADLObjectContainer::removeChild(AADLObject *child)
{
    int id = d->m_children.indexOf(child);
    if (id >= 0 && id < d->m_children.size()) {
        child->setParentObject(nullptr);
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
        ri->setParentObject(this);
        d->m_ris.append(ri);
        return true;
    }

    return false;
}

bool AADLObjectContainer::removeRI(AADLObjectIface *ri)
{
    int id = ris().indexOf(ri);
    if (id >= 0 && id < ris().size()) {
        ri->setParentObject(nullptr);
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
        pi->setParentObject(this);
        d->m_pis.append(pi);
        return true;
    }

    return false;
}

bool AADLObjectContainer::removePI(AADLObjectIface *pi)
{
    int id = pis().indexOf(pi);
    if (id >= 0 && id < pis().size()) {
        pi->setParentObject(nullptr);
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

QString AADLObjectContainer::language() const
{
    return attr(meta::token(meta::Token::language)).toString();
}

void AADLObjectContainer::setLanguage(const QString &lang)
{
    if (language() != lang)
        setAttr(meta::token(meta::Token::language), lang);
}

QString AADLObjectContainer::instanceOf() const
{
    return attr(meta::token(meta::Token::instance_of)).toString();
}

void AADLObjectContainer::setInstanceOf(const QString &instance)
{
    if (instanceOf() != instance)
        setAttr(meta::token(meta::Token::instance_of), instance);
}

QStringList AADLObjectContainer::activeInterfaces() const
{
    return prop(meta::token(meta::Token::Active_Interfaces)).toStringList();
}

void AADLObjectContainer::setActiveInterfaces(const QStringList &ifaces)
{
    if (activeInterfaces() != ifaces)
        setProp(meta::token(meta::Token::Active_Interfaces), ifaces);
}

} // ns aadl
} // ns taste3
