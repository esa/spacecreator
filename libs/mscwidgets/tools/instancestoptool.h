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

#include "basetool.h"

namespace msc {
class MscChart;
class MscInstance;

class InstanceStopTool : public BaseTool
{
    Q_OBJECT
public:
    explicit InstanceStopTool(QGraphicsView *view, QObject *parent = nullptr);
    void setView(QGraphicsView *view) override;
    void setAction(QAction *action) override;

    BaseTool::ToolType toolType() const override;

protected:
    bool onMousePress(QMouseEvent *e) override;
    bool onMouseRelease(QMouseEvent *e) override;
    bool onMouseMove(QMouseEvent *e) override;

private:
    void setExplicitStop();
    void updateEnabledState();

private:
};

}; // ns msc
