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

#pragma once

#include "exportableaadlobject.h"

namespace aadl {
class AADLObjectIface;
}

namespace ive {

/**
 * @brief The ExportableAADLIface is a class to export AADLObjectIface
 */
class ExportableAADLIface : public ExportableAADLObject
{
    Q_GADGET
    Q_PROPERTY(bool isProvided READ isProvided)
    Q_PROPERTY(bool isRequired READ isRequired)
    Q_PROPERTY(QVariantList params READ paramList)
    Q_PROPERTY(QVariantList connectedInterfaces READ connectedInterfaces)

public:
    explicit ExportableAADLIface(const aadl::AADLObjectIface *iface = nullptr);

    bool isProvided() const;
    bool isRequired() const;
    QVariantList paramList() const;
    QVariantList connectedInterfaces() const;
};

}

Q_DECLARE_METATYPE(ive::ExportableAADLIface)
Q_DECLARE_TYPEINFO(ive::ExportableAADLIface, Q_MOVABLE_TYPE);
