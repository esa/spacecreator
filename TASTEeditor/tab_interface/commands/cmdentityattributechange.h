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

#include "app/common.h"

#include <QPointer>
#include <QRect>
#include <QUndoCommand>

namespace taste3 {
namespace aadl {

class AADLObject;
class AADLObjectFunction;
class AADLObjectFunctionType;
namespace cmd {

class CmdEntityAttributeChange : public QUndoCommand
{
public:
    explicit CmdEntityAttributeChange(AADLObject *entity, const QVariantHash &attrs);
    ~CmdEntityAttributeChange() override;

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<AADLObject> m_entity;
    AADLObjectFunction *m_function { nullptr };

    const QVariantHash m_newAttrs;
    const QVariantHash m_oldAttrs;

    QHash<common::Id, QVector<QUndoCommand *>> m_cmdSet;
    QHash<common::Id, QVector<QUndoCommand *>> m_cmdUnset;

    void setAttrs(const QVariantHash &attrs, bool isRedo);
    AADLObjectFunctionType *functionTypeByName(const QString &name) const;
    void handleFunctionInstanceOf(const QVariant &attr, bool isRedo);

    QVector<QUndoCommand *> commandsUnsetPrevFunctionType(const AADLObjectFunctionType *fnType);
    QVector<QUndoCommand *> commandsSetNewFunctionType(const AADLObjectFunctionType *fnType);

    void prepareUnsetFunctionTypeCommands(const AADLObjectFunctionType *fnType);
    void prepareSetFunctionTypeCommands(const AADLObjectFunctionType *fnType);
};

} // namespace cmd
} // namespace aadl
} // namespace taste3
