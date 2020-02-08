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

#include "aadlobjectfunction.h"

#include "aadlcommonprops.h"
#include "aadlobject.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"

namespace taste3 {
namespace aadl {

bool isSameIface(const AADLObjectIface *a, const AADLObjectIface *b)
{
    const bool sameType = a->aadlType() == b->aadlType();
    const bool sameDir = a->direction() == b->direction();
    const bool sameName = a->title() == b->title();
    // TODO: check params?
    return sameType && sameDir && sameName;
}

struct AADLObjectFunctionPrivate {
    QPointer<AADLObjectFunctionType> m_fnType;
};

AADLObjectFunction::AADLObjectFunction(const QString &title, QObject *parent)
    : AADLObjectFunctionType(title, parent)
    , d(new AADLObjectFunctionPrivate)
{
    setAttr(meta::Props::token(meta::Props::Token::is_type), "NO");
    setAttr(meta::Props::token(meta::Props::Token::instance_of), QVariant());
}

AADLObjectFunction::~AADLObjectFunction() {}

AADLObject::AADLObjectType AADLObjectFunction::aadlType() const
{
    return AADLObjectType::AADLFunction;
}

void AADLObjectFunction::setAttr(const QString &name, const QVariant &val)
{
    AADLObjectFunctionType::setAttr(name, val);

    switch (meta::Props::token(name)) {
    case meta::Props::Token::instance_of: {
        setFunctionType(val.toString());
        break;
    }
    default:
        break;
    }
}

void AADLObjectFunction::setFunctionType(const QString &functionTypeName)
{
    if (!objectsModel())
        return;

    if (d->m_fnType && d->m_fnType->title() == functionTypeName)
        return;

    QPointer<AADLObjectFunctionType> newFnType;
    if (!functionTypeName.isEmpty()) {
        const QHash<QString, AADLObjectFunctionType *> &availableFnTypes =
                objectsModel()->getAvailableFunctionTypes(this);

        if (AADLObjectFunctionType *fnType = availableFnTypes.value(functionTypeName, nullptr)) {
            if (fnType == d->m_fnType)
                return;
            newFnType = fnType;
        } else {
            setFunctionTypeAttr(QString());
        }
    }

    setInstanceOf(newFnType);
}

void AADLObjectFunction::connectToFunctionType()
{
    if (!d->m_fnType)
        return;

    d->m_fnType->rememberInstance(this);

    connect(d->m_fnType, &AADLObjectFunctionType::attrChanged_isType, this, &AADLObjectFunction::onFunctionTypeUntyped);
    connect(d->m_fnType, &AADLObjectFunctionType::titleChanged, this, &AADLObjectFunction::onFunctionTypeRenamed);
    connect(d->m_fnType, &AADLObjectFunctionType::ifaceAdded, this, &AADLObjectFunction::cloneInterface);
    //    it's handled by CmdEntityRemove:
    //    connect(d->m_fnType, &AADLObjectFunctionType::ifaceRemoved, this, &AADLObjectFunction::uncloneInterface); //

    cloneInterfaces();
}

void AADLObjectFunction::cloneInterfaces()
{
    if (!d->m_fnType)
        return;

    const QVector<AADLObjectIface *> &theirInterfaces = d->m_fnType->interfaces();
    for (AADLObjectIface *theirIface : theirInterfaces)
        cloneInterface(theirIface);
}

void AADLObjectFunction::cloneInterface(AADLObjectIface *theirIface)
{
    if (!d->m_fnType || !theirIface)
        return;

    const QVector<AADLObjectIface *> &myInterfaces = interfaces();
    auto found = std::find_if(myInterfaces.cbegin(), myInterfaces.cend(),
                              [theirIface](AADLObjectIface *myIface) { return isSameIface(myIface, theirIface); });

    const bool notFound = found == myInterfaces.cend();
    if (notFound) {
        if (AADLObjectIface *myNewIface = AADLObjectIface::cloneIface(theirIface, this)) {
            connect(theirIface, &AADLObjectIface::attributeChanged, myNewIface, &AADLObjectIface::handleClonedAttr,
                    Qt::UniqueConnection);
            connect(theirIface, &AADLObjectIface::propertyChanged, myNewIface, &AADLObjectIface::handleClonedProp,
                    Qt::UniqueConnection);
        }
    }
}

void AADLObjectFunction::uncloneInterfaces(ClonedIfacesPolicy killClones)
{
    if (!d->m_fnType)
        return;

    for (AADLObjectIface *theirIface : d->m_fnType->interfaces())
        uncloneInterface(theirIface, killClones);
}

void AADLObjectFunction::uncloneInterface(AADLObjectIface *theirIface, ClonedIfacesPolicy killClones)
{
    if (!d->m_fnType || !theirIface)
        return;

    const QVector<AADLObjectIface *> &myInterfaces = interfaces();
    auto found = std::find_if(myInterfaces.cbegin(), myInterfaces.cend(),
                              [theirIface](AADLObjectIface *myIface) { return isSameIface(myIface, theirIface); });

    if (found != myInterfaces.cend()) {
        disconnect(theirIface, &AADLObjectIface::attributeChanged, theirIface, &AADLObjectIface::handleClonedAttr);
        disconnect(theirIface, &AADLObjectIface::propertyChanged, theirIface, &AADLObjectIface::handleClonedProp);

        if (killClones == ClonedIfacesPolicy::Kill) {
            AADLObjectIface *myIface = *found;
            removeInterface(myIface);
            if (theirIface->objectsModel())
                theirIface->objectsModel()->removeObject(myIface);
        }
    }
}

void AADLObjectFunction::onFunctionTypeRemoved()
{
    setFunctionTypeAttr(QString());
}

void AADLObjectFunction::onFunctionTypeUntyped(bool nowIsType)
{
    if (!nowIsType)
        setFunctionTypeAttr(QString());
}

void AADLObjectFunction::onFunctionTypeRenamed(const QString &newName)
{
    setFunctionTypeAttr(newName);
}

void AADLObjectFunction::setFunctionTypeAttr(const QString &functionTypeName)
{
    setAttr(meta::Props::token(meta::Props::Token::instance_of), functionTypeName);
}

void AADLObjectFunction::setInstanceOf(AADLObjectFunctionType *fnType, ClonedIfacesPolicy killClones)
{
    const AADLObjectFunctionType *prevFunctionType = d->m_fnType;

    if (d->m_fnType) {
        d->m_fnType->forgetInstance(this);
        disconnect(d->m_fnType, nullptr, this, nullptr);

        uncloneInterfaces(killClones);
    }

    d->m_fnType = fnType;

    if (d->m_fnType)
        connectToFunctionType();

    const QString prevName = attr(meta::Props::token(meta::Props::Token::instance_of), QString()).toString();
    const QString newName = d->m_fnType ? d->m_fnType->title() : QString();

    if (prevName != newName) {
        setFunctionTypeAttr(newName);
    } else if (prevFunctionType != d->m_fnType)
        emit attrChanged_instanceOf(d->m_fnType ? d->m_fnType->title() : QString());
}

void AADLObjectFunction::postInit()
{
    static const QString tokenName = meta::Props::token(meta::Props::Token::instance_of);
    setAttr(tokenName, attr(tokenName, QVariant()));
}

} // ns aadl
} // ns taste3
