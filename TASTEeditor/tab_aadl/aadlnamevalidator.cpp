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

AADLNameValidator::AADLNameValidator()
    : m_typePrefixes { { AADLObject::Type::FunctionType, QObject::tr("Function Type ") },
                       { AADLObject::Type::Function, QObject::tr("Function ") },
                       { AADLObject::Type::RequiredInterface, QObject::tr("RI ") },
                       { AADLObject::Type::ProvidedInterface, QObject::tr("PI ") },
                       { AADLObject::Type::Comment, QObject::tr("Comment ") },
                       { AADLObject::Type::Connection, QObject::tr("Comment ") } }
{
}

AADLNameValidator *AADLNameValidator::instance()
{
    if (!m_instance)
        m_instance = new AADLNameValidator;
    return m_instance;
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
    case AADLObject::Type::Comment: {
        QString result(name);
        result.replace('\n', "\\n");
        return result;
    }
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

QString AADLNameValidator::decodeName(const AADLObject::Type t, const QString &name)
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
                       [](const QChar &ch) { return ch.isLetterOrNumber() ? ch : QLatin1Char(' '); });
        return result;
    }
    case AADLObject::Type::Comment: {
        QString result(name);
        result.replace("\\n", "\n");
        return result;
    }
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

bool AADLNameValidator::isAutogeneratedName(const AADLObject *object, const QString &nameForCheck)
{
    if (!object)
        return false;

    const QString name = nameForCheck.isEmpty() ? object->title() : nameForCheck;
    if (name.isEmpty())
        return false;

    const AADLObject::Type t = object->aadlType();
    switch (t) {
    case AADLObject::Type::Function:
    case AADLObject::Type::FunctionType:
    case AADLObject::Type::RequiredInterface:
    case AADLObject::Type::ProvidedInterface:
    case AADLObject::Type::Comment: {
        const QString preffix = instance()->m_typePrefixes[t];
        if (!name.startsWith(preffix))
            return false;

        if (name == preffix)
            return false;

        for (int i = preffix.length(); i < name.length() - 1; ++i)
            if (!name.at(i).isDigit())
                return false;

        return true;
    }
    case AADLObject::Type::Connection: {
        return false;
    }
    case AADLObject::Type::Unknown: {
        qWarning() << "unknown object type";
        return false;
    }
    default: {
        qWarning() << "unsupported object type";
        return false;
    }
    }

    Q_UNREACHABLE();
    return false;
}

QString AADLNameValidator::nameOfType(AADLObject::Type t)
{
    switch (t) {
    case AADLObject::Type::Function:
    case AADLObject::Type::FunctionType:
    case AADLObject::Type::RequiredInterface:
    case AADLObject::Type::ProvidedInterface:
    case AADLObject::Type::Comment:
    case AADLObject::Type::Connection: {
        return instance()->m_typePrefixes[t];
    }
    case AADLObject::Type::Unknown: {
        const QString wrn = QObject::tr("Unknown object type");
        qWarning() << wrn;
        return wrn;
    }
    default: {
        const QString wrn = QObject::tr("Unsupported object type");
        qWarning() << wrn;
        return wrn;
    }
    }
    Q_UNREACHABLE();
    return QString();
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

QString AADLNameValidator::makeCountedName(const AADLObject *object, const QString &nameTemplate, int counter) const
{
    QString name = nameTemplate + QString::number(counter);
    while (!isAcceptableName(object, name))
        name = nameTemplate + QString::number(++counter);
    return name;
}

QString AADLNameValidator::nameFunctionType(const AADLObject *functionType) const
{
    static const QString nameTemplate = m_typePrefixes[functionType->aadlType()];

    size_t counter = 0;
    if (functionType && functionType->objectsModel()) {
        for (const auto fn : functionType->objectsModel()->objects())
            if (fn->isFunctionType())
                ++counter;
    } else
        counter = 0;
    ++counter;

    return makeCountedName(functionType, nameTemplate, counter);
}

QString AADLNameValidator::nameFunction(const AADLObject *function) const
{
    static const QString nameTemplate = m_typePrefixes[function->aadlType()];

    size_t counter = 0;
    if (function && function->objectsModel()) {
        for (const auto fn : function->objectsModel()->objects())
            if (fn->isFunction())
                ++counter;
    } else
        counter = 0;
    ++counter;

    return makeCountedName(function, nameTemplate, counter);
}

QString AADLNameValidator::nameRequiredInterface(const AADLObject *iface) const
{
    Q_ASSERT(iface);

    static const QString nameTemplate = m_typePrefixes[iface->aadlType()];

    const auto parent = iface->parentObject()->as<const AADLObjectFunctionType *>();
    size_t counter = parent ? parent->ris().size() : 0;
    ++counter;

    return makeCountedName(iface, nameTemplate, counter);
}

QString AADLNameValidator::nameProvidedInterface(const AADLObject *iface) const
{
    Q_ASSERT(iface);

    static const QString nameTemplate = m_typePrefixes[iface->aadlType()];

    const auto parent = iface->parentObject()->as<const AADLObjectFunctionType *>();
    size_t counter = parent ? parent->pis().size() : 0;
    ++counter;

    return makeCountedName(iface, nameTemplate, counter);
}

QString AADLNameValidator::nameComment(const AADLObject *comment) const
{
    static const QString nameTemplate = m_typePrefixes[comment->aadlType()];

    size_t counter = 0;
    if (comment && comment->objectsModel()) {
        for (const auto fn : comment->objectsModel()->objects())
            if (fn->isComment())
                ++counter;
    } else
        counter = 0;
    ++counter;

    return makeCountedName(comment, nameTemplate, counter);
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
