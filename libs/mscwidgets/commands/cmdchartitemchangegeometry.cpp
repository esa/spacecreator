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

#include "cmdchartitemchangegeometry.h"

#include "baseitems/common/coordinatesconverter.h"
#include "chartitem.h"
#include "mscchart.h"

namespace msc {
namespace cmd {

CmdChartItemChangeGeometry::CmdChartItemChangeGeometry(const QRectF &oldRect, const QRectF &newRect, MscChart *chart)
    : BaseCommand(chart)
    , m_chart(chart)
    , m_oldRect(oldRect)
    , m_newRect(newRect)
{
}

void CmdChartItemChangeGeometry::redo()
{
    //    if (m_chart)
    //        m_chart->setCifRect(m_newRect);
    if (ChartItem *chartItem = utils::CoordinatesConverter::currentChartItem())
        chartItem->setBox(m_newRect);
}

void CmdChartItemChangeGeometry::undo()
{
    //    if (m_chart)
    //        m_chart->setCifRect(m_oldRect);
    if (ChartItem *chartItem = utils::CoordinatesConverter::currentChartItem())
        chartItem->setBox(m_oldRect);
}

bool CmdChartItemChangeGeometry::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    m_newRect = static_cast<const CmdChartItemChangeGeometry *>(command)->m_newRect;
    return true;
}

int CmdChartItemChangeGeometry::id() const
{
    return msc::cmd::Id::ChangeChartGeometry;
}

} // namespace msc
} // namespace msc
