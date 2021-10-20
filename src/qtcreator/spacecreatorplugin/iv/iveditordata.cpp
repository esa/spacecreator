/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "iveditordata.h"

#include "iveditordocument.h"
#include "ivqtceditor.h"
#include "msc/msccontext.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectmanager.h"

#include <QUndoGroup>
#include <QUndoStack>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <utils/icon.h>
#include <utils/utilsicons.h>

namespace spctr {

IVEditorData::IVEditorData(SpaceCreatorProjectManager *projectManager, QObject *parent)
    : QObject(parent)
    , m_undoGroup(new QUndoGroup(this))
    , m_projectManager(projectManager)
{
    Core::Context contexts;
    contexts.add(spctr::Constants::K_IV_EDITOR_ID);

    QObject::connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged, this,
            &spctr::IVEditorData::onCurrentEditorChanged);

    QAction *undoAction = m_undoGroup->createUndoAction(this);
    undoAction->setIcon(Utils::Icons::UNDO_TOOLBAR.icon());
    undoAction->setToolTip(tr("Undo (Ctrl + Z)"));

    QAction *redoAction = m_undoGroup->createRedoAction(this);
    redoAction->setIcon(Utils::Icons::REDO_TOOLBAR.icon());
    redoAction->setToolTip(tr("Redo (Ctrl + Y)"));

    Core::ActionManager::registerAction(undoAction, Core::Constants::UNDO, contexts);
    Core::ActionManager::registerAction(redoAction, Core::Constants::REDO, contexts);

    contexts.add(Core::Constants::C_EDITORMANAGER);
    m_context = new MscContext(contexts, nullptr, this);
    Core::ICore::addContextObject(m_context);
}

IVEditorData::~IVEditorData()
{
    if (m_context) {
        Core::ICore::removeContextObject(m_context);
    }
}

Core::IEditor *IVEditorData::createEditor()
{
    auto *ivEditor = new IVQtCEditor(m_projectManager);
    return ivEditor;
}

void IVEditorData::onCurrentEditorChanged(Core::IEditor *editor)
{
    if (auto ivEditor = qobject_cast<IVQtCEditor *>(editor)) {
        m_undoGroup->setActiveStack(ivEditor->ivPlugin()->undoStack());
    }
}

}
