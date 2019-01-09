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

#ifndef MSCCREATE_H
#define MSCCREATE_H

#include "mscinstanceevent.h"

namespace msc {

class MscInstance;

class MscCreate : public MscInstanceEvent
{
    Q_OBJECT

public:
    explicit MscCreate(QObject *parent = nullptr);
    MscCreate(const QString &name, QObject *parent = nullptr);

    const QStringList &parameters() const;
    void addParameter(const QString &parameter);

    MscInstance *instance() const;
    void setInstance(MscInstance *instance);

    MscEntity::EntityType entityType() const override;

private:
    MscInstance *m_instance = nullptr;
    QStringList m_parameters;
};

} // namespace msc

#endif // MSCCREATE_H
