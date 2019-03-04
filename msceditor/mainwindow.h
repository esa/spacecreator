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

namespace msc {
class MscDocument;
}

class MainModel;
class QUndoGroup;
class QGraphicsView;
class QPlainTextEdit;

struct MainWindowPrivate;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QGraphicsView *currentView() const;

    bool processCommandLineArg(CommandLineParser::Positional arg, const QString &value);
    QPlainTextEdit *textOutputPane() const;

public Q_SLOTS:
    void createNewDocument();
    void selectAndOpenFile();
    void selectCurrentChart();
    void openAsn1Editor();

Q_SIGNALS:
    void currentGraphicsViewChanged(QGraphicsView *view);
    void selectionChanged(const msc::MscDocument *document);

private Q_SLOTS:
    void showSelection(const QModelIndex &current, const QModelIndex &previous);
    bool openFileMsc(const QString &file);
    bool openFileAsn(const QString &file);
    void saveMsc();
    void saveAsMsc();
    void showDocumentView(bool show);
    void showHierarchyView(bool show);
    void showErrorView();
    bool openMscChain(const QString &dirPath);
    void activateDefaultTool();
    void updateTextView();
    void showHierarchyTypeMenu(const QPoint &point);
    void changHeierarchyType();
    void updateTreeViewItem(const msc::MscDocument *document);
    void onCreateMessageToolRequested();

private:
    static const QLatin1String DotMscFileExtensionLow;
    std::unique_ptr<MainWindowPrivate> const d;

    void loadSettings();
    void saveSettings();

    void setupUi();
    void initActions();
    void initMenus();
    void initMenuFile();
    void initMenuEdit();
    void initMenuView();
    void initHierarchyTypeActions();
    void initMenuViewWindows();
    void initMenuHelp();
    void initTools();
    void initMainToolbar();
    void initConnections();

    void closeEvent(QCloseEvent *e) override;

    void keyPressEvent(QKeyEvent *e) override;

    void updateTitles();
    void clearUndoStacks();

    void changeEvent(QEvent *event) override;
    void onGeometryRestored();
    QSizeF prepareChartBoxSize() const;

    void updateMscToolbarActionsEnablement();
    void updateMscToolbarActionsChecked();

    static QStringList mscFileFilters();
};

#endif // MAINWINDOW_H
