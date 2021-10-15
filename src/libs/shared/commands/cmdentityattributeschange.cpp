/*
 Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdentityattributeschange.h"

#include "commandids.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"
#include "veobject.h"

namespace shared {
namespace cmd {

CmdEntityAttributesChange::CmdEntityAttributesChange(
        shared::PropertyTemplateConfig *config, VEObject *entity, const QList<EntityAttribute> &attrs)
    : UndoCommand()
    , m_config(config)
    , m_entity(entity)
    , m_newAttrs(attrs)
    , m_oldAttrs(entity ? entity->entityAttributes().values() : QList<EntityAttribute>())
{
    Q_ASSERT(entity != nullptr);
    setText(QObject::tr("Change Attribute(s)"));
}

void CmdEntityAttributesChange::redo()
{
    if (!m_entity || m_newAttrs.isEmpty()) {
        return;
    }

    for (const EntityAttribute &attr : qAsConst(m_newAttrs)) {
        QVariant oldValue = m_entity->entityAttributeValue(attr.name());
        m_entity->setEntityAttribute(attr);
        Q_EMIT attributeChanged(m_entity, attr.name(), oldValue);
    }
    QList<EntityAttribute> aboutToRemoveAttrs;
    for (PropertyTemplate *attrTemplate : m_config->propertyTemplatesForObject(m_entity)) {
        if (!attrTemplate->validate(m_entity)) {
            aboutToRemoveAttrs.append(m_entity->entityAttribute(attrTemplate->name()));
        }
    }
    for (const EntityAttribute &attr : aboutToRemoveAttrs) {
        m_entity->removeEntityAttribute(attr.name());
    }
}

void CmdEntityAttributesChange::undo()
{
    if (!m_entity || m_newAttrs.isEmpty()) {
        return;
    }

    QHash<QString, EntityAttribute> oldAttrs;
    QHash<QString, EntityAttribute> attrs;
    for (const EntityAttribute &attr : qAsConst(m_oldAttrs)) {
        const QString name = attr.name();
        oldAttrs[name] = m_entity->entityAttribute(name);
        attrs[name] = attr;
    }

    m_entity->setEntityAttributes(attrs);

    for (const EntityAttribute &attr : oldAttrs) {
        const QString name = attr.name();
        Q_EMIT attributeChanged(m_entity, name, oldAttrs[name].value());
    }
}

int CmdEntityAttributesChange::id() const
{
    return ChangeEntityAttributes;
}

} // namespace cmd
} // namespace shared
