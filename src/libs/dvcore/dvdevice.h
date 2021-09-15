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
class DVNode;
class DVPort;

class DVDevice : public DVObject
{
    Q_OBJECT
    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portChanged)
    Q_PROPERTY(QString qualifier READ qualifier WRITE setQualifier NOTIFY qualifierChanged)
    Q_PROPERTY(dvm::DVNode *node READ node)

public:
    explicit DVDevice(const DVPort &port, DVObject *parent = nullptr);
    explicit DVDevice(DVObject *parent = nullptr);

    DVNode *node() const;

    QString portName() const;
    void setPortName(const QString &name);

    QString qualifier() const;
    void setQualifier(const QString &name);

Q_SIGNALS:
    void portChanged(const QString &port);
    void qualifierChanged(const QString &qualifier);
};

} // namespace dvm

Q_DECLARE_METATYPE(dvm::DVDevice *)
