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

namespace aadl {
class AADLObject;
class AADLObjectsModel;
class AADLObjectFunctionType;
}

namespace aadlinterface {
namespace cmd {

class CmdEntityRemove : public shared::UndoCommand
{
    Q_OBJECT
public:
    explicit CmdEntityRemove(aadl::AADLObject *entity, aadl::AADLObjectsModel *model);
    ~CmdEntityRemove() override;

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

    aadl::AADLObject *entity() const;

Q_SIGNALS:
    void entityRemoved(aadl::AADLObject *obj, shared::UndoCommand *command);

private:
    QPointer<aadl::AADLObjectsModel> m_model;
    QPointer<aadl::AADLObject> m_entity;
    QVector<QPointer<aadl::AADLObject>> m_relatedEntities;
    QVector<QPointer<aadl::AADLObject>> m_relatedConnections;
    QVector<QPointer<aadl::AADLObject>> m_relatedIfaces;
    QHash<shared::Id, QPointer<aadl::AADLObjectFunctionType>> m_parentFunctions;

    void collectRelatedItems(aadl::AADLObject *toBeRemoved);
    void storeLinkedEntity(aadl::AADLObject *linkedEntity);

    aadl::AADLObjectFunctionType *putParentFunctionFor(const aadl::AADLObject *obj);
    aadl::AADLObjectFunctionType *popParentFunctionFor(const aadl::AADLObject *obj);
    void advancedRemove(aadl::AADLObject *obj);
    void advancedRestore(aadl::AADLObject *obj);
};

}
}
