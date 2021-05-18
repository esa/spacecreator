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

#include "dvpartition.h"

#include "dvfunction.h"

#include <QStringList>

namespace dvm {

struct DVPartitionPrivate {
    QList<QPointer<DVFunction>> functions;
};

DVPartition::DVPartition(DVObject *parent)
    : DVObject(DVObject::Type::Partition, {}, parent)
    , d(new DVPartitionPrivate)
{
}

DVPartition::~DVPartition() { }

void DVPartition::addFunction(DVFunction *function)
{
    d->functions.append(function);
}

QList<QPointer<DVFunction>> DVPartition::functions() const
{
    return d->functions;
}

QStringList DVPartition::functionsNames() const
{
    QStringList names;
    for (DVFunction *fn : qAsConst(d->functions)) {
        names.append(fn->title());
    }
    return names;
}

} // namespace deploy
