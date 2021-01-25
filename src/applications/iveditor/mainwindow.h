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
#include "common.h"

#include <QAction>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace shared {
namespace ui {
class MiniMap;
}
}
namespace ive {

class IVEditorCore;
class ZoomController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ive::IVEditorCore *core, QWidget *parent = nullptr);
    ~MainWindow() override;

    bool processCommandLineArg(shared::CommandLineParser::Positional arg, const QString &value);

protected:
    void closeEvent(QCloseEvent *e) override;

    void initSettings();

    void updateActions();

public Q_SLOTS:
    void onOpenFileRequested();
    void onCreateFileRequested();
    void onExportFunctionsRequested();
    void onExportTypeRequested();
    bool exportXml(const QString &savePath = QString(), const QString &templatePath = QString());
    bool exportXmlAs(const QString &savePath = QString(), const QString &templatePath = QString());
    void onQuitRequested();
    void onReportRequested();
    void onDocDirtyChanged(bool dirty);
    void onGraphicsViewInfo(const QString &info);
    void updateWindowTitle();
    void openAsn1Dialog();
    void showAsn1Errors(const QStringList &faultyInterfaces);

private:
    void initMenus();
    bool closeFile();
    bool prepareQuit();

    Ui::MainWindow *ui { nullptr };
    ZoomController *m_zoomCtrl { nullptr };
    QAction *m_actionSaveSceneRender { nullptr };
    QAction *m_actionShowAsnDialog { nullptr };

    bool m_dropUnsavedChangesSilently { false };

    ive::IVEditorCore *m_core { nullptr };
};

}
