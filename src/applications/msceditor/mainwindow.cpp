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
#include "colors/colormanagerdialog.h"
#include "commandlineparser.h"
#include "commands/cmdsetasn1file.h"
#include "documentitemmodel.h"
#include "geometry.h"
#include "graphicsview.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
#include "ivsystemchecks.h"
#include "mainmodel.h"
#include "minimap.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "msccondition.h"
#include "msccreate.h"
#include "mscdocument.h"
#include "msceditorcore.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "settingsmanager.h"
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
#include <QVector>

namespace msc {

const QLatin1String MainWindow::DotMscFileExtensionLow = QLatin1String(".msc");

struct MainWindow::MainWindowPrivate {
    explicit MainWindowPrivate(msc::MSCEditorCore *core, MainWindow *mainWindow)
        : ui(new Ui::MainWindow)
        , m_core(core)
        , mscTextBrowser(new TextViewDialog(mainWindow))
    {
    }

    ~MainWindowPrivate() { delete ui; }

    QSharedPointer<ive::IVEditorCore> m_ivCore { new ive::IVEditorCore() };

    Ui::MainWindow *ui = nullptr;

    msc::MSCEditorCore *m_core = nullptr;

    QComboBox *m_zoomBox = nullptr;
    QAction *m_actionShowDocument = nullptr;
    QAction *m_actionShowHierarchy = nullptr;
    QAction *m_actionOpenAadl = nullptr;

    msc::TextViewDialog *mscTextBrowser = nullptr;

    bool m_dropUnsavedChangesSilently = false;

    Q_DISABLE_COPY(MainWindowPrivate)
};

/*!
  \class MainWindow

  This is the main window class for the MSC editor.
*/

/*!
 * \brief MainWindow::MainWindow Create an empty view.
 * \param core hosts most code of the code for the MSC Editor UI of type \ref msc::MSCEditorCore
 * \param parent
 */
MainWindow::MainWindow(msc::MSCEditorCore *core, QWidget *parent)
    : QMainWindow(parent)
    , d(new MainWindowPrivate(core, this))
{
    setupUi();
    d->m_core->hierarchyView()->setRenderHints(
            QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    initConnections();
    d->m_core->initConnections();

    d->m_core->selectCurrentChart();
    d->m_core->showDocumentView(true);

    loadSettings();

    d->m_core->mscToolBar()->setVisible(d->m_core->centerView()->currentWidget() == d->m_core->chartView());
    d->m_core->hierarchyToolBar()->setVisible(d->m_core->centerView()->currentWidget() == d->m_core->hierarchyView());
}

MainWindow::~MainWindow()
{
    if (d->ui->documentTreeView->model()) {
        disconnect(d->ui->documentTreeView->model(), nullptr, this, nullptr);
    }
    disconnect(&(d->m_core->mainModel()->chartViewModel()), nullptr, this, nullptr);

    // Had this connection not dropped, the currentUndoStack() would need check
    // for nullptr d, d->ui, d->m_core->chartView()
    disconnect(d->m_core->mainModel()->undoStack(), &QUndoStack::indexChanged, this, &MainWindow::updateTitles);
}

/*!
 * \brief MainWindow::currentView Get the graphics view
 * \return
 */
QGraphicsView *MainWindow::currentView() const
{
    return d->m_core->chartView();
}

/*!
 * \brief MainWindow::createNewDocument Create a new document. This doesn't ask the user to save.
 */
void MainWindow::createNewDocument()
{
    if (!saveDocument()) {
        return;
    }

    d->m_core->mainModel()->chartViewModel().setPreferredChartBoxSize(prepareChartBoxSize());
    d->m_core->mainModel()->initialModel();

    d->m_core->chartView()->setZoom(100);
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

    const QString path = QFileInfo(d->m_core->mainModel()->currentFilePath()).absoluteFilePath();

    const QString filename = QFileDialog::getOpenFileName(this, tr("MSC"), path, suffixes.join(";;"));
    if (!filename.isEmpty()) {
        openFileMsc(filename);
    }
}

void MainWindow::openAadlFile()
{
    const QString &fileName =
            QFileDialog::getOpenFileName(this, tr("Open file"), "", d->m_ivCore->document()->supportedFileExtensions());
    if (!fileName.isEmpty()) {
        d->m_ivCore->document()->load(fileName);
        if (auto checker = qobject_cast<scs::IvSystemChecks *>(d->m_core->systemChecker())) {
            checker->setIvCore(d->m_ivCore);
        }
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

    d->m_core->mainModel()->chartViewModel().setPreferredChartBoxSize(QSizeF());

    const bool ok = d->m_core->mainModel()->loadFile(file);
    if (!ok) {
        QMessageBox::critical(this, tr("File Error"),
                tr("Could not read the file. Errors:\n%1").arg(d->m_core->mainModel()->mscErrorMessages().join("\n")));
        return false;
    }

    if (!d->m_core->mainModel()->mscErrorMessages().isEmpty()) {
        QMessageBox::warning(this, tr("Open File Warnings"),
                tr("Warnings found while opening the file:\n%1")
                        .arg(d->m_core->mainModel()->mscErrorMessages().join("\n")));
    }

    d->m_core->chartView()->setZoom(100);

    return true;
}

/*!
 * \brief MainWindow::updateTitles Populate the titles in save action and window
 */
void MainWindow::updateTitles()
{
    static const QString title = tr("%1 [%2]%3");

    const QString dirtyMarker(d->m_core->mainModel()->needSave() ? "*" : "");
    const QString &filename = d->m_core->mainModel()->currentFilePath();
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
    const QString &filename = d->m_core->mainModel()->currentFilePath();
    if (filename.isEmpty()) {
        saveAsMsc();
    } else {
        d->m_core->mainModel()->saveMsc(filename);
    }
}

/*!
 * \brief MainWindow::saveAsMsc Save the file after asking for the filename.
 */
void MainWindow::saveAsMsc()
{
    QFileDialog dialog(this, tr("Save as..."), QFileInfo(d->m_core->mainModel()->currentFilePath()).path(),
            tr("MSC files (%1);;All files (*.*)").arg(mscFileFilters().join(" ")));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(DotMscFileExtensionLow);
    if (dialog.exec() == QDialog::Accepted) {
        const QString fileName = dialog.selectedUrls().value(0).toLocalFile();
        d->m_core->mainModel()->setCurrentFilePath(fileName);
        saveMsc();
    }
}

/*!
 * \brief MainWindow::showDocumentView Set whether to show the document view or not.
 * \param show
 */
void MainWindow::showDocumentView(bool show)
{
    d->m_core->showDocumentView(show);
    if (show) {
        updateZoomBox(d->m_core->chartView()->zoom());
    }
}

/*!
 * \brief MainWindow::showHierarchyView Set whether to show the hierarchy view or not
 * \param show
 */
void MainWindow::showHierarchyView(bool show)
{
    d->m_core->showHierarchyView(show);
    if (show) {
        updateZoomBox(d->m_core->hierarchyView()->zoom());
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
            d->m_core->mainModel()->chartViewModel().setCurrentChart(document->charts()[0]);
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
        d->m_core->mainModel()->setSelectedDocument(nullptr);
        return;
    }

    if (auto chart = dynamic_cast<msc::MscChart *>(obj)) {
        d->m_core->mainModel()->chartViewModel().setCurrentChart(chart);
        showDocumentView(true);
    } else {
        showHierarchyView(true);

        if (auto document = dynamic_cast<msc::MscDocument *>(obj)) {
            d->m_core->mainModel()->setSelectedDocument(document);
            d->m_core->actionPaste()->setEnabled(
                    QApplication::clipboard()->mimeData()->hasFormat(MainModel::MscChartMimeType)
                    && d->m_core->mainModel()->selectedDocument()->isAddChildEnable());
        }
    }
}

/*!
 * \brief MainWindow::setupUi Create the UI from the form
 */
void MainWindow::setupUi()
{
    d->ui->setupUi(this);
    d->m_core->setViews(d->ui->centerView, d->ui->graphicsView, d->ui->hierarchyView);

    d->m_core->addToolBars(this);

    d->ui->documentTreeView->setModel(d->m_core->mainModel()->documentItemModel());
    d->mscTextBrowser->setModel(d->m_core->mainModel()->mscModel());

    initActions();

    d->m_core->initChartTools();
    d->m_core->initHierarchyViewActions();

    initMenus();
    initMainToolbar();

    d->m_core->setupMiniMap();

    // status bar
    d->m_zoomBox = new QComboBox(d->ui->statusBar);
    for (auto x = d->m_core->chartView()->minZoomPercent(); x <= d->m_core->chartView()->maxZoomPercent();
            x += d->m_core->chartView()->zoomStepPercent())
        d->m_zoomBox->addItem(QString("%1 %").arg(x), x);

    d->m_zoomBox->setCurrentIndex(d->m_zoomBox->findData(100));

    connect(d->m_zoomBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&](int index) {
        if (auto graphicsView = dynamic_cast<msc::GraphicsView *>(d->m_core->centerView()->currentWidget()))
            graphicsView->setZoom(qobject_cast<QComboBox *>(sender())->itemData(index).toDouble());
    });

    connect(d->m_core->chartView(), QOverload<double>::of(&msc::GraphicsView::zoomChanged), this,
            &MainWindow::updateZoomBox);
    connect(d->m_core->hierarchyView(), QOverload<double>::of(&msc::GraphicsView::zoomChanged), this,
            &MainWindow::updateZoomBox);

    statusBar()->addPermanentWidget(d->m_zoomBox);
    statusBar()->show();
}

/*!
 * \brief MainWindow::initActions Initialize the actions
 */
void MainWindow::initActions()
{
    // Connect the core actions
    connect(d->m_core->actionNewFile(), &QAction::triggered, this, &MainWindow::createNewDocument);
    connect(d->m_core->actionOpenFile(), &QAction::triggered, this, &MainWindow::selectAndOpenFile);
    connect(d->m_core->actionSaveFile(), &QAction::triggered, this, &MainWindow::saveMsc);
    connect(d->m_core->actionSaveFileAs(), &QAction::triggered, this, &MainWindow::saveAsMsc);
    connect(d->m_core->actionQuit(), &QAction::triggered, this, [&]() {
        if (this->saveDocument()) {
            this->saveSettings();
            QApplication::quit();
        }
    });
}

void MainWindow::initMenus()
{
    // Initialize the file menu
    auto menu = menuBar()->addMenu(tr("File"));
    menu->addAction(d->m_core->actionNewFile());
    menu->addAction(d->m_core->actionOpenFile());
    menu->addAction(d->m_core->actionSaveFile());
    menu->addAction(d->m_core->actionSaveFileAs());
    menu->addSeparator();
    menu->addAction(tr("Open AADL file ..."), this, &MainWindow::openAadlFile);
    menu->addSeparator();
    menu->addAction(QIcon(QLatin1String(":/sharedresources/icons/save.svg")), tr("Save Screenshot..."), this,
            &MainWindow::saveScreenshot, QKeySequence(Qt::ALT + Qt::Key_S));
    menu->addSeparator();
    menu->addAction(d->m_core->actionQuit());

    // Initialize the edit menu
    menu = menuBar()->addMenu(tr("Edit"));
    QAction *undoAction = d->m_core->actionUndo();
    undoAction->setShortcut(QKeySequence::Undo);
    menu->addAction(undoAction);
    QAction *redoAction = d->m_core->actionRedo();
    redoAction->setShortcut(QKeySequence::Redo);
    menu->addAction(redoAction);
    menu->addSeparator();
    menu->addAction(d->m_core->deleteTool()->action());
    menu->addSeparator();
    menu->addAction(d->m_core->createActionCopy(this));
    menu->addAction(d->m_core->createActionPaste(this));

    // Initialize the view menu
    menu = menuBar()->addMenu(tr("&View"));
    menu->addAction(d->m_core->actionToggleMinimap());
    menu->addSeparator();
    d->m_actionShowDocument = menu->addAction(tr("Show &Document"), this, &MainWindow::showDocumentView, tr("F8"));
    d->m_actionShowHierarchy = menu->addAction(tr("Show &Hierarchy"), this, &MainWindow::showHierarchyView, tr("F9"));
    d->m_actionShowDocument->setCheckable(true);
    d->m_actionShowHierarchy->setCheckable(true);
    auto group = new QActionGroup(menu);
    group->addAction(d->m_actionShowDocument);
    group->addAction(d->m_actionShowHierarchy);
    d->m_actionShowDocument->setChecked(true);
    menu->addSeparator();
    menu->addAction(d->m_core->actionMessageDeclarations());
    menu->addSeparator();
    auto menuWindows = menu->addMenu("Windows");
    menuWindows->addAction(dockWidgetDocumentToggleAction());
    menuWindows->addAction(mscTextViewToggleAction());
    menuWindows->addAction(dockWidgetAsn1ToggleAction());
    menu->addSeparator();
    menu->addAction(tr("Color Scheme..."), this, &MainWindow::editColorScheme);

    // Initialize the help menu
    menu = menuBar()->addMenu(tr("&Help"));
    menu->addAction(tr("Help"), d->m_core, &shared::EditorCore::showHelp);
    menu->addSeparator();
    menu->addAction(tr("About"), d->m_core, &shared::EditorCore::showAboutDialog);
    menu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
}

void MainWindow::initMainToolbar()
{
    auto mainToolBar = d->m_core->mainToolBar();
    mainToolBar->addSeparator();
    mainToolBar->addAction(d->m_core->deleteTool()->action());

    mainToolBar->addSeparator();
    mainToolBar->addAction(d->m_core->actionCopy());
    mainToolBar->addAction(d->m_core->actionPaste());
}

void MainWindow::initConnections()
{
    connect(d->ui->documentTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &MainWindow::showSelection);
    connect(d->ui->documentTreeView, &QTreeView::doubleClicked, this, &MainWindow::showChart);

    connect(d->m_core->mainModel(), &MainModel::selectedDocumentChanged, d->ui->documentTreeView,
            &DocumentTreeView::setSelectedDocument);

    connect(d->m_core->chartView(), &msc::GraphicsView::mouseMoved, this, &MainWindow::showCoordinatesInfo);

    connect(d->m_core->mainModel(), &MainModel::modelDataChanged, this, &MainWindow::updateModel);
    connect(d->m_core->mainModel(), &MainModel::modelUpdated, this, &MainWindow::updateModel);

    connect(d->m_core->mainModel()->documentItemModel(), &msc::DocumentItemModel::dataChanged, this,
            &MainWindow::showSelection);

    connect(d->m_core->mainModel()->undoStack(), &QUndoStack::cleanChanged, this, &MainWindow::updateTitles);

    // ASN1 view
    connect(d->m_core->mainModel(), &MainModel::currentFilePathChanged, this, [&](const QString &filename) {
        QFileInfo fileInfo(filename);
        d->ui->asn1Widget->setDirectory(fileInfo.absolutePath());
    });
    connect(d->m_core->mainModel(), &msc::MainModel::asn1FileNameChanged, d->ui->asn1Widget,
            &asn1::ASN1FileView::setFileName);
    connect(d->ui->asn1Widget, &asn1::ASN1FileView::asn1Selected, this, [this]() {
        msc::MscModel *model = d->m_core->mainModel()->mscModel();
        if (model->dataDefinitionString() != d->ui->asn1Widget->fileName()) {
            d->m_core->commandsStack()->push(new cmd::CmdSetAsn1File(model, d->ui->asn1Widget->fileName(), "ASN.1"));
        }
    });
    connect(d->m_core->mainModel(), &MainModel::asn1ParameterErrorDetected, this, &MainWindow::showAsn1Errors);
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

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    QMainWindow::keyPressEvent(e);

    switch (e->key()) {
    case Qt::Key_Escape: {
        if (!e->isAutoRepeat()) {
            if (QAction *pointerToolAction = d->m_core->mscToolBar()->actions().first())
                if (!pointerToolAction->isChecked())
                    pointerToolAction->setChecked(true);
        }
        break;
    }
#ifdef QT_DEBUG
    case Qt::Key_R: {
        if (!e->isAutoRepeat() && (e->modifiers() == Qt::ControlModifier)) {
            openFileMsc(d->m_core->mainModel()->currentFilePath());
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
        if (msc::BaseTool *tool = d->m_core->activeTool())
            tool->processKeyPress(e);
        break;
    }
#endif
    }
}

void MainWindow::loadSettings()
{
    using namespace shared;

    restoreGeometry(SettingsManager::load<QByteArray>(SettingsManager::Common::Geometry));
    restoreState(SettingsManager::load<QByteArray>(SettingsManager::Common::State));

    // the toolbar might be hidden from a streaming tool session
    d->m_core->mainToolBar()->show();

    const bool isDocViewMode = SettingsManager::load<bool>(SettingsManager::MSC::DocViewMode);
    if (isDocViewMode) {
        showDocumentView(true);
        d->m_actionShowDocument->setChecked(true);
    } else {
        showHierarchyView(true);
        d->m_actionShowHierarchy->setChecked(true);
    }
}

void MainWindow::saveSettings()
{
    using namespace shared;
    SettingsManager::store<QByteArray>(SettingsManager::Common::Geometry, saveGeometry());
    SettingsManager::store<QByteArray>(SettingsManager::Common::State, saveState());
    SettingsManager::store<bool>(SettingsManager::MSC::DocViewMode, d->m_core->centerView()->currentIndex() == 0);
    const QString path = d->m_core->mainModel()->currentFilePath();

    auto files = SettingsManager::load<QStringList>(SettingsManager::MSC::RecentFiles);
    files.removeAll(path);
    files.prepend(path);
    while (files.size() > 10) {
        files.removeLast();
    }
    SettingsManager::store<QStringList>(SettingsManager::MSC::RecentFiles, files);
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
        // is applied after QEvent::Show, so QEvent::Resize handler seems to be
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
    if (d->m_core->mainModel()->currentFilePath().isEmpty()) {
        if (auto chart = d->m_core->mainModel()->chartViewModel().currentChart()) {
            if (chart->isEmpty()) {
                QMetaObject::invokeMethod(this, "createNewDocument", Qt::QueuedConnection);
            }
        }
    }
}

QSizeF MainWindow::prepareChartBoxSize() const
{
    static constexpr qreal padding = 110.;
    if (centralWidget()) {
        return centralWidget()->geometry().size() - QSizeF(padding, padding);
    }
    return QSizeF();
}

void MainWindow::updateMscToolbarActionsEnablement()
{
    auto chart = d->m_core->mainModel()->chartViewModel().currentChart();
    const bool hasInstance = chart && !chart->instances().isEmpty();

    bool forceDefault(false);
    for (QAction *act : d->m_core->mscToolBar()->actions()) {
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

    d->m_core->checkGlobalComment();

    if (forceDefault) {
        d->m_core->activateDefaultTool();
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
    if (!d->m_dropUnsavedChangesSilently && d->m_core->mainModel()->needSave()) {
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
    d->mscTextBrowser->setModel(d->m_core->mainModel()->mscModel());
    updateMscToolbarActionsEnablement();
}

#ifdef QT_DEBUG

// A way to precisely move mouse pointer to scene coordinates without pixel hunting.
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
    d->m_core->openMessageDeclarationEditor(this);
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

    QFileDialog dialog(nullptr, tr("Save screenshot..."), defaultFileName, extensions.join(";; "));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(".png");
    if (dialog.exec() == QDialog::Accepted) {
        const QString fileName = dialog.selectedUrls().value(0).toLocalFile();
        saveSceneRender(fileName);
    }
}

void MainWindow::showAsn1Errors(const QStringList &faultyMessages)
{
    QMessageBox::warning(
            this, tr("ASN1 error"), tr("Following messages have ASN.1 errors:") + "\n" + faultyMessages.join("\n"));
}

void MainWindow::editColorScheme()
{
    shared::ColorManagerDialog *dialog = new shared::ColorManagerDialog(window());
    dialog->setFilterGroup("MSC");
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void MainWindow::saveSceneRender(const QString &filePath) const
{
    if (filePath.isEmpty())
        return;

    if (QGraphicsScene *scene = d->m_core->mainModel()->graphicsScene()) {
        QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
        img.fill(Qt::transparent);
        QPainter p(&img);
        scene->render(&p);
        img.save(filePath);
    }
}
}
