/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "commandlineparser.h"

#include <QMainWindow>
#include <QModelIndex>

#include <memory>

namespace Ui {
class MainWindow;
}

class MainModel;
class QUndoGroup;
class QGraphicsView;

struct MainWindowPrivate;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QGraphicsView *currentView() const;

    bool processCommandLineArg(CommandLineParser::Positional arg, const QString &value);

public Q_SLOTS:
    void selectAndOpenFile();
    void selectCurrentChart();
    void openAsn1Editor();

Q_SIGNALS:
    void currentGraphicsViewChanged(QGraphicsView *view);

private Q_SLOTS:
    void showSelection(const QModelIndex &current, const QModelIndex &previous);
    bool openFileMsc(const QString &file);
    bool openFileAsn(const QString &file);
    void saveMsc();
    void showDocumentView(bool show);
    void showHierarchyView(bool show);
    bool openMscChain(const QString &dirPath);

private:
    std::unique_ptr<MainWindowPrivate> const d;

    void setupUi();
    void initMenus();
    void initMenuFile();
    void initMenuEdit();
    void initMenuView();
    void initMenuViewWindows();
    void initMenuHelp();
    void initTools();
    void initConnections();

    void keyPressEvent(QKeyEvent *e) override;
};

#endif // MAINWINDOW_H
