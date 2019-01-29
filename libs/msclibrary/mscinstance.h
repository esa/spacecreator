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

#ifndef MSCINSTANCE_H
#define MSCINSTANCE_H

#include "mscentity.h"

namespace msc {

class MscInstance : public MscEntity
{
    Q_OBJECT

public:
    explicit MscInstance(QObject *parent = nullptr);
    MscInstance(const QString &name, QObject *parent = nullptr);

    const QString &denominator() const;
    void setDenominator(const QString &denominator);

    const QString &kind() const;
    void setKind(const QString &kind);

    const QString &inheritance() const;
    void setInheritance(const QString &inheritance);

    MscEntity::EntityType entityType() const override;

    MscInstance *explicitCreator() const;
    void setExplicitCreator(MscInstance *creator);

    bool explicitStop() const;
    void setExplicitStop(bool stop);

Q_SIGNALS:
    void denominatorChanged(const QString &denominator);
    void kindChanged(const QString &kind);
    void inheritanceChanged(const QString &inheritance);

private:
    QString m_denominator;
    QString m_kind;
    QString m_inheritance;
    MscInstance *m_explicitCreator = nullptr;
    bool m_explicitStop = false;
};

} // namespace msc

#endif // MSCINSTANCE_H
