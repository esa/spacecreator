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

class ExportableDVConnection : public ExportableDVObject
{
    Q_GADGET
    Q_PROPERTY(QVariantList messages READ messages)
    Q_PROPERTY(QString fromNode READ fromNode)
    Q_PROPERTY(QString fromDevice READ fromDevice)
    Q_PROPERTY(QString fromPort READ fromPort)
    Q_PROPERTY(QString toNode READ toNode)
    Q_PROPERTY(QString toDevice READ toDevice)
    Q_PROPERTY(QString toPort READ toPort)
    Q_PROPERTY(QString busName READ busName)

public:
    explicit ExportableDVConnection(const dvm::DVObject *dvObject = nullptr);

    QVariantList messages() const;

    QString fromNode() const;
    QString fromDevice() const;
    QString fromPort() const;

    QString toNode() const;
    QString toDevice() const;
    QString toPort() const;

    QString busName() const;
};

} // namespace dve

Q_DECLARE_METATYPE(dve::ExportableDVConnection)
Q_DECLARE_TYPEINFO(dve::ExportableDVConnection, Q_MOVABLE_TYPE);
