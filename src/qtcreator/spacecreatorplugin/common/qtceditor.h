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

#include <editormanager/ieditor.h>

class QAction;
class QUndoStack;

namespace shared {
class EditorCore;
}

namespace spctr {

/**
 * Base class for all QtCreator editors provided by space creator
 */
class QtCEditor : public Core::IEditor
{
    Q_OBJECT
public:
    QtCEditor();

    virtual shared::EditorCore *editorCore() const = 0;

    /**
     * Returns the undo stack of the editor/document
     */
    virtual QUndoStack *undoStack() const;
};

} // namespace spctr
