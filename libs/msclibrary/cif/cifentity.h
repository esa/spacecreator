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

#pragma once

#include <QMap>
#include <QObject>
#include <QSharedPointer>

namespace msc {
namespace cif {

class CifEntity : public QObject
{
    Q_OBJECT

public:
    enum class CifType
    {
        Action = 0,
        Call,
        Comment,
        Condition,
        Create,
        Collapsed,
        End,
        HyperLink,
        Instance,
        Import,
        Keep,
        LastModified,
        Message,
        MscDocument,
        Modified,
        MscPageSize,
        Nested,
        Position,
        Preview,
        Reset,
        Set,
        Stop,
        Submsc,
        Specific,
        Text,
        Timeout,
        TextMode,
        TextName,

        Unknown
    };
    Q_ENUM(CifType)

    CifEntity(CifType type, QObject *parent = nullptr);

    CifType entityType() const;

    virtual bool initFrom(const QString &sourceLine); // TODO: made it pure virtual

    static QString nameForType(CifEntity::CifType t);
    static CifEntity::CifType typeForName(const QString &name);

protected:
    const CifType m_type;
};

typedef QSharedPointer<CifEntity> CifEntityShared;
} // ns cif
} // ns msc
