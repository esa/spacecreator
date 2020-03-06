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

#include "baseitems/common/utils.h"
#include "commandids.h"
#include "tab_aadl/aadlobjectfunctiontype.h"
#include "tab_aadl/aadlobjectiface.h"
#include "tab_aadl/aadlobjectsmodel.h"

#include <QtMath>

namespace taste3 {
namespace aadl {
namespace cmd {

CmdConnectionItemCreate::CmdConnectionItemCreate(AADLObjectsModel *model, AADLObjectFunction *parent,
                                                 const common::Id sourceIfaceId, const common::Id &targetIfaceId,
                                                 const QVector<QPointF> &points)
    : CmdEntityGeometryChange({}, QObject::tr("Create Connection"))
    , m_model(model)
{
    Q_ASSERT(model);
    Q_ASSERT(!sourceIfaceId.isNull());
    Q_ASSERT(!targetIfaceId.isNull());

    AADLObjectIface *sourceIface = m_model->getInterface(sourceIfaceId);
    AADLObjectIface *targetIface = m_model->getInterface(targetIfaceId);
    m_entity = new AADLObjectConnection(sourceIface->function(), targetIface->function(), sourceIface, targetIface,
                                        parent);

    prepareData({ qMakePair(m_entity, points) });
}

void CmdConnectionItemCreate::redo()
{
    CmdEntityGeometryChange::redo();

    m_model->addObject(m_entity);
    m_entity->inheritLabel();
}

void CmdConnectionItemCreate::undo()
{
    CmdEntityGeometryChange::undo();

    m_entity->uninheritLabel();
    m_model->removeObject(m_entity);
}

int CmdConnectionItemCreate::id() const
{
    return CreateConnectionEntity;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
