/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "common.h"
#include "undocommand.h"

#include <QPointer>
#include <QVector>

namespace dvm {
class DVObject;
class DVModel;
}

namespace dve {
namespace cmd {

class CmdEntitiesRemove : public shared::UndoCommand
{
    Q_OBJECT
public:
    explicit CmdEntitiesRemove(const QList<QPointer<dvm::DVObject>> &entities, dvm::DVModel *model);
    ~CmdEntitiesRemove() override;

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<dvm::DVModel> m_model;
    QList<QPointer<dvm::DVObject>> m_entities;
    QList<QUndoCommand *> m_subCommands;
    QVector<QPointer<dvm::DVObject>> m_relatedEntities;
    QHash<shared::Id, QPointer<dvm::DVObject>> m_parentObjects;

    void collectRelatedItems(dvm::DVObject *toBeRemoved);

    dvm::DVObject *putParentFor(const dvm::DVObject *obj);
    dvm::DVObject *popParentFor(const dvm::DVObject *obj);
    void advancedRemove(dvm::DVObject *obj);
    void advancedRestore(dvm::DVObject *obj);
};

} // namespace cmd
} // namespace dve
