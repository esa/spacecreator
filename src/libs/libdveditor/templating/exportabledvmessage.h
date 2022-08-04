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

namespace dve {

class ExportableDVMessage : public ExportableDVObject
{
    Q_GADGET
    Q_PROPERTY(QString fromFunction READ fromFunction)
    Q_PROPERTY(QStringList fromFunctionPath READ fromFunctionPath)
    Q_PROPERTY(QString fromInterface READ fromInterface)
    Q_PROPERTY(QString toFunction READ toFunction)
    Q_PROPERTY(QStringList toFunctionPath READ toFunctionPath)
    Q_PROPERTY(QString toInterface READ toInterface)
public:
    ExportableDVMessage(const dvm::DVObject *dvObject = nullptr);

    QString fromFunction() const;
    QStringList fromFunctionPath() const;
    QString fromInterface() const;

    QString toFunction() const;
    QStringList toFunctionPath() const;
    QString toInterface() const;
};

} // namespace dve

Q_DECLARE_METATYPE(dve::ExportableDVMessage)
Q_DECLARE_TYPEINFO(dve::ExportableDVMessage, Q_MOVABLE_TYPE);
