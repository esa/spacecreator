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
 along with this program. If not, see
 <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "common.h"
#include "undocommand.h"

#include <QPointer>

namespace ivm {
class AADLObject;
class AADLFunction;
class AADLFunctionType;
}

namespace ive {
namespace cmd {

class CmdEntityAttributeChange : public shared::UndoCommand
{
    Q_OBJECT

public:
    explicit CmdEntityAttributeChange(ivm::AADLObject *entity, const QVariantHash &attrs);
    ~CmdEntityAttributeChange() override;

    void redo() override;
    void undo() override;
    int id() const override;

Q_SIGNALS:
    void nameChanged(ivm::AADLObject *entity, const QString &oldName, shared::UndoCommand *command);

private:
    QPointer<ivm::AADLObject> m_entity;
    ivm::AADLFunction *m_function { nullptr };

    const QVariantHash m_newAttrs;
    const QVariantHash m_oldAttrs;

    QHash<shared::Id, QVector<QUndoCommand *>> m_cmdSet;
    QHash<shared::Id, QVector<QUndoCommand *>> m_cmdUnset;

    void setAttrs(const QVariantHash &attrs, bool isRedo);
    ivm::AADLFunctionType *functionTypeByName(const QString &name) const;
    void handleFunctionInstanceOf(const QVariant &attr, bool isRedo);

    QVector<QUndoCommand *> commandsUnsetPrevFunctionType(const ivm::AADLFunctionType *fnType);
    QVector<QUndoCommand *> commandsSetNewFunctionType(const ivm::AADLFunctionType *fnType);

    void prepareUnsetFunctionTypeCommands(const ivm::AADLFunctionType *fnType);
    void prepareSetFunctionTypeCommands(const ivm::AADLFunctionType *fnType);
};

}
}
