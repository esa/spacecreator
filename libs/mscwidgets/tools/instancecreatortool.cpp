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

#include "commands/common/commandsstack.h"
#include "mscinstance.h"

#include <QDebug>

namespace msc {

InstanceCreatorTool::InstanceCreatorTool(ChartViewModel *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
{
    m_title = tr("Instance");
    m_description = tr("Create new Instance item");
    m_icon = QPixmap(":/icons/toolbar/instance.png");
}

void InstanceCreatorTool::setAction(QAction *action)
{
    if (m_action == action)
        return;

    if (m_action)
        disconnect(m_action, nullptr, this, nullptr);

    m_action = action;

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
    const QVariantList &cmdParams = { QVariant::fromValue<msc::MscInstance *>(instance),
                                      QVariant::fromValue<msc::MscChart *>(m_activeChart), pos };
    msc::cmd::CommandsStack::push(msc::cmd::Id::CreateInstance, cmdParams);
    Q_EMIT created();
}

}; // ns msc
