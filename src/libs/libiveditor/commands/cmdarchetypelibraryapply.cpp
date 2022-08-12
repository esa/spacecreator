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

#include "cmdarchetypelibraryapply.h"

#include "commandids.h"
#include "ivarchetypelibraryreference.h"
#include "ivmodel.h"

namespace ive {
namespace cmd {

CmdArchetypeLibraryApply::CmdArchetypeLibraryApply(
        ivm::IVModel *objectsModel, QVector<ivm::IVArchetypeLibraryReference *> references)
    : shared::UndoCommand()
    , m_model(objectsModel)
    , m_newReferences(references)
    , m_oldReferences(objectsModel->getArchetypeLibraryReferences())
{
}

void CmdArchetypeLibraryApply::redo()
{
    if (m_model != nullptr) {
        m_model->setArchetypeLibraryReferences(m_newReferences);
    }
}

void CmdArchetypeLibraryApply::undo()
{
    if (m_model != nullptr) {
        m_model->setArchetypeLibraryReferences(m_oldReferences);
    }
}

int CmdArchetypeLibraryApply::id() const
{
    return ApplyArchetypeLibraries;
}

} // namespace cmd
} // namespace ive
