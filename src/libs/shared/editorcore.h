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

#include "minimap.h"

#include <QObject>
#include <QPointer>
#include <QToolBar>

class QAction;
class QMainWindow;
class QMenu;
class QUndoStack;

namespace shared {

class CommandLineParser;

namespace ui {
class GraphicsViewBase;
}

class EditorCore : public QObject
{
    Q_OBJECT

public:
    explicit EditorCore(QObject *parent = nullptr);
    virtual ~EditorCore();

    // Override this to show or hide actions
    virtual void setPluginActive(bool active) = 0;

    QWidget *minimapView() const;
    virtual ui::GraphicsViewBase *chartView() = 0;
    virtual QToolBar *mainToolBar();
    virtual void addToolBars(QMainWindow *window) = 0;

    // Set up all the menus
    void initMenus(QMainWindow *window);
    virtual void addMenuFileActions(QMenu *menu, QMainWindow *window) = 0;
    virtual void addMenuEditActions(QMenu *menu, QMainWindow *window) = 0;
    virtual void addMenuViewActions(QMenu *menu, QMainWindow *window);
    virtual void addMenuHelpActions(QMenu *menu, QMainWindow *window) = 0;

    QAction *actionNewFile();
    QAction *actionOpenFile();
    QAction *actionSaveFile();
    QAction *actionSaveFileAs();
    QAction *actionOpenAadl();
    QAction *actionQuit();
    QAction *actionUndo();
    QAction *actionRedo();
    QAction *actionToggleMinimap();

    // Populate a CommandLineParser with the arguments this application can handle
    virtual void populateCommandLineArguments(CommandLineParser *parser) const = 0;

    void setupMiniMap();

    virtual QUndoStack *undoStack() const = 0;

    virtual bool renameAsnFile(const QString &oldName, const QString &newName) = 0;

    /// Returns the full file path of the file in this core
    virtual QString filePath() const = 0;
    /// Saves the file
    virtual bool save() = 0;

public Q_SLOTS:
    void showAboutDialog();

Q_SIGNALS:
    void editedExternally(shared::EditorCore *);

private:
    QMainWindow *m_mainWindow = { nullptr };
    QPointer<QToolBar> m_mainToolBar { nullptr };

    QPointer<ui::MiniMap> m_miniMap { nullptr };

    QAction *m_actionNewFile { nullptr };
    QAction *m_actionOpenFile { nullptr };
    QAction *m_actionSaveFile { nullptr };
    QAction *m_actionSaveFileAs { nullptr };
    QAction *m_actionCloseFile { nullptr };
    QAction *m_actionOpenAadl { nullptr };
    QAction *m_actionQuit { nullptr };
    QAction *m_actionUndo { nullptr };
    QAction *m_actionRedo { nullptr };
    QAction *m_actionToggleMinimap { nullptr };
};

}
