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

#ifndef MSCMESSAGE_H
#define MSCMESSAGE_H

#include "mscentity.h"

#include <QObject>
#include <QString>

namespace msc {
class MscInstance;

class MscMessage : public MscEntity
{
    Q_OBJECT
public:
    explicit MscMessage(QObject *parent = nullptr);
    MscMessage(const QString &name, QObject *parent = nullptr);

    MscInstance *sourceInstance() const;
    void setSourceInstance(MscInstance *source);

    MscInstance *targetInstance() const;
    void setTargetInstance(MscInstance *target);

    MscEntity::EntityType entityType() const override;

Q_SIGNALS:
    void sourceChanged(MscInstance *source);
    void targetChanged(MscInstance *target);

private:
    MscInstance *m_source = nullptr;
    MscInstance *m_target = nullptr;
};

} // namespace msc

#endif // MSCMESSAGE_H
