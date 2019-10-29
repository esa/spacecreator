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

#include "cmdfunctionitemcreate.h"

#include "commandids.h"

#include <QtDebug>
#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

static int sCounter = 0;

CmdFunctionItemCreate::CmdFunctionItemCreate(AADLObjectsModel *model, AADLObjectFunction *function,
                                             const QRectF &geometry)
    : QUndoCommand()
    , m_model(model)
    , m_geometry(geometry)
    , m_entity(new AADLObjectFunction(QObject::tr("Function_%1").arg(++sCounter), m_model))
    , m_parent(function)
{
}

void CmdFunctionItemCreate::redo()
{
    const QVector<qint32> coordinates {
        qRound(m_geometry.left()),
        qRound(m_geometry.top()),
        qRound(m_geometry.right()),
        qRound(m_geometry.bottom()),
    };
    m_entity->setCoordinates(coordinates);
    if (m_parent)
        m_parent->addChild(m_entity);
    if (m_model)
        m_model->addObject(m_entity);
}

void CmdFunctionItemCreate::undo()
{
    if (m_model)
        m_model->removeObject(m_entity);
    if (m_parent)
        m_parent->removeChild(m_entity);
}

bool CmdFunctionItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdFunctionItemCreate::id() const
{
    return CreateFunctionEntity;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
