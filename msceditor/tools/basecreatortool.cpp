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

#include "basecreatortool.h"
#include "mscchart.h"

namespace msc {

BaseCreatorTool::BaseCreatorTool(ChartViewModel *model, QGraphicsView *view, QObject *parent)
    : BaseTool(view, parent)
    , m_model(model)
    , m_activeChart(m_model->currentChart())

{
    connect(model, &ChartViewModel::currentChartChanged, this, &BaseCreatorTool::onCurrentChartChagend);
}

void BaseCreatorTool::setModel(ChartViewModel *model)
{
    m_model = model;
}

void BaseCreatorTool::onCurrentChartChagend(msc::MscChart *chart)
{
    m_activeChart = chart;
    if (isActive()) {
        createPreviewItem();
    }
}

} // ns msc
