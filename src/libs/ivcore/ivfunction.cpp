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

#include "ivfunction.h"

#include "errorhub.h"
#include "ivcommonprops.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "ivobject.h"

#include <QDebug>
#include <QScopedPointer>
#include <QUndoCommand>

namespace ivm {

struct IVFunctionPrivate {
    QPointer<IVFunctionType> m_fnType;
};

IVFunction::IVFunction(QObject *parent, const shared::Id &id)
    : IVFunctionType(IVObject::Type::Function, parent, id)
    , d(new IVFunctionPrivate)
{
}

IVFunction::~IVFunction() { }

bool IVFunction::postInit()
{
    if (auto objModel = model()) {
        const QString typeName =
                entityAttributeValue(meta::Props::token(meta::Props::Token::instance_of)).value<QString>();
        if (!typeName.isEmpty()) {
            const QHash<QString, IVFunctionType *> types = objModel->getAvailableFunctionTypes(this);
            if (auto typeObj = types.value(typeName)) {
                setInstanceOf(typeObj);
            } else {
                shared::ErrorHub::addError(
                        shared::ErrorItem::Warning, tr("Function type '%1' error").arg(typeName), "");
                return false;
            }
        }
    }

    return IVObject::postInit();
}

void IVFunction::setInstanceOf(IVFunctionType *fnType)
{
    if (d->m_fnType != fnType) {
        if (d->m_fnType) {
            disconnect(d->m_fnType, &IVFunction::contextParamsChanged, this, &IVFunction::reflectContextParam);
            disconnect(d->m_fnType, &IVFunction::attributeChanged, this, &IVFunction::reflectAttr);

            restoreInternals();
        }

        d->m_fnType = fnType;

        if (d->m_fnType) {
            cloneInternals();

            connect(d->m_fnType, &IVFunction::attributeChanged, this, &IVFunction::reflectAttr);
            connect(d->m_fnType, &IVFunction::contextParamsChanged, this, &IVFunction::reflectContextParam);
        }
    }
}

void IVFunction::cloneInternals()
{
    m_originalFields.collect(this);

    if (d->m_fnType) {
        d->m_fnType->rememberInstance(this);

        reflectAttrs(d->m_fnType->entityAttributes());
        reflectContextParams(d->m_fnType->contextParams());
    }
}

void IVFunction::restoreInternals()
{
    if (d->m_fnType)
        d->m_fnType->forgetInstance(this);

    if (m_originalFields.collected()) {
        reflectAttrs(m_originalFields.attrs);
        reflectContextParams(m_originalFields.params);
    }
}

const IVFunctionType *IVFunction::instanceOf() const
{
    return d->m_fnType;
}

bool IVFunction::inheritsFunctionType() const
{
    return instanceOf();
}

void IVFunction::reflectAttrs(const EntityAttributes &attributes)
{
    EntityAttributes prepared { attributes };
    static const QList<meta::Props::Token> excludeTokens = { meta::Props::Token::is_type,
        meta::Props::Token::instance_of, meta::Props::Token::name, meta::Props::Token::InnerCoordinates,
        meta::Props::Token::coordinates };
    for (meta::Props::Token t : excludeTokens) {
        const QString name = meta::Props::token(t);
        prepared[name] = entityAttribute(name);
    }

    setEntityAttributes(prepared);
}

void IVFunction::reflectAttr(const QString &attrName)

{
    if (!d->m_fnType) {
        if (const IVFunctionType *fnType = dynamic_cast<const IVFunctionType *>(sender()))
            shared::ErrorHub::addError(shared::ErrorItem::Warning,
                    tr("The Function type \"%1\" (%2) has not been disconnected, it seems")
                            .arg(fnType->title(), fnType->id().toString()));
        return;
    }
    const meta::Props::Token attr = meta::Props::token(attrName);

    switch (attr) {
    case meta::Props::Token::is_type:
    case meta::Props::Token::instance_of:
    case meta::Props::Token::name:
    case meta::Props::Token::RootCoordinates:
    case meta::Props::Token::InnerCoordinates:
    case meta::Props::Token::coordinates:
        break;
    case meta::Props::Token::Unknown: {
        reflectAttrs(d->m_fnType->entityAttributes());
        break;
    }
    default: {
        setEntityAttribute(d->m_fnType->entityAttribute(attrName));
        break;
    }
    }
}

void IVFunction::reflectContextParam()
{
    if (!d->m_fnType) {
        if (const IVFunctionType *fnType = dynamic_cast<const IVFunctionType *>(sender()))
            shared::ErrorHub::addError(shared::ErrorItem::Warning,
                    tr("The Function type \"%1\" (%2) has not been disconnected, it seems")
                            .arg(fnType->title(), fnType->id().toString()));
        return;
    }

    reflectContextParams(d->m_fnType->contextParams());
}

void IVFunction::reflectContextParams(const QVector<ContextParameter> &params)
{
    setContextParams(params);
}

}
