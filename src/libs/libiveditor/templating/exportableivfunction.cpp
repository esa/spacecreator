/*
   Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "exportableivfunction.h"

#include "exportableproperty.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivinterfacechain.h"
#include "ivmodel.h"

namespace ive {

ExportableIVFunction::ExportableIVFunction(const ivm::IVFunction *function)
    : ExportableIVFunctionType(function)
{
}

QVariantList ExportableIVFunction::implementations() const
{
    const auto ivFunction = exportedObject<ivm::IVFunction>();
    if (!ivFunction) {
        return {};
    }

    QVariantList result;
    for (const auto &implementation : ivFunction->implementations()) {
        result << QVariant::fromValue(shared::ExportableProperty(implementation.name(), implementation.value()));
    }
    return result;
}

}
