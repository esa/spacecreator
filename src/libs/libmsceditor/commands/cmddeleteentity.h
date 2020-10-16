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

#include <QMap>
#include <QPointer>
#include <QUndoCommand>
#include <QVector>

namespace msc {

class MscChart;
class MscDocument;
class MscEntity;
class MscInstanceEvent;

namespace cmd {

class CmdDeleteEntity : public ChartBaseCommand
{
public:
    CmdDeleteEntity(QVector<msc::MscEntity *> items, msc::MscDocument *document,
            ChartLayoutManager *layoutManager);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QMap<int, MscInstanceEvent *> m_events;
    QMap<int, MscEntity *> m_entities;
    QPointer<MscDocument> m_document;
};

} // namespace cmd
} // namespace msc
