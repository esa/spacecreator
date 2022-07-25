/*
   Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "archetypecommonprops.h"
#include "archetypeobject.h"

namespace ivm {

class ParameterArchetypePrivate;
class ParameterArchetype : public ArchetypeObject
{
    Q_OBJECT
public:
    enum class ParameterDirection
    {
        IN = 0,
        OUT
    };
    Q_ENUM(ParameterDirection)

    explicit ParameterArchetype(const QString &title, QObject *parent = nullptr, const QString &type = QString(),
            const ParameterDirection &direction = ParameterDirection::IN);
    ~ParameterArchetype() override;

    QString getType() const;
    void setType(const QString &type);

    ParameterDirection getDirection() const;
    void setDirection(const ParameterDirection &direction);

    static QString directionToString(const ParameterDirection &direction);
    static ParameterDirection directionFromString(
            const QString &directionString, const ParameterDirection &defaultKind = ParameterDirection::IN);

private:
    const std::unique_ptr<ParameterArchetypePrivate> m_parameterPrivate;
};

}
