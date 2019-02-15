/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#ifndef MSCCOREGION_H
#define MSCCOREGION_H

#include "mscinstanceevent.h"

namespace msc {

class MscCoregion : public MscInstanceEvent
{
    Q_OBJECT
    Q_PROPERTY(MscCoregion::Type type READ type WRITE setType NOTIFY typeChanged)

public:
    enum class Type
    {
        Begin,
        End
    };

    explicit MscCoregion(QObject *parent = nullptr);
    explicit MscCoregion(Type type, QObject *parent = nullptr);

    MscEntity::EntityType entityType() const { return MscEntity::EntityType::Coregion; }

    void setType(Type type);
    Type type() const { return m_type; }

    bool relatesTo(const MscInstance *instance) const override;

Q_SIGNALS:
    void typeChanged();

private:
    Type m_type;
};
}

#endif // MSCCOREGION_H
