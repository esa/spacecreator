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

#include "aadlfunction.h"

#include "aadlcommonprops.h"
#include "aadlobject.h"
#include "aadliface.h"
#include "aadlmodel.h"
//#include "interface/commands/commandsfactory.h"

#include <QDebug>
#include <QScopedPointer>
#include <QUndoCommand>

namespace ivm {

struct AADLFunctionPrivate {
    QPointer<AADLFunctionType> m_fnType;
};

AADLFunction::AADLFunction(const QString &title, QObject *parent)
    : AADLFunctionType(AADLObject::Type::Function, title, parent)
    , d(new AADLFunctionPrivate)
{
    setAttr(meta::Props::token(meta::Props::Token::is_type), QStringLiteral("NO"));
    setAttr(meta::Props::token(meta::Props::Token::instance_of), QVariant());
}

AADLFunction::~AADLFunction() { }

bool AADLFunction::postInit()
{
#ifdef FIXME_BEFORE_MR
    const QString attrName = meta::Props::token(meta::Props::Token::instance_of);
    const QString attrValue = attr(attrName, QVariant()).toString();
    if (!attrValue.isEmpty()) {
        const QVariantMap attributes = { { attrName, attrValue } };
        const QVariantList cmdParams = { QVariant::fromValue(this), QVariant::fromValue(attributes) };
        QScopedPointer<QUndoCommand> cmd(cmd::CommandsFactory::create(cmd::ChangeEntityAttributes, cmdParams));
        if (cmd)
            cmd->redo();
    }
#endif
    return true;
}

void AADLFunction::setInstanceOf(AADLFunctionType *fnType)
{
    if (d->m_fnType != fnType) {
        if (d->m_fnType) {

            disconnect(d->m_fnType, &AADLFunction::contextParamsChanged, this, &AADLFunction::reflectContextParam);
            disconnect(d->m_fnType, qOverload<ivm::meta::Props::Token>(&AADLFunction::propertyChanged), this,
                    &AADLFunction::reflectProp);
            disconnect(d->m_fnType, qOverload<ivm::meta::Props::Token>(&AADLFunction::attributeChanged), this,
                    &AADLFunction::reflectAttr);

            restoreInternals();
        }

        d->m_fnType = fnType;

        if (d->m_fnType) {
            cloneInternals();

            connect(d->m_fnType, qOverload<ivm::meta::Props::Token>(&AADLFunction::attributeChanged), this,
                    &AADLFunction::reflectAttr);
            connect(d->m_fnType, qOverload<ivm::meta::Props::Token>(&AADLFunction::propertyChanged), this,
                    &AADLFunction::reflectProp);
            connect(d->m_fnType, &AADLFunction::contextParamsChanged, this, &AADLFunction::reflectContextParam);
        }
    }
}

void AADLFunction::cloneInternals()
{
    m_originalFields.collect(this);

    if (d->m_fnType) {
        d->m_fnType->rememberInstance(this);

        reflectAttrs(d->m_fnType->attrs());
        reflectProps(d->m_fnType->props());
        reflectContextParams(d->m_fnType->contextParams());
    }
}

void AADLFunction::restoreInternals()
{
    if (d->m_fnType)
        d->m_fnType->forgetInstance(this);

    if (m_originalFields.collected()) {
        reflectAttrs(m_originalFields.attrs);
        reflectProps(m_originalFields.props);
        reflectContextParams(m_originalFields.params);
    }
}

const AADLFunctionType *AADLFunction::instanceOf() const
{
    return d->m_fnType;
}

bool AADLFunction::inheritsFunctionType() const
{
    return instanceOf();
}

void AADLFunction::reflectAttr(ivm::meta::Props::Token attr)
{
    if (!d->m_fnType) {
        if (const AADLFunctionType *fnType = dynamic_cast<const AADLFunctionType *>(sender()))
            qWarning() << QString("The Function type \"%1\" (%2) has not been disconnected, it seems")
                                  .arg(fnType->title(), fnType->id().toString());
        return;
    }

    switch (attr) {
    case meta::Props::Token::is_type:
    case meta::Props::Token::instance_of:
    case meta::Props::Token::name:
        break;
    case meta::Props::Token::Unknown: {
        reflectAttrs(d->m_fnType->attrs());
        break;
    }
    default: {
        const QString &attrName = meta::Props::token(attr);
        setAttr(attrName, d->m_fnType->attr(attrName));
        break;
    }
    }
}

void AADLFunction::reflectAttrs(const QHash<QString, QVariant> &attrs)
{
    QHash<QString, QVariant> prepared(attrs);
    for (meta::Props::Token t :
            { meta::Props::Token::is_type, meta::Props::Token::instance_of, meta::Props::Token::name }) {
        const QString name = meta::Props::token(t);
        if (prepared.contains(name))
            prepared.insert(name, attr(name));
    }

    setAttrs(prepared);
}

void AADLFunction::reflectProp(ivm::meta::Props::Token prop)
{
    if (!d->m_fnType) {
        if (const AADLFunctionType *fnType = dynamic_cast<const AADLFunctionType *>(sender()))
            qWarning() << QString("The Function type \"%1\" (%2) has not been disconnected, it seems")
                                  .arg(fnType->title(), fnType->id().toString());
        return;
    }

    switch (prop) {
    case meta::Props::Token::Unknown: {
        reflectProps(d->m_fnType->props());
        break;
    }
    case meta::Props::Token::InnerCoordinates:
    case meta::Props::Token::coordinates:
        break;
    default: {
        const QString &propName = meta::Props::token(prop);
        setProp(propName, d->m_fnType->prop(propName));
        break;
    }
    }
}

void AADLFunction::reflectProps(const QHash<QString, QVariant> &props)
{
    QHash<QString, QVariant> prepared(props);
    for (meta::Props::Token t : { meta::Props::Token::InnerCoordinates, meta::Props::Token::coordinates }) {
        const QString name = meta::Props::token(t);
        if (prepared.contains(name))
            prepared.insert(name, prop(name));
    }

    setProps(prepared);
}

void AADLFunction::reflectContextParam()
{
    if (!d->m_fnType) {
        if (const AADLFunctionType *fnType = dynamic_cast<const AADLFunctionType *>(sender()))
            qWarning() << QString("The Function type \"%1\" (%2) has not been disconnected, it seems")
                                  .arg(fnType->title(), fnType->id().toString());
        return;
    }

    reflectContextParams(d->m_fnType->contextParams());
}

void AADLFunction::reflectContextParams(const QVector<ContextParameter> &params)
{
    setContextParams(params);
}

}
