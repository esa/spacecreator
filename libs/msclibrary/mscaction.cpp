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

#include "mscinstance.h"

#include <QDebug>

namespace msc {

MscAction::MscAction(QObject *parent)
    : MscInstanceEvent(parent)
{
}

void MscAction::setActionType(MscAction::ActionType type)
{
    if (type == m_actionType) {
        return;
    }

    m_actionType = type;
    Q_EMIT actionTypeChanged();
    Q_EMIT dataChanged();
}

MscAction::ActionType MscAction::actionType() const
{
    return m_actionType;
}

void MscAction::setInformalAction(QString action)
{
    if (action == m_informalAction) {
        return;
    }

    m_informalAction = action;
    Q_EMIT informalActionChanged(m_informalAction);
    Q_EMIT dataChanged();
}

QString MscAction::informalAction() const
{
    return m_informalAction;
}

void MscAction::addDataStatement(const MscAction::DataStatement &statement)
{
    if (statement.m_type == DataStatement::StatementType::Binding) {
        qWarning() << "Action data statement type 'binding' is not supported";
        return;
    }

    m_dataStatementList.append(statement);
    Q_EMIT dataStatementsChanged();
    Q_EMIT dataChanged();
}

QVector<MscAction::DataStatement> MscAction::dataStatements() const
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

    m_instance = instance;
    Q_EMIT instanceChanged();
    Q_EMIT dataChanged();
}

bool MscAction::relatesTo(const MscInstance *instance) const
{
    return m_instance == instance;
}
}
