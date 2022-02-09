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

#pragma once

#include "common.h"
#include "ivobject.h"
#include "undocommand.h"

#include <QPointer>
#include <QUndoCommand>
#include <QVector>

namespace ivm {
class IVModel;
class IVFunctionType;
}

namespace ive {
namespace cmd {

class CmdEntitiesRemove : public shared::UndoCommand
{
    Q_OBJECT
public:
    explicit CmdEntitiesRemove(const QList<QPointer<ivm::IVObject>> &entities, ivm::IVModel *model);
    ~CmdEntitiesRemove() override;

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

Q_SIGNALS:
    void entitiesRemoved(const QList<QPointer<ivm::IVObject>> &entities, shared::UndoCommand *command);

private:
    QPointer<ivm::IVModel> m_model;
    QList<QPointer<ivm::IVObject>> m_entities;
    QVector<QPointer<ivm::IVObject>> m_relatedEntities;
    QVector<QPointer<ivm::IVObject>> m_relatedConnections;
    QVector<QPointer<ivm::IVObject>> m_relatedIfaces;
    QHash<shared::Id, QPointer<ivm::IVFunctionType>> m_parentFunctions;
    QList<QUndoCommand *> m_subCommands;

    void collectRelatedItems(ivm::IVObject *toBeRemoved);
    void storeLinkedEntity(ivm::IVObject *linkedEntity);

    ivm::IVFunctionType *putParentFunctionFor(const ivm::IVObject *obj);
    ivm::IVFunctionType *popParentFunctionFor(const ivm::IVObject *obj);
    void advancedRemove(ivm::IVObject *obj);
    void advancedRestore(ivm::IVObject *obj);
};

}
}
