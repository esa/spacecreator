/*
  Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see
  <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "cmdcontextparameterremove.h"

#include "commandids.h"

#include <QDebug>
#include <ivfunctiontype.h>
#include <ivmodel.h>

namespace ive {
namespace cmd {

CmdContextParameterRemove::CmdContextParameterRemove(ivm::IVFunctionType *entity, int at)
    : QUndoCommand()
    , m_entity(entity)
    , m_params()
{
    if (m_entity)
        if (at >= 0 && at < m_entity->contextParams().size())
            m_params.insert(at, m_entity->contextParams().at(at));

    setText(QObject::tr("Remove Context Parameter"));
}

void CmdContextParameterRemove::redo()
{
    if (!m_entity)
        return;

    for (const shared::ContextParameter &param : m_params)
        m_entity->removeContextParam(param);
}

void CmdContextParameterRemove::undo()
{
    if (!m_entity)
        return;

    QVector<shared::ContextParameter> params = m_entity->contextParams();

    QList<int> ids = m_params.keys();
    std::sort(ids.begin(), ids.end());
    for (int i : ids) {
        const shared::ContextParameter &param = m_params.value(i);
        params.insert(i, param);
    }
    m_entity->setContextParams(params);
}

bool CmdContextParameterRemove::mergeWith(const QUndoCommand *command)
{
    if (command->id() == id()) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        m_params.insert(static_cast<const CmdContextParameterRemove *>(command)->m_params);
#else
        m_params.unite(static_cast<const CmdContextParameterRemove *>(command)->m_params);
#endif
        return true;
    }
    return false;
}

int CmdContextParameterRemove::id() const
{
    return RemoveContextParameter;
}

}
}
