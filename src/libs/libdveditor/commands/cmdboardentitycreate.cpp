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

#include "cmdboardentitycreate.h"

#include "commandids.h"
#include "dvboard.h"
#include "dvmodel.h"

#include <QRectF>

namespace dve {
namespace cmd {

CmdBoardEntityCreate::CmdBoardEntityCreate(dvm::DVModel *model, const QRectF &geometry, const shared::Id &id)
    : shared::cmd::CmdEntityGeometryChange({}, QObject::tr("Create Board"))
    , m_model(model)
    , m_entity(new dvm::DVBoard)
{
    prepareData({ qMakePair(m_entity, QVector<QPointF> { geometry.topLeft(), geometry.bottomRight() }) });
}

CmdBoardEntityCreate::~CmdBoardEntityCreate()
{
    if (m_entity && !m_entity->parent()) {
        delete m_entity;
    }
}

void CmdBoardEntityCreate::redo()
{
    shared::cmd::CmdEntityGeometryChange::redo();

    m_entity->setParent(m_model);

    if (m_model)
        m_model->addObject(m_entity);
}

void CmdBoardEntityCreate::undo()
{
    shared::cmd::CmdEntityGeometryChange::undo();

    if (m_model)
        m_model->removeObject(m_entity);

    m_entity->setParent(this);
}

int CmdBoardEntityCreate::id() const
{
    return CreateBoardEntity;
}

} // namespace cmd
} // namespace dve
