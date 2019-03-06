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

#ifndef TIMERCREATORTOOL_H
#define TIMERCREATORTOOL_H

#include "basecreatortool.h"
#include "msctimer.h"

namespace msc {

class TimerCreatorTool : public BaseCreatorTool
{
    Q_OBJECT
public:
    TimerCreatorTool(MscTimer::TimerType type, ChartViewModel *model, QGraphicsView *view, QObject *parent);
    BaseTool::ToolType toolType() const override;

protected Q_SLOTS:
    void onCurrentChartChagend(msc::MscChart *chart) override;

protected:
    void createPreviewItem() override;
    void commitPreviewItem() override;
    void removePreviewItem() override;

private:
    MscTimer::TimerType m_timerType = MscTimer::TimerType::Start;
};

} // namespace msc

#endif // TIMERCREATORTOOL_H
