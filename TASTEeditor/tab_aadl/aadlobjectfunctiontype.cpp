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
#include "aadlobjectfunction.h"

namespace taste3 {
namespace aadl {

struct AADLObjectFunctionTypePrivate {
    QVector<AADLObject *> m_children {};
    QVector<AADLObjectIface *> m_ris {};
    QVector<AADLObjectIface *> m_pis {};
    QVector<ContextParameter> m_contextParams {};
    QVector<QPointer<AADLObjectFunction>> m_instances {};
};

AADLObjectFunctionType::AADLObjectFunctionType(const QString &title, QObject *parent)
    : AADLObject(title, parent)
    , d(new AADLObjectFunctionTypePrivate)
{
    setAttr(meta::Props::token(meta::Props::Token::language), QVariant());
    setAttr(meta::Props::token(meta::Props::Token::is_type), QStringLiteral("YES"));

    if (AADLObjectFunctionType *root = qobject_cast<AADLObjectFunctionType *>(parent))
        root->addChild(this);
}

AADLObjectFunctionType::~AADLObjectFunctionType() {}

AADLObject::AADLObjectType AADLObjectFunctionType::aadlType() const
{
    return AADLObject::AADLObjectType::AADLFunctionType;
}

QVector<AADLObject *> AADLObjectFunctionType::children() const
{
    return d->m_children;
}

bool AADLObjectFunctionType::addChild(AADLObject *child)
{
    if (child && !d->m_children.contains(child)) {
        child->setParentObject(this);
        d->m_children.append(child);
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
        return true;
    }

    return false;
}

QVector<AADLObjectIface *> AADLObjectFunctionType::ris() const
{
    return d->m_ris;
}

bool AADLObjectFunctionType::addRI(AADLObjectIface *ri)
{
    if (ri && !ris().contains(ri)) {
        ri->setParentObject(this);
        d->m_ris.append(ri);
        return true;
    }

    return false;
}

bool AADLObjectFunctionType::removeRI(AADLObjectIface *ri)
{
    int id = ris().indexOf(ri);
    if (id >= 0 && id < ris().size()) {
        ri->setParentObject(nullptr);
        d->m_ris.remove(id);
        return true;
    }

    return false;
}

QVector<AADLObjectIface *> AADLObjectFunctionType::pis() const
{
    return d->m_pis;
}

bool AADLObjectFunctionType::addPI(AADLObjectIface *pi)
{
    if (pi && !pis().contains(pi)) {
        pi->setParentObject(this);
        d->m_pis.append(pi);
        return true;
    }

    return false;
}

bool AADLObjectFunctionType::removePI(AADLObjectIface *pi)
{
    int id = pis().indexOf(pi);
    if (id >= 0 && id < pis().size()) {
        pi->setParentObject(nullptr);
        d->m_pis.remove(id);
        return true;
    }

    return false;
}

bool AADLObjectFunctionType::addInterface(AADLObjectIface *iface)
{
    const bool added = iface ? iface->isProvided() ? addPI(iface) : addRI(iface) : false;
    if (added)
        emit ifaceAdded(iface);

    return added;
}

bool AADLObjectFunctionType::removeInterface(AADLObjectIface *iface)
{
    const bool removed = iface ? iface->isProvided() ? removePI(iface) : removeRI(iface) : false;
    if (removed)
        emit ifaceRemoved(iface);

    return removed;
}

QVector<AADLObjectIface *> AADLObjectFunctionType::interfaces() const
{
    return ris() + pis();
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
    for (const auto child : d->m_children) {
        if (child->aadlType() == AADLObject::AADLObjectType::AADLFunction
            || child->aadlType() == AADLObject::AADLObjectType::AADLFunctionType) {
            functions << QVariant::fromValue(child);
        }
    }
    return functions;
}

QVariantList AADLObjectFunctionType::templateComments() const
{
    QVariantList comments;
    for (const auto child : d->m_children) {
        if (child->aadlType() == AADLObject::AADLObjectType::AADLComment)
            comments << QVariant::fromValue(child);
    }
    return comments;
}

QVariantList AADLObjectFunctionType::templateConnections() const
{
    QVariantList connections;
    for (const auto child : d->m_children) {
        if (child->aadlType() == AADLObject::AADLObjectType::AADLConnection)
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
