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

#pragma once

#include <QObject>
#include <id.h>

class QAction;
class QUndoGroup;

namespace Core {
class IEditor;
}

namespace spctr {

/**
 * Class do handle undo/redo and similar actions for the different editors/documents inside QtCreator
 */
class ActionHandler : public QObject
{
    Q_OBJECT
public:
    explicit ActionHandler(const Utils::Id &id, QObject *parent = nullptr);

private Q_SLOTS:
    void onCurrentEditorChanged(Core::IEditor *editor);

private:
    void syncActions(QAction *qtcAction, QAction *editorAction);

    Utils::Id m_contextId;
    QUndoGroup *m_undoGroup = nullptr;
    QAction *m_deleteAction = nullptr;
};

}
