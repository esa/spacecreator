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

#include "cif/cifline.h"

#include <QVector>

namespace msc {
namespace cif {

class CifBlock
{
public:
    virtual ~CifBlock();
    QVector<CifLineShared> lines() const;
    virtual void setLines(const QVector<CifLineShared> &lines);

    QString hashKey() const;

    virtual bool isPeculiar() const;

    virtual QVariant payload(CifLine::CifType forType = CifLine::CifType::Unknown) const;
    virtual void setPayload(const QVariant &p, CifLine::CifType forType = CifLine::CifType::Unknown);

    virtual CifLine::CifType blockType() const = 0;

protected:
    QVector<CifLineShared> m_lines;
    QString m_hashKey;
    QMap<CifLine::CifType, CifLineShared> m_linesByType;

    void updateHashKey();
    void addLine(const CifLineShared &line);
};

typedef QSharedPointer<CifBlock> CifBlockShared;

} // ns cif
} // ns msc
