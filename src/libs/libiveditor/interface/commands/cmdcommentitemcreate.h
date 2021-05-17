/*
 Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include "commands/cmdentitygeometrychange.h"

#include <QPointer>

namespace ivm {
class IVFunctionType;
class IVComment;
class IVModel;
}

namespace ive {
namespace cmd {

class CmdCommentItemCreate : public shared::cmd::CmdEntityGeometryChange
{
public:
    explicit CmdCommentItemCreate(ivm::IVModel *model, ivm::IVFunctionType *parent, const QRectF &geometry);

    ~CmdCommentItemCreate() override;

    void redo() override;
    void undo() override;
    int id() const override;

private:
    QPointer<ivm::IVModel> m_model;
    QPointer<ivm::IVFunctionType> m_parent;
    QPointer<ivm::IVComment> m_entity;
};

}
}
