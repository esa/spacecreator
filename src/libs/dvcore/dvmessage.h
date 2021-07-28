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

namespace dvm {

class DVMessage : public DVObject
{
    Q_OBJECT
public:
    explicit DVMessage(QObject *parent = nullptr);

    QString fromFunction() const;
    void setFromFunction(const QString &from);

    QString fromInterface() const;
    void setFromInterface(const QString &from);

    QString toFunction() const;
    void setToFunction(const QString &to);

    QString toInterface() const;
    void setToInterface(const QString &to);
};

} // namespace dvm
