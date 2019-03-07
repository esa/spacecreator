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

#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QVariant>

namespace msc {
namespace cif {

class CifLine : public QObject
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

    CifLine(CifType type, QObject *parent = nullptr);
    virtual ~CifLine();

    CifType entityType() const;

    virtual bool initFrom(const QString &sourceLine); // TODO: make it pure virtual

    static QString nameForType(CifLine::CifType t);
    static CifLine::CifType typeForName(const QString &name);

    bool operator==(const CifLine &other) const;

    QString sourceLine() const;

    QVariant payload() const;
    void setPayload(const QVariant &p);

protected:
    const CifType m_type;
    QString m_sourceLine;
    QVariant m_payload;
};

typedef QSharedPointer<CifLine> CifLineShared;

} // ns cif
} // ns msc
