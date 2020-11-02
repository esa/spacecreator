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

#include "instancecreatortool.h"

#include "chartlayoutmanager.h"
#include "commands/cmdinstanceitemcreate.h"
#include "msccommandsstack.h"
#include "mscinstance.h"

#include <QDebug>

namespace msc {

InstanceCreatorTool::InstanceCreatorTool(ChartLayoutManager *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
{
    m_title = tr("Instance");
    m_description = tr("Create new Instance item");
    m_icon = QIcon(":/icons/toolbar/instance.png");
}

void InstanceCreatorTool::setAction(QAction *action)
{
    BaseTool::setAction(action);

    if (m_action)
        connect(m_action, &QAction::triggered, this, &InstanceCreatorTool::onActionTriggered);
}

bool InstanceCreatorTool::onMousePress(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool InstanceCreatorTool::onMouseRelease(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool InstanceCreatorTool::onMouseMove(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

BaseTool::ToolType InstanceCreatorTool::toolType() const
{
    return BaseTool::ToolType::InstanceCreator;
}

void InstanceCreatorTool::onActionTriggered(bool activated)
{
    if (!activated)
        return;

    if (!m_activeChart)
        return;

    MscInstance *instance = m_activeChart->makeInstance();
    startWaitForModelLayoutComplete(instance);

    static constexpr int pos { -1 };
    m_model->undoStack()->push(new cmd::CmdInstanceItemCreate(instance, pos, m_model));
    Q_EMIT created();
}

}; // ns msc
