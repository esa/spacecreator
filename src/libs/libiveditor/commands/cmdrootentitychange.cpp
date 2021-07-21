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

#include "cmdrootentitychange.h"

#include "commandids.h"

#include <ivmodel.h>

namespace ive {
namespace cmd {

CmdRootEntityChange::CmdRootEntityChange(ivm::IVModel *model, const shared::Id &id)
    : shared::cmd::CmdEntityGeometryChange({}, QObject::tr("Change root item"))
    , m_model(model)
    , m_newId(id)
    , m_prevId(model->rootObjectId())
{
    Q_ASSERT(model);
}

void CmdRootEntityChange::redo()
{
    if (m_model)
        m_model->setRootObject(m_newId);

    shared::cmd::CmdEntityGeometryChange::redo();
}

void CmdRootEntityChange::undo()
{
    shared::cmd::CmdEntityGeometryChange::undo();

    if (m_model)
        m_model->setRootObject(m_prevId);
}

int CmdRootEntityChange::id() const
{
    return ChangeRootEntity;
}

}
}
