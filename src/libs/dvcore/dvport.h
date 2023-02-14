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
class DVBoard;

class DVPort : public DVObject
{
    Q_OBJECT
    Q_PROPERTY(dvm::DVObject *board READ board)
    Q_PROPERTY(QString busName READ busName)

public:
    explicit DVPort(QObject *parent = nullptr, const shared::Id &id = shared::InvalidId);

    QString title() const override;
    DVBoard *board() const;
    QString busName() const;
};

} // namespace dvm

Q_DECLARE_METATYPE(dvm::DVPort *)
