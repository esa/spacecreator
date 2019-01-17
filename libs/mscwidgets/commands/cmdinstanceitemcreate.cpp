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

#include "cmdinstanceitemcreate.h"
#include "chartviewmodel.h"
#include "baseitems/common/utils.h"

#include <QGraphicsScene>
#include <QDebug>

namespace msc {
namespace cmd {

CmdInstanceItemCreate::CmdInstanceItemCreate(QGraphicsScene *scene, ChartViewModel *model, const QPointF &pos)
    : BaseCommand()
    , m_scene(scene)
    , m_model(model)
    , m_pos(pos)
{
}

bool CmdInstanceItemCreate::validateStorages(const char *caller) const
{
    if (!m_model || !m_model->currentChart() || !m_scene) {
        qWarning() << caller << "Model, chart or scene is null, aborting." << m_model
                   << (m_model ? m_model->currentChart() : nullptr) << m_scene;
        return false;
    }
    return true;
}

void CmdInstanceItemCreate::redo()
{
    if (!validateStorages(Q_FUNC_INFO))
        return;

    m_instanceItem = m_model->createDefaultInstanceItem(nullptr, m_pos);
    m_scene->addItem(m_instanceItem);
    m_instanceItem->ensureNotOverlapped();
}

void CmdInstanceItemCreate::undo()
{
    if (!validateStorages(Q_FUNC_INFO))
        return;

    if (m_model->removeInstanceItem(m_instanceItem))
        m_instanceItem = nullptr;
}

bool CmdInstanceItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdInstanceItemCreate::id() const
{
    return msc::cmd::Id::CreateInstance;
}

} // ns cmd
} // ns msc
