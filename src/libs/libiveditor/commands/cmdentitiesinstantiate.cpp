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

#include "cmdentitiesinstantiate.h"

#include "asn1systemchecks.h"
#include "cmdinterfaceitemcreate.h"
#include "commandids.h"
#include "commands/cmdentityattributeschange.h"
#include "graphicsviewutils.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivpropertytemplateconfig.h"
#include "standardpaths.h"

#include <QTemporaryDir>

static inline void shiftObjects(const QVector<ivm::IVObject *> &objects, const QPointF &offset)
{
    for (auto obj : objects) {
        if (!obj) {
            continue;
        }
        auto points = shared::graphicsviewutils::polygon(obj->coordinates());
        std::transform(
                points.cbegin(), points.cend(), points.begin(), [offset](const QPointF &p) { return p + offset; });
        obj->setCoordinates(shared::graphicsviewutils::coordinates(points));
        if (obj->type() == ivm::IVObject::Type::FunctionType || obj->type() == ivm::IVObject::Type::Function) {
            shiftObjects(obj->as<ivm::IVFunctionType *>()->children(), offset);
        }
    }
}

namespace ive {
namespace cmd {

CmdEntitiesInstantiate::CmdEntitiesInstantiate(ivm::IVFunctionType *entity, ivm::IVFunctionType *parent,
        ivm::IVModel *model, shared::ComponentModel *componentModel, Asn1Acn::Asn1SystemChecks *asn1Checks,
        const QPointF &pos)
    : ComponentImportHelper(componentModel, asn1Checks)
    , QUndoCommand()
    , m_parent(parent)
    , m_model(model)

{
    Q_ASSERT(entity);
    m_instantiatedEntity =
            new ivm::IVFunction(m_parent ? qobject_cast<QObject *>(m_parent) : qobject_cast<QObject *>(m_model));
    m_instantiatedEntity->setTitle(
            ivm::IVNameValidator::nameForInstance(m_instantiatedEntity, entity->title() + QLatin1String("_Instance_")));

    QRectF typeGeometry = shared::graphicsviewutils::rect(entity->coordinates());
    typeGeometry.moveTo(pos);
    m_instantiatedEntity->setCoordinates(shared::graphicsviewutils::coordinates(typeGeometry));
    m_instantiatedEntity->setInstanceOf(entity);
    m_subCmds.append(
            new shared::cmd::CmdEntityAttributesChange(ivm::IVPropertyTemplateConfig::instance(), m_instantiatedEntity,
                    { EntityAttribute { ivm::meta::Props::token(ivm::meta::Props::Token::is_type), QLatin1String("NO"),
                            EntityAttribute::Type::Attribute } }));

    for (auto iface : entity->interfaces()) {
        const ivm::IVInterface::CreationInfo clone =
                ivm::IVInterface::CreationInfo::cloneIface(iface, m_instantiatedEntity);
        auto cmdRm = new CmdInterfaceItemCreate(clone);
        m_subCmds.append(cmdRm);
    }

    const QString nameKey = ivm::meta::Props::token(ivm::meta::Props::Token::instance_of);
    m_subCmds.append(new shared::cmd::CmdEntityAttributesChange(ivm::IVPropertyTemplateConfig::instance(),
            m_instantiatedEntity, { EntityAttribute { nameKey, entity->title(), EntityAttribute::Type::Attribute } }));
}

CmdEntitiesInstantiate::~CmdEntitiesInstantiate()
{
    qDeleteAll(m_subCmds);
    m_subCmds.clear();

    if (m_instantiatedEntity && !m_instantiatedEntity->parent())
        delete m_instantiatedEntity;
}

void CmdEntitiesInstantiate::redo()
{
    if (!m_instantiatedEntity.isNull()) {
        m_importedAsnFiles.clear();

        if (m_parent) {
            m_parent->addChild(m_instantiatedEntity);
        }
        if (m_model->addObject(m_instantiatedEntity)) {
            redoAsnFilesImport(m_instantiatedEntity->instanceOf());
        }
        for (QUndoCommand *cmd : qAsConst(m_subCmds)) {
            cmd->redo();
        }

        if (!m_tempDir.isNull()) {
            m_tempDir.reset();
        }
    }
}

void CmdEntitiesInstantiate::undo()
{
    m_tempDir.reset(new QTemporaryDir(shared::StandardPaths::writableLocation(QStandardPaths::CacheLocation)
            + QDir::separator() + QLatin1String("import")));

    if (!m_instantiatedEntity.isNull()) {
        for (QUndoCommand *cmd : qAsConst(m_subCmds)) {
            cmd->undo();
        }
        m_model->removeObject(m_instantiatedEntity);
        if (m_parent) {
            m_parent->removeChild(m_instantiatedEntity);
        } else {
            m_instantiatedEntity->setParentObject(nullptr);
        }
    }
    undoAsnFilesImport();
}

bool CmdEntitiesInstantiate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdEntitiesInstantiate::id() const
{
    return InstantiateEntities;
}

} // namespace ive
} // namespace cmd
