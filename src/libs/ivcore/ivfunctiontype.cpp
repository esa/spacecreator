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

#include "ivfunctiontype.h"

#include "ivcomment.h"
#include "ivcommonprops.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivinterfacegroup.h"

#include <QDebug>

namespace ivm {

struct IVFunctionTypePrivate {
    QVector<IVObject *> m_children {};
    QVector<ContextParameter> m_contextParams {};
    QVector<QPointer<IVFunction>> m_instances {};

    QVector<IVFunctionType *> m_functionTypes;
    QVector<IVFunction *> m_functions;
    QVector<IVInterface *> m_pis;
    QVector<IVInterface *> m_ris;
    QVector<IVInterfaceGroup *> m_ifaceGroups;
    QVector<IVComment *> m_comments;
    QVector<IVConnection *> m_connections;
    QVector<IVConnectionGroup *> m_connectionGroups;
};

IVFunctionType::IVFunctionType(const QString &title, QObject *parent, const shared::Id &id)
    : IVFunctionType(IVObject::Type::FunctionType, title, parent, id)
{
}

IVFunctionType::IVFunctionType(const IVObject::Type t, const QString &title, QObject *parent, const shared::Id &id)
    : IVObject(t, title, parent, id)
    , d(new IVFunctionTypePrivate)
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::is_type), QStringLiteral("YES"));
}

IVFunctionType::~IVFunctionType() { }

QVector<IVObject *> IVFunctionType::children() const
{
    return d->m_children;
}

bool IVFunctionType::addChild(IVObject *child)
{
    if (child && !d->m_children.contains(child)) {
        child->setParentObject(this);
        d->m_children.append(child);

        const IVObject::Type t = child->type();
        switch (t) {
        case IVObject::Type::FunctionType: {
            d->m_functionTypes.append(child->as<IVFunctionType *>());
            break;
        }
        case IVObject::Type::Function: {
            d->m_functionTypes.append(child->as<IVFunction *>());
            break;
        }
        case IVObject::Type::InterfaceGroup: {
            d->m_ifaceGroups.append(child->as<IVInterfaceGroup *>());
            break;
        }
        case IVObject::Type::ProvidedInterface: {
            d->m_pis.append(child->as<IVInterfaceProvided *>());
            break;
        }
        case IVObject::Type::RequiredInterface: {
            d->m_ris.append(child->as<IVInterfaceRequired *>());
            break;
        }
        case IVObject::Type::Comment: {
            d->m_comments.append(child->as<IVComment *>());
            break;
        }
        case IVObject::Type::ConnectionGroup: {
            d->m_connectionGroups.append(child->as<IVConnectionGroup *>());
            break;
        }
        case IVObject::Type::Connection: {
            d->m_connections.append(child->as<IVConnection *>());
            break;
        }
        default: {
            qWarning() << "attempt to reg unsupported Function child:" << t;
            return false;
        }
        }

        Q_EMIT childAdded(child->id());
        return true;
    }

    return false;
}

bool IVFunctionType::removeChild(IVObject *child)
{
    int id = d->m_children.indexOf(child);
    if (id >= 0 && id < d->m_children.size()) {
        child->setParentObject(nullptr);
        d->m_children.remove(id);

        const IVObject::Type t = child->type();
        switch (t) {
        case IVObject::Type::FunctionType: {
            d->m_functionTypes.removeAll(child->as<IVFunctionType *>());
            break;
        }
        case IVObject::Type::Function: {
            d->m_functionTypes.removeAll(child->as<IVFunction *>());
            break;
        }
        case IVObject::Type::InterfaceGroup: {
            d->m_ifaceGroups.removeAll(child->as<IVInterfaceGroup *>());
            break;
        }
        case IVObject::Type::RequiredInterface: {
            d->m_ris.removeAll(child->as<IVInterfaceRequired *>());
            break;
        }
        case IVObject::Type::ProvidedInterface: {
            d->m_pis.removeAll(child->as<IVInterfaceProvided *>());
            break;
        }
        case IVObject::Type::Comment: {
            d->m_comments.removeAll(child->as<IVComment *>());
            break;
        }
        case IVObject::Type::ConnectionGroup: {
            d->m_connectionGroups.removeAll(child->as<IVConnectionGroup *>());
            break;
        }
        case IVObject::Type::Connection: {
            d->m_connections.removeAll(child->as<IVConnection *>());
            break;
        }
        default: {
            qWarning() << "attempt to unreg unsupported Function child:" << t;
            return false;
        }
        }

        Q_EMIT childRemoved(child->id());
        return true;
    }

    return false;
}

QVector<IVFunctionType *> IVFunctionType::functionTypes() const
{
    return d->m_functionTypes;
}

QVector<IVFunction *> IVFunctionType::functions() const
{
    return d->m_functions;
}

QVector<IVConnection *> IVFunctionType::connections() const
{
    return d->m_connections;
}

QVector<IVConnectionGroup *> IVFunctionType::connectionGroups() const
{
    return d->m_connectionGroups;
}

QVector<IVComment *> IVFunctionType::comments() const
{
    return d->m_comments;
}

QVector<IVInterface *> IVFunctionType::interfaces() const
{
    QVector<IVInterface *> result;

    for (auto i : qAsConst(d->m_pis))
        result.append(i->as<IVInterface *>());

    for (auto i : qAsConst(d->m_ris))
        result.append(i->as<IVInterface *>());

    return result;
}

QVector<IVInterface *> IVFunctionType::allInterfaces() const
{
    QVector<IVInterface *> result;

    for (auto i : qAsConst(d->m_pis))
        result.append(i->as<IVInterface *>());

    for (auto i : qAsConst(d->m_ris))
        result.append(i->as<IVInterface *>());

    for (auto i : qAsConst(d->m_ifaceGroups))
        result.append(i->as<IVInterface *>());

    return result;
}

QVector<IVInterface *> IVFunctionType::ris() const
{
    return d->m_ris;
}

QVector<IVInterface *> IVFunctionType::pis() const
{
    return d->m_pis;
}

QVector<IVInterfaceGroup *> IVFunctionType::interfaceGroups() const
{
    return d->m_ifaceGroups;
}

bool IVFunctionType::hasNestedChildren() const
{
    return functionTypes().size() || functions().size() || comments().size();
}

/*!
   Returns ig the functions has an interface with the given \p name.
   \note The interface type is not relevant
 */
bool IVFunctionType::hasInterface(const QString &name, Qt::CaseSensitivity caseSensitivity) const
{
    for (auto i : qAsConst(d->m_pis)) {
        if (name.compare(i->title(), caseSensitivity) == 0) {
            return true;
        }
    }
    for (auto i : qAsConst(d->m_ris)) {
        if (name.compare(i->title(), caseSensitivity) == 0) {
            return true;
        }
    }

    return false;
}

QVector<ContextParameter> IVFunctionType::contextParams() const
{
    return d->m_contextParams;
}

ContextParameter IVFunctionType::contextParam(const QString &name) const
{
    if (!name.isEmpty())
        for (const ContextParameter &param : contextParams())
            if (param.name() == name)
                return param;
    return {};
}

void IVFunctionType::addContextParam(const ContextParameter &param)
{
    if (!d->m_contextParams.contains(param)) {
        d->m_contextParams.append(param);
        Q_EMIT contextParamsChanged();
    }
}

bool IVFunctionType::removeContextParam(const ContextParameter &param)
{
    const bool removed = d->m_contextParams.removeOne(param);
    if (removed)
        Q_EMIT contextParamsChanged();
    return removed;
}

void IVFunctionType::clearContextParams()
{
    const int prevSize = d->m_contextParams.size();
    d->m_contextParams.clear();
    const int newSize = d->m_contextParams.size();

    if (prevSize != newSize)
        Q_EMIT contextParamsChanged();
}

void IVFunctionType::setContextParams(const QVector<ContextParameter> &params)
{
    if (d->m_contextParams != params) {
        d->m_contextParams = params;
        Q_EMIT contextParamsChanged();
    }
}

QVector<QPointer<IVFunction>> IVFunctionType::instances() const
{
    return d->m_instances;
}

void IVFunctionType::rememberInstance(IVFunction *function)
{
    if (function && !instances().contains(function))
        d->m_instances.append(function);
}

void IVFunctionType::forgetInstance(IVFunction *function)
{
    if (function)
        d->m_instances.removeAll(function);
}

}
