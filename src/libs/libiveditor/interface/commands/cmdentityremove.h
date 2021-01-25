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
#include "undocommand.h"

#include <QPointer>
#include <QUndoCommand>
#include <QVector>

namespace ivm {
class AADLObject;
class AADLObjectsModel;
class AADLObjectFunctionType;
}

namespace ive {
namespace cmd {

class CmdEntityRemove : public shared::UndoCommand
{
    Q_OBJECT
public:
    explicit CmdEntityRemove(ivm::AADLObject *entity, ivm::AADLObjectsModel *model);
    ~CmdEntityRemove() override;

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

    ivm::AADLObject *entity() const;

Q_SIGNALS:
    void entityRemoved(ivm::AADLObject *obj, shared::UndoCommand *command);

private:
    QPointer<ivm::AADLObjectsModel> m_model;
    QPointer<ivm::AADLObject> m_entity;
    QVector<QPointer<ivm::AADLObject>> m_relatedEntities;
    QVector<QPointer<ivm::AADLObject>> m_relatedConnections;
    QVector<QPointer<ivm::AADLObject>> m_relatedIfaces;
    QHash<shared::Id, QPointer<ivm::AADLObjectFunctionType>> m_parentFunctions;

    void collectRelatedItems(ivm::AADLObject *toBeRemoved);
    void storeLinkedEntity(ivm::AADLObject *linkedEntity);

    ivm::AADLObjectFunctionType *putParentFunctionFor(const ivm::AADLObject *obj);
    ivm::AADLObjectFunctionType *popParentFunctionFor(const ivm::AADLObject *obj);
    void advancedRemove(ivm::AADLObject *obj);
    void advancedRestore(ivm::AADLObject *obj);
};

}
}
