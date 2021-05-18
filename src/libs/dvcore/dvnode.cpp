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

#include "dvdevice.h"
#include "dvprocessor.h"

#include <QPointer>

namespace dvm {

struct DVNodePrivate {
    QPointer<DVProcessor> cpu;
    QPointer<DVDevice> device;
};

DVNode::DVNode(DVObject *parent)
    : DVObject(DVObject::Type::Node, {}, parent)
    , d(std::make_unique<DVNodePrivate>())
{
}

DVNode::~DVNode() { }

void DVNode::setProcessor(DVProcessor *cpu)
{
    d->cpu = cpu;
}

DVProcessor *DVNode::processor() const
{
    return d->cpu;
}

void DVNode::setDevice(DVDevice *device)
{
    d->device = device;
}

DVDevice *DVNode::device() const
{
    return d->device;
}

} // namespace deploy
