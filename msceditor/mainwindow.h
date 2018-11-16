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

public Q_SLOTS:
    void openFile();
    void selectCurrentChart();

Q_SIGNALS:
    void currentGraphicsViewChanged(QGraphicsView *view);
private Q_SLOTS:
    void showSelection(const QModelIndex &current, const QModelIndex &previous);

private:
    std::unique_ptr<MainWindowPrivate> const d;

    void setupUi();
    void initMenus();
    void initMenuFile();
    void initMenuEdit();
    void initMenuHelp();
    void initTools();

    bool doOpenFile(const QString &file);
};

#endif // MAINWINDOW_H
