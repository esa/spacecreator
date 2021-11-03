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
#include "propertytemplate.h"
#include "propertytemplateconfig.h"

#include <QDebug>
#include <QScopedPointer>
#include <QUndoCommand>

namespace ivm {

struct IVFunctionPrivate {
    QPointer<IVFunctionType> m_fnType;
    QList<EntityAttribute> m_implementations;
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
            }
        }
        checkDefaultFunctionImplementation();
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

const QList<EntityAttribute> &IVFunction::implementations() const
{
    return d->m_implementations;
}

void IVFunction::setImplementation(int idx, const EntityAttribute &value)
{
    if (idx < 0 || idx >= d->m_implementations.size()) {
        return;
    }
    d->m_implementations[idx] = value;
}

bool IVFunction::hasImplementationName(const QString &name) const
{
    for (const EntityAttribute &implementation : d->m_implementations) {
        if (implementation.name() == name) {
            return true;
        }
    }
    return false;
}

void IVFunction::addImplementation(const QString &name, const QString &language)
{
    d->m_implementations.append(EntityAttribute(name, language, EntityAttribute::Type::Attribute));
}

/*!
   Inserts the implementation \p value at position idx
 */
void IVFunction::insertImplementation(int idx, const EntityAttribute &value)
{
    d->m_implementations.insert(idx, value);
}

void IVFunction::removeImplementation(int idx)
{
    d->m_implementations.removeAt(idx);
}

void IVFunction::setDefaultImplementation(const QString &name)
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::default_implementation), name);
}

QString IVFunction::defaultImplementation() const
{
    return entityAttributeValue(meta::Props::token(meta::Props::Token::default_implementation)).toString();
}

bool IVFunction::isPseudoFunction() const
{
    /// @todo update to multi language support
    return entityAttributeValue("language").toString() == "Pseudo function";
}

void IVFunction::reflectAttrs(const EntityAttributes &attributes)
{
    EntityAttributes prepared { attributes };
    static const QList<meta::Props::Token> excludeTokens = { meta::Props::Token::is_type,
        meta::Props::Token::instance_of, meta::Props::Token::name, meta::Props::Token::RootCoordinates,
        meta::Props::Token::coordinates, meta::Props::Token::enable_multicast };
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
    case meta::Props::Token::RootCoordinates:
    case meta::Props::Token::coordinates:
    case meta::Props::Token::enable_multicast:
        break;
    case meta::Props::Token::name:
        setEntityAttribute(meta::Props::token(meta::Props::Token::instance_of), d->m_fnType->title());
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

void IVFunction::reflectContextParams(const QVector<shared::ContextParameter> &params)
{
    setContextParams(params);
}

void IVFunction::checkDefaultFunctionImplementation()
{
    if (implementations().isEmpty()) {
        // Add a default language
        if (!model()) {
            return;
        }
        QString value = entityAttributeValue("language").toString();
        if (value.isEmpty()) {
            shared::PropertyTemplate *temp = model()->dynPropConfig()->propertyTemplateForObject(
                    this, meta::Props::token(meta::Props::Token::default_implementation));
            if (!temp) {
                return;
            }
            value = temp->defaultValue().toString();
        }
        addImplementation("default", value);
        setDefaultImplementation("default");
        return;
    }

    QString defaultImpl = defaultImplementation();
    for (const EntityAttribute &impl : implementations()) {
        if (impl.name() == defaultImpl) {
            return;
        }
    }
    // default not found - setting the first one
    setDefaultImplementation(implementations().at(0).name());
}

}
