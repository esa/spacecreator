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
    AADLObjectConnectionPrivate(AADLObject *source, AADLObject *target, AADLObjectIfaceRequired *reqIface,
                                AADLObjectIfaceProvided *provIface)
        : m_source(source)
        , m_target(target)
        , m_ri(reqIface)
        , m_pi(provIface)
    {
    }
    QPointer<AADLObject> m_source { nullptr };
    QPointer<AADLObject> m_target { nullptr };
    QPointer<AADLObjectIfaceRequired> m_ri { nullptr };
    QPointer<AADLObjectIfaceProvided> m_pi { nullptr };
};

AADLObjectConnection::AADLObjectConnection(AADLObject *from, AADLObject *to, AADLObjectIfaceRequired *ri,
                                           AADLObjectIfaceProvided *pi, QObject *parent)
    : AADLObject(QString(), parent)
    , d(new AADLObjectConnectionPrivate { from, to, ri, pi })
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

AADLObject *AADLObjectConnection::target() const
{
    return d->m_target;
}

AADLObjectIfaceRequired *AADLObjectConnection::requiredInterface() const
{
    return d->m_ri;
}

QString AADLObjectConnection::requiredInterfaceName() const
{
    return requiredInterface() ? requiredInterface()->title() : QString();
}

AADLObjectIfaceProvided *AADLObjectConnection::providedInterface() const
{
    return d->m_pi;
}

QString AADLObjectConnection::providedInterfaceName() const
{
    return providedInterface() ? providedInterface()->title() : QString();
}

void AADLObjectConnection::updateAttributes()
{
    AADLObject::setAttr(meta::Props::token(meta::Props::Token::from), source() ? source()->title() : QString());
    AADLObject::setAttr(meta::Props::token(meta::Props::Token::ri_name), requiredInterfaceName());
    AADLObject::setAttr(meta::Props::token(meta::Props::Token::to), target() ? target()->title() : QString());
    AADLObject::setAttr(meta::Props::token(meta::Props::Token::pi_name), providedInterfaceName());
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
    case meta::Props::Token::ri_name: {
        if (auto iface = objectsModel()->getIfaceByName(val.toString(), AADLObjectIface::IfaceType::Required)) {
            if (auto ri = qobject_cast<AADLObjectIfaceRequired *>(iface)) {
                if (ri != d->m_ri) {
                    attrUpdated = true;
                    d->m_ri = ri;
                }
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
    case meta::Props::Token::pi_name: {
        if (auto iface = objectsModel()->getIfaceByName(val.toString(), AADLObjectIface::IfaceType::Provided)) {
            if (auto pi = qobject_cast<AADLObjectIfaceProvided *>(iface)) {
                if (pi != d->m_pi) {
                    attrUpdated = true;
                    d->m_pi = pi;
                }
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
