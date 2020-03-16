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

#include "aadlobject.h"

namespace taste3 {
namespace aadl {

class AADLObject;
class AADLObjectsModel;
class AADLObjectFunction;
class AADLObjectFunctionType;
class AADLObjectIfaceRequired;
class AADLObjectIfaceProvided;
class AADLNameValidator
{
public:
    static AADLNameValidator *instance();

    static QString encodeName(const AADLObject *object, const QString &name);
    static QString encodeName(const AADLObject::Type t, const QString &name);

    static QString nextNameFor(const AADLObject *object);

    static bool isAcceptableName(const AADLObject *object, const QString &name);

private:
    static AADLNameValidator *m_instance;

    AADLNameValidator();

    QString nextName(const AADLObject *object) const;

    QString nameFunctionType(const AADLObject *parent) const;
    QString nameFunction(const AADLObject *fn) const;
    QString nameRequiredInterface(const AADLObject *iface) const;
    QString nameProvidedInterface(const AADLObject *iface) const;
    QString nameComment(const AADLObject *parent) const;

    bool isValidFunctionTypeName(const QString &name, const AADLObject *fnType) const;
    bool isValidFunctionName(const QString &name, const AADLObject *fn) const;
    bool isValidRequiredInterfaceName(const QString &name, const AADLObjectFunctionType *parent) const;
    bool isValidProvidedInterfaceName(const QString &name, const AADLObjectFunctionType *parent) const;
};

} // ns aadl
} // ns taste3
