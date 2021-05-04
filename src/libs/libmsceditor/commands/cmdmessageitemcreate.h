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

#include "chartbasecommand.h"
#include "chartindex.h"

#include <QHash>
#include <QPointF>
#include <QPointer>
#include <QVector>

namespace msc {

class ChartLayoutManager;
class MscChart;
class MscInstance;
class MscMessage;
class InstanceItem;

namespace cmd {

class CmdMessageItemCreate : public ChartBaseCommand
{
public:
    CmdMessageItemCreate(msc::MscMessage *message, const ChartIndexList &instanceIndexes,
            ChartLayoutManager *layoutManager, const QVector<QPoint> &points = QVector<QPoint>());

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    MscMessage *m_message = nullptr;
    ChartIndexList m_instanceIndexes;
    QVector<QPoint> m_msgPoints;

    struct InstanceGeometry {
        QVector<QPoint> m_cif;
        QPointF m_pos;
        qreal m_axis = 0.;
    };

    const InstanceGeometry m_sourceGeometryPrev;
    const InstanceGeometry m_targetGeometryPrev;

    static InstanceGeometry initGeometryHolder(msc::InstanceItem *from);
};

} // ns cmd
} // ns msc
