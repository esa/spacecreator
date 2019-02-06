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

#include "asn1editor.h"
#include "chartviewmodel.h"
#include "commandlineparser.h"
#include "commands/common/commandsstack.h"
#include "documentitemmodel.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "settings/appoptions.h"
#include "textview.h"
#include "tools/actioncreatortool.h"
#include "tools/entitydeletetool.h"
#include "tools/hierarchycreatortool.h"
#include "tools/instancecreatortool.h"
#include "tools/messagecreatortool.h"
#include "tools/pointertool.h"
#include "ui_mainwindow.h"

#include <QActionGroup>
#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QItemSelectionModel>
#include <QKeySequence>
#include <QMessageBox>
#include <QToolBar>
#include <QTreeView>
#include <QUndoGroup>
#include <QUndoStack>

struct MainWindowPrivate {
    explicit MainWindowPrivate(MainWindow *mainWindow)
        : ui(new Ui::MainWindow)
        , m_model(new MainModel(mainWindow))
        , m_mscToolBar(new QToolBar(QObject::tr("MSC"), mainWindow))
        , m_hierarchyToolBar(new QToolBar(QObject::tr("Hierarchy"), mainWindow))
        , m_undoGroup(new QUndoGroup(mainWindow))
    {
        m_mscToolBar->setObjectName("mscTools");
        m_mscToolBar->setAllowedAreas(Qt::AllToolBarAreas);
        mainWindow->addToolBar(Qt::LeftToolBarArea, m_mscToolBar);

        m_hierarchyToolBar->setObjectName("hierarchyTools");
        m_hierarchyToolBar->setAllowedAreas(Qt::AllToolBarAreas);
        mainWindow->addToolBar(Qt::LeftToolBarArea, m_hierarchyToolBar);
    }

    ~MainWindowPrivate() { delete ui; }

    Ui::MainWindow *ui = nullptr;

    QComboBox *m_zoomBox = nullptr;

    QString m_mscFileName;
    MainModel *m_model = nullptr;
    QToolBar *m_mscToolBar = nullptr;
    QToolBar *m_hierarchyToolBar = nullptr;
    QUndoGroup *m_undoGroup = nullptr;

    QMenu *m_menuFile = nullptr;
    QAction *m_actNewFile = nullptr;
    QAction *m_actOpenFile = nullptr;
    QAction *m_actSaveFile = nullptr;
    QAction *m_actSaveFileAs = nullptr;
    QAction *m_actQuit = nullptr;

    QMenu *m_menuEdit = nullptr;
    QAction *m_actUndo = nullptr;
    QAction *m_actRedo = nullptr;

    QMenu *m_menuView = nullptr;
    QAction *m_actShowDocument = nullptr;
    QAction *m_actShowHierarchy = nullptr;

    QMenu *m_menuViewWindows = nullptr;
    QAction *m_actToggleErrorView = nullptr;
    QAction *m_actToggleHierarchyView = nullptr;
    QAction *m_actToggleMscTextView = nullptr;

    QMenu *m_menuHelp = nullptr;
    QAction *m_actAboutQt = nullptr;

    QAction *m_actAsnEditor = nullptr;

    QVector<msc::BaseTool *> m_tools;
    QAction *m_defaultToolAction = nullptr;
    msc::EntityDeleteTool *m_deleteTool = nullptr;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate(this))
{
    setupUi();
    d->ui->hierarchyView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing
                                         | QPainter::SmoothPixmapTransform);

    initConnections();

    selectCurrentChart();

    loadSettings();

    d->m_mscToolBar->setVisible(d->ui->centerView->currentWidget() == d->ui->graphicsView);
    d->m_hierarchyToolBar->setVisible(d->ui->centerView->currentWidget() == d->ui->hierarchyView);
    d->ui->documentTreeView->expandAll();
}

QGraphicsView *MainWindow::currentView() const
{
    return d->ui->graphicsView;
}

MainWindow::~MainWindow()
{
    disconnect(&(d->m_model->chartViewModel()), nullptr, this, nullptr);
}

void MainWindow::createNewDocument()
{
    d->m_model->initialModel();
    d->ui->documentTreeView->expandAll();
    d->m_mscFileName.clear();
    clearUndoStacks();

    d->ui->graphicsView->centerOn(
            d->ui->graphicsView->mapFromScene(d->ui->graphicsView->scene()->sceneRect().topLeft()));

    d->ui->graphicsView->setZoom(100);

    updateTitles();
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
        d->m_mscFileName = file;
        d->ui->documentTreeView->expandAll();
        d->ui->graphicsView->centerOn(
                d->ui->graphicsView->mapFromScene(d->ui->graphicsView->scene()->sceneRect().topLeft()));

        d->ui->graphicsView->setZoom(100);
    } else {
        showErrorView();
    }

    clearUndoStacks();

    d->ui->errorTextEdit->appendHtml(d->m_model->errorMessages().join("\n"));
    d->ui->errorTextEdit->appendHtml(tr("Model loading: <b><font color=%2>%1</font></b><br>")
                                             .arg(ok ? tr("success") : tr("failed"), ok ? "black" : "red"));

    updateTitles();

    return ok;
}

void MainWindow::updateTitles()
{
    static const QString title = tr("%1 [%2]");

    const QString mscFileName(d->m_mscFileName.isEmpty() ? tr("Untitled") : QFileInfo(d->m_mscFileName).fileName());
    setWindowTitle(title.arg(qApp->applicationName(), mscFileName));

    d->m_actSaveFile->setText(tr("&Save \"%1\"").arg(mscFileName));
}

void MainWindow::clearUndoStacks()
{
    QUndoStack *undoStack = d->ui->graphicsView->undoStack();
    undoStack->clear();
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

void MainWindow::enableDefaultTool()
{
    Q_ASSERT(d->m_defaultToolAction);
    d->m_defaultToolAction->setChecked(true);
}

void MainWindow::updateTextView()
{
    if (!d->ui->dockWidgetMscText->isVisible()) {
        return;
    }
    d->ui->mscTextBrowser->updateView();
}

bool MainWindow::openFileAsn(const QString &file)
{
    QMessageBox::information(this, "Not implemented yet", QString("Opening the ASN file:\n%1").arg(file));
    return false;
}

void MainWindow::saveMsc()
{
    if (d->m_mscFileName.isEmpty()) {
        saveAsMsc();
    } else {
        d->m_model->saveMsc(d->m_mscFileName);
        updateTitles();
    }
}

void MainWindow::saveAsMsc()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."), QFileInfo(d->m_mscFileName).path(),
                                                    tr("MSC files (*.msc);;All files (*.*)"));
    if (!fileName.isEmpty()) {
        d->m_mscFileName = fileName;
        saveMsc();
    }
}

void MainWindow::showDocumentView(bool show)
{
    if (show) {
        d->ui->centerView->setCurrentWidget(d->ui->graphicsView);

        d->m_hierarchyToolBar->hide();
        d->m_mscToolBar->show();
    }
}

void MainWindow::showHierarchyView(bool show)
{
    if (show) {
        d->ui->centerView->setCurrentWidget(d->ui->hierarchyView);

        d->m_hierarchyToolBar->show();
        d->m_mscToolBar->hide();
    }
}

void MainWindow::showErrorView()
{
    if (!d->ui->dockWidgetErrors->isVisible()) {
        d->m_actToggleErrorView->trigger();
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

        // TODO: add support for dedicated stacks for each tab
    }

    if (d->m_deleteTool) {
        d->m_deleteTool->setCurrentChart(chart);
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
        showDocumentView(true);
    } else {
        showHierarchyView(true);
    }
}

void MainWindow::setupUi()
{
    d->ui->setupUi(this);
    d->ui->graphicsView->setScene(d->m_model->graphicsScene());
    d->ui->documentTreeView->setModel(d->m_model->documentItemModel());

    d->ui->hierarchyView->setBackgroundBrush(QImage(":/resources/resources/texture.png"));
    d->ui->hierarchyView->setScene(d->m_model->hierarchyScene());

    d->ui->mscTextBrowser->setModel(d->m_model->mscModel());

    initActions();
    initMenus();
    initTools();
    initMainToolbar();

    // status bar
    d->m_zoomBox = new QComboBox(d->ui->statusBar);
    for (auto x = d->ui->graphicsView->minZoomPercent(); x <= d->ui->graphicsView->maxZoomPercent();
         x += d->ui->graphicsView->zoomStepPercent())
        d->m_zoomBox->addItem(QString("%1 %").arg(x), x);

    d->m_zoomBox->setCurrentIndex(d->m_zoomBox->findData(100));

    connect(d->m_zoomBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&](int index) {
        d->ui->graphicsView->setZoom(qobject_cast<QComboBox *>(sender())->itemData(index).toDouble());
    });

    connect(d->ui->graphicsView, QOverload<double>::of(&msc::GraphicsView::zoomChanged), this,
            [&](qreal percent) { d->m_zoomBox->setCurrentIndex(d->m_zoomBox->findData(percent)); });

    statusBar()->addPermanentWidget(d->m_zoomBox);
    statusBar()->show();
}

void MainWindow::initActions()
{
    d->m_actUndo = d->m_undoGroup->createUndoAction(this, tr("Undo:"));
    d->m_actUndo->setShortcut(QKeySequence::Undo);
    d->m_actUndo->setIcon(QIcon::fromTheme("edit-undo", QIcon(":/icons/toolbar/undo.svg")));

    d->m_actRedo = d->m_undoGroup->createRedoAction(this, tr("Redo:"));
    d->m_actRedo->setShortcut(QKeySequence::Redo);
    d->m_actRedo->setIcon(QIcon::fromTheme("edit-redo", QIcon(":/icons/toolbar/redo.svg")));

    d->m_deleteTool = new msc::EntityDeleteTool(d->ui->graphicsView, this);
    d->m_deleteTool->setCurrentChart(d->m_model->chartViewModel().currentChart());

    d->m_actAsnEditor = new QAction(tr("ASN.1 Editor"), d->ui->mainToolBar);
    connect(d->m_actAsnEditor, &QAction::triggered, this, &MainWindow::openAsn1Editor);
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

    d->m_actNewFile = d->m_menuFile->addAction(style()->standardIcon(QStyle::SP_FileIcon), tr("New File"), this,
                                               &MainWindow::createNewDocument, QKeySequence::New);

    d->m_actOpenFile = d->m_menuFile->addAction(style()->standardIcon(QStyle::SP_DirOpenIcon), tr("&Open File"), this,
                                                &MainWindow::selectAndOpenFile, QKeySequence::Open);

    d->m_actSaveFile = d->m_menuFile->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Save"), this,
                                                &MainWindow::saveMsc, QKeySequence::Save);

    d->m_actSaveFileAs = d->m_menuFile->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("Save As..."),
                                                  this, &MainWindow::saveAsMsc, QKeySequence::SaveAs);

    d->m_menuFile->addSeparator();

    d->m_actQuit = d->m_menuFile->addAction(tr("&Quit"), qApp, &QApplication::quit, QKeySequence::Quit);
}

void MainWindow::initMenuEdit()
{
    d->m_menuEdit = menuBar()->addMenu(tr("Edit"));
    d->m_menuEdit->addAction(d->m_actUndo);
    d->m_menuEdit->addAction(d->m_actRedo);
    d->m_menuEdit->addSeparator();
    d->m_menuEdit->addAction(d->m_deleteTool->action());
}

void MainWindow::initMenuView()
{
    d->m_menuView = menuBar()->addMenu(tr("&View"));
    d->m_actShowDocument =
            d->m_menuView->addAction(tr("Show &Document"), this, &MainWindow::showDocumentView, tr("F8"));
    d->m_actShowHierarchy =
            d->m_menuView->addAction(tr("Show &Hierarchy"), this, &MainWindow::showHierarchyView, tr("F9"));

    d->m_actShowDocument->setCheckable(true);
    d->m_actShowHierarchy->setCheckable(true);
    auto group = new QActionGroup(d->m_menuView);
    group->addAction(d->m_actShowDocument);
    group->addAction(d->m_actShowHierarchy);
    d->m_actShowDocument->setChecked(true);

    initMenuViewWindows();
}

void MainWindow::initMenuViewWindows()
{
    d->m_menuView->addSeparator();
    d->m_menuViewWindows = d->m_menuView->addMenu("Windows");

    d->m_actToggleErrorView = d->ui->dockWidgetErrors->toggleViewAction();
    d->m_menuViewWindows->addAction(d->m_actToggleErrorView);

    d->m_actToggleHierarchyView = d->ui->dockWidgetDocument->toggleViewAction();
    d->m_menuViewWindows->addAction(d->m_actToggleHierarchyView);

    d->m_actToggleMscTextView = d->ui->dockWidgetMscText->toggleViewAction();
    d->m_menuViewWindows->addAction(d->m_actToggleMscTextView);
}

void MainWindow::initMenuHelp()
{
    d->m_menuHelp = menuBar()->addMenu(tr("Help"));
    d->m_actAboutQt = d->m_menuHelp->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
}

void MainWindow::initTools()
{
    auto pointerTool = new msc::PointerTool(nullptr, this);
    d->m_tools.append(pointerTool);

    auto instanceCreateTool = new msc::InstanceCreatorTool(&(d->m_model->chartViewModel()), nullptr, this);
    connect(instanceCreateTool, &msc::InstanceCreatorTool::created, this, &MainWindow::enableDefaultTool);
    d->m_tools.append(instanceCreateTool);

    auto messageCreateTool = new msc::MessageCreatorTool(&(d->m_model->chartViewModel()), nullptr, this);
    connect(messageCreateTool, &msc::MessageCreatorTool::created, this, &MainWindow::enableDefaultTool);
    d->m_tools.append(messageCreateTool);

    auto actionCreateTool = new msc::ActionCreatorTool(&(d->m_model->chartViewModel()), nullptr, this);
    connect(actionCreateTool, &msc::ActionCreatorTool::created, this, &MainWindow::enableDefaultTool);
    d->m_tools.append(actionCreateTool);

    QActionGroup *toolsActions = new QActionGroup(this);
    for (msc::BaseTool *tool : d->m_tools) {
        QAction *toolAction = d->m_mscToolBar->addAction(tool->title());
        toolAction->setCheckable(true);
        toolAction->setIcon(tool->icon());
        toolAction->setToolTip(tr("%1: %2").arg(tool->title(), tool->description()));
        tool->setView(currentView());
        connect(this, &MainWindow::currentGraphicsViewChanged, tool, &msc::BaseTool::setView);

        toolsActions->addAction(toolAction);
        connect(toolAction, &QAction::toggled, tool, &msc::BaseTool::setActive);
    }

    d->m_defaultToolAction = toolsActions->actions().first();
    enableDefaultTool();

    for (int toolType = static_cast<int>(msc::ToolType::HierarchyAndCreator);
         toolType <= static_cast<int>(msc::ToolType::HierarchyRepeatCreator); ++toolType) {
        auto tool = new msc::HierarchyCreatorTool(static_cast<msc::ToolType>(toolType), &(d->m_model->chartViewModel()),
                                                  nullptr, this);

        d->m_tools.append(tool);

        auto toolAction = d->m_hierarchyToolBar->addAction(tool->title());
        toolAction->setCheckable(true);
        toolAction->setIcon(tool->icon());
        toolAction->setToolTip(tr("%1: %2").arg(tool->title(), tool->description()));
        tool->setView(currentView());
    }
}

void MainWindow::initMainToolbar()
{
    d->ui->mainToolBar->addAction(d->m_actNewFile);
    d->ui->mainToolBar->addAction(d->m_actOpenFile);
    d->ui->mainToolBar->addAction(d->m_actSaveFile);

    d->ui->mainToolBar->addSeparator();
    d->ui->mainToolBar->addAction(d->m_actUndo);
    d->ui->mainToolBar->addAction(d->m_actRedo);

    d->ui->mainToolBar->addSeparator();
    d->ui->mainToolBar->addAction(d->m_deleteTool->action());

    // TODO: just for test Asn1Editor
    d->ui->mainToolBar->addSeparator();
    d->ui->mainToolBar->addAction(d->m_actAsnEditor);
}

void MainWindow::initConnections()
{
    connect(d->ui->documentTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &MainWindow::showSelection);

    connect(&(d->m_model->chartViewModel()), &msc::ChartViewModel::currentChartChanged, this,
            &MainWindow::selectCurrentChart);

    connect(d->m_model, &MainModel::showChartVew, this, [this]() { showDocumentView(true); });

    connect(d->ui->graphicsView, &msc::GraphicsView::mouseMoved, this,
            [this](const QPoint &screen, const QPointF &scene, const QPointF &item) {
                statusBar()->showMessage(tr("Screen: [%1;%2]\tScene: [%3;%4]\tObject: [%5;%6]")
                                                 .arg(screen.x())
                                                 .arg(screen.y())
                                                 .arg(scene.x())
                                                 .arg(scene.y())
                                                 .arg(item.x())
                                                 .arg(item.y()));
            });

    connect(d->m_model, &MainModel::modelDataChanged, this, [this]() {
        d->ui->mscTextBrowser->setModel(d->m_model->mscModel());
        d->ui->mscTextBrowser->updateView();
    });
    connect(d->m_actToggleMscTextView, &QAction::toggled, this, [this](bool on) {
        if (on) {
            QMetaObject::invokeMethod(d->ui->mscTextBrowser, "updateView", Qt::QueuedConnection);
        }
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
            if (QAction *pointerToolAction = d->m_mscToolBar->actions().first())
                if (!pointerToolAction->isChecked())
                    pointerToolAction->setChecked(true);
        break;
    }
    }
}

void MainWindow::loadSettings()
{
    // as default hide the msc text view
    d->ui->dockWidgetMscText->setVisible(false);

    restoreGeometry(AppOptions::MainWindow.Geometry->read().toByteArray());
    restoreState(AppOptions::MainWindow.State->read().toByteArray());
}

void MainWindow::saveSettings()
{
    AppOptions::MainWindow.Geometry->write(saveGeometry());
    AppOptions::MainWindow.State->write(saveState());
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    saveSettings();
    QMainWindow::closeEvent(e);
}
