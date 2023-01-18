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

#include "chartbasecommand.h"

namespace msc {

class MscEntity;

namespace cmd {

class CmdEntityNameChange : public ChartBaseCommand
{
    Q_OBJECT

public:
    CmdEntityNameChange(MscEntity *item, const QString &newName);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

Q_SIGNALS:
    void nameChanged(QObject *entity, const QString &oldName, shared::UndoCommand *command);

private:
    void setChartDocumentName(const QString &name);

private:
    QString m_oldName;
    QString m_newName;
};

} // namespace cmd
} // namespace msc
