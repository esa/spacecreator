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

#include "commandlineparser.h"
#include "document/abstracttabdocument.h"

#include <QMainWindow>
#include <QPointer>
#include <QToolBar>

class QMenu;
class QAction;
class QTabWidget;
class QUndoGroup;

namespace Ui {
class MainWindow;
}

namespace taste3 {

namespace document {
class DocumentsManager;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    bool processCommandLineArg(CommandLineParser::Positional arg, const QString &value);

protected:
    void changeEvent(QEvent *e) override;
    void closeEvent(QCloseEvent *e) override;

    void init();
    void initMenus();
    void initMenuFile();
    void initMenuEdit();
    void initMenuHelp();
    void initConnections();
    void initTabs();
    void initSettings();

    void updateActions();
    void saveSceneRender(const QString &filePath) const;

protected slots:
    void onOpenFileRequested();
    void onCreateFileRequested();
    bool onCloseFileRequested();
    void onSaveRenderRequested();
    void onQuitRequested();
    void onAboutRequested();
    void onTabSwitched(int);
    void onReportRequested();

private:
    static constexpr int TABDOC_ID_InterfaceView {0};

    Ui::MainWindow *ui { nullptr };
    QTabWidget *m_tabWidget;
    QPointer<QToolBar> m_docToolbar;
    document::DocumentsManager *m_docsManager;
    QPointer<document::AbstractTabDocument> m_currentDocument;
    QUndoGroup *m_undoGroup { nullptr };
    bool m_dropUnsavedChangesSilently { false };

    QMenu *m_menuFile { nullptr };
    QMenu *m_menuEdit { nullptr };
    QMenu *m_menuHelp { nullptr };

    QAction *m_actOpenFile { nullptr };
    QAction *m_actCreateFile { nullptr };
    QAction *m_actCloseFile { nullptr };
    QAction *m_actSaveSceneRender { nullptr };
    QAction *m_actQuit { nullptr };

    QAction *m_actUndo { nullptr };
    QAction *m_actRedo { nullptr };

    QAction *m_actAbout { nullptr };
    QAction *m_actReport { nullptr };

    void showNIY(const QString &caller);
};

} // ns taste3
