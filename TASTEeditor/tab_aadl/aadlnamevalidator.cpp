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

static size_t s_counter_ri = 0;
static size_t s_counter_pi = 0;

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

    return isAcceptableName(object->aadlType(), object->parentObject(), name);
}

bool AADLNameValidator::isAcceptableName(const AADLObject::Type t, const AADLObject *parent, const QString &name)
{
    if (t == AADLObject::Type::Unknown || name.isEmpty())
        return false;

    switch (t) {
    case AADLObject::Type::RequiredInterface: {
        Q_ASSERT(parent);
        return instance()->isValidRequiredInterfaceName(name, parent->as<const AADLObjectFunctionType *>());
    }
    case AADLObject::Type::ProvidedInterface: {
        Q_ASSERT(parent);
        return instance()->isValidProvidedInterfaceName(name, parent->as<const AADLObjectFunctionType *>());
    }
    default: {
        qWarning() << "Not implemented yet for" << t;
        return false;
    }
    }

    Q_UNREACHABLE();
    return false;
}

QString AADLNameValidator::nextNameFor(const AADLObject::Type t, const AADLObject *parent)
{
    return instance()->nextName(t, parent);
}

QString AADLNameValidator::nextName(const AADLObject::Type t, const AADLObject *parent) const
{
    if (t == AADLObject::Type::Unknown)
        return QString();

    if (!(parent->isFunction() || parent->isFunctionType()))
        return QString();

    switch (t) {
    case AADLObject::Type::Function:
        return nameFunction(parent);
    case AADLObject::Type::FunctionType:
        return nameFunctionType(parent);
    case AADLObject::Type::RequiredInterface:
        return nameRequiredInterface(parent);
    case AADLObject::Type::ProvidedInterface:
        return nameProvidedInterface(parent);
    case AADLObject::Type::Comment:
        return nameComment(parent);
    default:
        break;
    }

    qWarning() << "Unsupported object type:" << t;
    return QString();
}

QString AADLNameValidator::nameFunctionType(const AADLObject *parent) const
{
    return QStringLiteral("Not implemented yet");
}

QString AADLNameValidator::nameFunction(const AADLObject *parent) const
{
    return QStringLiteral("Not implemented yet");
}

QString AADLNameValidator::nameRequiredInterface(const AADLObject *parent) const
{
    static const QString nameTemplate = QObject::tr("RI_%1");

    const auto fn = parent ? parent->as<const AADLObjectFunctionType *>() : nullptr;
    size_t counter = fn ? fn->ris().size() + 1 : ++s_counter_ri;

    QString name = nameTemplate.arg(counter);
    if (fn)
        while (!isAcceptableName(AADLObject::Type::RequiredInterface, fn, name))
            name = nameTemplate.arg(++counter);

    return name;
}

QString AADLNameValidator::nameProvidedInterface(const AADLObject *parent) const
{
    static const QString nameTemplate = QObject::tr("PI_%1");

    const auto fn = parent ? parent->as<const AADLObjectFunctionType *>() : nullptr;
    size_t counter = fn ? fn->pis().size() + 1 : ++s_counter_pi;

    QString name = nameTemplate.arg(counter);
    if (fn)
        while (!isAcceptableName(AADLObject::Type::ProvidedInterface, fn, name))
            name = nameTemplate.arg(++counter);

    return name;
}

QString AADLNameValidator::nameComment(const AADLObject *parent) const
{
    return QStringLiteral("Not implemented yet");
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
