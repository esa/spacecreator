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

#include <QPointer>
#include <memory>

namespace dvm {
struct DVNodePrivate;
class DVPartition;
class DVDevice;

class DVNode : public DVObject
{
    Q_OBJECT
public:
    explicit DVNode(DVObject *parent = nullptr);
    ~DVNode() override;

    void addPartition(DVPartition *partition);
    QList<DVPartition *> partitions() const;

    void addDevice(DVDevice *device);
    QList<QPointer<DVDevice>> devices() const;

private:
    std::unique_ptr<DVNodePrivate> d;
};

} // namespace deploy
