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

#include "cmdentitygeometrychange.h"

#include "commandids.h"

#include <tab_aadl/aadlobjectsmodel.h>

static inline QVector<qint32> convertCoordinates(const QVector<QPointF> &points)
{
    QVector<qint32> coordinates;
    for (const QPointF &point : points)
        coordinates << qRound(point.x()) << qRound(point.y());
    return coordinates;
}

namespace taste3 {
namespace aadl {
namespace cmd {

CmdEntityGeometryChange::CmdEntityGeometryChange(AADLObject *object, const QVector<QPointF> &points)
    : QUndoCommand()
    , m_entity(object)
    , m_prevCoordinates(object->coordinates())
    , m_newCoordinates(convertCoordinates(points))
{
}

void CmdEntityGeometryChange::redo()
{
    if (m_entity)
        m_entity->setCoordinates(m_newCoordinates);
}

void CmdEntityGeometryChange::undo()
{
    if (m_entity)
        m_entity->setCoordinates(m_prevCoordinates);
}

bool CmdEntityGeometryChange::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdEntityGeometryChange::id() const
{
    return ChangeEntityGeometry;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
