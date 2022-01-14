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

#pragma once

#include "commandids.h"
#include "commands/cmdentitygeometrychange.h"
#include "common.h"
#include "ivconnectionlayertype.h"
#include "ivmodel.h"
#include "ivobject.h"

namespace ive {
namespace cmd {

class CmdConnectionLayerManage : public shared::UndoCommand
{
public:
    CmdConnectionLayerManage(ivm::IVModel *layersMode, ivm::IVModel *objectsModell);

    virtual ~CmdConnectionLayerManage();

    inline int id() const override { return ManageConnectionLayer; };

    auto model() const -> ivm::IVModel *;

    auto layer() const -> ivm::IVConnectionLayerType *;

    auto find(const QString &name) const -> ivm::IVConnectionLayerType *;

protected:
    auto addLayer() -> void;
    auto deleteLayer() -> void;

    ivm::IVModel *m_layersModel;
    ivm::IVModel *m_objectsModel;
    ivm::IVObject *m_parent;
    ivm::IVConnectionLayerType *m_layer;
    QString m_oldName;
    QString m_newName;
};

class CmdConnectionLayerCreate : public CmdConnectionLayerManage
{
public:
    explicit CmdConnectionLayerCreate(const QString &name, ivm::IVModel *layersModel, ivm::IVModel *objectsModel);
    ~CmdConnectionLayerCreate() override;

    void redo() override;

    void undo() override;
};

class CmdConnectionLayerRename : public CmdConnectionLayerManage
{
public:
    explicit CmdConnectionLayerRename(const QString &oldName, const QString &newName, ivm::IVModel *layersModel, ivm::IVModel *objectsModel);
    ~CmdConnectionLayerRename() override;

    void redo() override;

    void undo() override;
};

class CmdConnectionLayerDelete : public CmdConnectionLayerManage
{
public:
    explicit CmdConnectionLayerDelete(const QString &name, ivm::IVModel *layersModel, ivm::IVModel *objectsModel);
    ~CmdConnectionLayerDelete() override;

    void redo() override;

    void undo() override;
};

}
}
