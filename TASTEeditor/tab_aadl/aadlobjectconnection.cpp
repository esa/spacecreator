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

#include "aadlobjectconnection.h"

#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"
#include "tab_aadl/aadlcommonprops.h"

#include <QDebug>
#include <QPointer>

namespace taste3 {
namespace aadl {

struct AADLObjectConnectionPrivate {
    AADLObjectConnectionPrivate() {}
    AADLObjectConnectionPrivate(AADLObject *source, AADLObject *target, AADLObjectIface *ifaceSource,
                                AADLObjectIface *ifaceTarget)
        : m_source(source)
        , m_target(target)
        , m_ifaceSource(ifaceSource)
        , m_ifaceTarget(ifaceTarget)
    {
    }
    QPointer<AADLObject> m_source { nullptr };
    QPointer<AADLObject> m_target { nullptr };
    QPointer<AADLObjectIface> m_ifaceSource { nullptr };
    QPointer<AADLObjectIface> m_ifaceTarget { nullptr };
};

AADLObjectConnection::AADLObjectConnection(AADLObject *source, AADLObject *target, AADLObjectIface *ifaceSource,
                                           AADLObjectIface *ifaceTarget, QObject *parent)
    : AADLObject(QString(), parent)
    , d(new AADLObjectConnectionPrivate { source, target, ifaceSource, ifaceTarget })
{
    updateAttributes();
}

AADLObjectConnection::AADLObjectConnection(QObject *parent)
    : AADLObject(QString(), parent)
    , d(new AADLObjectConnectionPrivate)
{
    updateAttributes();
}

AADLObjectConnection::~AADLObjectConnection() {}

AADLObject::AADLObjectType AADLObjectConnection::aadlType() const
{
    return AADLObject::AADLObjectType::AADLConnection;
}

AADLObject *AADLObjectConnection::source() const
{
    return d->m_source;
}

void AADLObjectConnection::setSource(AADLObject *source)
{
    setAttr(meta::Props::token(meta::Props::Token::from), source ? source->title() : QString());
}

AADLObject *AADLObjectConnection::target() const
{
    return d->m_target;
}

void AADLObjectConnection::setTarget(AADLObject *target)
{
    setAttr(meta::Props::token(meta::Props::Token::to), target ? target->title() : QString());
}

AADLObjectIface *AADLObjectConnection::sourceInterface() const
{
    return d->m_ifaceSource;
}

void AADLObjectConnection::setSourceInterface(AADLObjectIface *iface)
{
    setAttr(meta::Props::token(meta::Props::Token::si_name), iface->title());
}

QString AADLObjectConnection::sourceInterfaceName() const
{
    return sourceInterface() ? sourceInterface()->title() : QString();
}

AADLObjectIface *AADLObjectConnection::targetInterface() const
{
    return d->m_ifaceTarget;
}

void AADLObjectConnection::setTargetInterface(AADLObjectIface *iface)
{
    setAttr(meta::Props::token(meta::Props::Token::ti_name), iface->title());
}

QString AADLObjectConnection::targetInterfaceName() const
{
    return targetInterface() ? targetInterface()->title() : QString();
}

void AADLObjectConnection::updateAttributes()
{
    AADLObject::setAttr(meta::Props::token(meta::Props::Token::from), source() ? source()->title() : QString());
    AADLObject::setAttr(meta::Props::token(meta::Props::Token::si_name), sourceInterfaceName());
    AADLObject::setAttr(meta::Props::token(meta::Props::Token::to), target() ? target()->title() : QString());
    AADLObject::setAttr(meta::Props::token(meta::Props::Token::ti_name), targetInterfaceName());
}

void AADLObjectConnection::setAttr(const QString &name, const QVariant &val)
{
    bool attrUpdated(false);
    const meta::Props::Token attr = meta::Props::token(name);
    switch (attr) {
    case meta::Props::Token::from: {
        if (auto src = objectsModel()->getObjectByName(val.toString())) {
            if (src != d->m_source) {
                attrUpdated = true;
                d->m_source = src;
            }
        } else {
            qWarning() << "Object not found:" << name << val;
            return;
        }
        break;
    }
    case meta::Props::Token::si_name: {
        if (auto iface = objectsModel()->getIfaceByName(val.toString())) {
            if (iface != d->m_ifaceSource) {
                attrUpdated = true;
                d->m_ifaceSource = iface;
            }
        } else {
            qWarning() << "Object not found:" << name << val;
            return;
        }
        break;
    }
    case meta::Props::Token::to: {
        if (auto dst = objectsModel()->getObjectByName(val.toString())) {
            if (dst != d->m_target) {
                attrUpdated = true;
                d->m_target = dst;
            }
        } else {
            qWarning() << "Object not found:" << name << val;
            return;
        }
        break;
    }
    case meta::Props::Token::ti_name: {
        if (auto iface = objectsModel()->getIfaceByName(val.toString())) {
            if (iface != d->m_ifaceTarget) {
                attrUpdated = true;
                d->m_ifaceTarget = iface;
            }
        } else {
            qWarning() << "Object not found:" << name << val;
            return;
        }
        break;
    }
    case meta::Props::Token::Unknown: {
        qWarning() << "Unknow connection property:" << name << val;
        return;
    }
    default:
        break;
    }

    AADLObject::setAttr(name, val);
    if (attrUpdated)
        updateAttributes();
}

} // ns aadl
} // ns taste3
