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

#include "instancestoptool.h"

#include "baseitems/common/mscutils.h"
#include "baseitems/instanceenditem.h"
#include "chartlayoutmanager.h"
#include "commands/cmdinstancestopchange.h"
#include "instanceitem.h"
#include "msccommandsstack.h"
#include "mscinstance.h"

#include <QGraphicsScene>
#include <QGraphicsView>

namespace msc {

InstanceStopTool::InstanceStopTool(ChartLayoutManager *chartLayoutManager, QGraphicsView *view, QObject *parent)
    : BaseTool(view, parent)
    , m_model(chartLayoutManager)
{
    m_title = tr("Instance");
    m_description = tr("Stop Instance item");
    m_icon = QPixmap(":/icons/toolbar/instance_stop.svg");

    if (m_view) {
        connect(m_view->scene(), &QGraphicsScene::selectionChanged, this, &InstanceStopTool::updateEnabledState);
    }
    updateEnabledState();
}

void InstanceStopTool::setView(QGraphicsView *view)
{
    if (m_view)
        disconnect(m_view->scene(), nullptr, this, nullptr);

    BaseTool::setView(view);

    if (m_view) {
        connect(m_view->scene(), &QGraphicsScene::selectionChanged, this, &InstanceStopTool::updateEnabledState);
    }
    updateEnabledState();
}

bool InstanceStopTool::onMousePress(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool InstanceStopTool::onMouseRelease(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool InstanceStopTool::onMouseMove(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

BaseTool::ToolType InstanceStopTool::toolType() const
{
    return BaseTool::ToolType::InstanceStopper;
}

void InstanceStopTool::setAction(QAction *action)
{
    BaseTool::setAction(action);

    connect(m_action, &QAction::triggered, this, &InstanceStopTool::setExplicitStop);
    updateEnabledState();
}

void InstanceStopTool::setExplicitStop()
{
    if (!m_view || !m_view->scene())
        return;

    const auto instances = getEntityFromSelection<msc::InstanceItem, msc::MscInstance>(m_view->scene());
    if (instances.size() != 1)
        return;

    MscInstance *instance = instances.value(0);
    if (!instance)
        return;

    const bool newValue = !instance->explicitStop();
    instance->setExplicitStop(newValue);

    m_model->undoStack()->push(new cmd::CmdInstanceStopChange(instance, newValue, m_model));
}

void InstanceStopTool::updateEnabledState()
{
    if (!m_action)
        return;

    if (m_view && m_view->scene()) {
        const auto instances = getEntityFromSelection<msc::InstanceItem, msc::MscInstance>(m_view->scene());
        if (instances.size() == 1) {
            m_action->setEnabled(true);
            m_action->setChecked(instances.front()->explicitStop());
            return;
        }
    }
    m_action->setChecked(false);
    m_action->setEnabled(false);
}

}; // ns msc
