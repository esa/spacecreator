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

#include "ivnamevalidator.h"

#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"
#include "ivobject.h"

#include <QDebug>
#include <QDir>
#include <QRegularExpression>
#include <QStandardPaths>

namespace ivm {

IVNameValidator *IVNameValidator::m_instance = nullptr;
static const QString namePatternUI("^[a-zA-Z][\\w ]*(?(?<=_)[a-zA-Z0-9])$");

IVNameValidator::IVNameValidator()
    : m_typePrefixes {
        { IVObject::Type::FunctionType, QObject::tr("Function_Type_") },
        { IVObject::Type::Function, QObject::tr("Function_") },
        { IVObject::Type::RequiredInterface, QObject::tr("RI_") },
        { IVObject::Type::ProvidedInterface, QObject::tr("PI_") },
        { IVObject::Type::InterfaceGroup, QObject::tr("Interface_Group_") },
        { IVObject::Type::Comment, QObject::tr("Comment_") },
        { IVObject::Type::Connection, QObject::tr("Connection_") },
        { IVObject::Type::ConnectionGroup, QObject::tr("Connection_Group_") },
    }
{
}

IVNameValidator *IVNameValidator::instance()
{
    if (!m_instance)
        m_instance = new IVNameValidator;
    return m_instance;
}

QString IVNameValidator::encodeName(const IVObject::Type t, const QString &name)
{
    if (name.isEmpty())
        return QString();

    switch (t) {
    case IVObject::Type::ConnectionGroup:
    case IVObject::Type::Connection:
    case IVObject::Type::Function:
    case IVObject::Type::FunctionType:
    case ivm::IVObject::Type::ProvidedInterface:
    case ivm::IVObject::Type::RequiredInterface: {
        QString result;
        std::transform(name.cbegin(), name.cend(), std::back_inserter(result),
                [](const QChar &ch) { return ch.isLetterOrNumber() ? ch : QLatin1Char('_'); });
        return result;
    }
    case IVObject::Type::Comment: {
        QString result(name);
        result.replace('\n', "\\n");
        return result;
    }
    case IVObject::Type::Unknown: {
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

QString IVNameValidator::decodeName(const IVObject::Type t, const QString &name)
{
    if (name.isEmpty())
        return QString();

    switch (t) {
    case IVObject::Type::ConnectionGroup:
    case IVObject::Type::Connection:
    case IVObject::Type::Function:
    case IVObject::Type::FunctionType:
    case ivm::IVObject::Type::ProvidedInterface:
    case ivm::IVObject::Type::RequiredInterface: {
        QString result;
        std::transform(name.cbegin(), name.cend(), std::back_inserter(result),
                [](const QChar &ch) { return ch.isLetterOrNumber() ? ch : QLatin1Char(' '); });
        return result;
    }
    case IVObject::Type::Comment: {
        QString result(name);
        result.replace("\\n", "\n");
        return result;
    }
    case IVObject::Type::Unknown: {
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

/*!
    Returns ths set of words couldn't be used as entity name
 */
static inline QSet<QString> forbiddenNamesSet()
{
    static const QString kFilePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("forbidden_names.txt");
    static const QString kDefaultPath = QLatin1String(":/defaults/resources/forbidden_names.txt");
    if (shared::ensureFileExists(kFilePath, kDefaultPath)) {
        QFile f(kFilePath);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Can't open file:" << kFilePath << f.errorString();
            return {};
        }
        QSet<QString> names;
        QTextStream stream(&f);
        QString line;
        while (stream.readLineInto(&line)) {
            names << line.trimmed();
        }
        return names;
    }
    return {};
}

/*!
   Returns is the given \p name is usable as name in general.
 */
bool IVNameValidator::isValidName(const QString &name)
{
    if (name.isEmpty()) {
        return false;
    }

    static const QSet<QString> reservedWords = forbiddenNamesSet();
    if (reservedWords.contains(name.trimmed())) {
        return false;
    }

    static QRegularExpression re(ivm::namePatternUI);
    QRegularExpressionMatch match = re.match(name);
    return match.hasMatch();
}

/*!
   The regular expression pattern for aadl names, used for the UI (encoded name)
 */
const QString &IVNameValidator::namePatternUI()
{
    return ivm::namePatternUI;
}

/*!
   Check if the name can be used for that object.
   It checks if the name is usable at all, and if the names is used already by another relevant object
 */
bool IVNameValidator::isAcceptableName(const IVObject *object, const QString &name)
{
    if (!object || !isValidName(name)) {
        return false;
    }

    const IVObject::Type t = object->type();
    switch (t) {
    case IVObject::Type::FunctionType: {
        return instance()->isFunctionTypeNameUsed(name, object);
    }
    case IVObject::Type::Function: {
        return instance()->isFunctionNameUsed(name, object);
    }
    case IVObject::Type::RequiredInterface: {
        auto parent = object->parentObject() ? object->parentObject()->as<const IVFunctionType *>() : nullptr;
        return instance()->isRequiredInterfaceNameUsed(name, parent);
    }
    case IVObject::Type::ProvidedInterface: {
        auto parent = object->parentObject() ? object->parentObject()->as<const IVFunctionType *>() : nullptr;
        return instance()->isProvidedInterfaceNameUsed(name, parent);
    }
    case IVObject::Type::InterfaceGroup:
    case IVObject::Type::ConnectionGroup:
    case IVObject::Type::Connection:
    case IVObject::Type::Comment: {
        return true;
    }
    case IVObject::Type::Unknown: {
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

bool IVNameValidator::isAutogeneratedName(const IVObject *object, const QString &nameForCheck)
{
    if (!object)
        return false;

    const QString name = nameForCheck.isEmpty() ? object->title() : nameForCheck;
    if (name.isEmpty())
        return false;

    const IVObject::Type t = object->type();
    switch (t) {
    case IVObject::Type::Function:
    case IVObject::Type::FunctionType:
    case IVObject::Type::RequiredInterface:
    case IVObject::Type::ProvidedInterface:
    case IVObject::Type::Comment: {
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
    case IVObject::Type::ConnectionGroup:
    case IVObject::Type::InterfaceGroup:
    case IVObject::Type::Connection: {
        return false;
    }
    case IVObject::Type::Unknown: {
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

QString IVNameValidator::nameForInstance(const IVFunction *object, const QString &suggestedName)
{
    if (suggestedName.isEmpty()) {
        return nextNameFor(object);
    } else if (isAcceptableName(object, suggestedName)) {
        return suggestedName;
    }
    return instance()->makeCountedName(object, suggestedName, 2);
}

QString IVNameValidator::nameOfType(IVObject::Type t)
{
    switch (t) {
    case IVObject::Type::Function:
    case IVObject::Type::FunctionType:
    case IVObject::Type::InterfaceGroup:
    case IVObject::Type::RequiredInterface:
    case IVObject::Type::ProvidedInterface:
    case IVObject::Type::Comment:
    case IVObject::Type::ConnectionGroup:
    case IVObject::Type::Connection: {
        return instance()->m_typePrefixes[t];
    }
    case IVObject::Type::Unknown: {
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

QString IVNameValidator::nextNameFor(const IVObject *object)
{
    return instance()->nextName(object);
}

QString IVNameValidator::nextName(const IVObject *object) const
{
    if (!object) {
        return QString();
    }

    const IVObject::Type t = object->type();
    switch (t) {
    case IVObject::Type::Function:
        return nameFunction(object);
    case IVObject::Type::FunctionType:
        return nameFunctionType(object);
    case IVObject::Type::RequiredInterface:
        return nameRequiredInterface(object);
    case IVObject::Type::ProvidedInterface:
        return nameProvidedInterface(object);
    case IVObject::Type::Comment:
        return nameComment(object);
    case IVObject::Type::ConnectionGroup:
    case IVObject::Type::Connection:
        return nameConnection(object);
    case IVObject::Type::InterfaceGroup:
    case IVObject::Type::Unknown:
        return QString();
    default:
        break;
    }

    qWarning() << "Unsupported object type:" << t;
    return QString();
}

QString IVNameValidator::makeCountedName(const IVObject *object, const QString &nameTemplate, int counter) const
{
    QString name = nameTemplate + QString::number(counter);
    while (!isAcceptableName(object, name)) {
        name = nameTemplate + QString::number(++counter);
    }
    return name;
}

QString IVNameValidator::nameFunctionType(const IVObject *functionType) const
{
    const QString nameTemplate =
            functionType->title().isEmpty() ? m_typePrefixes[functionType->type()] : functionType->title();

    int counter = 0;
    if (functionType && functionType->model()) {
        for (const auto fn : functionType->model()->objects()) {
            if (fn->isFunctionType()) {
                ++counter;
            }
        }
    } else {
        counter = 0;
    }
    ++counter;

    return makeCountedName(functionType, nameTemplate, counter);
}

QString IVNameValidator::nameFunction(const IVObject *function) const
{
    const QString nameTemplate = function->title().isEmpty() ? m_typePrefixes[function->type()] : function->title();

    int counter = 0;
    if (function && function->model()) {
        for (const auto fn : function->model()->objects()) {
            if (fn->isFunction()) {
                ++counter;
            }
        }
    } else {
        counter = 0;
    }
    ++counter;

    return makeCountedName(function, nameTemplate, counter);
}

QString IVNameValidator::nameRequiredInterface(const IVObject *iface) const
{
    Q_ASSERT(iface);

    const QString nameTemplate = m_typePrefixes[iface->type()];

    const auto parent = iface->parentObject()->as<const IVFunctionType *>();
    int counter = parent ? parent->ris().size() : 0;
    ++counter;

    return makeCountedName(iface, nameTemplate, counter);
}

QString IVNameValidator::nameProvidedInterface(const IVObject *iface) const
{
    Q_ASSERT(iface);

    const QString nameTemplate = m_typePrefixes[iface->type()];

    const auto parent = iface->parentObject()->as<const IVFunctionType *>();
    int counter = parent ? parent->pis().size() : 0;
    ++counter;

    return makeCountedName(iface, nameTemplate, counter);
}

QString IVNameValidator::nameComment(const IVObject *comment) const
{
    const QString nameTemplate = m_typePrefixes[comment->type()];

    int counter = 0;
    if (comment && comment->model()) {
        for (const auto fn : comment->model()->objects())
            if (fn->isComment())
                ++counter;
    } else
        counter = 0;
    ++counter;

    return makeCountedName(comment, nameTemplate, counter);
}

QString IVNameValidator::nameConnection(const IVObject *connection) const
{
    if (auto connectionPtr = qobject_cast<const IVConnection *>(connection)) {
        return QString("%1.%2 <-> %3.%4")
                .arg(connectionPtr->sourceName(), connectionPtr->sourceInterfaceName(), connectionPtr->targetName(),
                        connectionPtr->targetInterfaceName());
    } else if (auto connectionPtr = qobject_cast<const IVConnectionGroup *>(connection)) {
        QStringList sourceNames, targetNames;
        for (const auto &sourceIface : connectionPtr->groupedSourceInterfaces()) {
            sourceNames.append(sourceIface->title());
        }
        for (const auto &targetIface : connectionPtr->groupedTargetInterfaces()) {
            targetNames.append(targetIface->title());
        }
        return QString("%1.{%2} <-> %3.{%4}")
                .arg(connectionPtr->sourceName(), sourceNames.join(QLatin1Char('|')), connectionPtr->targetName(),
                        targetNames.join(QLatin1Char('|')));
    }
    return {};
}

bool IVNameValidator::isFunctionTypeNameUsed(const QString &name, const IVObject *fnType) const
{
    if (name.isEmpty() || !fnType) {
        return false;
    }

    if (fnType->model()) {
        for (const auto fn : fnType->model()->objects()) {
            if (fn->isFunctionType()) {
                if (fn->title() == name) {
                    return false;
                }
            }
        }

        return true;
    }

    if (fnType->parentObject()) {
        if (const IVFunction *parent = fnType->parentObject()->as<const IVFunction *>()) {
            for (const auto c : parent->children()) {
                if (c->isFunctionType()) {
                    if (c->title() == name) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool IVNameValidator::isFunctionNameUsed(const QString &name, const IVObject *function) const
{
    if (name.isEmpty() || !function) {
        return false;
    }

    if (function->model()) {
        for (const auto fn : function->model()->objects()) {
            if (fn->isFunction()) {
                if (fn->title() == name) {
                    return false;
                }
            }
        }

        return true;
    }

    if (auto fn = function->as<const IVFunction *>()) {
        for (const auto c : fn->children()) {
            if (c->isFunction()) {
                if (c->title() == name) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool IVNameValidator::isRequiredInterfaceNameUsed(const QString &name, const IVFunctionType *parent) const
{
    if (name.isEmpty()) {
        return false;
    }

    for (const auto ri : parent->ris()) {
        if (ri->title() == name) {
            return false;
        }
    }

    return true;
}

bool IVNameValidator::isProvidedInterfaceNameUsed(const QString &name, const IVFunctionType *parent) const
{
    if (name.isEmpty()) {
        return false;
    }

    for (const auto pi : parent->pis()) {
        if (pi->title() == name) {
            return false;
        }
    }

    return true;
}

}
