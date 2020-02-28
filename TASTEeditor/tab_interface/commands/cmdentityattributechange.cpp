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

#include "cmdentityattributechange.h"

#include "cmdentityremove.h"
#include "commandids.h"
#include "commandsfactory.h"

#include <QDebug>
#include <tab_aadl/aadlobjectfunction.h>
#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

static inline QVariantHash getCurrentAttributes(AADLObject *entity, const QVariantHash &attrs)
{
    QVariantHash result;
    for (auto it = attrs.constBegin(); it != attrs.constEnd(); ++it)
        result.insert(it.key(), entity->attr(it.key()));
    return result;
}

CmdEntityAttributeChange::CmdEntityAttributeChange(AADLObject *entity, const QVariantHash &attrs)
    : QUndoCommand()
    , m_entity(entity)
    , m_function(m_entity ? m_entity->as<AADLObjectFunction *>() : nullptr)
    , m_newAttrs(attrs)
    , m_oldAttrs(getCurrentAttributes(entity, attrs))
{
    setText(QObject::tr("Change Attribute"));
}

CmdEntityAttributeChange::~CmdEntityAttributeChange()
{
    for (const common::Id key : m_cmdSet.keys()) {
        if (m_cmdUnset.contains(key))
            m_cmdUnset.remove(key);

        qDeleteAll(m_cmdSet.take(key));
    }

    for (auto cmds : m_cmdUnset)
        qDeleteAll(cmds);
}

void CmdEntityAttributeChange::redo()
{
    setAttrs(m_newAttrs, true);
}

void CmdEntityAttributeChange::undo()
{
    setAttrs(m_oldAttrs, false);
}

bool CmdEntityAttributeChange::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdEntityAttributeChange::id() const
{
    return ChangeEntityAttributes;
}

void CmdEntityAttributeChange::setAttrs(const QVariantHash &attrs, bool isRedo)
{
    if (!m_entity)
        return;

    for (auto it = attrs.constBegin(); it != attrs.constEnd(); ++it) {
        const QString name = it.key();
        const QVariant val = it.value();
        switch (meta::Props::token(name)) {
        case meta::Props::Token::instance_of: {
            if (m_function)
                handleFunctionInstanceOf(val, isRedo);
            else
                m_entity->setAttr(name, val);
            break;
        }
        default: {
            m_entity->setAttr(name, val);
            break;
        }
        }
    }
}

AADLObjectFunctionType *CmdEntityAttributeChange::functionTypeByName(const QString &name) const
{
    if (name.isEmpty() || !m_function || !m_function->objectsModel())
        return nullptr;

    return m_function->objectsModel()->getAvailableFunctionTypes(m_function).value(name, nullptr);
}

void CmdEntityAttributeChange::handleFunctionInstanceOf(const QVariant &attr, bool isRedo)
{
    const AADLObjectFunctionType *oldInstanceOf = m_function->instanceOf();
    AADLObjectFunctionType *newInstanceOf = functionTypeByName(attr.toString());
    if (oldInstanceOf == newInstanceOf)
        return;

    if (oldInstanceOf) {
        for (QUndoCommand *cmd : commandsUnsetPrevFunctionType(oldInstanceOf)) {
            if (isRedo)
                cmd->redo();
            else
                cmd->undo();
        }
    }

    if (newInstanceOf) {
        for (QUndoCommand *cmd : commandsSetNewFunctionType(newInstanceOf)) {
            if (isRedo)
                cmd->redo();
            else
                cmd->undo();
        }
    }

    m_function->setInstanceOf(newInstanceOf);
    m_function->setAttr(meta::Props::token(meta::Props::Token::instance_of), attr);
}

typedef QHash<common::Id, QVector<QUndoCommand *>> CmdStorage;
typedef QVector<QUndoCommand *> CmdSet;

CmdSet getCommandsSet(const AADLObjectFunctionType *fnType, const CmdStorage &cmdStorage,
                      CmdEntityAttributeChange *caller,
                      void (CmdEntityAttributeChange::*prepareMethod)(const AADLObjectFunctionType *fn))
{
    if (!fnType || !caller || !prepareMethod)
        return {};

    const common::Id &fnTypeId = fnType->id();
    if (!cmdStorage.contains(fnTypeId))
        (caller->*prepareMethod)(fnType);

    return cmdStorage.value(fnTypeId, {});
}

QVector<QUndoCommand *> CmdEntityAttributeChange::commandsUnsetPrevFunctionType(const AADLObjectFunctionType *fnType)
{
    return getCommandsSet(fnType, m_cmdUnset, this, &CmdEntityAttributeChange::prepareUnsetFunctionTypeCommands);
}

QVector<QUndoCommand *> CmdEntityAttributeChange::commandsSetNewFunctionType(const AADLObjectFunctionType *fnType)
{
    return getCommandsSet(fnType, m_cmdSet, this, &CmdEntityAttributeChange::prepareSetFunctionTypeCommands);
}

bool useOppositeCommands(CmdStorage &commands, const CmdStorage &oppositeCommands, const common::Id &fnTypeId)
{
    if (oppositeCommands.contains(fnTypeId)) {
        commands[fnTypeId] = oppositeCommands.value(fnTypeId, {});
        return !commands[fnTypeId].isEmpty();
    }

    return false;
}

void CmdEntityAttributeChange::prepareUnsetFunctionTypeCommands(const AADLObjectFunctionType *fnType)
{
    if (!fnType)
        return;

    const common::Id fnTypeId = fnType->id();
    if (useOppositeCommands(m_cmdUnset, m_cmdSet, fnTypeId))
        return;

    QVector<QUndoCommand *> &cmdStorage = m_cmdUnset[fnTypeId];
    const QVector<AADLObjectIface *> &fnIfaces = m_function->interfaces();
    const QVector<AADLObjectIface *> &fnTypeIfaces = fnType->interfaces();
    for (auto fnTypeIface : fnTypeIfaces) {
        for (auto clone : fnTypeIface->clones()) {
            auto found = std::find_if(fnIfaces.cbegin(), fnIfaces.cend(),
                                      [clone](AADLObjectIface *fnIface) { return clone == fnIface; });

            if (found != fnIfaces.cend()) {
                const QVariantList params = { QVariant::fromValue(clone.data()),
                                              QVariant::fromValue(clone->objectsModel()) };
                if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::RemoveEntity, params))
                    cmdStorage.append(cmdRm);
            }
        }
    }
}

void CmdEntityAttributeChange::prepareSetFunctionTypeCommands(const AADLObjectFunctionType *fnType)
{
    if (!fnType)
        return;

    const common::Id fnTypeId = fnType->id();
    if (useOppositeCommands(m_cmdSet, m_cmdUnset, fnTypeId))
        return;

    QVector<QUndoCommand *> &cmdStorage = m_cmdSet[fnTypeId];
    const QVector<AADLObjectIface *> &fnTypeIfaces = fnType->interfaces();
    for (auto fnTypeIface : fnTypeIfaces) {
        const AADLObjectIface::CreationInfo clone = AADLObjectIface::CreationInfo::cloneIface(fnTypeIface, m_function);
        if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, clone.toVarList()))
            cmdStorage.append(cmdRm);
    }
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
