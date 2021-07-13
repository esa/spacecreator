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

#include "dvnode.h"

#include "dvboard.h"
#include "dvdevice.h"

#include <QPointer>

namespace dvm {

struct DVNodePrivate {
    QList<DVPartition *> partitions;
    QList<QPointer<DVDevice>> devices;
};

DVNode::DVNode(const DVBoard &board, DVObject *parent)
    : DVNode(parent)
{
    setEntityAttributes(board.entityAttributes());
}

DVNode::DVNode(DVObject *parent)
    : DVObject(DVObject::Type::Node, {}, parent)
    , d(std::make_unique<DVNodePrivate>())
{
}

DVNode::~DVNode() { }

void DVNode::addPartition(DVPartition *partition)
{
    if (!d->partitions.contains(partition)) {
        d->partitions.append(partition);
    }
}

void DVNode::removePartition(DVPartition *partition)
{
    d->partitions.removeAll(partition);
}

QList<DVPartition *> DVNode::partitions() const
{
    return d->partitions;
}

void DVNode::addDevice(DVDevice *device)
{
    if (!d->devices.contains(device)) {
        d->devices.append(device);
    }
}

void DVNode::removeDevice(DVDevice *device)
{
    d->devices.removeAll(device);
}

QList<QPointer<dvm::DVDevice>> DVNode::devices() const
{
    return d->devices;
}

} // namespace dvm
