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
#include "asn1componentsimport.h"

#include <QDebug>
#include <ivcoreutils.h>
#include <ivfunction.h>
#include <ivmodel.h>

namespace ive {
namespace cmd {

typedef QVector<QUndoCommand *> Commands;
typedef QHash<shared::Id, Commands> CommandsStorage;

static inline QVariantHash getCurrentAttributes(ivm::IVObject *entity, const QList<EntityAttribute> &attrs)
{
    QVariantHash result;
    for (auto it = attrs.constBegin(); it != attrs.constEnd(); ++it)
        result.insert(it->name(), entity->entityAttributeValue(it->name()));
    return result;
}

static inline QVariantHash convertAttributes(const QList<EntityAttribute> &attrs)
{
    QVariantHash result;
    for (auto it = attrs.constBegin(); it != attrs.constEnd(); ++it)
        result.insert(it->name(), it->value());
    return result;
}

CmdFunctionAttrChange::CmdFunctionAttrChange(shared::PropertyTemplateConfig *config, ivm::IVFunction *entity, const QList<EntityAttribute> &attrs)
    : shared::cmd::CmdEntityAttributesChange(config, entity, attrs)
    , m_asn1Importer(new ASN1ComponentsImport(nullptr, shared::sharedTypesPath(), QString()))
    , m_entity(entity)
    , m_newAttrs(convertAttributes(attrs))
    , m_oldAttrs(getCurrentAttributes(entity, attrs))
{
    setText(QObject::tr("Change Attribute"));
    connect(m_asn1Importer, &ASN1ComponentsImport::asn1FilesImported, this, &CmdFunctionAttrChange::asn1FilesImported);
    connect(m_asn1Importer, &ASN1ComponentsImport::asn1FilesRemoved, this, &CmdFunctionAttrChange::asn1FilesRemoved);
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

void CmdFunctionAttrChange::setAsn1SystemChecks(Asn1Acn::Asn1SystemChecks *asn1Checks)
{
    m_asn1Importer->setAsn1SystemChecks(asn1Checks);
}

void CmdFunctionAttrChange::redo()
{
    setAttrs(m_newAttrs, true);

    const QString nameKey = ivm::meta::Props::token(ivm::meta::Props::Token::name);
    if (m_oldAttrs.contains(nameKey) && m_entity->title() != m_oldAttrs[nameKey].toString()) {
        const QString oldName = m_oldAttrs[nameKey].toString();
        Q_EMIT nameChanged(m_entity, oldName, this);
    }

    if (m_newAttrs.contains(ivm::meta::Props::token((ivm::meta::Props::Token::default_implementation)))) {
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

    if (m_newAttrs.contains(ivm::meta::Props::token((ivm::meta::Props::Token::default_implementation)))) {
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
        switch (ivm::meta::Props::token(it.key())) {
        case ivm::meta::Props::Token::instance_of: {
            handleFunctionInstanceOf(it.value(), isRedo);
            break;
        }
        default: {
            break;
        }
        }
    }
    if (isRedo) {
        shared::cmd::CmdEntityAttributesChange::redo();
    } else {
        shared::cmd::CmdEntityAttributesChange::undo();
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

    if (oldInstanceOf) {
        performCommands(commandsUnsetPrevFunctionType(oldInstanceOf));
    }

    if (newInstanceOf) {
        performCommands(commandsSetNewFunctionType(newInstanceOf));
        if (isRedo)
            m_asn1Importer->redoAsnFileImport(newInstanceOf);
        else
            m_asn1Importer->undoAsnFileImport();

    }

    m_entity->setInstanceOf(newInstanceOf);
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
