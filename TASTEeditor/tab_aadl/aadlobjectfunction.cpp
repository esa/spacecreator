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
#include "tab_interface/commands/commandsfactory.h"

#include <QDebug>
#include <QScopedPointer>
#include <QUndoCommand>

namespace taste3 {
namespace aadl {

struct AADLObjectFunctionPrivate {
    QPointer<AADLObjectFunctionType> m_fnType;
};

AADLObjectFunction::AADLObjectFunction(const QString &title, QObject *parent)
    : AADLObjectFunctionType(title, parent)
    , d(new AADLObjectFunctionPrivate)
{
    setAttr(meta::Props::token(meta::Props::Token::is_type), QStringLiteral("NO"));
    setAttr(meta::Props::token(meta::Props::Token::instance_of), QVariant());
}

AADLObjectFunction::~AADLObjectFunction() {}

AADLObject::Type AADLObjectFunction::aadlType() const
{
    return Type::Function;
}

void AADLObjectFunction::postInit()
{
    static const QString attrName = meta::Props::token(meta::Props::Token::instance_of);
    const QString attrValue = attr(attrName, QVariant()).toString();
    if (!attrValue.isEmpty()) {
        const QVariantMap attributes = { { attrName, attrValue } };
        const QVariantList cmdParams = { QVariant::fromValue(this), QVariant::fromValue(attributes) };
        QScopedPointer<QUndoCommand> cmd(cmd::CommandsFactory::create(cmd::ChangeEntityAttributes, cmdParams));
        if (cmd)
            cmd->redo();
    }
}

void AADLObjectFunction::setInstanceOf(AADLObjectFunctionType *fnType)
{
    if (d->m_fnType != fnType) {
        if (d->m_fnType) {

            disconnect(d->m_fnType, &AADLObjectFunction::contextParamsChanged, this,
                       &AADLObjectFunction::reflectContextParam);
            disconnect(d->m_fnType, &AADLObjectFunction::propertyChanged, this, &AADLObjectFunction::reflectProp);
            disconnect(d->m_fnType, &AADLObjectFunction::attributeChanged, this, &AADLObjectFunction::reflectAttr);

            restoreInternals();
        }

        d->m_fnType = fnType;

        if (d->m_fnType) {
            cloneInternals();

            connect(d->m_fnType, &AADLObjectFunction::attributeChanged, this, &AADLObjectFunction::reflectAttr);
            connect(d->m_fnType, &AADLObjectFunction::propertyChanged, this, &AADLObjectFunction::reflectProp);
            connect(d->m_fnType, &AADLObjectFunction::contextParamsChanged, this,
                    &AADLObjectFunction::reflectContextParam);
        }
    }
}

void AADLObjectFunction::cloneInternals()
{
    m_originalFields.collect(this);

    if (d->m_fnType) {
        d->m_fnType->rememberInstance(this);

        reflectAttrs(d->m_fnType->attrs());
        reflectProps(d->m_fnType->props());
        reflectContextParams(d->m_fnType->contextParams());
    }
}

void AADLObjectFunction::restoreInternals()
{
    if (d->m_fnType)
        d->m_fnType->forgetInstance(this);

    if (m_originalFields.collected()) {
        reflectAttrs(m_originalFields.attrs);
        reflectProps(m_originalFields.props);
        reflectContextParams(m_originalFields.params);
    }
}

const AADLObjectFunctionType *AADLObjectFunction::instanceOf() const
{
    return d->m_fnType;
}

bool AADLObjectFunction::inheritsFunctionType() const
{
    return instanceOf();
}

void AADLObjectFunction::reflectAttr(taste3::aadl::meta::Props::Token attr)
{
    if (!d->m_fnType) {
        if (const AADLObjectFunctionType *fnType = dynamic_cast<const AADLObjectFunctionType *>(sender()))
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

void AADLObjectFunction::reflectAttrs(const QHash<QString, QVariant> &attrs)
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

void AADLObjectFunction::reflectProp(taste3::aadl::meta::Props::Token prop)
{
    if (!d->m_fnType) {
        if (const AADLObjectFunctionType *fnType = dynamic_cast<const AADLObjectFunctionType *>(sender()))
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

void AADLObjectFunction::reflectProps(const QHash<QString, QVariant> &props)
{
    QHash<QString, QVariant> prepared(props);
    for (meta::Props::Token t : { meta::Props::Token::InnerCoordinates, meta::Props::Token::coordinates }) {
        const QString name = meta::Props::token(t);
        if (prepared.contains(name))
            prepared.insert(name, prop(name));
    }

    setProps(prepared);
}

void AADLObjectFunction::reflectContextParam()
{
    if (!d->m_fnType) {
        if (const AADLObjectFunctionType *fnType = dynamic_cast<const AADLObjectFunctionType *>(sender()))
            qWarning() << QString("The Function type \"%1\" (%2) has not been disconnected, it seems")
                                  .arg(fnType->title(), fnType->id().toString());
        return;
    }

    reflectContextParams(d->m_fnType->contextParams());
}

void AADLObjectFunction::reflectContextParams(const QVector<ContextParameter> &params)
{
    setContextParams(params);
}

} // ns aadl
} // ns taste3
