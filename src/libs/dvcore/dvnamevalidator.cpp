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

#include "dvnamevalidator.h"

#include "common.h"
#include "dvmodel.h"
#include "dvobject.h"

namespace dvm {

QString DVNameValidator::nameForObject(const DVObject *object, DVModel *model, const QString &nameBase)
{
    static const QString kSuffix { "_%1" };
    int counter = 0;
    const QString templateName = (nameBase.isEmpty() ? shared::typeName(object->type()) : nameBase) + kSuffix;
    DVModel *dvModel = model ? model : object->model();
    QString name = templateName.arg(++counter);
    if (!dvModel) {
        return name;
    }
    while (dvModel->getObjectByName(name, object->type(), Qt::CaseInsensitive) != nullptr) {
        name = templateName.arg(++counter);
    }
    return name;
}

} // namespace dvm
