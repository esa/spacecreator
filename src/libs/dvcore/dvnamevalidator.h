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

#pragma once

#include "dvobject.h"

#include <QString>

namespace dvm {
class DVModel;

class DVNameValidator
{
public:
    static QString nameForObject(const DVObject *object, dvm::DVModel *model = nullptr, const QString &nameBase = {});
    static QString encodeName(const DVObject::Type t, const QString &name);
    static QString decodeName(const DVObject::Type t, const QString &name);
    static bool isAcceptableName(const DVObject *object, const QString &name);
};

} // namespace dvm
