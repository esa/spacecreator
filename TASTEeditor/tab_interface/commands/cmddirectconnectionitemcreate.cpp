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
#include "tab_aadl/aadlobjectfunctiontype.h"
#include "tab_aadl/aadlobjectiface.h"
#include "tab_aadl/aadlobjectsmodel.h"

namespace taste3 {
namespace aadl {
namespace cmd {

CmdDirectConnectionItemCreate::CmdDirectConnectionItemCreate(
        AADLObjectsModel *model, AADLObjectFunctionType *startFunction, AADLObjectIfaceProvided *providedIface,
        const QPointF &startPoint, AADLObjectFunctionType *endFunction, AADLObjectIfaceRequired *requiredIface,
        const QPointF &endPoint)
    : QUndoCommand()
    , m_model(model)
    , m_startFunction(startFunction)
    , m_endFunction(endFunction)
    , m_providedIface(providedIface)
    , m_requiredIface(requiredIface)
    , m_entity(nullptr)
{
    Q_ASSERT(m_startFunction);
    Q_ASSERT(m_endFunction);

    if (!m_providedIface)
        m_providedIface = new AADLObjectIfaceProvided(m_startFunction);

    if (!startPoint.isNull())
        m_providedIface->setCoordinates({ qRound(startPoint.x()), qRound(startPoint.y()) });

    if (!m_requiredIface)
        m_requiredIface = new AADLObjectIfaceRequired(m_startFunction);

    if (!endPoint.isNull())
        m_requiredIface->setCoordinates({ qRound(endPoint.x()), qRound(endPoint.y()) });

    m_entity = new AADLObjectConnection(m_startFunction, m_endFunction, m_requiredIface, m_providedIface, m_model);
}

void CmdDirectConnectionItemCreate::redo()
{
    m_entity->setCoordinates({});

    m_startFunction->addPI(m_providedIface);
    m_endFunction->addRI(m_requiredIface);

    m_model->addObject(m_providedIface);
    m_model->addObject(m_requiredIface);
    m_model->addObject(m_entity);
}

void CmdDirectConnectionItemCreate::undo()
{
    m_startFunction->removePI(m_providedIface);
    m_endFunction->removeRI(m_requiredIface);

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
