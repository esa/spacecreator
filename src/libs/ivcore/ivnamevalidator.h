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

#pragma once

#include "ivobject.h"

#include <QHash>
#include <QString>

namespace ivm {

class IVFunction;
class IVFunctionType;

class IVNameValidator
{
public:
    static IVNameValidator *instance();

    static QString encodeName(const IVObject::Type t, const QString &name);
    static QString decodeName(const IVObject::Type t, const QString &name);

    static QString nextNameFor(const IVObject *object);

    static bool isAcceptableName(const IVObject *object, const QString &name);

    static bool isAutogeneratedName(const IVObject *object, const QString &nameForCheck = QString());

    static QString nameForInstance(const IVFunction *object, const QString &suggestedName = QString());

    static QString nameOfType(IVObject::Type t);

private:
    static IVNameValidator *m_instance;
    const QHash<ivm::IVObject::Type, QString> m_typePrefixes;

    IVNameValidator();

    QString nextName(const IVObject *object) const;

    QString makeCountedName(const IVObject *object, const QString &nameTemplate, int counter) const;

    QString nameFunctionType(const IVObject *parent) const;
    QString nameFunction(const IVObject *fn) const;
    QString nameRequiredInterface(const IVObject *iface) const;
    QString nameProvidedInterface(const IVObject *iface) const;
    QString nameComment(const IVObject *comment) const;
    QString nameConnection(const IVObject *connection) const;

    bool isFunctionTypeNameUsed(const QString &name, const IVObject *fnType) const;
    bool isFunctionNameUsed(const QString &name, const IVObject *fn) const;
    bool isRequiredInterfaceNameUsed(const QString &name, const IVObject *reqIface) const;
    bool isProvidedInterfaceNameUsed(const QString &name, const IVObject *parent) const;
};

}
