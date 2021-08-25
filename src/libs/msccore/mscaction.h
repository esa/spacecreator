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

#pragma once

#include "mscinstanceevent.h"

#include <QVector>

namespace msc {
class DataStatement;
class MscInstance;

/**
   @brief The MscAction class
 */
class MscAction : public MscInstanceEvent
{
    Q_OBJECT
    Q_PROPERTY(msc::MscAction::ActionType actionType READ actionType WRITE setActionType NOTIFY actionTypeChanged)
    Q_PROPERTY(QString informalAction READ informalAction WRITE setInformalAction NOTIFY informalActionChanged)
    Q_PROPERTY(bool isAssignAction READ isAssignAction NOTIFY isAssignActionChanged)
    Q_PROPERTY(QVector<msc::DataStatement *> dataStatements READ dataStatements NOTIFY dataStatementsChanged)
    Q_PROPERTY(msc::MscInstance *instance READ instance WRITE setInstance NOTIFY instanceChanged)

public:
    enum class ActionType
    {
        Informal,
        Formal
    };
    Q_ENUM(ActionType)

    explicit MscAction(QObject *parent = nullptr);
    MscAction(const QString &name, QObject *parent = nullptr);

    MscEntity::EntityType entityType() const override { return MscEntity::EntityType::Action; }

    void setActionType(ActionType type);
    ActionType actionType() const;

    void setInformalAction(const QString &action);
    QString informalAction() const;

    bool isAssignAction() const;

    void addDataStatement(DataStatement *statement);
    QVector<DataStatement *> dataStatements() const;

    MscInstance *instance() const;
    void setInstance(MscInstance *instance);

    bool relatesTo(const MscInstance *instance) const override;

    QString toDbgString() const override;

Q_SIGNALS:
    void actionTypeChanged();
    void informalActionChanged(const QString &text);
    void isAssignActionChanged();
    void dataStatementsChanged();
    void instanceChanged();

private:
    ActionType m_actionType = ActionType::Informal;
    QString m_informalAction;
    QVector<DataStatement *> m_dataStatementList;
    MscInstance *m_instance = nullptr;
};
}
