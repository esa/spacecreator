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

#include "asn1editor.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainmodel.h"
#include "commandlineparser.h"

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
#include <QMessageBox>
#include <QDebug>

struct MainWindowPrivate {
    explicit MainWindowPrivate(MainWindow *mainWindow)
        : ui(new Ui::MainWindow)
        , m_model(new MainModel(mainWindow))
        , m_toolBar(new QToolBar(QObject::tr("Tools"), mainWindow))
        , m_undoGroup(new QUndoGroup(mainWindow))
        , m_tools({ new msc::PointerTool(nullptr, mainWindow),
                    new msc::InstanceCreatorTool(&(m_model->chartViewModel()), nullptr, mainWindow),
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
    QAction *m_actSaveFile = nullptr;
    QAction *m_actQuit = nullptr;

    QMenu *m_menuEdit = nullptr;
    QAction *m_actUndo = nullptr;
    QAction *m_actRedo = nullptr;

    QMenu *m_menuView = nullptr;
    QAction *m_actShowDocument = nullptr;
    QAction *m_actShowHierarchy = nullptr;

    QMenu *m_menuHelp = nullptr;
    QAction *m_actAboutQt = nullptr;

    const QVector<msc::BaseTool *> m_tools;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate(this))
{
    setupUi();

    // TODO: just for test Asn1Editor
    QAction *actionAsn1Editor = new QAction("ASN.1 Editor", d->ui->mainToolBar);
    d->ui->mainToolBar->addAction(actionAsn1Editor);
    connect(actionAsn1Editor, &QAction::triggered, this, &MainWindow::openAsn1Editor);

    initConnections();

    selectCurrentChart();
}

QGraphicsView *MainWindow::currentView() const
{
    return d->ui->graphicsView;
}

MainWindow::~MainWindow()
{
    disconnect(&(d->m_model->chartViewModel()), nullptr, this, nullptr);
}

void MainWindow::selectAndOpenFile()
{
    static const QLatin1String suffixMsc(".msc");
    static const QLatin1String suffixAsn(".asn");
    static const QStringList suffixes = { "*" + suffixMsc, "*" + suffixAsn };

    const QString path =
#if defined(QT_DEBUG) && defined(Q_OS_WIN)
            QDir(QDir::current().path() + QString("/../../esa/examples")).path();
#else
            "../../msceditor/examples";
#endif // QT_DEBUG and Q_OS_WIN

    const QString filename = QFileDialog::getOpenFileName(this, tr("MSC"), path, suffixes.join(" "));
    if (!filename.isEmpty()) {
        d->ui->errorTextEdit->clear();
        if (filename.endsWith(suffixMsc))
            openFileMsc(filename);
        else if (filename.endsWith(suffixAsn))
            openFileAsn(filename);
    }
}

bool MainWindow::openFileMsc(const QString &file)
{
    d->ui->errorTextEdit->appendPlainText(tr("Opening file: %1").arg(file));

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

    d->ui->errorTextEdit->appendHtml(d->m_model->errorMessages().join("\n"));
    d->ui->errorTextEdit->appendHtml(tr("Model loading: <b><font color=%2>%1</font></b><br>")
                                             .arg(ok ? tr("success") : tr("failed"),
                                                  ok ? "black" : "red"));

    return ok;
}

bool MainWindow::openMscChain(const QString &dirPath)
{
    if (dirPath.isEmpty())
        return false;

    QDir dir(dirPath);
    if (!dir.exists() || !dir.isReadable())
        return false;

    for (const QFileInfo &file : dir.entryInfoList({ "*.msc" })) {
        openFileMsc(file.absoluteFilePath());
    }
    return true;
}

bool MainWindow::openFileAsn(const QString &file)
{
    QMessageBox::information(this, "Not implemented yet", QString("Opening the ASN file:\n%1").arg(file));
    return false;
}

void MainWindow::saveMsc()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("MSC"),
                                                    "",
                                                    tr("MSC files (*.msc);;All files (*.*)"));
    d->m_model->saveMsc(fileName);
}

void MainWindow::showDocumentView(bool show)
{
    if (show) {
        d->ui->centerView->setCurrentWidget(d->ui->graphicsView);
    }
}

void MainWindow::showHierarchyView(bool show)
{
    if (show) {
        d->ui->centerView->setCurrentWidget(d->ui->hierarchyView);
    }
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

void MainWindow::openAsn1Editor()
{
    asn1::Asn1Editor editor;
    editor.exec();
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

    d->ui->hierarchyView->setBackgroundBrush(QImage(":/resources/resources/texture.png"));
    d->ui->hierarchyView->setScene(d->m_model->hierarchyScene());

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
    initMenuView();
    initMenuHelp();
}

void MainWindow::initMenuFile()
{
    d->m_menuFile = menuBar()->addMenu(tr("File"));

    d->m_actOpenFile = d->m_menuFile->addAction(style()->standardIcon(QStyle::SP_DirOpenIcon), tr("&Open File"), this, &MainWindow::selectAndOpenFile, QKeySequence::Open);
    d->ui->mainToolBar->addAction(d->m_actOpenFile);

    d->m_actSaveFile = d->m_menuFile->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Save"), this, &MainWindow::saveMsc, QKeySequence::Save);
    d->ui->mainToolBar->addAction(d->m_actSaveFile);

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

void MainWindow::initMenuView()
{
    d->m_menuView = menuBar()->addMenu(tr("&View"));
    d->m_actShowDocument = d->m_menuView->addAction(tr("Show &Document"), this, &MainWindow::showDocumentView, tr("F8"));
    d->m_actShowHierarchy = d->m_menuView->addAction(tr("Show &Hierarchy"), this, &MainWindow::showHierarchyView, tr("F9"));

    d->m_actShowDocument->setCheckable(true);
    d->m_actShowHierarchy->setCheckable(true);
    auto group = new QActionGroup(d->m_menuView);
    group->addAction(d->m_actShowDocument);
    group->addAction(d->m_actShowHierarchy);
    d->m_actShowDocument->setChecked(true);
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

bool MainWindow::processCommandLineArg(CommandLineParser::Positional arg, const QString &value)
{
    switch (arg) {
    case CommandLineParser::Positional::OpenFileMsc: {
        return openFileMsc(value);
    }
    case CommandLineParser::Positional::OpenFileAsn: {
        return openFileAsn(value);
    }
    case CommandLineParser::Positional::DbgOpenMscExamplesChain: {
        return openMscChain(value);
    }
    default:
        qWarning() << Q_FUNC_INFO << "Unhandled option:" << arg << value;
        break;
    }
    return false;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    QMainWindow::keyPressEvent(e);

    switch (e->key()) {
    case Qt::Key_Escape: {
        if (!e->isAutoRepeat())
            if (QAction *pointerToolAction = d->m_toolBar->actions().first())
                if (!pointerToolAction->isChecked())
                    pointerToolAction->setChecked(true);
        break;
    }
    }
}
