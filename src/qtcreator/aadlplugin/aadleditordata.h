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

#pragma once

#include <coreplugin/icontext.h>

class QUndoGroup;
class QToolBar;

namespace aadlinterface {
class IVEditorCore;
}

namespace Core {
class EditorToolBar;
class IEditor;
}

namespace AadlPlugin {

class AadlTextEditorFactory;
class AadlEditorStack;
class AadlContext;

class AadlEditorData : public QObject
{
    Q_OBJECT
public:
    AadlEditorData(QObject *parent = nullptr);
    ~AadlEditorData() override;

    void fullInit();
    Core::IEditor *createEditor();

    void showAsn1Dialog();
    void showMinimap(bool visible);
    void onAttributesManagerRequested();
    void onColorSchemeMenuInvoked();
    void onDynContextEditorMenuInvoked();

    aadlinterface::IVEditorCore *ivPlugin(const QString &fileName);

private:
    void updateToolBar();
    QWidget *createModeWidget();
    Core::EditorToolBar *createMainToolBar();

    AadlContext *m_context = nullptr;
    Core::Context m_contexts;
    QWidget *m_modeWidget = nullptr;
    AadlEditorStack *m_widgetStack = nullptr;
    QToolBar *m_widgetToolBar = nullptr;
    Core::EditorToolBar *m_mainToolBar = nullptr;
    QUndoGroup *m_undoGroup = nullptr;
    QAction *m_undoAction = nullptr;
    QAction *m_redoAction = nullptr;

    AadlTextEditorFactory *m_editorFactory = nullptr;
    bool m_minimapVisible = false;
};

}
