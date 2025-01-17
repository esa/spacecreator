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

#include "exportabledvobject.h"

namespace dvm {
class DVObject;
}

namespace dve {

class ExportableDVDevice : public ExportableDVObject
{
    Q_GADGET
    Q_PROPERTY(QString fullName READ fullName)
    Q_PROPERTY(QString portName READ portName)
    Q_PROPERTY(QString qualifier READ qualifier)
    Q_PROPERTY(QString nodeName READ nodeName)
    Q_PROPERTY(QString nodeLabel READ nodeLabel)

public:
    explicit ExportableDVDevice(const dvm::DVObject *dvObject = nullptr);

    QString fullName() const;
    QString portName() const;
    QString qualifier() const;
    QString nodeName() const;
    QString nodeLabel() const;
};

} // namespace dve

Q_DECLARE_METATYPE(dve::ExportableDVDevice)
Q_DECLARE_TYPEINFO(dve::ExportableDVDevice, Q_MOVABLE_TYPE);
