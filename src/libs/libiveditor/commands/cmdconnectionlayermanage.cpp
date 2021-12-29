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

namespace ive {
namespace cmd {

CmdConnectionLayerManage::CmdConnectionLayerManage(ivm::IVModel *objectsModel)
    : shared::UndoCommand()
    , m_objectsModel(objectsModel)
    , m_parent(objectsModel != nullptr ? objectsModel->rootObject() : nullptr)
{
}

ivm::IVConnectionLayerType *CmdConnectionLayerManage::createLayer()
{
    QString newName;
    if (model() != nullptr && model()->getConnectionLayersModel() != nullptr) {
        if (model()->getConnectionLayerByName(newName) == nullptr) {
            model()->getConnectionLayersModel()->addObject(
                    new ivm::IVConnectionLayerType(newName, m_parent, shared::createId()));
        }
    }
    return model()->getConnectionLayerByName(newName);
}

ivm::IVConnectionLayerType *CmdConnectionLayerManage::renameLayer()
{
    QString oldName;
    QString newName;
    if (model() != nullptr) {
        auto *layer = model()->getConnectionLayerByName(oldName);
        layer->rename(newName);
        return layer;
    }
    return nullptr;
}

bool CmdConnectionLayerManage::deleteLayer()
{
    QString name;
    if (model() != nullptr && model()->getConnectionLayersModel() != nullptr) {
        auto *layer = model()->getConnectionLayerByName(name);
        if (layer != nullptr) {
            return model()->getConnectionLayersModel()->removeObject(layer);
        }
    }
    return false;
}

ivm::IVModel *CmdConnectionLayerManage::model() const
{
    return m_objectsModel;
}

}
}
