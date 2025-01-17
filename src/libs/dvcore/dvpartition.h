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
class DVFunction;
class DVNode;
struct DVPartitionPrivate;

class DVPartition : public DVObject
{
    Q_OBJECT
    Q_PROPERTY(QList<dvm::DVFunction *> functions READ functions)
    Q_PROPERTY(QStringList functionsNames READ functionsNames)
    Q_PROPERTY(dvm::DVObject *node READ node)

public:
    explicit DVPartition(DVObject *parent = nullptr, const shared::Id &id = shared::InvalidId);
    ~DVPartition() override;

    void addFunction(DVFunction *function);
    void removeFunction(DVFunction *function);
    QList<DVFunction *> functions() const;
    QStringList functionsNames() const;

    DVNode *node() const;

Q_SIGNALS:
    void functionAdded(shared::Id id);
    void functionRemoved(shared::Id id);
    void functionChanged(shared::Id id);

private:
    std::unique_ptr<DVPartitionPrivate> d;
};

} // namespace dvm

Q_DECLARE_METATYPE(dvm::DVPartition *)
