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

#include "cmdmanualconnectionitemcreate.h"

#include "commandids.h"
#include "tab_aadl/aadlobjectfunctiontype.h"
#include "tab_aadl/aadlobjectiface.h"
#include "tab_aadl/aadlobjectsmodel.h"

#include <QtMath>

namespace taste3 {
namespace aadl {
namespace cmd {

CmdManualConnectionItemCreate::CmdManualConnectionItemCreate(AADLObjectsModel *model, AADLObjectFunction *startFunction,
                                                             AADLObjectFunction *endFunction,
                                                             AADLObjectIfaceProvided *providedIface,
                                                             AADLObjectIfaceRequired *requiredIface,
                                                             const QVector<QPointF> &points)
    : m_model(model)
    , m_startFunction(startFunction)
    , m_endFunction(endFunction)
    , m_providedIface(providedIface)
    , m_requiredIface(requiredIface)
    , m_entity(new AADLObjectConnection(m_startFunction, m_endFunction, m_requiredIface, m_providedIface, m_model))
{
    Q_ASSERT(m_startFunction);
    Q_ASSERT(m_endFunction);
    Q_ASSERT(m_providedIface);
    Q_ASSERT(m_requiredIface);

    for (auto it = points.constBegin(); it != points.constEnd(); ++it)
        m_coordinates << qRound(it->x()) << qRound(it->y());

    setText(QObject::tr("Create Connection"));
}

void CmdManualConnectionItemCreate::redo()
{
    m_entity->setCoordinates(m_coordinates);
    m_model->addObject(m_entity);
}

void CmdManualConnectionItemCreate::undo()
{
    m_model->removeObject(m_entity);
}

bool CmdManualConnectionItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdManualConnectionItemCreate::id() const
{
    return CreateManualConnectionEntity;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
