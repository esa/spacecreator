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
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "mscaction.h"

#include "datastatement.h"
#include "mscinstance.h"

#include <QDebug>

namespace msc {

MscAction::MscAction(QObject *parent)
    : MscInstanceEvent(parent)
{
}

MscAction::MscAction(const QString &name, QObject *parent)
    : MscInstanceEvent(name, parent)
{
}

void MscAction::setActionType(MscAction::ActionType type)
{
    if (type == m_actionType) {
        return;
    }

    m_actionType = type;
    Q_EMIT actionTypeChanged();
    Q_EMIT isAssignActionChanged();
    Q_EMIT dataChanged();
}

MscAction::ActionType MscAction::actionType() const
{
    return m_actionType;
}

void MscAction::setInformalAction(const QString &action)
{
    if (action == m_informalAction) {
        return;
    }

    m_informalAction = action;
    Q_EMIT informalActionChanged(m_informalAction);
    Q_EMIT isAssignActionChanged();
    Q_EMIT dataChanged();
}

QString MscAction::informalAction() const
{
    return m_informalAction;
}

bool MscAction::isAssignAction() const
{
    return m_actionType == ActionType::Informal && m_informalAction.contains("=");
}

void MscAction::addDataStatement(DataStatement *statement)
{
    if (statement->type() == DataStatement::StatementType::Binding) {
        qWarning() << "Action data statement type 'binding' is not supported";
        return;
    }

    m_dataStatementList.append(statement);
    Q_EMIT dataStatementsChanged();
    Q_EMIT dataChanged();
}

QVector<DataStatement *> MscAction::dataStatements() const
{
    return m_dataStatementList;
}

MscInstance *MscAction::instance() const
{
    return m_instance;
}

void MscAction::setInstance(MscInstance *instance)
{
    if (instance == m_instance) {
        return;
    }

    MscInstance *oldInstance = m_instance;
    m_instance = instance;
    Q_EMIT instanceChanged();
    Q_EMIT instanceRelationChanged(m_instance, oldInstance);
    Q_EMIT dataChanged();
}

MscEntity *MscAction::instanceObj() const
{
    return m_instance;
}

void MscAction::setInstanceObj(MscEntity *instance)
{
    if (auto inst = qobject_cast<MscInstance *>(instance)) {
        setInstance(inst);
    }
}

bool MscAction::relatesTo(const MscInstance *instance) const
{
    return m_instance == instance;
}

QString MscAction::toDbgString() const
{
    return QString("%1('%2')").arg(entityTypeName(), m_informalAction);
}

}
