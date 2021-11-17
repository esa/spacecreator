/*
 Copyright (C) 2018-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commands/cmdentityattributeschange.h"
#include "common.h"

#include <QPointer>

namespace ivm {
class IVObject;
class IVFunction;
class IVFunctionType;
}

namespace ive {
namespace cmd {

class CmdFunctionAttrChange : public shared::cmd::CmdEntityAttributesChange
{
    Q_OBJECT
public:
    explicit CmdFunctionAttrChange(
            shared::PropertyTemplateConfig *config, ivm::IVFunction *entity, const QList<EntityAttribute> &attrs);
    ~CmdFunctionAttrChange() override;

    void redo() override;
    void undo() override;
    int id() const override;

Q_SIGNALS:
    void nameChanged(ivm::IVObject *entity, const QString &oldName, shared::UndoCommand *command);
    void defaultImplementationChanged(ivm::IVFunction *entity);

protected:
    QPointer<ivm::IVFunction> m_entity;

private:
    const QVariantHash m_newAttrs;
    const QVariantHash m_oldAttrs;

    QHash<shared::Id, QVector<QUndoCommand *>> m_cmdSet;
    QHash<shared::Id, QVector<QUndoCommand *>> m_cmdUnset;

    void setAttrs(const QVariantHash &attrs, bool isRedo);
    ivm::IVFunctionType *functionTypeByName(const QString &name) const;
    void handleFunctionInstanceOf(const QVariant &attr, bool isRedo);

    QVector<QUndoCommand *> commandsUnsetPrevFunctionType(const ivm::IVFunctionType *fnType);
    QVector<QUndoCommand *> commandsSetNewFunctionType(const ivm::IVFunctionType *fnType);

    void prepareUnsetFunctionTypeCommands(const ivm::IVFunctionType *fnType);
    void prepareSetFunctionTypeCommands(const ivm::IVFunctionType *fnType);
};

} // namespace cmd
} // namespace ive
