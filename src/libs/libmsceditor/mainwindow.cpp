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
#include "chartitem.h"
#include "chartlayoutmanager.h"
#include "commandlineparser.h"
#include "commands/common/commandsstack.h"
#include "documentitemmodel.h"
#include "geometry.h"
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
#include "textviewdialog.h"
#include "tools/entitydeletetool.h"
#include "ui/graphicsviewbase.h"
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
#include <QScreen>
#include <QToolBar>
#include <QUndoGroup>
#include <QUndoStack>
#include <QVector>

namespace msc {

const QLatin1String MainWindow::DotMscFileExtensionLow = QLatin1String(".msc");

struct MainWindow::MainWindowPrivate {
    explicit MainWindowPrivate(msc::MSCPlugin *plugin, MainWindow *mainWindow)
        : ui(new Ui::MainWindow)
        , m_plugin(plugin)
        , mscTextBrowser(new TextViewDialog(mainWindow))
    {
    }

    ~MainWindowPrivate() { delete ui; }

    Ui::MainWindow *ui = nullptr;

    msc::MSCPlugin *m_plugin;

    QComboBox *m_zoomBox = nullptr;

    msc::TextViewDialog *mscTextBrowser = nullptr;

    RemoteControlWebServer *m_remoteControlWebServer = nullptr;
    RemoteControlHandler *m_remoteControlHandler = nullptr;

    bool m_dropUnsavedChangesSilently = false;

    bool m_streamingMode = false;

    Q_DISABLE_COPY(MainWindowPrivate);
};

/*!
  \class MainWindow

  This is the main window class for the MSC editor.
*/

/*!
 * \brief MainWindow::MainWindow Create an empty view.
 * \param plugin hosts most code of the code for the MSC Editor UI of type \ref msc::MSCPlugin
 * \param parent
 */
MainWindow::MainWindow(msc::MSCPlugin *plugin, QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate(plugin, this))
{
    setupUi();
    d->m_plugin->hierarchyView()->setRenderHints(
            QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    initConnections();
    d->m_plugin->initConnections();

    d->m_plugin->selectCurrentChart();
    d->m_plugin->showDocumentView(true);

    loadSettings();

    d->m_plugin->mscToolBar()->setVisible(d->m_plugin->centerView()->currentWidget() == d->m_plugin->chartView());
    d->m_plugin->hierarchyToolBar()->setVisible(
            d->m_plugin->centerView()->currentWidget() == d->m_plugin->hierarchyView());
}

MainWindow::~MainWindow()
{
    if (d->ui->documentTreeView->model()) {
        disconnect(d->ui->documentTreeView->model(), nullptr, this, nullptr);
    }
    disconnect(&(d->m_plugin->mainModel()->chartViewModel()), nullptr, this, nullptr);

    // Had this connection not dropped, the currentUndoStack() would need check
    // for nullptr d, d->ui, d->m_plugin->chartView()
    disconnect(d->m_plugin->mainModel()->undoStack(), &QUndoStack::indexChanged, this, &MainWindow::updateTitles);
}

/*!
 * \brief MainWindow::currentView Get the graphics view
 * \return
 */
QGraphicsView *MainWindow::currentView() const
{
    return d->m_plugin->chartView();
}

/*!
 * \brief MainWindow::createNewDocument Create a new document. This doesn't ask the user to save.
 */
void MainWindow::createNewDocument()
{
    if (!saveDocument()) {
        return;
    }

    d->m_plugin->mainModel()->chartViewModel().setPreferredChartBoxSize(prepareChartBoxSize());
    d->m_plugin->mainModel()->initialModel();

    d->m_plugin->chartView()->setZoom(100);
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

    const QString path = QFileInfo(d->m_plugin->mainModel()->currentFilePath()).absoluteFilePath();

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

    d->m_plugin->mainModel()->chartViewModel().setPreferredChartBoxSize(QSizeF());

    const bool ok = d->m_plugin->mainModel()->loadFile(file);
    if (!ok) {
        QMessageBox::critical(this, tr("File Error"),
                tr("Could not read the file. Errors:\n%1")
                        .arg(d->m_plugin->mainModel()->mscErrorMessages().join("\n")));
        return false;
    }

    if (!d->m_plugin->mainModel()->mscErrorMessages().isEmpty()) {
        QMessageBox::warning(this, tr("Open File Warnings"),
                tr("Warnings found while opening the file:\n%1")
                        .arg(d->m_plugin->mainModel()->mscErrorMessages().join("\n")));
    }

    d->m_plugin->chartView()->setZoom(100);

    return true;
}

/*!
 * \brief MainWindow::updateTitles Populate the titles in save action and window
 */
void MainWindow::updateTitles()
{
    static const QString title = tr("%1 [%2]%3");

    const QString dirtyMarker(d->m_plugin->mainModel()->needSave() ? "*" : "");
    const QString &filename = d->m_plugin->mainModel()->currentFilePath();
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
 * \brief MainWindow::saveMsc Save the MSC file. Ask for the filename if necessary.
 */
void MainWindow::saveMsc()
{
    const QString &filename = d->m_plugin->mainModel()->currentFilePath();
    if (filename.isEmpty()) {
        saveAsMsc();
    } else {
        d->m_plugin->mainModel()->saveMsc(filename);
    }
}

/*!
 * \brief MainWindow::saveAsMsc Save the file after asking for the filename.
 */
void MainWindow::saveAsMsc()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
            QFileInfo(d->m_plugin->mainModel()->currentFilePath()).path(),
            tr("MSC files (%1);;All files (*.*)").arg(mscFileFilters().join(" ")));
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(DotMscFileExtensionLow))
            fileName.append(DotMscFileExtensionLow);
        d->m_plugin->mainModel()->setCurrentFilePath(fileName);

        saveMsc();
    }
}

/*!
 * \brief MainWindow::showDocumentView Set whether to show the document view or not.
 * \param show
 */
void MainWindow::showDocumentView(bool show)
{
    d->m_plugin->showDocumentView(show);
    if (show) {
        updateZoomBox(d->m_plugin->chartView()->zoom());
    }
}

/*!
 * \brief MainWindow::showHierarchyView Set whether to show the hierarchy view or not
 * \param show
 */
void MainWindow::showHierarchyView(bool show)
{
    d->m_plugin->showHierarchyView(show);
    if (show) {
        updateZoomBox(d->m_plugin->hierarchyView()->zoom());
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
            d->m_plugin->mainModel()->chartViewModel().setCurrentChart(document->charts()[0]);
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
        d->m_plugin->mainModel()->setSelectedDocument(nullptr);
        return;
    }

    if (auto chart = dynamic_cast<msc::MscChart *>(obj)) {
        d->m_plugin->mainModel()->chartViewModel().setCurrentChart(chart);
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

            d->m_plugin->mainModel()->setSelectedDocument(document);
            d->m_plugin->actionPaste()->setEnabled(
                    QApplication::clipboard()->mimeData()->hasFormat(MainModel::MscChartMimeType)
                    && d->m_plugin->mainModel()->selectedDocument()->isAddChildEnable());
        }
    }
}

/*!
 * \brief MainWindow::setupUi Create the UI from the form
 */
void MainWindow::setupUi()
{
    d->ui->setupUi(this);
    d->m_plugin->setViews(d->ui->centerView, d->ui->graphicsView, d->ui->hierarchyView);

    d->m_plugin->addToolBars(this);

    d->ui->documentTreeView->setModel(d->m_plugin->mainModel()->documentItemModel());
    d->mscTextBrowser->setModel(d->m_plugin->mainModel()->mscModel());
    d->ui->asn1Widget->setModel(d->m_plugin->mainModel()->mscModel());

    initActions();

    d->m_plugin->initChartTools();
    d->m_plugin->initHierarchyViewActions();
    d->m_plugin->initMenus(this);
    initMainToolbar();

    // status bar
    d->m_zoomBox = new QComboBox(d->ui->statusBar);
    for (auto x = d->m_plugin->chartView()->minZoomPercent(); x <= d->m_plugin->chartView()->maxZoomPercent();
            x += d->m_plugin->chartView()->zoomStepPercent())
        d->m_zoomBox->addItem(QString("%1 %").arg(x), x);

    d->m_zoomBox->setCurrentIndex(d->m_zoomBox->findData(100));

    connect(d->m_zoomBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&](int index) {
        if (auto graphicsView = dynamic_cast<msc::GraphicsView *>(d->m_plugin->centerView()->currentWidget()))
            graphicsView->setZoom(qobject_cast<QComboBox *>(sender())->itemData(index).toDouble());
    });

    connect(d->m_plugin->chartView(), QOverload<double>::of(&msc::GraphicsView::zoomChanged), this,
            &MainWindow::updateZoomBox);
    connect(d->m_plugin->hierarchyView(), QOverload<double>::of(&msc::GraphicsView::zoomChanged), this,
            &MainWindow::updateZoomBox);

    statusBar()->addPermanentWidget(d->m_zoomBox);
    statusBar()->show();
}

/*!
 * \brief MainWindow::initActions Initialize the actions
 */
void MainWindow::initActions()
{
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
        d->m_remoteControlHandler->setModel(d->m_plugin->mainModel());

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

void MainWindow::initMainToolbar()
{
    auto mainToolBar = d->m_plugin->mainToolBar();
    mainToolBar->addSeparator();
    mainToolBar->addAction(d->m_plugin->deleteTool()->action());

    mainToolBar->addSeparator();
    mainToolBar->addAction(d->m_plugin->actionCopy());
    mainToolBar->addAction(d->m_plugin->actionPaste());
}

void MainWindow::initConnections()
{
    connect(d->ui->documentTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &MainWindow::showSelection);
    connect(d->ui->documentTreeView, &QTreeView::doubleClicked, this, &MainWindow::showChart);

    connect(d->m_plugin->mainModel(), &MainModel::selectedDocumentChanged, d->ui->documentTreeView,
            &DocumentTreeView::setSelectedDocument);

    connect(d->m_plugin->chartView(), &msc::GraphicsView::mouseMoved, this, &MainWindow::showCoordinatesInfo);

    connect(d->m_plugin->mainModel(), &MainModel::modelDataChanged, this, &MainWindow::updateModel);
    connect(d->m_plugin->mainModel(), &MainModel::modelUpdated, this, &MainWindow::updateModel);
    connect(d->m_plugin->mainModel(), &MainModel::modelUpdated, d->ui->asn1Widget, &ASN1FileView::setModel);

    connect(d->m_plugin->mainModel()->documentItemModel(), &msc::DocumentItemModel::dataChanged, this,
            &MainWindow::showSelection);

    connect(d->m_plugin->mainModel()->undoStack(), &QUndoStack::indexChanged, this, &MainWindow::updateTitles);
    connect(d->m_plugin->mainModel(), &MainModel::lasteSaveUndoChange, this, &MainWindow::updateTitles);

    connect(d->m_plugin->mainModel(), &MainModel::currentFilePathChanged, this, [&](const QString &filename) {
        QFileInfo fileInfo(filename);
        d->ui->asn1Widget->setCurrentDirectory(fileInfo.absolutePath());
    });

    connect(d->m_plugin->mainModel()->graphicsScene(), &QGraphicsScene::sceneRectChanged, this,
            &MainWindow::adaptWindowSizeToChart);

    connect(d->m_plugin->mainModel(), &MainModel::asn1ParameterErrorDetected, this, &MainWindow::showAsn1Errors);
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
            d->m_streamingMode = true;
            showDocumentView(true);
            menuBar()->setVisible(false);

            d->m_plugin->mscToolBar()->setVisible(false);
            d->m_plugin->hierarchyToolBar()->setVisible(false);
            d->m_plugin->mainToolBar()->setVisible(false);

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

QAction *MainWindow::dockWidgetDocumentToggleAction()
{
    return d->ui->dockWidgetDocument->toggleViewAction();
}

QAction *MainWindow::mscTextViewToggleAction()
{
    return d->mscTextBrowser->toggleViewAction();
}

QAction *MainWindow::dockWidgetAsn1ToggleAction()
{
    return d->ui->dockWidgetAsn1->toggleViewAction();
}

void MainWindow::copyCurrentChart()
{
    d->m_plugin->mainModel()->copyCurrentChart();
}

void MainWindow::copyCurrentChartAsPicture()
{
    d->m_plugin->mainModel()->copyCurrentChartAsPicture();
}

void MainWindow::pasteChart()
{
    d->m_plugin->mainModel()->pasteChart();
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
            openFileMsc(d->m_plugin->mainModel()->currentFilePath());
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
        if (msc::BaseTool *tool = d->m_plugin->activeTool())
            tool->processKeyPress(e);
        break;
    }
#endif
    }
}

void MainWindow::loadSettings()
{
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
    AppOptions::MainWindow.LastFilePath->write(d->m_plugin->mainModel()->currentFilePath());
    AppOptions::MainWindow.DocOrHierarchyViewMode->write(d->m_plugin->centerView()->currentIndex());
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
    if (d->m_plugin->mainModel()->currentFilePath().isEmpty()) {
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
    auto chart = d->m_plugin->mainModel()->chartViewModel().currentChart();
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

    d->m_plugin->checkGlobalComment();

    if (forceDefault) {
        d->m_plugin->activateDefaultTool();
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
    if (!d->m_dropUnsavedChangesSilently && d->m_plugin->mainModel()->needSave()) {
        auto result = QMessageBox::warning(this, windowTitle(),
                tr("You have unsaved data. Do you want to save the MSC document?"),
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);

        if (result == QMessageBox::Cancel) {
            return false;
        } else if (result == QMessageBox::Save) {
            saveMsc();
        }
    }

    return true;
}

void MainWindow::updateModel()
{
    d->mscTextBrowser->setModel(d->m_plugin->mainModel()->mscModel());
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
                scenePos = CoordinatesConverter::cifToScene(scenePos.toPoint());

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
    msc::MscModel *model = d->m_plugin->mainModel()->mscModel();
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
        const QVariantList params { QVariant::fromValue(d->m_plugin->mainModel()->mscModel()), dialog.fileName(),
            "ASN.1" };
        msc::cmd::CommandsStack::push(msc::cmd::Id::SetAsn1File, params);
        d->m_plugin->mainModel()->mscModel()->setAsn1TypesData(dialog.asn1Types());
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

void MainWindow::adaptWindowSizeToChart(const QRectF &rect)
{
    if (!d->m_streamingMode) {
        return;
    }

    QRect windowRect = geometry();
    QRect widgetRect = d->m_plugin->chartView()->geometry();
    const QSize offsets(windowRect.width() - widgetRect.width(), windowRect.height() - widgetRect.height());
    widgetRect = rect.marginsAdded(ChartItem::chartMargins()).toRect();
    windowRect.setSize(rect.size().toSize() + offsets);

    const QRect availableRect = screen()->availableGeometry();
    windowRect = shared::rectInRect(windowRect, availableRect);

    setGeometry(windowRect);
}

void MainWindow::showAsn1Errors(const QStringList &faultyMessages)
{
    QMessageBox::warning(
            this, tr("ASN1 error"), tr("Following messgages have ASN.1 errors:") + "\n" + faultyMessages.join("\n"));
}

void MainWindow::saveSceneRender(const QString &filePath) const
{
    if (filePath.isEmpty())
        return;

    if (QGraphicsScene *scene = d->m_plugin->mainModel()->graphicsScene()) {
        QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
        img.fill(Qt::transparent);
        QPainter p(&img);
        scene->render(&p);
        img.save(filePath);
    }
}

/**
   @todo With Qt >= 5.14 use QWidget::screen()
 */
QScreen *MainWindow::screen() const
{
    if (auto screenByPos = QGuiApplication::screenAt(geometry().center())) {
        return screenByPos;
    }

    return QGuiApplication::primaryScreen();
}

}
