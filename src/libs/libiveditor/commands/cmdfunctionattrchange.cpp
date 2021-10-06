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

#include "cmdfunctionattrchange.h"

#include "cmdentitiesremove.h"
#include "cmdinterfaceitemcreate.h"
#include "commandids.h"

#include <QDebug>
#include <ivcoreutils.h>
#include <ivfunction.h>
#include <ivmodel.h>

namespace ive {
namespace cmd {

typedef QVector<QUndoCommand *> Commands;
typedef QHash<shared::Id, Commands> CommandsStorage;

static inline QVariantHash getCurrentAttributes(ivm::IVObject *entity, const QVariantHash &attrs)
{
    QVariantHash result;
    for (auto it = attrs.constBegin(); it != attrs.constEnd(); ++it)
        result.insert(it.key(), entity->entityAttributeValue(it.key()));
    return result;
}

CmdFunctionAttrChange::CmdFunctionAttrChange(ivm::IVFunction *entity, const QVariantHash &attrs)
    : shared::UndoCommand()
    , m_entity(entity)
    , m_newAttrs(attrs)
    , m_oldAttrs(getCurrentAttributes(entity, attrs))
{
    setText(QObject::tr("Change Attribute"));
}

CmdFunctionAttrChange::~CmdFunctionAttrChange()
{
    for (const shared::Id &key : m_cmdSet.keys()) {
        m_cmdUnset.remove(key);
        qDeleteAll(m_cmdSet.take(key));
    }

    for (const auto &cmds : m_cmdUnset)
        qDeleteAll(cmds);
}

void CmdFunctionAttrChange::redo()
{
    setAttrs(m_newAttrs, true);

    const QString nameKey = ivm::meta::Props::token(ivm::meta::Props::Token::name);
    if (m_oldAttrs.contains(nameKey) && m_entity->title() != m_oldAttrs[nameKey].toString()) {
        const QString oldName = m_oldAttrs[nameKey].toString();
        Q_EMIT nameChanged(m_entity, oldName, this);
    }

    if (m_newAttrs.contains(ivm::meta::Props::token((ivm::meta::Props::Token::default_language)))) {
        Q_EMIT defaultImplementationChanged(m_entity);
    }

    m_firstRedo = false;
}

void CmdFunctionAttrChange::undo()
{
    setAttrs(m_oldAttrs, false);

    const QString nameKey = ivm::meta::Props::token(ivm::meta::Props::Token::name);
    if (m_oldAttrs.contains(nameKey) && m_entity->title() != m_newAttrs[nameKey].toString()) {
        const QString oldName = m_newAttrs[nameKey].toString();
        Q_EMIT nameChanged(m_entity, oldName, this);
    }

    if (m_newAttrs.contains(ivm::meta::Props::token((ivm::meta::Props::Token::default_language)))) {
        Q_EMIT defaultImplementationChanged(m_entity);
    }
}

int CmdFunctionAttrChange::id() const
{
    return ChangeFunctionAttribute;
}

void CmdFunctionAttrChange::setAttrs(const QVariantHash &attrs, bool isRedo)
{
    if (!m_entity)
        return;

    for (auto it = attrs.constBegin(); it != attrs.constEnd(); ++it) {
        const QString name = it.key();
        const QVariant val = it.value();
        switch (ivm::meta::Props::token(name)) {
        case ivm::meta::Props::Token::instance_of: {
            handleFunctionInstanceOf(val, isRedo);
            break;
        }
        default: {
            m_entity->setEntityAttribute(name, val);
            break;
        }
        }
    }
}

ivm::IVFunctionType *CmdFunctionAttrChange::functionTypeByName(const QString &name) const
{
    if (name.isEmpty() || !m_entity || !m_entity->model())
        return nullptr;

    return m_entity->model()->getAvailableFunctionTypes(m_entity).value(name, nullptr);
}

void CmdFunctionAttrChange::handleFunctionInstanceOf(const QVariant &attr, bool isRedo)
{
    const ivm::IVFunctionType *oldInstanceOf = m_entity->instanceOf();
    ivm::IVFunctionType *newInstanceOf = functionTypeByName(attr.toString());
    if (oldInstanceOf == newInstanceOf)
        return;

    auto performCommands = [isRedo](const Commands &cmds) {
        for (QUndoCommand *cmd : cmds) {
            if (isRedo)
                cmd->redo();
            else
                cmd->undo();
        }
    };

    if (oldInstanceOf)
        performCommands(commandsUnsetPrevFunctionType(oldInstanceOf));

    if (newInstanceOf)
        performCommands(commandsSetNewFunctionType(newInstanceOf));

    m_entity->setInstanceOf(newInstanceOf);
    m_entity->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::instance_of), attr);
}

Commands getCommands(const ivm::IVFunctionType *fnType, const CommandsStorage &cmdStorage,
        CmdFunctionAttrChange *caller, void (CmdFunctionAttrChange::*prepareMethod)(const ivm::IVFunctionType *fn))
{
    if (!fnType || !caller || !prepareMethod)
        return {};

    const shared::Id &fnTypeId = fnType->id();
    if (!cmdStorage.contains(fnTypeId))
        (caller->*prepareMethod)(fnType);

    return cmdStorage.value(fnTypeId, {});
}

Commands CmdFunctionAttrChange::commandsUnsetPrevFunctionType(const ivm::IVFunctionType *fnType)
{
    return getCommands(fnType, m_cmdUnset, this, &CmdFunctionAttrChange::prepareUnsetFunctionTypeCommands);
}

Commands CmdFunctionAttrChange::commandsSetNewFunctionType(const ivm::IVFunctionType *fnType)
{
    return getCommands(fnType, m_cmdSet, this, &CmdFunctionAttrChange::prepareSetFunctionTypeCommands);
}

bool useOppositeCommands(CommandsStorage &commands, const CommandsStorage &oppositeCommands, const shared::Id &fnTypeId)
{
    if (oppositeCommands.contains(fnTypeId)) {
        commands[fnTypeId] = oppositeCommands.value(fnTypeId, {});
        return !commands[fnTypeId].isEmpty();
    }

    return false;
}

void CmdFunctionAttrChange::prepareUnsetFunctionTypeCommands(const ivm::IVFunctionType *fnType)
{
    if (!fnType)
        return;

    const shared::Id fnTypeId = fnType->id();
    if (useOppositeCommands(m_cmdUnset, m_cmdSet, fnTypeId))
        return;

    Commands &cmdStorage = m_cmdUnset[fnTypeId];
    const QVector<ivm::IVInterface *> &fnIfaces = m_entity->interfaces();
    const QVector<ivm::IVInterface *> &fnTypeIfaces = fnType->interfaces();
    QList<QPointer<ivm::IVObject>> entities;
    for (auto fnTypeIface : fnTypeIfaces) {
        for (const auto &clone : fnTypeIface->clones()) {
            auto found = std::find_if(fnIfaces.cbegin(), fnIfaces.cend(),
                    [clone](ivm::IVInterface *fnIface) { return clone == fnIface; });

            if (found != fnIfaces.cend()) {
                entities.append(clone.data());
            }
        }
    }
    if (!entities.isEmpty()) {
        auto cmdRm = new cmd::CmdEntitiesRemove(entities, m_entity->model());
        cmdStorage.append(cmdRm);
    }
}

void CmdFunctionAttrChange::prepareSetFunctionTypeCommands(const ivm::IVFunctionType *fnType)
{
    if (!fnType)
        return;

    const shared::Id fnTypeId = fnType->id();
    if (useOppositeCommands(m_cmdSet, m_cmdUnset, fnTypeId))
        return;

    Commands &cmdStorage = m_cmdSet[fnTypeId];
    const QVector<ivm::IVInterface *> &fnTypeIfaces = fnType->interfaces();
    for (auto fnTypeIface : fnTypeIfaces) {
        if (ivm::IVInterface *existingIface = ivm::utils::findExistingClone(m_entity, fnTypeIface)) {
            existingIface->setCloneOrigin(fnTypeIface);
        } else {
            const ivm::IVInterface::CreationInfo clone =
                    ivm::IVInterface::CreationInfo::cloneIface(fnTypeIface, m_entity);
            auto cmdRm = new cmd::CmdInterfaceItemCreate(clone);
            cmdStorage.append(cmdRm);
        }
    }
}

} // namespace cmd
} // namespace ive
