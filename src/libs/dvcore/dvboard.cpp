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

#include "dvboard.h"

#include "dvdevice.h"

namespace dvm {

struct DVBoardPrivate {
    QList<DVPort *> ports;
};

DVBoard::DVBoard(DVObject *parent)
    : DVObject(DVObject::Type::Board, {}, parent)
    , d(std::make_unique<DVBoardPrivate>())
{
}

DVBoard::~DVBoard() { }

void DVBoard::addPort(DVPort *port)
{
    if (d->ports.contains(port))
        return;

    d->ports.append(port);
}

/*!
   Returns all devices/ports of this device
 */
QList<DVPort *> DVBoard::ports() const
{
    return d->ports;
}

} // namespace dvm
