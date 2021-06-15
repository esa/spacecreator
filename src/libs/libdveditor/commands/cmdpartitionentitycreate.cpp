/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdpartitionentitycreate.h"

#include "commandids.h"
#include "dvmodel.h"
#include "dvnode.h"
#include "dvpartition.h"

#include <QRectF>

namespace dve {
namespace cmd {

CmdPartitionEntityCreate::CmdPartitionEntityCreate(
        dvm::DVModel *model, dvm::DVNode *parent, const QRectF &geometry, const shared::Id &id)
    : shared::cmd::CmdEntityGeometryChange({}, QObject::tr("Create Partition"))
    , m_model(model)
    , m_parent(parent)
    , m_entity(new dvm::DVPartition(m_parent))
{
    m_entity->setTitle(tr("Partition_%1").arg(parent->partitions().size() + 1));
    prepareData({ qMakePair(m_entity, QVector<QPointF> { geometry.topLeft(), geometry.bottomRight() }) });
}

CmdPartitionEntityCreate::~CmdPartitionEntityCreate()
{
    if (m_entity && !m_entity->parent()) {
        delete m_entity;
    }
}

void CmdPartitionEntityCreate::redo()
{
    shared::cmd::CmdEntityGeometryChange::redo();

    m_entity->setParent(m_parent);
    if (m_parent)
        m_parent->addPartition(m_entity);
    if (m_model)
        m_model->addObject(m_entity);
}

void CmdPartitionEntityCreate::undo()
{
    shared::cmd::CmdEntityGeometryChange::undo();

    if (m_model)
        m_model->removeObject(m_entity);
    if (m_parent)
        m_parent->removePartition(m_entity);
    m_entity->setParent(this);
}

int CmdPartitionEntityCreate::id() const
{
    return CreatePartitionEntity;
}

} // namespace cmd
} // namespace dve
