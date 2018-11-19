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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainmodel.h"

#include <documentitemmodel.h>
#include <mscchart.h>
#include <chartviewmodel.h>

#include <commands/common/commandsstack.h>

#include <tools/pointertool.h>
#include <tools/instancecreatortool.h>
#include <tools/messagecreatortool.h>

#include <QApplication>
#include <QComboBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QItemSelectionModel>
#include <QTreeView>
#include <QKeySequence>
#include <QApplication>
#include <QUndoGroup>
#include <QUndoStack>
#include <QToolBar>
#include <QActionGroup>

struct MainWindowPrivate {
    explicit MainWindowPrivate(MainWindow *mainWindow)
        : ui(new Ui::MainWindow)
        , m_model(new MainModel(mainWindow))
        , m_toolBar(new QToolBar(QObject::tr("Tools"), mainWindow))
        , m_undoGroup(new QUndoGroup(mainWindow))
        , m_tools({ new msc::PointerTool(nullptr, mainWindow),
                    new msc::InstanceCreatorTool(nullptr, mainWindow),
                    new msc::MessageCreatorTool(&(m_model->chartViewModel()), nullptr, mainWindow) })
    {
        m_toolBar->setAllowedAreas(Qt::AllToolBarAreas);
        mainWindow->addToolBar(Qt::LeftToolBarArea, m_toolBar);
    }

    ~MainWindowPrivate()
    {
        delete ui;
    }

    Ui::MainWindow *ui = nullptr;
    MainModel *m_model = nullptr;
    QToolBar *m_toolBar = nullptr;
    QUndoGroup *m_undoGroup = nullptr;

    QMenu *m_menuFile = nullptr;
    QAction *m_actOpenFile = nullptr;
    QAction *m_actQuit = nullptr;

    QMenu *m_menuEdit = nullptr;
    QAction *m_actUndo = nullptr;
    QAction *m_actRedo = nullptr;

    QMenu *m_menuHelp = nullptr;
    QAction *m_actAboutQt = nullptr;

    const QVector<msc::BaseTool *> m_tools;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate(this))
{
    setupUi();
    initConnections();

#ifdef DEVELOPER_AUTO_OPEN_MSC
    doOpenFile(QString(DEVELOPER_AUTO_OPEN_MSC).append("dengof.sample2.local.msc"));
#endif //DEVELOPER_AUTO_OPEN_MSC
}

QGraphicsView *MainWindow::currentView() const
{
    return d->ui->graphicsView;
}

MainWindow::~MainWindow()
{
    disconnect(&(d->m_model->chartViewModel()), nullptr, this, nullptr);
}

void MainWindow::openFile()
{
    const QString filename = QFileDialog::getOpenFileName(this, tr("MSC"), QString("../../msceditor/examples"), QString("*.msc"));
    if (!filename.isEmpty())
        doOpenFile(filename);
}

bool MainWindow::doOpenFile(const QString &file)
{
    d->ui->errorTextEdit->setPlainText(tr("Opening file: %1").arg(file));

    if (!QFileInfo::exists(file)) {
        d->ui->errorTextEdit->appendPlainText(tr("File not exists."));
        return false;
    }

    const bool ok = d->m_model->loadFile(file);
    if (ok) {
        static const QString title = tr("%1 [%2]");
        setWindowTitle(title.arg(qApp->applicationName()).arg(file));
        d->ui->documentTreeView->expandAll();
        d->ui->graphicsView->centerOn(d->ui->graphicsView->mapFromScene(d->ui->graphicsView->scene()->sceneRect().topLeft()));
    }

    d->ui->errorTextEdit->appendPlainText(d->m_model->errorMessages().join("\n"));
    d->ui->errorTextEdit->appendPlainText(tr("Model loading: %1").arg(ok ? tr("success") : tr("failed")));

    return ok;
}

void MainWindow::selectCurrentChart()
{
    msc::MscChart *chart = d->m_model->chartViewModel().currentChart();

    if (chart != nullptr) {
        const QModelIndex idx = d->m_model->documentItemModel()->index(chart);
        d->ui->documentTreeView->selectionModel()->select(idx, QItemSelectionModel::SelectCurrent);

        if (QUndoStack *currentStack = d->ui->graphicsView->undoStack()) {
            if (!d->m_undoGroup->stacks().contains(currentStack))
                d->m_undoGroup->addStack(currentStack);
            d->m_undoGroup->setActiveStack(currentStack);
        } else {
            d->m_undoGroup->removeStack(d->m_undoGroup->activeStack());
        }
        msc::cmd::CommandsStack::setCurrent(d->m_undoGroup->activeStack());

        // TODO: add routine to clear a stack on file close
        // TODO: add support for dedicated stacks for each tab
    }
}

void MainWindow::showSelection(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (!current.isValid()) {
        return;
    }

    auto *obj = static_cast<QObject *>(current.internalPointer());
    auto chart = dynamic_cast<msc::MscChart *>(obj);

    if (chart) {
        d->m_model->chartViewModel().fillView(chart);
    }
}

void MainWindow::setupUi()
{
    d->ui->setupUi(this);
    d->ui->graphicsView->setScene(d->m_model->graphicsScene());
    d->ui->documentTreeView->setModel(d->m_model->documentItemModel());

    initMenus();
    initTools();

    // status bar
    auto zoomBox = new QComboBox(d->ui->statusBar);
    zoomBox->addItem(" 50 %");
    zoomBox->addItem("100 %");
    zoomBox->addItem("200 %");
    zoomBox->addItem("400 %");
    zoomBox->setCurrentIndex(1);
    connect(zoomBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&](int index) {
        double percent = 100.0;
        if (index == 0) {
            percent = 50.0;
        }
        if (index == 2) {
            percent = 200.0;
        }
        if (index == 3) {
            percent = 400.0;
        }
        d->ui->graphicsView->setZoom(percent);
    });
    statusBar()->addPermanentWidget(zoomBox);
    statusBar()->show();
}

void MainWindow::initMenus()
{
    initMenuFile();
    initMenuEdit();
    initMenuHelp();
}

void MainWindow::initMenuFile()
{
    d->m_menuFile = menuBar()->addMenu(tr("File"));
    d->m_actOpenFile = d->m_menuFile->addAction(style()->standardIcon(QStyle::SP_DirOpenIcon), tr("&Open File"), this, &MainWindow::openFile, QKeySequence::Open);
    d->ui->mainToolBar->addAction(d->m_actOpenFile);
    d->m_menuFile->addSeparator();
    d->m_actQuit = d->m_menuFile->addAction(tr("&Quit"), qApp, &QApplication::quit, QKeySequence::Quit);
}

void MainWindow::initMenuEdit()
{
    d->m_actUndo = d->m_undoGroup->createUndoAction(this, tr("Undo:"));
    d->m_actUndo->setShortcut(QKeySequence::Undo);

    d->m_actRedo = d->m_undoGroup->createRedoAction(this, tr("Redo:"));
    d->m_actRedo->setShortcut(QKeySequence::Redo);

    d->m_menuEdit = menuBar()->addMenu(tr("Edit"));
    d->m_menuEdit->addAction(d->m_actUndo);
    d->m_menuEdit->addAction(d->m_actRedo);
    d->m_menuEdit->addSeparator();
}

void MainWindow::initMenuHelp()
{
    d->m_menuHelp = menuBar()->addMenu(tr("Help"));
    d->m_actAboutQt = d->m_menuHelp->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
}

void MainWindow::initTools()
{
    QActionGroup *toolsActions = new QActionGroup(this);
    for (msc::BaseTool *tool : d->m_tools) {
        QAction *toolAction = d->m_toolBar->addAction(tool->title());
        toolAction->setCheckable(true);
        toolAction->setIcon(tool->icon());
        toolAction->setToolTip(tr("%1: %2").arg(tool->title(), tool->description()));
        tool->setView(currentView());
        connect(this, &MainWindow::currentGraphicsViewChanged, tool, &msc::BaseTool::setView);

        toolsActions->addAction(toolAction);
        connect(toolAction, &QAction::toggled, tool, &msc::BaseTool::setActive);
    }

    toolsActions->actions().first()->setChecked(true);
}

void MainWindow::initConnections()
{
    connect(d->ui->documentTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::showSelection);

    connect(&(d->m_model->chartViewModel()), &msc::ChartViewModel::currentChartChagend, this, &MainWindow::selectCurrentChart);

    connect(d->ui->graphicsView, &msc::GraphicsView::mouseMoved, [this](const QPoint &screen, const QPointF &scene, const QPointF &item) {
        statusBar()->showMessage(tr("Screen: [%1;%2]\tScene: [%3;%4]\tObject: [%5;%6]")
                                         .arg(screen.x())
                                         .arg(screen.y())
                                         .arg(scene.x())
                                         .arg(scene.y())
                                         .arg(item.x())
                                         .arg(item.y()));
    });
}
