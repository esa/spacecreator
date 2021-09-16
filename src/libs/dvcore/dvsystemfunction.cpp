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

#include "dvsystemfunction.h"

#include "dvsysteminterface.h"

namespace dvm {

/**
   Constructore that copies all attributes and parameters from \p copy
 */
DVSystemFunction::DVSystemFunction(const DVSystemFunction &copy, DVObject *parent)
    : DVSystemFunction(parent)
{
    setEntityAttributes(copy.entityAttributes());
}

DVSystemFunction::DVSystemFunction(DVObject *parent)
    : DVObject(DVObject::Type::SystemFunction, {}, parent)
{
}

QList<DVSystemInterface *> DVSystemFunction::interfaces() const
{
    return typedChildren<DVSystemInterface>();
}

} // namespace dvm
