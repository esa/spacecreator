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
#include "aadlobjectfunction.h"

#include <QDebug>

namespace taste3 {
namespace aadl {

struct AADLObjectFunctionTypePrivate {
    QVector<AADLObject *> m_children {};
    QVector<ContextParameter> m_contextParams {};
    QVector<QPointer<AADLObjectFunction>> m_instances {};

    QVector<AADLObjectFunctionType *> m_functionTypes;
    QVector<AADLObjectFunction *> m_functions;
    QVector<AADLObjectIface *> m_pis;
    QVector<AADLObjectIface *> m_ris;
    QVector<AADLObjectComment *> m_comments;
    QVector<AADLObjectConnection *> m_connections;
};

AADLObjectFunctionType::AADLObjectFunctionType(const QString &title, QObject *parent)
    : AADLObject(AADLObject::Type::FunctionType, title, parent)
    , d(new AADLObjectFunctionTypePrivate)
{
    init();
}

AADLObjectFunctionType::AADLObjectFunctionType(const AADLObject::Type t, const QString &title, QObject *parent)
    : AADLObject(t, title, parent)
    , d(new AADLObjectFunctionTypePrivate)
{
    init();
}

void AADLObjectFunctionType::init()
{
    setAttr(meta::Props::token(meta::Props::Token::language), QVariant());
    setAttr(meta::Props::token(meta::Props::Token::is_type), QStringLiteral("YES"));

    //    if (AADLObjectFunctionType *root = qobject_cast<AADLObjectFunctionType *>(parent))
    //        root->addChild(this);
}

AADLObjectFunctionType::~AADLObjectFunctionType() {}

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
        case AADLObject::Type::Interface: {
            if (AADLObjectIface *iface = child->as<AADLObjectIface *>()) {
                if (iface->isRequired())
                    d->m_ris.append(child->as<AADLObjectIfaceRequired *>());
                else
                    d->m_pis.append(child->as<AADLObjectIfaceProvided *>());
            }
            break;
        }
        case AADLObject::Type::Comment: {
            d->m_comments.append(child->as<AADLObjectComment *>());
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
        case AADLObject::Type::Interface: {
            if (AADLObjectIface *iface = child->as<AADLObjectIface *>()) {
                if (iface->isRequired())
                    d->m_ris.removeAll(child->as<AADLObjectIfaceRequired *>());
                else
                    d->m_pis.removeAll(child->as<AADLObjectIfaceProvided *>());
            }
            break;
        }
        case AADLObject::Type::Comment: {
            d->m_comments.removeAll(child->as<AADLObjectComment *>());
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

QVector<AADLObjectIface *> AADLObjectFunctionType::interfaces() const
{
    QVector<AADLObjectIface *> result;

    for (auto i : d->m_pis)
        result.append(i->as<AADLObjectIface *>());

    for (auto i : d->m_ris)
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

QVariantList AADLObjectFunctionType::templateInterfaces() const
{
    QVariantList ifaceList;

    for (AADLObjectIface *iface : interfaces())
        ifaceList << QVariant::fromValue(iface);

    return ifaceList;
}

QVariantList AADLObjectFunctionType::templateFunctions() const
{
    QVariantList functions;

    for (auto i : d->m_functionTypes)
        functions.append(QVariant::fromValue(i));
    for (auto i : d->m_functions)
        functions.append(QVariant::fromValue(i));

    return functions;
}

QVariantList AADLObjectFunctionType::templateComments() const
{
    QVariantList comments;
    for (const auto child : d->m_comments) {
        comments << QVariant::fromValue(child);
    }
    return comments;
}

QVariantList AADLObjectFunctionType::templateConnections() const
{
    QVariantList connections;
    for (const auto child : d->m_connections) {
        connections << QVariant::fromValue(child);
    }
    return connections;
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
        emit contextParamsChanged();
    }
}

bool AADLObjectFunctionType::removeContextParam(const ContextParameter &param)
{
    const bool removed = d->m_contextParams.removeOne(param);
    if (removed)
        emit contextParamsChanged();
    return removed;
}

void AADLObjectFunctionType::clearContextParams()
{
    const int prevSize = d->m_contextParams.size();
    d->m_contextParams.clear();
    const int newSize = d->m_contextParams.size();

    if (prevSize != newSize)
        emit contextParamsChanged();
}

void AADLObjectFunctionType::setContextParams(const QVector<ContextParameter> &params)
{
    if (d->m_contextParams != params) {
        d->m_contextParams = params;
        emit contextParamsChanged();
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

} // ns aadl
} // ns taste3
