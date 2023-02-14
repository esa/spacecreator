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
#include "dvport.h"

#include <memory>
#include <qobjectlistmodelt.h>

namespace dvm {
struct DVBoardPrivate;
class DVSystemFunction;

class DVBoard : public DVObject
{
    Q_OBJECT
    Q_PROPERTY(QList<dvm::DVPort *> ports READ ports)

public:
    explicit DVBoard(DVObject *parent = nullptr, const shared::Id &id = shared::InvalidId);
    ~DVBoard() override;

    virtual QString titleUI() const override;

    void addPort(DVPort *port);

    QList<DVPort *> ports() const;

    QList<DVSystemFunction *> systemFunctions() const;

private:
    std::unique_ptr<DVBoardPrivate> d;
};

} // namespace dvm

Q_DECLARE_METATYPE(dvm::DVBoard *)
