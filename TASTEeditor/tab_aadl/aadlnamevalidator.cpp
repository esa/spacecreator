/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlnamevalidator.h"

#include "aadlobject.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectsmodel.h"

#include <QDebug>

namespace taste3 {
namespace aadl {

AADLNameValidator *AADLNameValidator::m_instance = nullptr;

AADLNameValidator::AADLNameValidator() {}

AADLNameValidator *AADLNameValidator::instance()
{
    if (!m_instance)
        m_instance = new AADLNameValidator;
    return m_instance;
}

QString AADLNameValidator::encodeName(const AADLObject *object, const QString &name)
{
    if (!object || name.isEmpty())
        return QString();

    return encodeName(object->aadlType(), name);
}

QString AADLNameValidator::encodeName(const AADLObject::Type t, const QString &name)
{
    if (name.isEmpty())
        return QString();

    switch (t) {
    case AADLObject::Type::Function:
    case AADLObject::Type::FunctionType:
    case aadl::AADLObject::Type::ProvidedInterface:
    case aadl::AADLObject::Type::RequiredInterface: {
        QString result;
        std::transform(name.cbegin(), name.cend(), std::back_inserter(result),
                       [](const QChar &ch) { return ch.isLetterOrNumber() ? ch : QLatin1Char('_'); });
        return result;
    }
    case AADLObject::Type::Comment:
        break;
    case AADLObject::Type::Connection: {
        qWarning() << "Connection does not support naming";
        break;
    }
    case AADLObject::Type::Unknown: {
        qWarning() << "Unknown object does not support naming";
        break;
    }
    default: {
        qWarning() << "Unsupported object type" << t;
        break;
    }
    }

    return name;
}

bool AADLNameValidator::isAcceptableName(const AADLObject *object, const QString &name)
{
    if (!object || name.isEmpty())
        return false;

    const AADLObject::Type t = object->aadlType();
    switch (t) {
    case AADLObject::Type::FunctionType: {
        return instance()->isValidFunctionTypeName(name, object);
    }
    case AADLObject::Type::Function: {
        return instance()->isValidFunctionName(name, object);
    }
    case AADLObject::Type::RequiredInterface: {
        auto parent = object->parentObject() ? object->parentObject()->as<const AADLObjectFunctionType *>() : nullptr;
        return instance()->isValidRequiredInterfaceName(name, parent);
    }
    case AADLObject::Type::ProvidedInterface: {
        auto parent = object->parentObject() ? object->parentObject()->as<const AADLObjectFunctionType *>() : nullptr;
        return instance()->isValidProvidedInterfaceName(name, parent);
    }
    case AADLObject::Type::Connection:
    case AADLObject::Type::Comment: {
        return true;
    }
    case AADLObject::Type::Unknown: {
        qWarning() << "Unsupported object type" << t;
        return false;
    }
    default: {
        qWarning() << "Not implemented yet for" << t;
        return false;
    }
    }

    Q_UNREACHABLE();
    return false;
}

QString AADLNameValidator::nextNameFor(const AADLObject *object)
{
    return instance()->nextName(object);
}

QString AADLNameValidator::nextName(const AADLObject *object) const
{
    if (!object)
        return QString();

    const AADLObject::Type t = object->aadlType();
    if (t == AADLObject::Type::Unknown)
        return QString();

    switch (t) {
    case AADLObject::Type::Function:
        return nameFunction(object);
    case AADLObject::Type::FunctionType:
        return nameFunctionType(object);
    case AADLObject::Type::RequiredInterface:
        return nameRequiredInterface(object);
    case AADLObject::Type::ProvidedInterface:
        return nameProvidedInterface(object);
    case AADLObject::Type::Comment:
        return nameComment(object);
    default:
        break;
    }

    qWarning() << "Unsupported object type:" << t;
    return QString();
}

QString AADLNameValidator::nameFunctionType(const AADLObject *function) const
{
    static const QString nameTemplate = QObject::tr("Function_Type_%1");

    size_t counter = 0;
    if (function && function->objectsModel()) {
        for (const auto fn : function->objectsModel()->objects())
            if (fn->isFunctionType())
                ++counter;
    } else
        counter = 0;
    ++counter;

    QString name = nameTemplate.arg(counter);
    while (!isAcceptableName(function, name))
        name = nameTemplate.arg(++counter);

    return name;
}

QString AADLNameValidator::nameFunction(const AADLObject *function) const
{
    static const QString nameTemplate = QObject::tr("Function_%1");

    size_t counter = 0;
    if (function && function->objectsModel()) {
        for (const auto fn : function->objectsModel()->objects())
            if (fn->isFunction())
                ++counter;
    } else
        counter = 0;
    ++counter;

    QString name = nameTemplate.arg(counter);
    while (!isAcceptableName(function, name))
        name = nameTemplate.arg(++counter);

    return name;
}

QString AADLNameValidator::nameRequiredInterface(const AADLObject *iface) const
{
    Q_ASSERT(iface);

    static const QString nameTemplate = QObject::tr("RI_%1");

    const auto parent = iface->parentObject()->as<const AADLObjectFunctionType *>();
    size_t counter = parent ? parent->ris().size() : 0;
    ++counter;

    QString name = nameTemplate.arg(counter);
    while (!isAcceptableName(iface, name))
        name = nameTemplate.arg(++counter);

    return name;
}

QString AADLNameValidator::nameProvidedInterface(const AADLObject *iface) const
{
    static const QString nameTemplate = QObject::tr("PI_%1");

    const auto parent = iface->parentObject()->as<const AADLObjectFunctionType *>();
    size_t counter = parent ? parent->pis().size() : 0;
    ++counter;

    QString name = nameTemplate.arg(counter);
    while (!isAcceptableName(iface, name))
        name = nameTemplate.arg(++counter);

    return name;
}

QString AADLNameValidator::nameComment(const AADLObject *comment) const
{
    static const QString nameTemplate = QObject::tr("Comment_%1");

    size_t counter = 0;
    if (comment && comment->objectsModel()) {
        for (const auto fn : comment->objectsModel()->objects())
            if (fn->isComment())
                ++counter;
    } else
        counter = 0;
    ++counter;

    QString name = nameTemplate.arg(counter);
    while (!isAcceptableName(comment, name))
        name = nameTemplate.arg(++counter);

    return name;
}

bool AADLNameValidator::isValidFunctionTypeName(const QString &name, const AADLObject *fnType) const
{
    if (name.isEmpty() || !fnType)
        return false;

    if (fnType->objectsModel()) {
        for (const auto fn : fnType->objectsModel()->objects())
            if (fn->isFunctionType())
                if (fn->title() == name)
                    return false;

        return true;
    }

    if (fnType->parentObject())
        if (const AADLObjectFunction *parent = fnType->parentObject()->as<const AADLObjectFunction *>())
            for (const auto c : parent->children())
                if (c->isFunctionType())
                    if (c->title() == name)
                        return false;

    return false;
}

bool AADLNameValidator::isValidFunctionName(const QString &name, const AADLObject *function) const
{
    if (name.isEmpty() || !function)
        return false;

    if (function->objectsModel()) {
        for (const auto fn : function->objectsModel()->objects())
            if (fn->isFunction())
                if (fn->title() == name)
                    return false;

        return true;
    }

    if (auto fn = function->as<const AADLObjectFunction *>())
        for (const auto c : fn->children())
            if (c->isFunction())
                if (c->title() == name)
                    return false;

    return false;
}

bool AADLNameValidator::isValidRequiredInterfaceName(const QString &name, const AADLObjectFunctionType *parent) const
{
    if (name.isEmpty())
        return false;

    for (const auto ri : parent->ris())
        if (ri->title() == name)
            return false;

    return true;
}

bool AADLNameValidator::isValidProvidedInterfaceName(const QString &name, const AADLObjectFunctionType *parent) const
{
    if (name.isEmpty())
        return false;

    for (const auto pi : parent->pis())
        if (pi->title() == name)
            return false;

    return true;
}

} // ns aadl
} // ns taste3
