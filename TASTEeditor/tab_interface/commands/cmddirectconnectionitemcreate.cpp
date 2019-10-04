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

#include "cmddirectconnectionitemcreate.h"

#include "commandids.h"
#include "tab_aadl/aadlobjectcontainer.h"
#include "tab_aadl/aadlobjectiface.h"
#include "tab_aadl/aadlobjectsmodel.h"

namespace taste3 {
namespace aadl {
namespace cmd {

CmdDirectConnectionItemCreate::CmdDirectConnectionItemCreate(AADLObjectsModel *model,
                                                             AADLObjectContainer *startContainer,
                                                             AADLObjectContainer *endContainer,
                                                             const QPointF &startPoint, const QPointF &endPoint)
    : QUndoCommand()
    , m_model(model)
    , m_startContainer(startContainer)
    , m_endContainer(endContainer)
    , m_providedIface(nullptr)
    , m_requiredIface(nullptr)
    , m_entity(nullptr)
{
    Q_ASSERT(m_startContainer);
    Q_ASSERT(m_endContainer);

    m_providedIface = new AADLObjectIfaceProvided(m_startContainer);
    m_providedIface->setCoordinates({ qRound(startPoint.x()), qRound(startPoint.y()) });

    m_requiredIface = new AADLObjectIfaceRequired(m_startContainer);
    m_requiredIface->setCoordinates({ qRound(endPoint.x()), qRound(endPoint.y()) });

    m_entity = new AADLObjectConnection(m_startContainer, m_endContainer, m_requiredIface, m_providedIface, m_model);
}

void CmdDirectConnectionItemCreate::redo()
{
    m_entity->setCoordinates({});

    m_startContainer->addPI(m_providedIface);
    m_endContainer->addRI(m_requiredIface);

    m_model->addObject(m_providedIface);
    m_model->addObject(m_requiredIface);
    m_model->addObject(m_entity);
}

void CmdDirectConnectionItemCreate::undo()
{
    m_startContainer->removePI(m_providedIface);
    m_endContainer->removeRI(m_requiredIface);

    m_model->removeObject(m_entity);
    m_model->removeObject(m_providedIface);
    m_model->removeObject(m_requiredIface);
}

bool CmdDirectConnectionItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdDirectConnectionItemCreate::id() const
{
    return CreateDirectConnectionEntity;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
