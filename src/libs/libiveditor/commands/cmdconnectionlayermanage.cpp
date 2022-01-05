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

CmdConnectionLayerManage::CmdConnectionLayerManage(ivm::IVModel *layersModel, ivm::IVModel *objectsModel)
    : shared::UndoCommand()
    , m_layersModel(layersModel)
    , m_objectsModel(objectsModel)
    , m_parent(layersModel != nullptr ? layersModel->rootObject() : nullptr)
    , m_renameValue("")
{
}

CmdConnectionLayerManage::~CmdConnectionLayerManage() {}

ivm::IVModel *CmdConnectionLayerManage::model() const
{
    return m_layersModel;
}

ivm::IVConnectionLayerType *CmdConnectionLayerManage::layer() const
{
    return m_layer;
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
    if (model() != nullptr && m_layer != nullptr) {
        if (find(m_layer->name()) == nullptr) {
            model()->addObject(m_layer);
        }
    }
}

void CmdConnectionLayerManage::renameLayer()
{
    if (model() != nullptr && m_layer != nullptr && m_objectsModel != nullptr) {
        if (find(m_layer->name()) != nullptr) {
            auto name = m_layer->name();
            m_layer->rename(m_renameValue);
            m_renameValue = name;
        }
    }
}

void CmdConnectionLayerManage::deleteLayer()
{
    if (model() != nullptr && m_layer != nullptr) {
        if (find(m_layer->name()) != nullptr) {
            model()->removeObject(m_layer);
        }
    }
}

CmdConnectionLayerCreate::CmdConnectionLayerCreate(
        const QString &name, ivm::IVModel *layersModel, ivm::IVModel *objectsModel)
    : CmdConnectionLayerManage(layersModel, objectsModel)
{
    if (model() != nullptr && find(name) == nullptr) {
        model()->addObject(new ivm::IVConnectionLayerType(name, m_parent, shared::createId()));
        m_layer = find(name);
    } else {
        m_layer = nullptr;
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
        const QString &oldName, const QString &newName, ivm::IVModel *layersModel, ivm::IVModel *objectsModel)
    : CmdConnectionLayerManage(layersModel, objectsModel)
{
    if (model() != nullptr) {
        m_layer = find(oldName);
        if (m_layer != nullptr && find(newName) == nullptr) {
            m_renameValue = newName;
        } else {
            m_layer = nullptr;
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

CmdConnectionLayerDelete::CmdConnectionLayerDelete(
        const QString &name, ivm::IVModel *layersModel, ivm::IVModel *objectsModel)
    : CmdConnectionLayerManage(layersModel, objectsModel)
{
    if (model() != nullptr) {
        m_layer = find(name);
        if (m_layer != nullptr) {
            model()->removeObject(m_layer);
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
