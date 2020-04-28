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
#include "aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"

#include <QtMath>

namespace aadlinterface {
namespace cmd {

CmdConnectionItemCreate::CmdConnectionItemCreate(aadl::AADLObjectsModel *model, aadl::AADLObjectFunction *parent,
                                                 const utils::Id sourceIfaceId, const utils::Id &targetIfaceId,
                                                 const QVector<QPointF> &points)
    : CmdEntityGeometryChange({}, QObject::tr("Create Connection"))
    , m_model(model)
    , m_parent(parent)
{
    Q_ASSERT(model);
    Q_ASSERT(!sourceIfaceId.isNull());
    Q_ASSERT(!targetIfaceId.isNull());

    aadl::AADLObjectIface *sourceIface = m_model->getInterface(sourceIfaceId);
    aadl::AADLObjectIface *targetIface = m_model->getInterface(targetIfaceId);
    m_entity = new aadl::AADLObjectConnection(sourceIface->function(), targetIface->function(), sourceIface, targetIface, parent);
    prepareData({ qMakePair(m_entity, points) });
}

void CmdConnectionItemCreate::redo()
{
    CmdEntityGeometryChange::redo();

    if (m_parent)
        m_parent->addChild(m_entity);
    m_model->addObject(m_entity);
    m_entity->setInheritPI();
}

void CmdConnectionItemCreate::undo()
{
    CmdEntityGeometryChange::undo();

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
