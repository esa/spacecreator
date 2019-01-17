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

#include "cmdmessageitemcreate.h"
#include "chartviewmodel.h"
#include "baseitems/common/utils.h"
#include "baseitems/arrowitem.h"

#include <mscchart.h>

#include <QGraphicsScene>
#include <QDebug>

namespace msc {
namespace cmd {

CmdMessageItemCreate::CmdMessageItemCreate(QGraphicsScene *scene, ChartViewModel *model, const QPointF &pos)
    : BaseCommand()
    , m_scene(scene)
    , m_model(model)
    , m_pos(pos)
{
}

bool CmdMessageItemCreate::validateStorages(const char *caller) const
{
    if (!m_model || !m_model->currentChart() || !m_scene) {
        qWarning() << caller << "Model, chart or scene is null, aborting." << m_model
                   << (m_model ? m_model->currentChart() : nullptr) << m_scene;
        return false;
    }
    return true;
}

void CmdMessageItemCreate::redo()
{
    if (!validateStorages(Q_FUNC_INFO))
        return;

    m_messageItem = m_model->createDefaultMessageItem(nullptr, m_pos);
    m_scene->addItem(m_messageItem);
    m_messageItem->performSnap();
    m_model->relayout();
}

void CmdMessageItemCreate::undo()
{
    if (!validateStorages(Q_FUNC_INFO))
        return;

    if (m_model->removeMessageItem(m_messageItem))
        m_messageItem = nullptr;
}

bool CmdMessageItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdMessageItemCreate::id() const
{
    return msc::cmd::Id::CreateMessage;
}

} // ns cmd
} // ns msc
