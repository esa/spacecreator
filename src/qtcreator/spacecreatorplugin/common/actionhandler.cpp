/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "actionhandler.h"

#include "editorcore.h"
#include "qtceditor.h"
#include "spacecreatorpluginconstants.h"

#include <QAction>
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

/**
 * @param id the ID of the editor type
 */
ActionHandler::ActionHandler(const Utils::Id &id, QObject *parent)
    : QObject(parent)
    , m_contextId(id)
    , m_undoGroup(new QUndoGroup(this))
{
    QObject::connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged, this,
            &spctr::ActionHandler::onCurrentEditorChanged);

    Core::Context contexts;
    contexts.add(m_contextId);

    QAction *undoAction = m_undoGroup->createUndoAction(this);
    undoAction->setIcon(Utils::Icons::UNDO_TOOLBAR.icon());
    undoAction->setToolTip(tr("Undo (Ctrl + Z)"));
    Core::ActionManager::registerAction(undoAction, Core::Constants::UNDO, contexts);

    QAction *redoAction = m_undoGroup->createRedoAction(this);
    redoAction->setIcon(Utils::Icons::REDO_TOOLBAR.icon());
    redoAction->setToolTip(tr("Redo (Ctrl + Y)"));
    Core::ActionManager::registerAction(redoAction, Core::Constants::REDO, contexts);

    m_deleteAction = new QAction(tr("Delete"), this);
    m_deleteAction->setToolTip(tr("Delete selected item"));
    m_deleteAction->setShortcut(QKeySequence::Delete);
    Core::Command *command =
            Core::ActionManager::registerAction(m_deleteAction, Constants::REMOVE_SELECTED_ELEMENTS, contexts);
    command->setDefaultKeySequence(m_deleteAction->shortcut());
}

void ActionHandler::onCurrentEditorChanged(Core::IEditor *editor)
{
    disconnect(m_deleteAction, nullptr, nullptr, nullptr);
    m_deleteAction->setEnabled(true);

    auto qtEditor = qobject_cast<QtCEditor *>(editor);
    if (!qtEditor) {
        return;
    }

    if (!qtEditor->context().contains(m_contextId)) {
        return;
    }

    shared::EditorCore *core = qtEditor->editorCore();

    m_undoGroup->setActiveStack(qtEditor->undoStack());

    syncActions(m_deleteAction, core->actionDelete());
}

void ActionHandler::syncActions(QAction *qtcAction, QAction *editorAction)
{
    if (qtcAction == nullptr || editorAction == nullptr) {
        return;
    }

    qtcAction->setEnabled(editorAction->isEnabled());
    connect(editorAction, &QAction::enabledChanged, qtcAction, &QAction::setEnabled);
    connect(qtcAction, &QAction::triggered, editorAction, &QAction::trigger);
}

}
