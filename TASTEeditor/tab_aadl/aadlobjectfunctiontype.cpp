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

namespace taste3 {
namespace aadl {

struct AADLObjectFunctionTypePrivate {
    QVector<AADLObject *> m_children {};
    QVector<AADLObjectIface *> m_ris {};
    QVector<AADLObjectIface *> m_pis {};
    QVector<ContextParameter> m_contextParams {};
};

AADLObjectFunctionType::AADLObjectFunctionType(const QString &title, QObject *parent)
    : AADLObject(title, parent)
    , d(new AADLObjectFunctionTypePrivate)
{
}

AADLObjectFunctionType::~AADLObjectFunctionType() {}

AADLObject::AADLObjectType AADLObjectFunctionType::aadlType() const
{
    return AADLObject::AADLObjectType::AADLFunctionType;
}

QString AADLObjectFunctionType::isType() const
{
    return aadlType() == AADLObjectType::AADLFunctionType ? QStringLiteral("YES") : QStringLiteral("NO");
}

QString AADLObjectFunctionType::instanceOf() const
{
    return QString();
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

QVariantList AADLObjectFunctionType::interfaces() const
{
    QVariantList ifaceList;

    for (AADLObjectIface *iface : ris())
        ifaceList << QVariant::fromValue(iface);

    for (AADLObjectIface *iface : pis())
        ifaceList << QVariant::fromValue(iface);

    return ifaceList;
}

bool AADLObjectFunctionType::addInterface(AADLObjectIface *iface)
{
    return iface ? iface->isProvided() ? addPI(iface) : addRI(iface) : false;
}

bool AADLObjectFunctionType::removeInterface(AADLObjectIface *iface)
{
    return iface ? iface->isProvided() ? removePI(iface) : removeRI(iface) : false;
}

QVariantList AADLObjectFunctionType::functions() const
{
    QVariantList functionList;
    for (const auto child : d->m_children) {
        if (child->aadlType() == AADLObject::AADLObjectType::AADLFunction ||
            child->aadlType() == AADLObject::AADLObjectType::AADLFunctionType) {
            functionList << QVariant::fromValue(child);
        }
    }
    return functionList;
}

QString AADLObjectFunctionType::language() const
{
    return attr(meta::Props::token(meta::Props::Token::language)).toString();
}

void AADLObjectFunctionType::setLanguage(const QString &lang)
{
    if (language() != lang)
        setAttr(meta::Props::token(meta::Props::Token::language), lang);
}

QStringList AADLObjectFunctionType::activeInterfaces() const
{
    return prop(meta::Props::token(meta::Props::Token::Active_Interfaces)).toStringList();
}

void AADLObjectFunctionType::setActiveInterfaces(const QStringList &ifaces)
{
    if (activeInterfaces() != ifaces)
        setProp(meta::Props::token(meta::Props::Token::Active_Interfaces), ifaces);
}

QVector<ContextParameter> AADLObjectFunctionType::contextParams() const
{
    return d->m_contextParams;
}

void AADLObjectFunctionType::addContextParam(const ContextParameter &param)
{
    if (!d->m_contextParams.contains(param))
        d->m_contextParams.append(param);
}

bool AADLObjectFunctionType::removeContextParam(const ContextParameter &param)
{
    return d->m_contextParams.removeOne(param);
}

void AADLObjectFunctionType::clearContextParams()
{
    d->m_contextParams.clear();
}

void AADLObjectFunctionType::setContextParams(const QVector<ContextParameter> &params)
{
    if (d->m_contextParams != params) {
        clearContextParams();
        d->m_contextParams = params;
    }
}

} // ns aadl
} // ns taste3
