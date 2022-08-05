/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "entityattribute.h"
#include "undocommand.h"

#include <QPointer>
#include <QVector>

namespace ivm {
class IVFunctionType;
class IVArchetypeReference;
class ArchetypeModel;
}

namespace ive {
namespace cmd {

class CmdFunctionArchetypesApply : public shared::UndoCommand
{
    Q_OBJECT
public:
    explicit CmdFunctionArchetypesApply(ivm::IVFunctionType *function, QVector<ivm::IVArchetypeReference *> references,
            ivm::ArchetypeModel *archetypeModel);

    void redo() override;
    void undo() override;
    int id() const override;

private:
    QPointer<ivm::IVFunctionType> m_function;
    QVector<ivm::IVArchetypeReference *> m_newReferences;
    QVector<ivm::IVArchetypeReference *> m_oldReferences;
    QPointer<ivm::ArchetypeModel> m_archetypeModel;
};

} // namespace cmd
} // namespace ive
