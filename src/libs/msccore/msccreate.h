/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscmessage.h"

namespace msc {

class MscCreate : public MscMessage
{
    Q_OBJECT

public:
    explicit MscCreate(QObject *parent = nullptr);
    explicit MscCreate(const QString &name, QObject *parent = nullptr);
    MscCreate(const QString &name, MscInstance *source, MscInstance *target, QObject *parent = nullptr);

    MscEntity::EntityType entityType() const override;
};

} // namespace msc
