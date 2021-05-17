/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdconnectionitemcreate.h"

#include "baseitems/common/ivutils.h"
#include "commandids.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivmodel.h"

#include <QtMath>

namespace ive {
namespace cmd {

CmdConnectionItemCreate::CmdConnectionItemCreate(ivm::IVModel *model, ivm::IVFunction *parent,
        const shared::Id sourceIfaceId, const shared::Id &targetIfaceId, const QVector<QPointF> &points)
    : shared::cmd::CmdEntityGeometryChange({}, QObject::tr("Create Connection"))
    , m_model(model)
    , m_parent(parent)
{
    Q_ASSERT(model);
    Q_ASSERT(!sourceIfaceId.isNull());
    Q_ASSERT(!targetIfaceId.isNull());

    ivm::IVInterface *sourceIface = m_model->getInterface(sourceIfaceId);
    ivm::IVInterface *targetIface = m_model->getInterface(targetIfaceId);
    m_entity = new ivm::IVConnection(sourceIface, targetIface, parent);
    prepareData({ qMakePair(m_entity, points) });
}

CmdConnectionItemCreate::~CmdConnectionItemCreate()
{
    if (m_entity && !m_entity->parent()) {
        delete m_entity;
    }
}

void CmdConnectionItemCreate::redo()
{
    shared::cmd::CmdEntityGeometryChange::redo();

    if (m_parent)
        m_parent->addChild(m_entity);
    m_model->addObject(m_entity);
    m_entity->setInheritPI();
}

void CmdConnectionItemCreate::undo()
{
    shared::cmd::CmdEntityGeometryChange::undo();

    m_entity->unsetInheritPI();
    m_model->removeObject(m_entity);
    if (m_parent)
        m_parent->removeChild(m_entity);
}

int CmdConnectionItemCreate::id() const
{
    return CreateConnectionEntity;
}

}
}
