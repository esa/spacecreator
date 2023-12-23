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
#include "topohelper/geometry.h"
#include "veobject.h"

namespace shared {
namespace cmd {

CmdEntityGeometryChange::CmdEntityGeometryChange(
        const QList<QPair<shared::VEObject *, QVector<QPointF>>> &objectsData, const QString &title)
    : UndoCommand(title.isEmpty() ? QObject::tr("Change item(s) geometry/position") : title)
    , m_data(convertData(objectsData))
{
}

CmdEntityGeometryChange::~CmdEntityGeometryChange() { }

void CmdEntityGeometryChange::redo()
{
    for (auto it = m_data.cbegin(); it != m_data.cend(); ++it) {
        if (!it->entity)
            continue;

        it->entity->setCoordinates(it->newCoordinates);
    }
}

void CmdEntityGeometryChange::undo()
{
    for (auto it = m_data.cbegin(); it != m_data.cend(); ++it) {
        if (!it->entity)
            continue;

        it->entity->setCoordinates(it->prevCoordinates);
    }
}

int CmdEntityGeometryChange::id() const
{
    return ChangeEntityGeometry;
}

bool CmdEntityGeometryChange::mergeGeometryData(const QList<QPair<shared::VEObject *, QVector<QPointF>>> &objectsData)
{
    if (objectsData.isEmpty()) {
        return false;
    }

    const QList<ObjectData> data = convertData(objectsData);
    if (data.isEmpty()) {
        return false;
    }

    for (const ObjectData &objGeometryData : data) {
        if (!m_data.isEmpty()) {
            const ObjectData &objData = m_data.last();
            if (objData.entity == objGeometryData.entity) {
                if (objData.newCoordinates != objGeometryData.newCoordinates
                        || objData.prevCoordinates != objGeometryData.prevCoordinates) {
                    objData.entity->setCoordinates(objGeometryData.newCoordinates);
                }
                continue;
            }
        }

        m_data.append(objGeometryData);
        objGeometryData.entity->setCoordinates(objGeometryData.newCoordinates);
    }

    return true;
}

QList<CmdEntityGeometryChange::ObjectData> CmdEntityGeometryChange::convertData(
        const QList<QPair<shared::VEObject *, QVector<QPointF>>> &objectsData)
{
    QList<ObjectData> result;
    for (const auto &objectData : objectsData)
        result.append(
                { objectData.first, objectData.first->coordinates(), topohelp::geom::coordinates(objectData.second) });

    return result;
}

void CmdEntityGeometryChange::prepareData(const QList<QPair<shared::VEObject *, QVector<QPointF>>> &objectsData)
{
    m_data = convertData(objectsData);
}

bool CmdEntityGeometryChange::ObjectData::operator==(const ObjectData &data) const
{
    return data.entity == entity && data.newCoordinates == newCoordinates && data.prevCoordinates == prevCoordinates;
}
}
}
