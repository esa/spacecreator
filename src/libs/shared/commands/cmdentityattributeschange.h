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

#pragma once

#include "undocommand.h"

#include <QPointer>

class EntityAttribute;
namespace shared {
class VEObject;
class PropertyTemplateConfig;
namespace cmd {

class CmdEntityAttributesChange : public UndoCommand
{
    Q_OBJECT
public:
    explicit CmdEntityAttributesChange(
            shared::PropertyTemplateConfig *config, VEObject *entity, const QList<EntityAttribute> &attrs);

    void redo() override;
    void undo() override;
    int id() const override;

Q_SIGNALS:
    void attributeChanged(shared::VEObject *entity, const QString &attrName, const QVariant &oldValue);

private:
    shared::PropertyTemplateConfig *m_config;
    QPointer<VEObject> m_entity;
    const QList<EntityAttribute> m_newAttrs;
    const QList<EntityAttribute> m_oldAttrs;
};

} // namespace cmd
} // namespace shared
