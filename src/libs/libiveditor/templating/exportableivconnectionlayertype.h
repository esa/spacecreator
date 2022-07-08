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

#pragma once

#include "exportableivobject.h"

namespace ivm {
class IVConnectionLayerType;
}

namespace ive {

/**
 * @brief The ExportableIVConnectionLayerType is a class to export IVConnectionLayerType
 */
class ExportableIVConnectionLayerType : public ExportableIVObject
{
    Q_GADGET
    Q_PROPERTY(QString name READ connectionLayerName)

public:
    explicit ExportableIVConnectionLayerType(const ivm::IVConnectionLayerType *layer = nullptr);
    QString connectionLayerName() const;
};

}

Q_DECLARE_METATYPE(ive::ExportableIVConnectionLayerType)
Q_DECLARE_TYPEINFO(ive::ExportableIVConnectionLayerType, Q_MOVABLE_TYPE);
