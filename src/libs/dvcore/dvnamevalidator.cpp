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
#include "dvdevice.h"
#include "dvmodel.h"
#include "dvnode.h"
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

QString DVNameValidator::encodeName(const DVObject::Type t, const QString &name)
{
    if (t == DVObject::Type::Node) {
        return name;
    }

    QString result;
    std::transform(name.cbegin(), name.cend(), std::back_inserter(result),
            [](const QChar &ch) { return ch.isLetterOrNumber() ? ch : QLatin1Char('_'); });
    return result;
}

QString DVNameValidator::decodeName(const DVObject::Type t, const QString &name)
{
    if (t == DVObject::Type::Node) {
        return name;
    }

    QString result;
    std::transform(name.cbegin(), name.cend(), std::back_inserter(result),
            [](const QChar &ch) { return ch.isLetterOrNumber() ? ch : QLatin1Char(' '); });
    return result;
}

bool DVNameValidator::isAcceptableName(const DVObject *object, const QString &name)
{
    if (!object) {
        return false;
    }
    if (object->type() != DVObject::Type::Node && !shared::isValidName(name)) {
        return false;
    }

    DVModel *dvModel = object->model();
    if (!dvModel) {
        return true;
    }

    if (object->type() == DVObject::Type::Device) {
        // Devices are special, as devices of different nodes can have the same name
        auto device = dynamic_cast<const DVDevice *>(object);
        if (!device) {
            return true;
        }
        DVNode *node = device->node();
        if (!node) {
            return true;
        }

        for (const DVDevice *dev : node->devices()) {
            if (dev != device && dev->title().compare(name, Qt::CaseInsensitive) == 0) {
                return false;
            }
        }
        return true;
    }

    DVObject *obj = dvModel->getObjectByName(name, object->type(), Qt::CaseInsensitive);
    return (!obj || obj == object); // no device of this name exists already, or is the object itself
}

} // namespace dvm
