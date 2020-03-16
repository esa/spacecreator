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

#include <QDebug>

namespace taste3 {
namespace aadl {

QString AADLNameValidator::validateName(const AADLObject *object, const QString &name)
{
    if (!object || name.isEmpty())
        return QString();

    return validateName(object->aadlType(), name);
}

QString AADLNameValidator::validateName(const AADLObject::Type t, const QString &name)
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

} // ns aadl
} // ns taste3
