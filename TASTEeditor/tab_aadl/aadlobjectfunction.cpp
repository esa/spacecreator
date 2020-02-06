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
#include "aadlobjectsmodel.h"

namespace taste3 {
namespace aadl {

struct AADLObjectFunctionPrivate {
    QPointer<const AADLObjectFunctionType> m_fnType;
};

AADLObjectFunction::AADLObjectFunction(const QString &title, QObject *parent)
    : AADLObjectFunctionType(title, parent)
    , d(new AADLObjectFunctionPrivate)
{
    setAttr(meta::Props::token(meta::Props::Token::is_type), "NO");
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

    QPointer<const AADLObjectFunctionType> newFnType;
    if (!functionTypeName.isEmpty()) {
        const QHash<QString, const AADLObjectFunctionType *> &availableFnTypes =
                objectsModel()->getAvailableFunctionTypes(this);

        if (const AADLObjectFunctionType *fnType = availableFnTypes.value(functionTypeName, nullptr)) {
            if (fnType == d->m_fnType)
                return;
            newFnType = fnType;
        } else {
            setFunctionTypeAttr(QString());
        }
    }

    if (d->m_fnType) {
        disconnect(d->m_fnType, nullptr, this, nullptr);
    }

    d->m_fnType = newFnType;

    if (d->m_fnType) {
        connect(d->m_fnType, &AADLObject::removed, this, &AADLObjectFunction::onFunctionTypeRemoved);
        connect(d->m_fnType, &AADLObjectFunctionType::attrChanged_isType, this,
                &AADLObjectFunction::onFunctionTypeUntyped);
        connect(d->m_fnType, &AADLObject::titleChanged, this, &AADLObjectFunction::onFunctionTypeRenamed);
    }

    emit attrChanged_instanceOf(d->m_fnType ? d->m_fnType->title() : QString());
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

void AADLObjectFunction::postInit()
{
    static const QString tokenName = meta::Props::token(meta::Props::Token::instance_of);
    setAttr(tokenName, attr(tokenName, QVariant()));
}

} // ns aadl
} // ns taste3
