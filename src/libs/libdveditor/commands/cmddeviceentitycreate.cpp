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

#include "cmddeviceentitycreate.h"

#include "commandids.h"
#include "dvdevice.h"
#include "dvmodel.h"
#include "dvnode.h"

namespace dve {
namespace cmd {

CmdDeviceEntityCreate::CmdDeviceEntityCreate(
        dvm::DVModel *model, dvm::DVNode *parent, const QPointF &pos, const shared::Id &id)
    : shared::cmd::CmdEntityGeometryChange({}, QObject::tr("Create Device"))
    , m_model(model)
    , m_parent(parent)
{
    prepareData({ qMakePair<shared::VEObject *, QVector<QPointF>>(m_entity, { pos }) });
}

CmdDeviceEntityCreate::~CmdDeviceEntityCreate()
{
    if (m_entity && !m_entity->parent()) {
        delete m_entity;
    }
}

void CmdDeviceEntityCreate::redo()
{
    shared::cmd::CmdEntityGeometryChange::redo();

    m_entity->setParent(m_parent);
    if (m_parent)
        m_parent->addDevice(m_entity);

    if (m_model)
        m_model->addObject(m_entity);
}

void CmdDeviceEntityCreate::undo()
{
    if (m_parent)
        m_parent->removeDevice(m_entity);
    if (m_model)
        m_model->removeObject(m_entity);
    m_entity->setParent(this);

    shared::cmd::CmdEntityGeometryChange::undo();
}

int CmdDeviceEntityCreate::id() const
{
    return CreateDeviceEntity;
}

} // namespace cmd
} // namespace dve
