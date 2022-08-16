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
#include "interfacedocument.h"
#include "ivarchetypelibraryreference.h"
#include "ivarchetypereference.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"

namespace ive {
namespace cmd {

CmdArchetypeLibraryApply::CmdArchetypeLibraryApply(ive::InterfaceDocument *document, ivm::IVModel *objectsModel,
        QVector<ivm::IVArchetypeLibraryReference *> references)
    : shared::UndoCommand()
    , m_document(document)
    , m_model(objectsModel)
    , m_newReferences(references)
    , m_oldReferences(objectsModel->getArchetypeLibraryReferences())
{
    auto allFunctionTypes = m_model->allObjectsByType<ivm::IVFunctionType>();

    for (auto functionType : allFunctionTypes) {
        m_oldFunctionsArchetypeReferences.insert(functionType->id(), functionType->archetypeReferences());
    }

    for (auto id : m_oldFunctionsArchetypeReferences.keys()) {
        QVector<ivm::IVArchetypeReference *> newReferences;
        for (auto reference : m_oldFunctionsArchetypeReferences[id]) {
            if (isReferencePresent(reference->getLibraryName())) {
                newReferences.append(reference);
            }
        }
        m_newFunctionsArchetypeReferences.insert(id, newReferences);
    }
}

void CmdArchetypeLibraryApply::redo()
{
    if (m_model != nullptr && m_document != nullptr) {
        m_model->setArchetypeLibraryReferences(m_newReferences);
        applyReferences(m_newFunctionsArchetypeReferences);
        m_document->loadArchetypes();
    }
}

void CmdArchetypeLibraryApply::undo()
{
    if (m_model != nullptr && m_document != nullptr) {
        m_model->setArchetypeLibraryReferences(m_oldReferences);
        applyReferences(m_oldFunctionsArchetypeReferences);
        m_document->loadArchetypes();
    }
}

int CmdArchetypeLibraryApply::id() const
{
    return ApplyArchetypeLibraries;
}

void CmdArchetypeLibraryApply::applyReferences(
        QHash<shared::Id, QVector<ivm::IVArchetypeReference *>> refencesForFunctions)
{
    for (auto id : refencesForFunctions.keys()) {
        auto functionType = qobject_cast<ivm::IVFunctionType *>(m_model->getObject(id));
        if (functionType != nullptr) {
            functionType->setArchetypeReferences(refencesForFunctions[id]);
        }
    }
}

bool CmdArchetypeLibraryApply::isReferencePresent(const QString &libraryName)
{
    if (libraryName.isEmpty()) {
        return false;
    }

    for (auto reference : m_newReferences) {
        if (reference->getLibraryName() == libraryName) {
            return true;
        }
    }
    return false;
}

} // namespace cmd
} // namespace ive
