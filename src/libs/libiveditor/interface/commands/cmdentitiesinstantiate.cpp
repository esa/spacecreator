/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdentitiesinstantiate.h"

#include "aadlfunction.h"
#include "aadlfunctiontype.h"
#include "aadlmodel.h"
#include "aadlnamevalidator.h"
#include "baseitems/common/aadlutils.h"
#include "cmdentityattributechange.h"
#include "commandids.h"

static inline void shiftObjects(const QVector<ivm::AADLObject *> &objects, const QPointF &offset)
{
    for (auto obj : objects) {
        if (!obj) {
            continue;
        }
        auto points = ive::polygon(obj->coordinates());
        std::transform(
                points.cbegin(), points.cend(), points.begin(), [offset](const QPointF &p) { return p + offset; });
        obj->setCoordinates(ive::coordinates(points));
        if (obj->aadlType() == ivm::AADLObject::Type::FunctionType
                || obj->aadlType() == ivm::AADLObject::Type::Function) {
            shiftObjects(obj->as<ivm::AADLFunctionType *>()->children(), offset);
        }
    }
}

namespace ive {
namespace cmd {

CmdEntitiesInstantiate::CmdEntitiesInstantiate(
        ivm::AADLFunctionType *entity, ivm::AADLFunctionType *parent, ivm::AADLModel *model, const QPointF &pos)
    : QUndoCommand()
    , m_parent(parent)
    , m_model(model)

{
    Q_ASSERT(entity);
    m_instantiatedEntity =
            new ivm::AADLFunction({}, m_parent ? qobject_cast<QObject *>(m_parent) : qobject_cast<QObject *>(m_model));
    m_instantiatedEntity->setTitle(ivm::AADLNameValidator::nameForInstance(
            m_instantiatedEntity, entity->title() + QLatin1String("_Instance_")));
    m_instantiatedEntity->setCoordinates(entity->coordinates());

    const QRectF typeGeometry = ive::rect(entity->coordinates());
    m_offset = pos - typeGeometry.topLeft();
    const QString nameKey = ivm::meta::Props::token(ivm::meta::Props::Token::instance_of);
    m_subCmd = new CmdEntityAttributeChange(m_instantiatedEntity, { { nameKey, entity->title() } });
}

CmdEntitiesInstantiate::~CmdEntitiesInstantiate()
{
    delete m_subCmd;
    m_subCmd = nullptr;

    if (m_instantiatedEntity && !m_instantiatedEntity->parent())
        delete m_instantiatedEntity;
}

void CmdEntitiesInstantiate::redo()
{
    if (!m_instantiatedEntity.isNull()) {
        if (m_parent) {
            m_parent->addChild(m_instantiatedEntity);
        }
        m_model->addObject(m_instantiatedEntity);
        m_subCmd->redo();
        shiftObjects({ m_instantiatedEntity }, m_offset);
    }
}

void CmdEntitiesInstantiate::undo()
{
    if (!m_instantiatedEntity.isNull()) {
        shiftObjects({ m_instantiatedEntity }, -m_offset);
        m_subCmd->undo();
        m_model->removeObject(m_instantiatedEntity);
        if (m_parent) {
            m_parent->removeChild(m_instantiatedEntity);
        } else {
            m_instantiatedEntity->setParentObject(nullptr);
        }
    }
}

bool CmdEntitiesInstantiate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdEntitiesInstantiate::id() const
{
    return InstantiateEntities;
}

} // namespace ive
} // namespace cmd
