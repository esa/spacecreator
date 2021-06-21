/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace dve {

class DVEditorCore;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(dve::DVEditorCore *core, QWidget *parent = nullptr);
    ~MainWindow() override;

public Q_SLOTS:
    void onSaveFileRequested();
    void onSaveFileAsRequested();
    void onOpenFileRequested();
    void showColorScheme();
    void editHwLibrary();

protected:
    void closeEvent(QCloseEvent *e) override;

private:
    void initActions();
    void initMenus();
    void initSettings();
    bool closeFile();
    bool prepareQuit();
    void updateWindowTitle();
    void onDocDirtyChanged(bool dirty);
    void onQuitRequested();

    Ui::MainWindow *ui { nullptr };
    dve::DVEditorCore *m_core { nullptr };
};

} // namespace dve
