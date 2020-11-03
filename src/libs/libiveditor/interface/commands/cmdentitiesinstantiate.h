/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QPointer>
#include <QUndoCommand>
#include <QVector>

namespace aadl {
class AADLObject;
class AADLObjectFunction;
class AADLObjectFunctionType;
class AADLObjectsModel;
}

namespace aadlinterface {
namespace cmd {
class CmdEntityAttributeChange;

class CmdEntitiesInstantiate : public QUndoCommand
{
public:
    explicit CmdEntitiesInstantiate(aadl::AADLObjectFunctionType *entity, aadl::AADLObjectFunctionType *parent,
            aadl::AADLObjectsModel *model, const QPointF &pos);
    ~CmdEntitiesInstantiate() override;

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<aadl::AADLObjectFunctionType> m_parent;
    QPointer<aadl::AADLObjectsModel> m_model;
    QPointer<aadl::AADLObjectFunction> m_instantiatedEntity;
    CmdEntityAttributeChange *m_subCmd { nullptr };
};

} // namespace aadlinterface
} // namespace cmd
