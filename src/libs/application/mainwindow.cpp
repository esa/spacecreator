/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "baseitems/common/coordinatesconverter.h"
#include "chartviewmodel.h"
#include "commandlineparser.h"
#include "commands/common/commandsstack.h"
#include "documentitemmodel.h"
#include "graphicsview.h"
#include "mainmodel.h"
#include "messagedeclarationsdialog.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccondition.h"
#include "msccreate.h"
#include "mscdocument.h"
#include "mscmessage.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "mscplugin.h"
#include "msctimer.h"
#include "remotecontrolhandler.h"
#include "remotecontrolwebserver.h"
#include "settings/appoptions.h"
#include "textview.h"
#include "tools/actioncreatortool.h"
#include "tools/commentcreatortool.h"
#include "tools/conditioncreatortool.h"
#include "tools/coregioncreatortool.h"
#include "tools/entitydeletetool.h"
#include "tools/hierarchycreatortool.h"
#include "tools/instancecreatortool.h"
#include "tools/instancestoptool.h"
#include "tools/messagecreatortool.h"
#include "tools/pointertool.h"
#include "tools/timercreatortool.h"
#include "ui_mainwindow.h"

#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QComboBox>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QImageWriter>
#include <QInputDialog>
#include <QItemSelectionModel>
#include <QKeyEvent>
#include <QKeySequence>
#include <QMessageBox>
#include <QMetaEnum>
#include <QMimeData>
#include <QToolBar>
#include <QUndoGroup>
#include <QUndoStack>
#include <QVector>

static const char *HIERARCHY_TYPE_TAG = "hierarchyTag";

namespace msc {

const QLatin1String MainWindow::DotMscFileExtensionLow = QLatin1String(".msc");

struct MainWindow::MainWindowPrivate {
    explicit MainWindowPrivate(msc::MSCPlugin *plugin, MainWindow *mainWindow)
        : ui(new Ui::MainWindow)
        , m_plugin(plugin)
        , m_model(new MainModel(mainWindow))
    {
    }

    ~MainWindowPrivate() { delete ui; }

    Ui::MainWindow *ui = nullptr;

    msc::MSCPlugin *m_plugin;

    QComboBox *m_zoomBox = nullptr;

    MainModel *m_model = nullptr;

    QAction *m_actScreenshot = nullptr;

    QAction *m_actToggleErrorView = nullptr;
    QAction *m_actToggleHierarchyView = nullptr;
    QAction *m_actToggleMscTextView = nullptr;
    QAction *m_actToggleAsn1View = nullptr;

    QMenu *m_menuHelp = nullptr;
    QAction *m_actAboutQt = nullptr;

    QVector<msc::BaseTool *> m_tools;
    QAction *m_defaultToolAction = nullptr;
    msc::InstanceStopTool *m_instanceStopTool = nullptr;
    msc::InstanceCreatorTool *m_instanceCreatorTool = nullptr;
    msc::EntityDeleteTool *m_deleteTool = nullptr;
    msc::MessageCreatorTool *m_messageCreateTool = nullptr;

    RemoteControlWebServer *m_remoteControlWebServer = nullptr;
    RemoteControlHandler *m_remoteControlHandler = nullptr;

    bool m_dropUnsavedChangesSilently = false;
};

/*!
  \class MainWindow

  This is the main window class for the MSC editor.
*/

/*!
 * \brief MainWindow::MainWindow Create an empty view.
 * \param parent
 */
MainWindow::MainWindow(msc::MSCPlugin *plugin, QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate(plugin, this))
{
    setupUi();
    d->ui->hierarchyView->setRenderHints(
            QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    initConnections();

    selectCurrentChart();

    loadSettings();

    d->m_plugin->mscToolBar()->setVisible(d->ui->centerView->currentWidget() == d->ui->graphicsView);
    d->m_plugin->hierarchyToolBar()->setVisible(d->ui->centerView->currentWidget() == d->ui->hierarchyView);
}

MainWindow::~MainWindow()
{
    if (d->ui->documentTreeView->model()) {
        disconnect(d->ui->documentTreeView->model(), nullptr, this, nullptr);
    }
    disconnect(&(d->m_model->chartViewModel()), nullptr, this, nullptr);

    // Had this connection not dropped, the currentUndoStack() would need check
    // for nullptr d, d->ui, d->ui->graphicsView
    disconnect(d->m_model->undoStack(), &QUndoStack::indexChanged, this, &MainWindow::updateTitles);
}

/*!
 * \brief MainWindow::currentView Get the graphics view
 * \return
 */
QGraphicsView *MainWindow::currentView() const
{
    return d->ui->graphicsView;
}

/*!
 * \brief MainWindow::createNewDocument Create a new document. This doesn't ask the user to save.
 */
void MainWindow::createNewDocument()
{
    if (!saveDocument()) {
        return;
    }

    d->m_model->chartViewModel().setPreferredChartBoxSize(prepareChartBoxSize());
    d->m_model->initialModel();

    d->ui->graphicsView->setZoom(100);
}

/*!
 * \brief MainWindow::selectAndOpenFile Show a file dialog and open the file.
 */
void MainWindow::selectAndOpenFile()
{
    if (!saveDocument()) {
        return;
    }

    static const QStringList suffixes = { QString("MSC files (%1)").arg(mscFileFilters().join(" ")),
        QString("All files (*.*)") };

    const QString path = QFileInfo(d->m_model->currentFilePath()).absoluteFilePath();

    const QString filename = QFileDialog::getOpenFileName(this, tr("MSC"), path, suffixes.join(";;"));
    if (!filename.isEmpty()) {
        openFileMsc(filename);
    }
}

/*!
 * \brief MainWindow::openFileMsc Read an MSC file and populate the view with the contents
 * \param file Filename
 * \return True if success
 */
bool MainWindow::openFileMsc(const QString &file)
{
    QFileInfo fileInfo(file);
    if (!fileInfo.exists()) {
        QMessageBox::critical(this, tr("Open file problem"), tr("The file does not exist."));
        return false;
    }

    d->m_model->chartViewModel().setPreferredChartBoxSize(QSizeF());

    const bool ok = d->m_model->loadFile(file);
    if (!ok) {
        QMessageBox::critical(this, tr("File Error"),
                tr("Could not read the file. Errors:\n%1").arg(d->m_model->mscErrorMessages().join("\n")));
        return false;
    }

    if (!d->m_model->mscErrorMessages().isEmpty()) {
        QMessageBox::warning(this, tr("Open File Warnings"),
                tr("Warnings found while opening the file:\n%1").arg(d->m_model->mscErrorMessages().join("\n")));
    }

    d->ui->graphicsView->setZoom(100);

    return true;
}

/*!
 * \brief MainWindow::updateTitles Populate the titles in save action and window
 */
void MainWindow::updateTitles()
{
    static const QString title = tr("%1 [%2]%3");

    const QString dirtyMarker(d->m_model->needSave() ? "*" : "");
    const QString &filename = d->m_model->currentFilePath();
    const QString mscFileName(filename.isEmpty() ? tr("Untitled") : QFileInfo(filename).fileName());
    setWindowTitle(title.arg(qApp->applicationName(), mscFileName, dirtyMarker));
}

/*!
 * \brief MainWindow::openMscChain Open MSC files in a directory
 * \param dirPath
 * \return
 */
bool MainWindow::openMscChain(const QString &dirPath)
{
    if (dirPath.isEmpty())
        return false;

    QDir dir(dirPath);
    if (!dir.exists() || !dir.isReadable())
        return false;

    for (const QFileInfo &file : dir.entryInfoList(mscFileFilters())) {
        openFileMsc(file.absoluteFilePath());
    }
    return true;
}

/*!
 * \brief MainWindow::activateDefaultTool Set default tool to be active
 */
void MainWindow::activateDefaultTool()
{
    Q_ASSERT(d->m_defaultToolAction);
    d->m_defaultToolAction->setChecked(true);
}

/*!
 * \brief MainWindow::saveMsc Save the MSC file. Ask for the filename if necessary.
 */
void MainWindow::saveMsc()
{
    const QString &filename = d->m_model->currentFilePath();
    if (filename.isEmpty()) {
        saveAsMsc();
    } else {
        d->m_model->saveMsc(filename);
    }
}

/*!
 * \brief MainWindow::saveAsMsc Save the file after asking for the filename.
 */
void MainWindow::saveAsMsc()
{
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save as..."), QFileInfo(d->m_model->currentFilePath()).path(),
                    tr("MSC files (%1);;All files (*.*)").arg(mscFileFilters().join(" ")));
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(DotMscFileExtensionLow))
            fileName.append(DotMscFileExtensionLow);
        d->m_model->setCurrentFilePath(fileName);

        saveMsc();
    }
}

/*!
 * \brief MainWindow::showDocumentView Set whether to show the document view or not.
 * \param show
 */
void MainWindow::showDocumentView(bool show)
{
    if (show) {
        d->ui->centerView->setCurrentWidget(d->ui->graphicsView);

        d->m_plugin->hierarchyToolBar()->hide();
        d->m_plugin->mscToolBar()->show();

        d->m_plugin->actionCopy()->setEnabled(true);
        msc::MscChart *chart = d->m_model->chartViewModel().currentChart();
        QClipboard *clipboard = QApplication::clipboard();
        const QMimeData *mimeData = clipboard->mimeData();
        const bool clipBoardHasMscChart = mimeData ? mimeData->hasFormat(MainModel::MscChartMimeType) : false;
        d->m_plugin->actionPaste()->setEnabled(clipBoardHasMscChart && chart && chart->instances().isEmpty());

        d->m_deleteTool->setView(d->ui->graphicsView);
        d->m_deleteTool->setCurrentChart(chart);

        updateZoomBox(d->ui->graphicsView->zoom());
    }
}

/*!
 * \brief MainWindow::showHierarchyView Set whether to show the hierarchy view or not
 * \param show
 */
void MainWindow::showHierarchyView(bool show)
{
    if (show) {
        d->ui->centerView->setCurrentWidget(d->ui->hierarchyView);

        d->m_plugin->hierarchyToolBar()->show();
        d->m_plugin->mscToolBar()->hide();

        d->m_plugin->actionCopy()->setEnabled(false);
        d->m_plugin->actionPaste()->setEnabled(false);

        d->m_deleteTool->setView(d->ui->hierarchyView);
        d->m_deleteTool->setCurrentChart(nullptr);

        updateZoomBox(d->ui->hierarchyView->zoom());
    }
}

/*!
 * \brief MainWindow::selectCurrentChart Set the current chart as the currently selected.
 */
void MainWindow::selectCurrentChart()
{
    msc::MscChart *chart = d->m_model->chartViewModel().currentChart();

    if (chart != nullptr) {
        if (QUndoStack *currentStack = d->m_model->undoStack()) {
            if (!d->m_plugin->undoGroup()->stacks().contains(currentStack)) {
                d->m_plugin->undoGroup()->addStack(currentStack);
            }
            d->m_plugin->undoGroup()->setActiveStack(currentStack);
        } else {
            d->m_plugin->undoGroup()->removeStack(d->m_plugin->undoGroup()->activeStack());
        }
        msc::cmd::CommandsStack::setCurrent(d->m_plugin->undoGroup()->activeStack());

        // TODO: add support for dedicated stacks for each tab
    }

    if (d->m_deleteTool) {
        d->m_deleteTool->setCurrentChart(chart);
    }
}

/*!
 * \brief MainWindow::showChart Show the chart \a index
 */
void MainWindow::showChart(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    auto *obj = static_cast<QObject *>(index.internalPointer());
    if (obj == nullptr) {
        return;
    }

    if (auto document = dynamic_cast<msc::MscDocument *>(obj)) {
        if (!document->charts().empty()) {
            d->m_model->chartViewModel().fillView(document->charts()[0]);
            showDocumentView(true);
        }
    }
}

void MainWindow::showSelection(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    if (!current.isValid()) {
        return;
    }

    auto *obj = static_cast<QObject *>(current.internalPointer());
    if (obj == nullptr) {
        d->m_model->setSelectedDocument(nullptr);
        return;
    }

    if (auto chart = dynamic_cast<msc::MscChart *>(obj)) {
        d->m_model->chartViewModel().fillView(chart);
        showDocumentView(true);
    } else {
        showHierarchyView(true);

        if (auto document = dynamic_cast<msc::MscDocument *>(obj)) {
            bool canNewChild = true;
            if (document->hierarchyType() == msc::MscDocument::HierarchyType::HierarchyLeaf)
                canNewChild = false;
            if (document->documents().size() > 0
                    && (document->hierarchyType() == msc::MscDocument::HierarchyType::HierarchyRepeat
                            || document->hierarchyType() == msc::MscDocument::HierarchyType::HierarchyIs
                            || document->hierarchyType() == msc::MscDocument::HierarchyType::HierarchyException)) {
                canNewChild = false;
            }
            for (auto action : d->m_plugin->hierarchyToolBar()->actions()) {
                action->setEnabled(canNewChild);
            }

            d->m_model->setSelectedDocument(document);
            d->m_plugin->actionPaste()->setEnabled(
                    QApplication::clipboard()->mimeData()->hasFormat(MainModel::MscChartMimeType)
                    && d->m_model->selectedDocument()->isAddChildEnable());
        }
    }
}

/*!
 * \brief MainWindow::setupUi Create the UI from the form
 */
void MainWindow::setupUi()
{
    d->ui->setupUi(this);

    d->m_plugin->addToolBars(this);

    d->ui->graphicsView->setScene(d->m_model->graphicsScene());
    //    d->ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft); // make scene's origin be great^W in topLeft
    d->ui->documentTreeView->setModel(d->m_model->documentItemModel());

    d->ui->hierarchyView->setBackgroundBrush(QImage(":/resources/resources/texture.png"));
    d->ui->hierarchyView->setScene(d->m_model->hierarchyScene());

    d->ui->mscTextBrowser->setModel(d->m_model->mscModel());
    d->ui->asn1Widget->setModel(d->m_model->mscModel());

    initActions();

    d->m_plugin->initMenus(this);

    initTools();
    initMainToolbar();
    initDocumentViewActions();

    // status bar
    d->m_zoomBox = new QComboBox(d->ui->statusBar);
    for (auto x = d->ui->graphicsView->minZoomPercent(); x <= d->ui->graphicsView->maxZoomPercent();
            x += d->ui->graphicsView->zoomStepPercent())
        d->m_zoomBox->addItem(QString("%1 %").arg(x), x);

    d->m_zoomBox->setCurrentIndex(d->m_zoomBox->findData(100));

    connect(d->m_zoomBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&](int index) {
        if (auto graphicsView = dynamic_cast<msc::GraphicsView *>(d->ui->centerView->currentWidget()))
            graphicsView->setZoom(qobject_cast<QComboBox *>(sender())->itemData(index).toDouble());
    });

    connect(d->ui->graphicsView, QOverload<double>::of(&msc::GraphicsView::zoomChanged), this,
            &MainWindow::updateZoomBox);
    connect(d->ui->hierarchyView, QOverload<double>::of(&msc::GraphicsView::zoomChanged), this,
            &MainWindow::updateZoomBox);

    statusBar()->addPermanentWidget(d->m_zoomBox);
    statusBar()->show();
}

/*!
 * \brief MainWindow::initActions Initialize the actions
 */
void MainWindow::initActions()
{
    d->m_deleteTool = new msc::EntityDeleteTool(&(d->m_model->chartViewModel()), d->ui->graphicsView, this);
    d->m_deleteTool->setCurrentChart(d->m_model->chartViewModel().currentChart());

    // Connect the plugin actions
    connect(d->m_plugin->actionNewFile(), &QAction::triggered, this, &MainWindow::createNewDocument);
    connect(d->m_plugin->actionOpenFile(), &QAction::triggered, this, &MainWindow::selectAndOpenFile);
    connect(d->m_plugin->actionSaveFile(), &QAction::triggered, this, &MainWindow::saveMsc);
    connect(d->m_plugin->actionSaveFileAs(), &QAction::triggered, this, &MainWindow::saveAsMsc);
    connect(d->m_plugin->actionQuit(), &QAction::triggered, this, [&]() {
        if (this->saveDocument()) {
            this->saveSettings();
            QApplication::quit();
        }
    });
}

void MainWindow::initDocumentViewActions()
{
    auto addAction = [&](msc::MscDocument::HierarchyType type) {
        // create tool
        auto tool = new msc::HierarchyCreatorTool(type, &(d->m_model->hierarchyViewModel()), nullptr, this);
        auto action = d->m_plugin->hierarchyToolBar()->addAction(tool->title());
        action->setProperty(HIERARCHY_TYPE_TAG, type);
        action->setCheckable(true);
        action->setIcon(tool->icon());
        action->setToolTip(tr("%1: %2").arg(tool->title(), tool->description()));
        tool->setView(d->ui->hierarchyView);

        connect(tool, &msc::HierarchyCreatorTool::documentCreated, this, [&](msc::MscDocument *document) {
            activateDefaultTool();
            d->m_model->setSelectedDocument(document);
        });

        connect(action, &QAction::toggled, tool, &msc::BaseTool::setActive);
        connect(action, &QAction::toggled, this, [this]() {
            for (QAction *action : d->m_plugin->hierarchyToolBar()->actions())
                action->setChecked(false);
        });
    };

    addAction(msc::MscDocument::HierarchyAnd);
    addAction(msc::MscDocument::HierarchyOr);
    addAction(msc::MscDocument::HierarchyParallel);
    addAction(msc::MscDocument::HierarchyIs);
    addAction(msc::MscDocument::HierarchyRepeat);
    addAction(msc::MscDocument::HierarchyException);
    addAction(msc::MscDocument::HierarchyLeaf);
}

void MainWindow::onCreateMessageToolRequested()
{
    d->m_messageCreateTool->activate();
}

/*!
 * \brief MainWindow::startRemoteControl Start the remote app controller
 * \param port Listen on this port
 * \return True if success
 */
bool MainWindow::startRemoteControl(quint16 port)
{
    if (!d->m_remoteControlWebServer) {
        d->m_remoteControlWebServer = new RemoteControlWebServer(this);
        d->m_remoteControlHandler = new RemoteControlHandler(this);
        d->m_remoteControlHandler->setModel(d->m_model);

        connect(d->m_remoteControlWebServer, &RemoteControlWebServer::executeCommand, d->m_remoteControlHandler,
                &RemoteControlHandler::handleRemoteCommand);
        connect(d->m_remoteControlHandler, &RemoteControlHandler::commandDone, d->m_remoteControlWebServer,
                &RemoteControlWebServer::commandDone);
    }
    if (d->m_remoteControlWebServer->start(port))
        return true;

    d->m_remoteControlWebServer->deleteLater();
    d->m_remoteControlWebServer = nullptr;
    d->m_remoteControlHandler->deleteLater();
    d->m_remoteControlHandler = nullptr;
    qWarning() << "Continue app running without remote control enabled";
    return false;
}

void MainWindow::initTools()
{
    auto pointerTool = new msc::PointerTool(nullptr, this);
    d->m_tools.append(pointerTool);

    d->m_instanceCreatorTool = new msc::InstanceCreatorTool(&(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(d->m_instanceCreatorTool);

    d->m_instanceStopTool = new msc::InstanceStopTool(nullptr, this);
    d->m_tools.append(d->m_instanceStopTool);

    d->m_messageCreateTool = new msc::MessageCreatorTool(
            msc::MscMessage::MessageType::Message, &(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(d->m_messageCreateTool);

    auto messageCreateTool = new msc::MessageCreatorTool(
            msc::MscMessage::MessageType::Create, &(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(messageCreateTool);

    auto commentCreateTool = new msc::CommentCreatorTool(false, &(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(commentCreateTool);

    auto globalCommentCreateTool = new msc::CommentCreatorTool(true, &(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(globalCommentCreateTool);

    auto coregionCreateTool = new msc::CoregionCreatorTool(&(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(coregionCreateTool);

    auto actionCreateTool = new msc::ActionCreatorTool(&(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(actionCreateTool);

    auto conditionCreateTool = new msc::ConditionCreatorTool(false, &(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(conditionCreateTool);

    auto sharedConditionCreateTool =
            new msc::ConditionCreatorTool(true, &(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(sharedConditionCreateTool);

    auto startTimerCreateTool =
            new msc::TimerCreatorTool(msc::MscTimer::TimerType::Start, &(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(startTimerCreateTool);

    auto stopTimerCreateTool =
            new msc::TimerCreatorTool(msc::MscTimer::TimerType::Stop, &(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(stopTimerCreateTool);

    auto timeoutCreateTool = new msc::TimerCreatorTool(
            msc::MscTimer::TimerType::Timeout, &(d->m_model->chartViewModel()), nullptr, this);
    d->m_tools.append(timeoutCreateTool);

    QActionGroup *toolsActions = new QActionGroup(this);
    toolsActions->setExclusive(false);
    for (msc::BaseTool *tool : d->m_tools) {
        QAction *toolAction = d->m_plugin->mscToolBar()->addAction(tool->title());
        toolAction->setCheckable(true);
        toolAction->setIcon(tool->icon());
        toolAction->setToolTip(tr("%1: %2").arg(tool->title(), tool->description()));
        toolAction->setData(QVariant::fromValue<msc::BaseTool::ToolType>(tool->toolType()));
        tool->setView(currentView());
        tool->setAction(toolAction);

        connect(this, &MainWindow::currentGraphicsViewChanged, tool, &msc::BaseTool::setView);
        if (msc::BaseCreatorTool *creatorTool = qobject_cast<msc::BaseCreatorTool *>(tool))
            connect(creatorTool, &msc::BaseCreatorTool::created, this, &MainWindow::activateDefaultTool);

        toolsActions->addAction(toolAction);
        connect(toolAction, &QAction::toggled, this, &MainWindow::updateMscToolbarActionsChecked);
    }

    d->m_defaultToolAction = toolsActions->actions().first();
    d->m_defaultToolAction->setVisible(false);
    activateDefaultTool();
}

void MainWindow::initMainToolbar()
{
    auto mainToolBar = d->m_plugin->mainToolBar();
    mainToolBar->addSeparator();
    mainToolBar->addAction(d->m_deleteTool->action());

    mainToolBar->addSeparator();
    mainToolBar->addAction(d->m_plugin->actionCopy());
    mainToolBar->addAction(d->m_plugin->actionPaste());
}

void MainWindow::initConnections()
{
    connect(d->ui->documentTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &MainWindow::showSelection);
    connect(d->ui->documentTreeView, &QTreeView::doubleClicked, this, &MainWindow::showChart);

    connect(&(d->m_model->chartViewModel()), &msc::ChartViewModel::currentChartChanged, this,
            &MainWindow::selectCurrentChart);

    connect(d->m_model, &MainModel::showChartVew, this, [this]() { showDocumentView(true); });

    connect(d->m_model, &MainModel::selectedDocumentChanged, d->ui->documentTreeView,
            &DocumentTreeView::setSelectedDocument);

    connect(d->ui->graphicsView, &msc::GraphicsView::mouseMoved, this, &MainWindow::showCoordinatesInfo);

    connect(d->ui->graphicsView, &msc::GraphicsView::createMessageToolRequested, this,
            &MainWindow::onCreateMessageToolRequested);

    connect(d->m_model, &MainModel::modelDataChanged, this, &MainWindow::updateModel);
    connect(d->m_model, &MainModel::modelUpdated, this, &MainWindow::updateModel);
    connect(d->m_model, &MainModel::modelUpdated, d->ui->asn1Widget, &ASN1FileView::setModel);

    connect(d->m_actToggleMscTextView, &QAction::toggled, this, [this](bool on) {
        if (on) {
            QMetaObject::invokeMethod(d->ui->mscTextBrowser, "updateView", Qt::QueuedConnection);
        }
    });

    connect(d->m_model->documentItemModel(), &msc::DocumentItemModel::dataChanged, this, &MainWindow::showSelection);

    connect(d->m_model->undoStack(), &QUndoStack::indexChanged, this, &MainWindow::updateTitles);
    connect(d->m_model, &MainModel::lasteSaveUndoChange, this, &MainWindow::updateTitles);

    connect(d->m_model, &MainModel::currentFilePathChanged, this, [&](const QString &filename) {
        QFileInfo fileInfo(filename);
        d->ui->asn1Widget->setCurrentDirectory(fileInfo.absolutePath());
    });
}

/*!
 * \brief MainWindow::processCommandLineArg Handle comm
 * \param arg
 * \param value
 * \return
 */
bool MainWindow::processCommandLineArg(shared::CommandLineParser::Positional arg, const QString &value)
{
    switch (arg) {
    case shared::CommandLineParser::Positional::OpenFileMsc:
        return openFileMsc(value);
    case shared::CommandLineParser::Positional::DbgOpenMscExamplesChain:
        return openMscChain(value);
    case shared::CommandLineParser::Positional::StartRemoteControl:
        if (startRemoteControl(value.toUShort())) {
            menuBar()->setVisible(false);

            d->m_plugin->mscToolBar()->setVisible(false);
            d->m_plugin->hierarchyToolBar()->setVisible(false);
            d->m_plugin->mainToolBar()->setVisible(false);

            d->ui->dockWidgetMscText->hide();
            d->ui->dockWidgetDocument->hide();
            d->ui->dockWidgetDocumenetContents->hide();
            d->ui->dockWidgetAsn1->hide();

            statusBar()->hide();
            return true;
        }
        break;
    case shared::CommandLineParser::Positional::DropUnsavedChangesSilently:
        d->m_dropUnsavedChangesSilently = true;
        return true;
    default:
        qWarning() << Q_FUNC_INFO << "Unhandled option:" << arg << value;
        break;
    }
    return false;
}

BaseTool *MainWindow::deleteTool()
{
    return d->m_deleteTool;
}

QAction *MainWindow::dockWidgetDocumentToggleAction()
{
    return d->ui->dockWidgetDocument->toggleViewAction();
}

QAction *MainWindow::dockWidgetMscTextToggleAction()
{
    return d->ui->dockWidgetMscText->toggleViewAction();
}

QAction *MainWindow::dockWidgetAsn1ToggleAction()
{
    return d->ui->dockWidgetAsn1->toggleViewAction();
}

void MainWindow::copyCurrentChart()
{
    d->m_model->copyCurrentChart();
}

void MainWindow::copyCurrentChartAsPicture()
{
    d->m_model->copyCurrentChartAsPicture();
}

void MainWindow::pasteChart()
{
    d->m_model->pasteChart();
}

msc::BaseTool *MainWindow::activeTool() const
{
    for (auto tool : d->m_tools)
        if (tool->isActive())
            return tool;

    return nullptr;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    QMainWindow::keyPressEvent(e);

    switch (e->key()) {
    case Qt::Key_Escape: {
        if (!e->isAutoRepeat()) {
            if (QAction *pointerToolAction = d->m_plugin->mscToolBar()->actions().first())
                if (!pointerToolAction->isChecked())
                    pointerToolAction->setChecked(true);
        }
        break;
    }
#ifdef QT_DEBUG
    case Qt::Key_R: {
        if (!e->isAutoRepeat() && (e->modifiers() == Qt::ControlModifier)) {
            openFileMsc(d->m_model->currentFilePath());
        }
        break;
    }
    case Qt::Key_M: {
        if (!e->isAutoRepeat() && e->modifiers().testFlag(Qt::AltModifier)
                && e->modifiers().testFlag(Qt::ControlModifier) && e->modifiers().testFlag(Qt::ShiftModifier)) {
            showMousePositioner();
        }
        break;
    }
    default: {
        if (msc::BaseTool *tool = activeTool())
            tool->processKeyPress(e);
        break;
    }
#endif
    }
}

void MainWindow::loadSettings()
{
    // as default hide the msc text view
    d->ui->dockWidgetMscText->setVisible(false);

    restoreGeometry(AppOptions::MainWindow.Geometry->read().toByteArray());
    restoreState(AppOptions::MainWindow.State->read().toByteArray());

    // the toolbar might be hidden from a streaming tool session
    d->m_plugin->mainToolBar()->show();

    const bool isDocViewMode = 0 == AppOptions::MainWindow.DocOrHierarchyViewMode->read().toInt();
    if (isDocViewMode)
        showDocumentView(true);
    else
        showHierarchyView(true);
    QAction *changeViewModeAction =
            isDocViewMode ? d->m_plugin->actionShowDocument() : d->m_plugin->actionShowHierarchy();
    changeViewModeAction->setChecked(true);
}

void MainWindow::saveSettings()
{
    AppOptions::MainWindow.Geometry->write(saveGeometry());
    AppOptions::MainWindow.State->write(saveState());
    AppOptions::MainWindow.LastFilePath->write(d->m_model->currentFilePath());
    AppOptions::MainWindow.DocOrHierarchyViewMode->write(d->ui->centerView->currentIndex());
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (saveDocument()) {
        saveSettings();
        QMainWindow::closeEvent(e);
    } else {
        e->ignore();
    }
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);

    if (e->type() == QEvent::ActivationChange) {

        // In order to automatically create on startup a default doc with a ChartItem
        // box sized accordingly to this->centralWidget(), we have to wait while
        // processing of restoreState(...) & restoreGeometry(...) will be finished.
        // The final geometry (which is stored in a QByteArray and not in a QRect)
        // is applyed after QEvent::Show, so QEvent::Resize handler seems to be
        // a proper entry point, but actually it's not:
        // in case the saved window state is "normal", the resizeEvent(...) will
        // be called once, but for "maximized" there would be one more call, and
        // there is no easy way to detect without additional routines is it necessary
        // to wait for the second call or not.
        // That's why it's performed here and in such way.
        // (But now I suspect writing those additional routines would take about
        // same time as writing this comment did)

        static bool isFirstCall(true);
        if (isFirstCall) {
            isFirstCall = false;
            onGeometryRestored();
        }
    }
}

void MainWindow::onGeometryRestored()
{
    // A new document should be created only if no files were opened by command line args
    if (d->m_model->currentFilePath().isEmpty()) {
        QMetaObject::invokeMethod(this, "createNewDocument", Qt::QueuedConnection);
    }
}

QSizeF MainWindow::prepareChartBoxSize() const
{
    static constexpr qreal padding = 100.;
    if (centralWidget()) {
        return centralWidget()->geometry().size() - QSizeF(padding, padding);
    }
    return QSizeF();
}

void MainWindow::updateMscToolbarActionsEnablement()
{
    auto chart = d->m_model->chartViewModel().currentChart();
    const bool hasInstance = chart && !chart->instances().isEmpty();

    bool forceDefault(false);
    for (QAction *act : d->m_plugin->mscToolBar()->actions()) {
        const msc::BaseTool::ToolType toolType(act->data().value<msc::BaseTool::ToolType>());
        switch (toolType) {
        case msc::BaseTool::ToolType::ActionCreator:
        case msc::BaseTool::ToolType::ConditionCreator:
        case msc::BaseTool::ToolType::MessageCreator:
        case msc::BaseTool::ToolType::CommentCreator:
        case msc::BaseTool::ToolType::CoregionCreator:
        case msc::BaseTool::ToolType::EntityDeleter:
        case msc::BaseTool::ToolType::InstanceStopper:
        case msc::BaseTool::ToolType::TimerCreator: {
            const bool changed = act->isEnabled() && !hasInstance;
            forceDefault = forceDefault || changed;
            act->setEnabled(hasInstance);
            break;
        }
        case msc::BaseTool::ToolType::Pointer:
        case msc::BaseTool::ToolType::InstanceCreator:
        default: {
            act->setEnabled(true);
            break;
        }
        }
    }

    if (forceDefault)
        activateDefaultTool();
}

void MainWindow::updateMscToolbarActionsChecked()
{
    if (QAction *senderAction = qobject_cast<QAction *>(sender()))
        if (senderAction->isChecked()) {
            for (QAction *action : d->m_plugin->mscToolBar()->actions())
                if (action != senderAction)
                    action->setChecked(false);
        }
}

QStringList MainWindow::mscFileFilters()
{
    static QStringList filters;
    if (filters.isEmpty()) {
        const QString asterisk("*%1");
        filters << asterisk.arg(DotMscFileExtensionLow) << asterisk.arg(DotMscFileExtensionLow).toUpper();
    }
    return filters;
}

bool MainWindow::saveDocument()
{
    if (!d->m_dropUnsavedChangesSilently && d->m_model->needSave()) {
        auto result = QMessageBox::warning(this, windowTitle(),
                tr("You have unsaved data. Do you want to save the MSC document?"),
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);

        if (result == QMessageBox::Cancel) {
            return false;
        } else if (result == QMessageBox::Yes) {
            saveMsc();
        }
    }

    return true;
}

void MainWindow::updateModel()
{
    d->ui->mscTextBrowser->setModel(d->m_model->mscModel());
    updateMscToolbarActionsEnablement();
}

#ifdef QT_DEBUG

// A way to precisiousely move mouse pointer to scene coordinates without pixel hunting.
// Invoked by CTRL+ALT+SHIFT+M
void MainWindow::showMousePositioner()
{
    const bool isItemMove = currentView()->scene()->selectedItems().size() == 1;
    bool gotText(false);
    QString input = QInputDialog::getText(
            this, isItemMove ? "Move selected item to" : "Move mouse to", "x y:", QLineEdit::Normal, "0 0", &gotText)
                            .trimmed();
    if (!gotText || input.size() < 3) // minimal valid input is "0 0"
        return;

    bool isCif = input.at(0).toUpper() == 'C';
    if (isCif)
        input.remove(0, 1);

    static const QRegularExpression rxPoint("(-?\\d+\\.?\\d*),?\\s?(-?\\d+\\.?\\d*)");
    QRegularExpressionMatch m = rxPoint.match(input);
    const QStringList &coords = m.capturedTexts();
    if (coords.size() == 3) {
        bool xOk(false), yOk(false);
        const QPointF &inputPos = { coords.at(1).toDouble(&xOk), coords.at(2).toDouble(&yOk) };
        if (xOk && yOk) {
            QPointF scenePos(inputPos);
            if (isCif)
                scenePos = msc::utils::CoordinatesConverter::cifToScene(scenePos.toPoint());

            const QPoint &localPos = currentView()->mapFromScene(scenePos);
            const QPoint &globalPos = currentView()->mapToGlobal(localPos);

            if (isItemMove) {
                if (QGraphicsItem *item = currentView()->scene()->selectedItems().first())
                    item->setPos(scenePos);
            } else {
                QCursor::setPos(globalPos);

                // Update the status bar info:
                QMouseEvent event(QEvent::MouseMove, localPos, globalPos, Qt::NoButton, Qt::NoButton, Qt::NoModifier);
                QApplication::sendEvent(currentView()->viewport(), &event);
            }
        }
    }
}
#endif

void MainWindow::showCoordinatesInfo(const QString &info)
{
    statusBar()->showMessage(info);
}

void MainWindow::openMessageDeclarationEditor()
{
    msc::MscModel *model = d->m_model->mscModel();
    if (!model)
        return;

    QVector<msc::MscDocument *> docs = model->documents();
    if (docs.isEmpty()) {
        return;
    }

    MessageDeclarationsDialog dialog(docs.at(0)->messageDeclarations(), model->asn1TypesData(), this);
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        msc::cmd::CommandsStack::current()->beginMacro("Edit message declarations");
        const QVariantList cmdParams = { QVariant::fromValue<msc::MscDocument *>(docs.at(0)),
            QVariant::fromValue<msc::MscMessageDeclarationList *>(dialog.declarations()) };
        msc::cmd::CommandsStack::push(msc::cmd::Id::SetMessageDeclarations, cmdParams);
        const QVariantList params { QVariant::fromValue(d->m_model->mscModel()), dialog.fileName(), "ASN.1" };
        msc::cmd::CommandsStack::push(msc::cmd::Id::SetAsn1File, params);
        d->m_model->mscModel()->setAsn1TypesData(dialog.asn1Types());
        msc::cmd::CommandsStack::current()->endMacro();
    }
}

void MainWindow::updateZoomBox(double percent)
{
    const QSignalBlocker blocker(d->m_zoomBox);

    d->m_zoomBox->setCurrentIndex(d->m_zoomBox->findData(percent));
}

QStringList supportedImgFileExtensions()
{
    static QStringList extensions;
    if (extensions.isEmpty()) {
        const QList<QByteArray> &formats = QImageWriter::supportedImageFormats();
        for (const QByteArray &format : formats)
            extensions << ("*." + format.toLower());
    }
    return extensions;
}

void MainWindow::saveScreenshot()
{
    const QString defaultFileName = QString("%1.png").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy_HH-mm-ss"));
    const QStringList &extensions = supportedImgFileExtensions();
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save screenshot..."), defaultFileName, extensions.join(";; "));

    if (!fileName.isEmpty()) {
        QFileInfo selectedFile(fileName);
        const QString usedExtension = "*." + selectedFile.suffix().toLower();
        if (!extensions.contains(usedExtension))
            fileName.append(".png");

        saveSceneRender(fileName);
    }
}

void MainWindow::saveSceneRender(const QString &filePath) const
{
    if (filePath.isEmpty())
        return;

    if (QGraphicsScene *scene = d->m_model->graphicsScene()) {
        QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
        img.fill(Qt::transparent);
        QPainter p(&img);
        scene->render(&p);
        img.save(filePath);
    }
}

}
