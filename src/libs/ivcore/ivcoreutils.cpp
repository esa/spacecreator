/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivcoreutils.h"

#include "ivfunction.h"
#include "ivinterface.h"

namespace ivm {
namespace utils {

IVInterface *findExistingClone(IVFunction *function, IVInterface *protoIface) {
    auto mayBeClone = [protoIface](const IVInterface *iface) {
        return iface->direction() == protoIface->direction() && iface->title() == protoIface->title();
    };

    if (protoIface) {
        for (auto iface : function->interfaces()) {
            if (mayBeClone(iface)) {
                return iface;
            }
        }
    }

    return nullptr;

}

int nestingLevel(ivm::IVObject *object)
{
    if (!object)
        return -1;

    if (object->type() == ivm::IVObject::Type::InterfaceGroup
            || object->type() == ivm::IVObject::Type::ProvidedInterface
            || object->type() == ivm::IVObject::Type::RequiredInterface) {
        object = object->parentObject();
    }

    if (!object)
        return -1;

    int level = 0;
    while (auto parentObject = object->parentObject()) {
        if ((parentObject->type() == ivm::IVObject::Type::Function
                    || parentObject->type() == ivm::IVObject::Type::FunctionType)) {
            ++level;
        }
        object = parentObject;
    }
    return level;
}

} // namespace utils
} // namespace ivm
