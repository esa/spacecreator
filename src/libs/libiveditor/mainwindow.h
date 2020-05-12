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

namespace document {
class DocumentsManager;
}

namespace aadlinterface {
class IVEditorPlugin;
}

namespace taste3 {

class ZoomController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    bool processCommandLineArg(CommandLineParser::Positional arg, const QString &value);

protected:
    void closeEvent(QCloseEvent *e) override;

    void initMenus();
    void initMenuFile();
    void initMenuEdit();
    void initMenuHelp();
    void initConnections();
    void initTabs();
    void initSettings();

    void updateActions();
    void saveSceneRender(const QString &filePath) const;

protected Q_SLOTS:
    void onOpenFileRequested();
    void onCreateFileRequested();
    bool onCloseFileRequested();
    void onSaveRenderRequested();
    bool onExportXml();
    bool onExportAs();
    void onQuitRequested();
    void onAboutRequested();
    void onTabSwitched(int);
    void onReportRequested();
    void onDocDirtyChanged(bool dirty);
    void onGraphicsViewInfo(const QString &info);

private:
    static constexpr int TABDOC_ID_InterfaceView { 0 };

    Ui::MainWindow *ui { nullptr };
    QTabWidget *m_tabWidget { nullptr };
    ZoomController *m_zoomCtrl { nullptr };
    document::DocumentsManager *m_docsManager { nullptr };

    bool m_dropUnsavedChangesSilently { false };

    QMenu *m_menuFile { nullptr };
    QMenu *m_menuEdit { nullptr };
    QMenu *m_menuHelp { nullptr };

    QAction *m_actSaveSceneRender { nullptr };
    QAction *m_actExportAs { nullptr };

    QAction *m_actAbout { nullptr };
    QAction *m_actReport { nullptr };

    aadlinterface::IVEditorPlugin *m_plugin;

    document::AbstractTabDocument *currentDoc() const;
    bool closeTab(int id);
    bool prepareQuit();

    bool exportCurrentDocAsXml(const QString &savePath = QString(), const QString &templatePath = QString());
    bool exportDocsAs(const QString &savePath = QString(), const QString &templatePath = QString());

    bool exportDocAsXml(document::AbstractTabDocument *doc, const QString &pathToSave = QString(),
            const QString &templateToUse = QString());
    bool exportDocInteractive(document::AbstractTabDocument *doc, const QString &pathToSave = QString(),
            const QString &templateToUse = QString());
};

}
