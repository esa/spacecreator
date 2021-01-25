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

#include "aadlfunctiontype.h"

#include "aadlcomment.h"
#include "aadlcommonprops.h"
#include "aadlconnection.h"
#include "aadlconnectiongroup.h"
#include "aadlfunction.h"
#include "aadliface.h"
#include "aadlifacegroup.h"

#include <QDebug>

namespace ivm {

struct AADLFunctionTypePrivate {
    QVector<AADLObject *> m_children {};
    QVector<ContextParameter> m_contextParams {};
    QVector<QPointer<AADLFunction>> m_instances {};

    QVector<AADLFunctionType *> m_functionTypes;
    QVector<AADLFunction *> m_functions;
    QVector<AADLIface *> m_pis;
    QVector<AADLIface *> m_ris;
    QVector<AADLIfaceGroup *> m_ifaceGroups;
    QVector<AADLComment *> m_comments;
    QVector<AADLConnection *> m_connections;
    QVector<AADLConnectionGroup *> m_connectionGroups;
};

AADLFunctionType::AADLFunctionType(const QString &title, QObject *parent)
    : AADLFunctionType(AADLObject::Type::FunctionType, title, parent)
{
}

AADLFunctionType::AADLFunctionType(const AADLObject::Type t, const QString &title, QObject *parent)
    : AADLObject(t, title, parent)
    , d(new AADLFunctionTypePrivate)
{
    setAttr(meta::Props::token(meta::Props::Token::language), QVariant());
    setAttr(meta::Props::token(meta::Props::Token::is_type), QStringLiteral("YES"));
}

AADLFunctionType::~AADLFunctionType() { }

QVector<AADLObject *> AADLFunctionType::children() const
{
    return d->m_children;
}

bool AADLFunctionType::addChild(AADLObject *child)
{
    if (child && !d->m_children.contains(child)) {
        child->setParentObject(this);
        d->m_children.append(child);

        const AADLObject::Type t = child->aadlType();
        switch (t) {
        case AADLObject::Type::FunctionType: {
            d->m_functionTypes.append(child->as<AADLFunctionType *>());
            break;
        }
        case AADLObject::Type::Function: {
            d->m_functionTypes.append(child->as<AADLFunction *>());
            break;
        }
        case AADLObject::Type::InterfaceGroup: {
            d->m_ifaceGroups.append(child->as<AADLIfaceGroup *>());
            break;
        }
        case AADLObject::Type::ProvidedInterface: {
            d->m_pis.append(child->as<AADLIfaceProvided *>());
            break;
        }
        case AADLObject::Type::RequiredInterface: {
            d->m_ris.append(child->as<AADLIfaceRequired *>());
            break;
        }
        case AADLObject::Type::Comment: {
            d->m_comments.append(child->as<AADLComment *>());
            break;
        }
        case AADLObject::Type::ConnectionGroup: {
            d->m_connectionGroups.append(child->as<AADLConnectionGroup *>());
            break;
        }
        case AADLObject::Type::Connection: {
            d->m_connections.append(child->as<AADLConnection *>());
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

bool AADLFunctionType::removeChild(AADLObject *child)
{
    int id = d->m_children.indexOf(child);
    if (id >= 0 && id < d->m_children.size()) {
        child->setParentObject(nullptr);
        d->m_children.remove(id);

        const AADLObject::Type t = child->aadlType();
        switch (t) {
        case AADLObject::Type::FunctionType: {
            d->m_functionTypes.removeAll(child->as<AADLFunctionType *>());
            break;
        }
        case AADLObject::Type::Function: {
            d->m_functionTypes.removeAll(child->as<AADLFunction *>());
            break;
        }
        case AADLObject::Type::InterfaceGroup: {
            d->m_ifaceGroups.removeAll(child->as<AADLIfaceGroup *>());
            break;
        }
        case AADLObject::Type::RequiredInterface: {
            d->m_ris.removeAll(child->as<AADLIfaceRequired *>());
            break;
        }
        case AADLObject::Type::ProvidedInterface: {
            d->m_pis.removeAll(child->as<AADLIfaceProvided *>());
            break;
        }
        case AADLObject::Type::Comment: {
            d->m_comments.removeAll(child->as<AADLComment *>());
            break;
        }
        case AADLObject::Type::ConnectionGroup: {
            d->m_connectionGroups.removeAll(child->as<AADLConnectionGroup *>());
            break;
        }
        case AADLObject::Type::Connection: {
            d->m_connections.removeAll(child->as<AADLConnection *>());
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

QVector<AADLFunctionType *> AADLFunctionType::functionTypes() const
{
    return d->m_functionTypes;
}

QVector<AADLFunction *> AADLFunctionType::functions() const
{
    return d->m_functions;
}

QVector<AADLConnection *> AADLFunctionType::connections() const
{
    return d->m_connections;
}

QVector<AADLConnectionGroup *> AADLFunctionType::connectionGroups() const
{
    return d->m_connectionGroups;
}

QVector<AADLComment *> AADLFunctionType::comments() const
{
    return d->m_comments;
}

QVector<AADLIface *> AADLFunctionType::interfaces() const
{
    QVector<AADLIface *> result;

    for (auto i : d->m_pis)
        result.append(i->as<AADLIface *>());

    for (auto i : d->m_ris)
        result.append(i->as<AADLIface *>());

    return result;
}

QVector<AADLIface *> AADLFunctionType::allInterfaces() const
{
    QVector<AADLIface *> result;

    for (auto i : d->m_pis)
        result.append(i->as<AADLIface *>());

    for (auto i : d->m_ris)
        result.append(i->as<AADLIface *>());

    for (auto i : d->m_ifaceGroups)
        result.append(i->as<AADLIface *>());

    return result;
}

QVector<AADLIface *> AADLFunctionType::ris() const
{
    return d->m_ris;
}

QVector<AADLIface *> AADLFunctionType::pis() const
{
    return d->m_pis;
}

QVector<AADLIfaceGroup *> AADLFunctionType::interfaceGroups() const
{
    return d->m_ifaceGroups;
}

bool AADLFunctionType::hasNestedChildren() const
{
    return functionTypes().size() || functions().size() || comments().size();
}

/*!
   Returns ig the functions has an interface with the given \p name.
   \note The interface type is not relevant
 */
bool AADLFunctionType::hasInterface(const QString &name, Qt::CaseSensitivity caseSensitivity) const
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

QVector<ContextParameter> AADLFunctionType::contextParams() const
{
    return d->m_contextParams;
}

ContextParameter AADLFunctionType::contextParam(const QString &name) const
{
    if (!name.isEmpty())
        for (const ContextParameter &param : contextParams())
            if (param.name() == name)
                return param;
    return {};
}

void AADLFunctionType::addContextParam(const ContextParameter &param)
{
    if (!d->m_contextParams.contains(param)) {
        d->m_contextParams.append(param);
        Q_EMIT contextParamsChanged();
    }
}

bool AADLFunctionType::removeContextParam(const ContextParameter &param)
{
    const bool removed = d->m_contextParams.removeOne(param);
    if (removed)
        Q_EMIT contextParamsChanged();
    return removed;
}

void AADLFunctionType::clearContextParams()
{
    const int prevSize = d->m_contextParams.size();
    d->m_contextParams.clear();
    const int newSize = d->m_contextParams.size();

    if (prevSize != newSize)
        Q_EMIT contextParamsChanged();
}

void AADLFunctionType::setContextParams(const QVector<ContextParameter> &params)
{
    if (d->m_contextParams != params) {
        d->m_contextParams = params;
        Q_EMIT contextParamsChanged();
    }
}

QVector<QPointer<AADLFunction>> AADLFunctionType::instances() const
{
    return d->m_instances;
}

void AADLFunctionType::rememberInstance(AADLFunction *function)
{
    if (function && !instances().contains(function))
        d->m_instances.append(function);
}

void AADLFunctionType::forgetInstance(AADLFunction *function)
{
    if (function)
        d->m_instances.removeAll(function);
}

}
