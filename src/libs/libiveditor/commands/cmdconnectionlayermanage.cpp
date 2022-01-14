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
    , m_layer(nullptr)
    , m_oldName("")
    , m_newName("")
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

void CmdConnectionLayerManage::deleteLayer()
{
    if (model() != nullptr && m_layer != nullptr) {
        if (find(m_layer->name()) != nullptr) {
            for (auto *iface : m_objectsModel->allObjectsByType<ivm::IVInterface>()) {
                if (iface->layerName().compare(m_layer->name()) == 0) {
                    iface->setLayerName(ivm::IVConnectionLayerType::DefaultLayerName);
                }
            }
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
    if (model() != nullptr && m_objectsModel != nullptr) {
        if (oldName.compare(newName) != 0 && find(oldName) != nullptr && find(newName) == nullptr) {
            m_layer = find(oldName);
            m_oldName = oldName;
            m_newName = newName;
        }
    }
}

CmdConnectionLayerRename::~CmdConnectionLayerRename() {}

void CmdConnectionLayerRename::undo()
{
    if (model() != nullptr && m_layer != nullptr && m_objectsModel != nullptr) {
        for (auto *iface : m_objectsModel->allObjectsByType<ivm::IVInterface>()) {
            if (iface->layerName().compare(m_layer->name()) == 0) {
                iface->setLayerName(m_oldName);
            }
        }
        m_layer->rename(m_oldName);
    }
}

void CmdConnectionLayerRename::redo()
{
    if (model() != nullptr && m_layer != nullptr && m_objectsModel != nullptr) {
        for (auto *iface : m_objectsModel->allObjectsByType<ivm::IVInterface>()) {
            if (iface->layerName().compare(m_layer->name()) == 0) {
                iface->setLayerName(m_newName);
            }
        }
        m_layer->rename(m_newName);
    }
}

CmdConnectionLayerDelete::CmdConnectionLayerDelete(
        const QString &name, ivm::IVModel *layersModel, ivm::IVModel *objectsModel)
    : CmdConnectionLayerManage(layersModel, objectsModel)
{
    if (model() != nullptr) {
        m_layer = find(name);
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
