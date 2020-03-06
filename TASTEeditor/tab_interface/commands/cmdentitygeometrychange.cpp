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

#include "app/commandsstack.h"
#include "commandids.h"

#include <QtDebug>
#include <baseitems/common/utils.h>
#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

CmdEntityGeometryChange::CmdEntityGeometryChange(const QList<QPair<AADLObject *, QVector<QPointF>>> &objectsData,
                                                 const QString &title)
    : QUndoCommand(title.isEmpty() ? QObject::tr("Change Geometry") : title)
    , m_internalData(objectsData)
    , m_data(convertData(m_internalData))
{
}

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

bool CmdEntityGeometryChange::mergeWith(const QUndoCommand *command)
{
    if (command->id() != AutoLayoutEntity)
        return false;

    auto cmd = static_cast<const CmdEntityGeometryChange *>(command);
    QList<ObjectData> newEntries;
    for (auto cmdData : cmd->m_data) {
        Q_ASSERT(cmdData.entity);
        if (!cmdData.entity)
            continue;

        const auto it =
                std::find_if(m_data.begin(), m_data.end(), [id = cmdData.entity->id()](const ObjectData &objData) {
                    return objData.entity->id() == id;
                });
        if (it == m_data.cend())
            newEntries.append(cmdData);
        else
            it->newCoordinates = cmdData.newCoordinates;
    }
    m_data.append(newEntries);
    return true;
}

int CmdEntityGeometryChange::id() const
{
    return ChangeEntityGeometry;
}

static inline int parentLevel(AADLObject *object)
{
    int idx = 0;
    while (auto parentObject = object->parentObject()) {
        ++idx;
        object = parentObject;
    }
    return idx;
}

QList<CmdEntityGeometryChange::ObjectData>
CmdEntityGeometryChange::convertData(const QList<QPair<AADLObject *, QVector<QPointF>>> &objectsData)
{
    QList<ObjectData> result;
    for (const auto &objectData : objectsData)
        result.append({ objectData.first, objectData.first->coordinates(), utils::coordinates(objectData.second) });

    std::sort(result.begin(), result.end(), [](const ObjectData &data1, const ObjectData &data2) {
        if (data1.entity->aadlType() == data2.entity->aadlType())
            return parentLevel(data1.entity) < parentLevel(data2.entity);

        return data1.entity->aadlType() < data2.entity->aadlType();
    });

    return result;
}

void CmdEntityGeometryChange::prepareData(const QList<QPair<AADLObject *, QVector<QPointF>>> &objectsData)
{
    m_data = convertData(objectsData);
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
