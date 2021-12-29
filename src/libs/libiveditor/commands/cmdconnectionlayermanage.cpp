/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "cmdconnectionlayermanage.h"

#include "commandids.h"
#include "commands/cmdentitygeometrychange.h"
#include "ivconnectionlayertype.h"
#include "ivmodel.h"
#include "ivobject.h"

#include <QDebug>

namespace ive {
namespace cmd {

CmdConnectionLayerManage::CmdConnectionLayerManage(ivm::IVModel *layersModel)
    : shared::UndoCommand()
    , m_layersModel(layersModel)
    , m_parent(layersModel != nullptr ? layersModel->rootObject() : nullptr)
    , m_entityPrevName("")
{
}

CmdConnectionLayerManage::~CmdConnectionLayerManage() {}

ivm::IVModel *CmdConnectionLayerManage::model() const
{
    return m_layersModel;
}

ivm::IVConnectionLayerType *CmdConnectionLayerManage::find(const QString &name) const
{
    if (model() != nullptr) {
        for (auto *layer : model()->allObjectsByType<ivm::IVConnectionLayerType>()) {
            if (layer->name().compare(name) == 0) {
                return layer;
            }
        }
    }
    return nullptr;
}

void CmdConnectionLayerManage::addLayer()
{
    if (model() != nullptr && m_entity != nullptr) {
        if (find(m_entity->name()) == nullptr) {
            model()->addObject(m_entity);
        }
    }
}

void CmdConnectionLayerManage::renameLayer()
{
    if (model() != nullptr && m_entity != nullptr) {
        if (find(m_entity->name()) != nullptr) {
            auto name = m_entity->name();
            m_entity->rename(m_entityPrevName);
            m_entityPrevName = name;
        }
    }
}

void CmdConnectionLayerManage::deleteLayer()
{
    if (model() != nullptr && m_entity != nullptr) {
        if (find(m_entity->name()) != nullptr) {
            model()->removeObject(m_entity);
        }
    }
}

CmdConnectionLayerCreate::CmdConnectionLayerCreate(const QString &name, ivm::IVModel *layersModel)
    : CmdConnectionLayerManage(layersModel)
{
    if (model() != nullptr && find(name) == nullptr) {
        model()->addObject(new ivm::IVConnectionLayerType(name, m_parent, shared::createId()));
        m_entity = find(name);
    } else {
        m_entity = nullptr;
    }
}

CmdConnectionLayerCreate::~CmdConnectionLayerCreate() {}

void CmdConnectionLayerCreate::undo()
{
    deleteLayer();
}

void CmdConnectionLayerCreate::redo()
{
    addLayer();
}

CmdConnectionLayerRename::CmdConnectionLayerRename(
        const QString &oldName, const QString &newName, ivm::IVModel *layersModel)
    : CmdConnectionLayerManage(layersModel)
{
    if (model() != nullptr) {
        m_entity = find(oldName);
        if (m_entity != nullptr) {
            m_entityPrevName = m_entity->name();
            m_entity->rename(newName);
        }
    }
}

CmdConnectionLayerRename::~CmdConnectionLayerRename() {}

void CmdConnectionLayerRename::undo()
{
    renameLayer();
}

void CmdConnectionLayerRename::redo()
{
    renameLayer();
}

CmdConnectionLayerDelete::CmdConnectionLayerDelete(const QString &name, ivm::IVModel *layersModel)
    : CmdConnectionLayerManage(layersModel)
{
    if (model() != nullptr) {
        m_entity = find(name);
        if (m_entity != nullptr) {
            model()->removeObject(m_entity);
        }
    }
}

CmdConnectionLayerDelete::~CmdConnectionLayerDelete() {}

void CmdConnectionLayerDelete::undo()
{
    addLayer();
}

void CmdConnectionLayerDelete::redo()
{
    deleteLayer();
}

}
}
