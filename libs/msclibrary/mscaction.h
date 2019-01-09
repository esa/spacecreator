/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#ifndef MSCACTION_H
#define MSCACTION_H

#include "mscinstanceevent.h"

#include <QVector>

namespace msc {

class MscInstance;

class MscAction : public MscInstanceEvent
{
public:
    enum class ActionType { Informal, Formal };

    struct DataStatement {
        enum class StatementType { Define, UnDefine, Binding };

        StatementType m_type = StatementType::Define;
        QString m_variableString;
    };

    explicit MscAction(QObject *parent = nullptr);

    MscEntity::EntityType entityType() const override { return MscEntity::EntityType::Action; }

    void setActionType(ActionType type) { m_actionType = type; }
    ActionType actionType() const { return m_actionType; }

    void setInformalAction(QString action) { m_informalAction = action; }
    QString informalAction() const { return m_informalAction; }

    void addDataStatement(const DataStatement &statement);
    QVector<DataStatement> dataStatements() const { return m_dataStatementList; }

    MscInstance *instance() const;
    void setInstance(MscInstance *instance);

private:
    ActionType m_actionType = ActionType::Informal;
    QString m_informalAction;
    QVector<DataStatement> m_dataStatementList;
    MscInstance *m_instance = nullptr;
};

}

#endif // MSCACTION_H
