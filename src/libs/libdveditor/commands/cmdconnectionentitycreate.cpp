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

#include "cmdconnectionentitycreate.h"

#include "commandids.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvmodel.h"

namespace dve {
namespace cmd {

CmdConnectionEntityCreate::CmdConnectionEntityCreate(dvm::DVModel *model, const QVector<QPointF> &points,
        const shared::Id sourceDeviceId, const shared::Id &targetDeviceId, const shared::Id &id)
    : shared::cmd::CmdEntityGeometryChange({}, QObject::tr("Create Connection"))
    , m_model(model)
{
    auto sourceDevice = qobject_cast<dvm::DVDevice *>(m_model->getObject(sourceDeviceId));
    auto targetDevice = qobject_cast<dvm::DVDevice *>(m_model->getObject(targetDeviceId));
    m_entity = new dvm::DVConnection(sourceDevice, targetDevice);
    prepareData({ qMakePair(m_entity, points) });
}

CmdConnectionEntityCreate::~CmdConnectionEntityCreate()
{
    if (m_entity && !m_entity->parent()) {
        delete m_entity;
    }
}

void CmdConnectionEntityCreate::redo()
{
    shared::cmd::CmdEntityGeometryChange::redo();

    m_entity->setParent(m_model);

    if (m_model)
        m_model->addObject(m_entity);
}

void CmdConnectionEntityCreate::undo()
{
    if (m_model)
        m_model->removeObject(m_entity);

    m_entity->setParent(this);

    shared::cmd::CmdEntityGeometryChange::undo();
}

int CmdConnectionEntityCreate::id() const
{
    return CreateConnectionEntity;
}

} // namespace cmd
} // namespace dve
