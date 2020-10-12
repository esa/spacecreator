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

#include "aadlobjectfunctiontype.h"

#include "aadlcommonprops.h"
#include "aadlobjectcomment.h"
#include "aadlobjectconnection.h"
#include "aadlobjectconnectiongroup.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"
#include "aadlobjectifacegroup.h"

#include <QDebug>

namespace aadl {

struct AADLObjectFunctionTypePrivate {
    QVector<AADLObject *> m_children {};
    QVector<ContextParameter> m_contextParams {};
    QVector<QPointer<AADLObjectFunction>> m_instances {};

    QVector<AADLObjectFunctionType *> m_functionTypes;
    QVector<AADLObjectFunction *> m_functions;
    QVector<AADLObjectIface *> m_pis;
    QVector<AADLObjectIface *> m_ris;
    QVector<AADLObjectIfaceGroup *> m_ifaceGroups;
    QVector<AADLObjectComment *> m_comments;
    QVector<AADLObjectConnection *> m_connections;
    QVector<AADLObjectConnectionGroup *> m_connectionGroups;
};

AADLObjectFunctionType::AADLObjectFunctionType(const QString &title, QObject *parent)
    : AADLObjectFunctionType(AADLObject::Type::FunctionType, title, parent)
{
}

AADLObjectFunctionType::AADLObjectFunctionType(const AADLObject::Type t, const QString &title, QObject *parent)
    : AADLObject(t, title, parent)
    , d(new AADLObjectFunctionTypePrivate)
{
    setAttr(meta::Props::token(meta::Props::Token::language), QVariant());
    setAttr(meta::Props::token(meta::Props::Token::is_type), QStringLiteral("YES"));

    //    if (AADLObjectFunctionType *root = qobject_cast<AADLObjectFunctionType *>(parent))
    //        root->addChild(this);
}

AADLObjectFunctionType::~AADLObjectFunctionType() { }

QVector<AADLObject *> AADLObjectFunctionType::children() const
{
    return d->m_children;
}

bool AADLObjectFunctionType::addChild(AADLObject *child)
{
    if (child && !d->m_children.contains(child)) {
        child->setParentObject(this);
        d->m_children.append(child);

        const AADLObject::Type t = child->aadlType();
        switch (t) {
        case AADLObject::Type::FunctionType: {
            d->m_functionTypes.append(child->as<AADLObjectFunctionType *>());
            break;
        }
        case AADLObject::Type::Function: {
            d->m_functionTypes.append(child->as<AADLObjectFunction *>());
            break;
        }
        case AADLObject::Type::InterfaceGroup: {
            d->m_ifaceGroups.append(child->as<AADLObjectIfaceGroup *>());
            break;
        }
        case AADLObject::Type::ProvidedInterface: {
            d->m_pis.append(child->as<AADLObjectIfaceProvided *>());
            break;
        }
        case AADLObject::Type::RequiredInterface: {
            d->m_ris.append(child->as<AADLObjectIfaceRequired *>());
            break;
        }
        case AADLObject::Type::Comment: {
            d->m_comments.append(child->as<AADLObjectComment *>());
            break;
        }
        case AADLObject::Type::ConnectionGroup: {
            d->m_connectionGroups.append(child->as<AADLObjectConnectionGroup *>());
            break;
        }
        case AADLObject::Type::Connection: {
            d->m_connections.append(child->as<AADLObjectConnection *>());
            break;
        }
        default: {
            qWarning() << "attempt to reg unsupported Function child:" << t;
            break;
        }
        }

        return true;
    }

    return false;
}

bool AADLObjectFunctionType::removeChild(AADLObject *child)
{
    int id = d->m_children.indexOf(child);
    if (id >= 0 && id < d->m_children.size()) {
        child->setParentObject(nullptr);
        d->m_children.remove(id);

        const AADLObject::Type t = child->aadlType();
        switch (t) {
        case AADLObject::Type::FunctionType: {
            d->m_functionTypes.removeAll(child->as<AADLObjectFunctionType *>());
            break;
        }
        case AADLObject::Type::Function: {
            d->m_functionTypes.removeAll(child->as<AADLObjectFunction *>());
            break;
        }
        case AADLObject::Type::InterfaceGroup: {
            d->m_ifaceGroups.removeAll(child->as<AADLObjectIfaceGroup *>());
            break;
        }
        case AADLObject::Type::RequiredInterface: {
            d->m_ris.removeAll(child->as<AADLObjectIfaceRequired *>());
            break;
        }
        case AADLObject::Type::ProvidedInterface: {
            d->m_pis.removeAll(child->as<AADLObjectIfaceProvided *>());
            break;
        }
        case AADLObject::Type::Comment: {
            d->m_comments.removeAll(child->as<AADLObjectComment *>());
            break;
        }
        case AADLObject::Type::ConnectionGroup: {
            d->m_connectionGroups.removeAll(child->as<AADLObjectConnectionGroup *>());
            break;
        }
        case AADLObject::Type::Connection: {
            d->m_connections.removeAll(child->as<AADLObjectConnection *>());
            break;
        }
        default: {
            qWarning() << "attempt to unreg unsupported Function child:" << t;
            break;
        }
        }

        return true;
    }

    return false;
}

QVector<AADLObjectFunctionType *> AADLObjectFunctionType::functionTypes() const
{
    return d->m_functionTypes;
}

QVector<AADLObjectFunction *> AADLObjectFunctionType::functions() const
{
    return d->m_functions;
}

QVector<AADLObjectConnection *> AADLObjectFunctionType::connections() const
{
    return d->m_connections;
}

QVector<AADLObjectConnectionGroup *> AADLObjectFunctionType::connectionGroups() const
{
    return d->m_connectionGroups;
}

QVector<AADLObjectComment *> AADLObjectFunctionType::comments() const
{
    return d->m_comments;
}

QVector<AADLObjectIface *> AADLObjectFunctionType::interfaces() const
{
    QVector<AADLObjectIface *> result;

    for (auto i : d->m_pis)
        result.append(i->as<AADLObjectIface *>());

    for (auto i : d->m_ris)
        result.append(i->as<AADLObjectIface *>());

    for (auto i : d->m_ifaceGroups)
        result.append(i->as<AADLObjectIface *>());

    return result;
}

QVector<AADLObjectIface *> AADLObjectFunctionType::ris() const
{
    return d->m_ris;
}

QVector<AADLObjectIface *> AADLObjectFunctionType::pis() const
{
    return d->m_pis;
}

QVector<AADLObjectIfaceGroup *> AADLObjectFunctionType::interfaceGroups() const
{
    return d->m_ifaceGroups;
}

bool AADLObjectFunctionType::hasNestedChildren() const
{
    return functionTypes().size() || functions().size() || comments().size();
}

QVector<ContextParameter> AADLObjectFunctionType::contextParams() const
{
    return d->m_contextParams;
}

ContextParameter AADLObjectFunctionType::contextParam(const QString &name) const
{
    if (!name.isEmpty())
        for (const ContextParameter &param : contextParams())
            if (param.name() == name)
                return param;
    return {};
}

void AADLObjectFunctionType::addContextParam(const ContextParameter &param)
{
    if (!d->m_contextParams.contains(param)) {
        d->m_contextParams.append(param);
        Q_EMIT contextParamsChanged();
    }
}

bool AADLObjectFunctionType::removeContextParam(const ContextParameter &param)
{
    const bool removed = d->m_contextParams.removeOne(param);
    if (removed)
        Q_EMIT contextParamsChanged();
    return removed;
}

void AADLObjectFunctionType::clearContextParams()
{
    const int prevSize = d->m_contextParams.size();
    d->m_contextParams.clear();
    const int newSize = d->m_contextParams.size();

    if (prevSize != newSize)
        Q_EMIT contextParamsChanged();
}

void AADLObjectFunctionType::setContextParams(const QVector<ContextParameter> &params)
{
    if (d->m_contextParams != params) {
        d->m_contextParams = params;
        Q_EMIT contextParamsChanged();
    }
}

QVector<QPointer<AADLObjectFunction>> AADLObjectFunctionType::instances() const
{
    return d->m_instances;
}

void AADLObjectFunctionType::rememberInstance(AADLObjectFunction *function)
{
    if (function && !instances().contains(function))
        d->m_instances.append(function);
}

void AADLObjectFunctionType::forgetInstance(AADLObjectFunction *function)
{
    if (function)
        d->m_instances.removeAll(function);
}

}
