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

#include <QMainWindow>

class QMenu;
class QAction;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

    void init();
    void initMenus();
    void initMenuFile();
    void initMenuHelp();

protected slots:
    void onOpenFileRequested();
    void onCreateFileRequested();
    bool onCloseFileRequested();
    void onQuitRequested();

    void onAboutRequested();

private:
    Ui::MainWindow *ui { nullptr };
    QMenu *m_menuFile { nullptr };
    QMenu *m_menuHelp { nullptr };

    QAction *m_actOpenFile { nullptr };
    QAction *m_actCreateFile { nullptr };
    QAction *m_actCloseFile { nullptr };
    QAction *m_actQuit { nullptr };

    QAction *m_actAbout { nullptr };

    void showNIY(const QString &caller);
};
