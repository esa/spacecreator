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

#include "cmdcommentitemcreate.h"

#include "commandids.h"

#include <baseitems/common/utils.h>
#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

CmdCommentItemCreate::CmdCommentItemCreate(AADLObjectsModel *model, AADLObjectFunctionType *parent,
                                           const QRectF &geometry)
    : m_model(model)
    , m_parent(parent)
    , m_entity(new AADLObjectComment(QObject::tr("Comment"), m_parent))
    , m_geometry(geometry)
{
    setText(QObject::tr("Create Comment"));
}

void CmdCommentItemCreate::redo()
{
    m_entity->setCoordinates(utils::coordinates(m_geometry));

    if (m_parent)
        m_parent->addChild(m_entity);
    if (m_model)
        m_model->addObject(m_entity);
}

void CmdCommentItemCreate::undo()
{
    if (m_model)
        m_model->removeObject(m_entity);
    if (m_parent)
        m_parent->removeChild(m_entity);
}

bool CmdCommentItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdCommentItemCreate::id() const
{
    return CreateCommentEntity;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
