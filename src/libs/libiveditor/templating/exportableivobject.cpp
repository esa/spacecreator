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

#include "exportableivobject.h"

#include "exportableivconnection.h"
#include "exportableivconnectiongroup.h"
#include "exportableivfunction.h"
#include "exportableivfunctiontype.h"
#include "exportableivinterface.h"
#include "exportableproperty.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivobject.h"

namespace ive {

ExportableIVObject::ExportableIVObject(const ivm::IVObject *ivObject)
    : AbstractExportableObject(ivObject)
{
}

QString ExportableIVObject::name() const
{
    return exportedObject<ivm::IVObject>()->title();
}

/**
 * @brief ExportableIVObject::createFrom creates appropriate exported class and casts to QVariant
 * @param ivObject exported object
 * @return created exported object as QVariant
 */
QVariant ExportableIVObject::createFrom(const ivm::IVObject *ivObject)
{
    switch (ivObject->type()) {
    case ivm::IVObject::Type::InterfaceGroup:
        return {};
    case ivm::IVObject::Type::Function:
        return QVariant::fromValue(ExportableIVFunction(static_cast<const ivm::IVFunction *>(ivObject)));
    case ivm::IVObject::Type::FunctionType:
        return QVariant::fromValue(ExportableIVFunctionType(static_cast<const ivm::IVFunctionType *>(ivObject)));
    case ivm::IVObject::Type::RequiredInterface:
    case ivm::IVObject::Type::ProvidedInterface:
        return QVariant::fromValue(ExportableIVInterface(static_cast<const ivm::IVInterface *>(ivObject)));
    case ivm::IVObject::Type::Comment:
        return QVariant::fromValue(ExportableIVObject(ivObject));
    case ivm::IVObject::Type::Connection:
        return QVariant::fromValue(ExportableIVConnection(static_cast<const ivm::IVConnection *>(ivObject)));
    case ivm::IVObject::Type::ConnectionGroup:
        return QVariant::fromValue(ExportableIVConnectionGroup(static_cast<const ivm::IVConnectionGroup *>(ivObject)));
    default:
        Q_UNREACHABLE();
    }
    return QVariant();
}
/**
 * @brief ExportableIVObject::attributes returns list of attributes for using in string templates.
 * @return list of attributes.
 */
QVariantList ExportableIVObject::attributes() const
{
    return exportedObject<ivm::IVObject>()->generateProperties(false);
}

/**
 * @brief ExportableIVObject::properties returns list of properties for using in string templates.
 * @return list of properties.
 */
QVariantList ExportableIVObject::properties() const
{
    return exportedObject<ivm::IVObject>()->generateProperties(true);
}

QStringList ExportableIVObject::path() const
{
    return exportedObject<ivm::IVObject>()->path();
}

}
