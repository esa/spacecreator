﻿/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "app/commandsstack.h"
#include "common.h"
#include "app/context/action/actionsmanager.h"
#include "app/xmldocexporter.h"
#include "app/zoomcontroller.h"
#include "baseitems/common/utils.h"
#include "document/documentsmanager.h"
#include "tabdocumentfactory.h"
#include "logging/loghandler.h"
#include "reports/bugreportdialog.h"
#include "settings/appoptions.h"
#include "aadlobjectfunctiontype.h"
#include "aadltabdocument.h"
#include "tab_concurrency/concurrencytabdocument.h"
#include "tab_data/datatabdocument.h"
#include "tab_deployment/deploymenttabdocument.h"
#include "tab_interface/interfacetabdocument.h"
#include "tab_msc/msctabdocument.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsView>
#include <QImageWriter>
#include <QMessageBox>
#include <QStatusBar>
#include <QTabWidget>
#include <QUndoGroup>

namespace taste3 {

/*!
\class taste3::MainWindow
\brief Main appllication window - the place to store and manage supported document types, import/export data,
process command line arguments and user actions.

\sa taste3::document::AbstractTabDocument, taste3::document::DocumentsManager, taste3::CommandLineParser
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_tabWidget(new QTabWidget(this))
    , m_docToolbar(new QToolBar(this))
    , m_zoomCtrl(new ZoomController())
    , m_docsManager(new document::DocumentsManager(m_tabWidget, this))
    , m_undoGroup(new QUndoGroup(this))
{
    init();
}

/*!
 * \brief D-tor clears the stuff generated by QDesigner.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief handler is used to update UI for language change, etc.
 * \a e
 */
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    if (e->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
}

/*!
 * \brief perform check for unsaved changes.
 * \a e - the event to be bypassed to the basic implementation.
 */
void MainWindow::closeEvent(QCloseEvent *e)
{
    if (prepareQuit())
        QMainWindow::closeEvent(e);
    else
        e->ignore();
}

/*!
 * \brief Initialization of the UI, status bar, custom toolbar, menus, actions and so on.
 */
void MainWindow::init()
{
    ui->setupUi(this);

    statusBar()->addPermanentWidget(m_zoomCtrl);

    setCentralWidget(m_tabWidget);
    m_docToolbar->setObjectName("Document ToolBar");
    m_docToolbar->setAllowedAreas(Qt::AllToolBarAreas);
    m_docToolbar->setMovable(true);
    addToolBar(m_docToolbar);
    m_docToolbar->hide();

    initMenus();

    initTabs();

    initConnections();

    initSettings();
}

/*!
 * \brief Fills the menu bar.
 */
void MainWindow::initMenus()
{
    initMenuFile();
    initMenuEdit();
    initMenuHelp();
}

/*!
 * \brief Fills the File menu with actions.
 */
void MainWindow::initMenuFile()
{
    m_menuFile = menuBar()->addMenu(tr("&File"));
    m_actOpenFile = m_menuFile->addAction(tr("Open"), this, &MainWindow::onOpenFileRequested, QKeySequence::Open);
    m_actCreateFile = m_menuFile->addAction(tr("Create"), this, &MainWindow::onCreateFileRequested, QKeySequence::New);
    m_actCloseFile = m_menuFile->addAction(tr("Close"), this, &MainWindow::onCloseFileRequested, QKeySequence::Close);
    m_menuFile->addSeparator();
    m_actSaveSceneRender = m_menuFile->addAction(tr("Render Scene..."), this, &MainWindow::onSaveRenderRequested);
    m_menuFile->addSeparator();
    m_actExportXml =
            m_menuFile->addAction(tr("Export XML"), this, &MainWindow::onExportXml, QKeySequence(Qt::CTRL + Qt::Key_E));
    m_actExportAs = m_menuFile->addAction(tr("Export As..."), this, &MainWindow::onExportAs,
                                          QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_E));
    m_menuFile->addSeparator();
    m_actQuit = m_menuFile->addAction(tr("Quit"), this, &MainWindow::onQuitRequested, QKeySequence::Quit);

    ctx::ActionsManager::registerAction(Q_FUNC_INFO, m_actOpenFile, "Open file", "Show Open File dialog");
    ctx::ActionsManager::registerAction(Q_FUNC_INFO, m_actCreateFile, "Create file", "Create new empty file");
    ctx::ActionsManager::registerAction(Q_FUNC_INFO, m_actCloseFile, "Close file", "Close current file");
    ctx::ActionsManager::registerAction(Q_FUNC_INFO, m_actSaveSceneRender, "Render", "Save current scene complete render.");
    ctx::ActionsManager::registerAction(Q_FUNC_INFO, m_actQuit, "Quit", "Quite the application");
}

/*!
 * \brief Fills the Edit menu with actions.
 */
void MainWindow::initMenuEdit()
{
    m_actUndo = m_undoGroup->createUndoAction(this, tr("Undo:"));
    m_actUndo->setShortcut(QKeySequence::Undo);
    m_actUndo->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/undo.svg")));
    ui->mainToolBar->addAction(m_actUndo);

    m_actRedo = m_undoGroup->createRedoAction(this, tr("Redo:"));
    m_actRedo->setShortcut(QKeySequence::Redo);
    m_actRedo->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/redo.svg")));
    ui->mainToolBar->addAction(m_actRedo);

    m_menuEdit = menuBar()->addMenu(tr("&Edit"));
    m_menuEdit->addAction(m_actUndo);
    m_menuEdit->addAction(m_actRedo);
    m_menuEdit->addSeparator();

    ctx::ActionsManager::registerAction(Q_FUNC_INFO, m_actUndo, "Undo", "Undo the last operation");
    ctx::ActionsManager::registerAction(Q_FUNC_INFO, m_actRedo, "Redo", "Redo the last undone operation");
}

/*!
 * \brief Fills the Help menu with actions.
 */
void MainWindow::initMenuHelp()
{
    m_menuHelp = menuBar()->addMenu(tr("&Help"));
    m_actReport = m_menuHelp->addAction(tr("Send report..."), this, &MainWindow::onReportRequested);
    m_actAbout = m_menuHelp->addAction(tr("About"), this, &MainWindow::onAboutRequested, QKeySequence::HelpContents);

    ctx::ActionsManager::registerAction(Q_FUNC_INFO, m_actReport, "Report", "Send the debug information");
    ctx::ActionsManager::registerAction(Q_FUNC_INFO, m_actAbout, "About", "Show About dialog");
}

/*!
 * \brief  The entry point to setup common connections.
 */
void MainWindow::initConnections()
{
    connect(m_docsManager, &document::DocumentsManager::currentDocIdChanged, this, &MainWindow::onTabSwitched);
}

/*!
 * \brief Handler for File->open action.
 */
void MainWindow::onOpenFileRequested()
{
    if (document::AbstractTabDocument *doc = currentDoc()) {
        const QString prevPath(doc->path());
        const QString &fileName =
                QFileDialog::getOpenFileName(this, tr("Open file"), prevPath, doc->supportedFileExtensions());
        if (!fileName.isEmpty() && onCloseFileRequested())
            doc->load(fileName);
    }
}

/*!
 * \brief Handler for File->create action.
 */
void MainWindow::onCreateFileRequested()
{
    onCloseFileRequested();
    if (document::AbstractTabDocument *doc = currentDoc())
        doc->create();
}

/*!
 * \brief Helper function to handle requests for closing current file.
 * Calls the bool MainWindow::closeTab(int id) with current doc id.
 * \sa bool MainWindow::closeTab(int id)
 * Returns \c true if document is closed.
 */
bool MainWindow::onCloseFileRequested()
{
    if (document::AbstractTabDocument *doc = currentDoc())
        return closeTab(m_docsManager->docId(doc));

    return true;
}

/*!
 * \brief \a id - the id of the to be closed doc.
 *
 * Performs check for unsaved changes, asking the user to save, if necessary.
 * Returns true if the file is closed.
 */
bool MainWindow::closeTab(int id)
{
    if (document::AbstractTabDocument *doc = m_docsManager->docById(id)) {
        if (doc->isDirty() && !m_dropUnsavedChangesSilently) {
            const QMessageBox::StandardButtons btns(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
            auto btn = QMessageBox::question(this, tr("Document closing"), tr("There are unsaved changes.\nWould you like to save the document?"), btns);
            if (btn == QMessageBox::Save) {
                if (app::XmlDocExporter::canExportXml(doc)) {
                    if (!exportDocAsXml(doc))
                        return false;
                } else {
                    qWarning() << "Not implemented yet";
                }
            } else if (btn == QMessageBox::Cancel) {
                return false;
            }
        }

        doc->close();
    }

    return true;
}

/*!
 * \brief Return the list of image formats which the Qt is available to write.
 */
static QStringList supportedImgFileExtensions()
{
    QStringList extensions;
    const QList<QByteArray> &formats = QImageWriter::supportedImageFormats();
    for (const QByteArray &format : formats)
        extensions << ("*." + format.toLower());
    return extensions;
}

/*!
 * \brief Handler for render scene request.
 */
void MainWindow::onSaveRenderRequested()
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

/**
 * \brief Slot to open Template Editor dialog
 */
bool MainWindow::onExportAs()
{
    return exportDocsAs();
}

/**
 * \brief slot to export the current document to XML
 * with current/default values (the template and result file names)
 */
bool MainWindow::onExportXml()
{
    return exportCurrentDocAsXml();
}

/*!
 * \brief Helper function to save the carrent doc.
 * Passes parameters to
 * \sa bool MainWindow::exportDocAsXml(document::AbstractTabDocument *doc, const QString &pathToSave, const QString
 * &templateToUse) and returns its result.
 */
bool MainWindow::exportCurrentDocAsXml(const QString &savePath, const QString &templatePath)
{
    return exportDocAsXml(currentDoc(), savePath, templatePath);
}

/*!
 * \brief Helper function to save all the documents to the \a savePath using the same template file \a templatePath
 * The first failed export breaks the exporting loop and returns \c false
 */
bool MainWindow::exportDocsAs(const QString &savePath, const QString &templatePath)
{
    for (int i = 0; i < m_tabWidget->count(); ++i)
        if (!exportDocInteractive(m_docsManager->docById(i), savePath, templatePath))
            return false;
    return true;
}

void MainWindow::onQuitRequested()
{
    if (prepareQuit())
        qApp->quit();
}

void MainWindow::onAboutRequested()
{
    QString info = qApp->applicationName();

#ifdef VS_BUILD_HASH
    info += QString("@%1").arg(VS_BUILD_HASH);
#endif

    QMessageBox::information(this, tr("About"), info);
}

void MainWindow::onTabSwitched(int tab)
{
    QUndoStack *currentStack { nullptr };
    if (document::AbstractTabDocument *doc = m_docsManager->docById(tab)) {
        doc->fillToolBar(m_docToolbar);
        m_docToolbar->show();
        currentStack = doc->commandsStack();
        if (auto view = qobject_cast<aadlinterface::GraphicsView *>(doc->view())) {
            m_zoomCtrl->setView(view);
            connect(view, &aadlinterface::GraphicsView::mouseMoved, this, &MainWindow::onGraphicsViewInfo, Qt::UniqueConnection);
        }
    }

    if (currentStack) {
        if (m_undoGroup->stacks().contains(currentStack))
            m_undoGroup->addStack(currentStack);
        m_undoGroup->setActiveStack(currentStack);
    } else {
        m_undoGroup->removeStack(m_undoGroup->activeStack());
    }

    aadlinterface::cmd::CommandsStack::setCurrent(currentStack);

    updateActions();
}

void MainWindow::onReportRequested()
{
    QList<QPixmap> images;
    for (int idx = 0; idx < m_docsManager->docCount(); ++idx) {
        if (document::AbstractTabDocument *doc = m_docsManager->docById(idx)) {
            if (QGraphicsScene *scene = doc->scene()) {
                const QSize sceneSize = scene->sceneRect().size().toSize();
                if (sceneSize.isNull())
                    continue;

                QPixmap pix(sceneSize);
                pix.fill(Qt::transparent);

                QPainter p;
                p.begin(&pix);
                p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
                scene->render(&p);
                p.end();

                images.append(pix);
            }
        }
    }
    BugreportDialog *dialog = new BugreportDialog(LogHandler::logPath(), images, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}

void MainWindow::initTabs()
{
    using namespace document;

    //    m_docsManager->addDocument(TabDocumentFactory::createDataTabDocument(this));
    m_docsManager->addDocument(TabDocumentFactory::createInterfaceTabDocument(this));
    //    m_docsManager->addDocument(TabDocumentFactory::createDeploymentTabDocument(this));
    //    m_docsManager->addDocument(TabDocumentFactory::createConcurrencyTabDocument(this));
    //    m_docsManager->addDocument(TabDocumentFactory::createAADLTabDocument(this));
    //    m_docsManager->addDocument(TabDocumentFactory::createMSCTabDocument(this));

    QMenu *tabsCustom = new QMenu(tr("&Tabs"), this);
    for (auto doc : m_docsManager->documents()) {
        if (QMenu *menu = doc->customMenu())
            tabsCustom->addMenu(menu);
        connect(doc, &AbstractTabDocument::dirtyChanged, this, &MainWindow::onDocDirtyChanged);
    }
    if (tabsCustom->children().size())
        menuBar()->addMenu(tabsCustom);
    else
        delete tabsCustom;
}

void MainWindow::initSettings()
{
    restoreGeometry(aadlinterface::AppOptions::MainWindow.Geometry.read().toByteArray());
    restoreState(aadlinterface::AppOptions::MainWindow.State.read().toByteArray());
    onTabSwitched(aadlinterface::AppOptions::MainWindow.LastTab.read().toInt());
}

void MainWindow::updateActions()
{
    bool renderAvailable(false);
    if (document::AbstractTabDocument *doc = currentDoc()) {
        if (QGraphicsScene *scene = doc->scene()) {
            renderAvailable = !scene->sceneRect().isEmpty() && !scene->items().isEmpty();
        }

        m_actExportXml->setEnabled(doc->isDirty() && app::XmlDocExporter::canExportXml(doc));
    }
    m_actSaveSceneRender->setEnabled(renderAvailable);
}

/*!
 * \brief Handles startup options from command line
 * \a arg - the option to be processed,
 * \a value - its value.
 * Returns \c true if the option \a arg \a value is known and handled.
 */
bool MainWindow::processCommandLineArg(CommandLineParser::Positional arg, const QString &value)
{
    switch (arg) {
    case CommandLineParser::Positional::DropUnsavedChangesSilently: {
        m_dropUnsavedChangesSilently = true;
        return true;
    }
    case CommandLineParser::Positional::OpenAADLXMLFile: {
        if (!value.isEmpty())
            if (document::AbstractTabDocument *doc = m_docsManager->docById(TABDOC_ID_InterfaceView))
                if (doc->load(value)) {
                    m_tabWidget->setCurrentIndex(TABDOC_ID_InterfaceView);
                    return true;
                }

        return false;
    }
    case CommandLineParser::Positional::OpenStringTemplateFile:
        if (!value.isEmpty())
            return exportDocsAs(QString(), value);
        return false;
    case CommandLineParser::Positional::ExportToFile:
        if (!value.isEmpty())
            return exportCurrentDocAsXml(value);
        return false;
    case CommandLineParser::Positional::ListScriptableActions: {
        ctx::ActionsManager::listRegisteredActions();
        QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
        return true;
    }
    case CommandLineParser::Positional::Unknown:
        break;
    }
    qWarning() << Q_FUNC_INFO << "Unhandled option:" << arg << value;
    return false;
}

/*!
 * \brief Renders the whole scene and saves it to the \a filePath
 */
void MainWindow::saveSceneRender(const QString &filePath) const
{
    if (filePath.isEmpty())
        return;

    if (document::AbstractTabDocument *doc = currentDoc()) {
        if (QGraphicsScene *scene = doc->scene()) {
            QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
            img.fill(Qt::transparent);
            QPainter p(&img);
            scene->render(&p);
            img.save(filePath);
        }
    }
}

/*!
 * \brief Helper method, returns pointer to the current document (the doc for currently active tab).
 */
document::AbstractTabDocument *MainWindow::currentDoc() const
{
    return m_docsManager->currentDoc();
}

/*!
 * \brief Updates the title of a tab related to the updated doc.
 * (Adds or removes the "*" sign when doc is changed/saved).
 */
void MainWindow::onDocDirtyChanged(bool /*dirty*/)
{
    if (document::AbstractTabDocument *caller = qobject_cast<document::AbstractTabDocument *>(sender())) {
        int docId = m_docsManager->docId(caller);
        if (m_docsManager->isValidDocId(docId))
            m_tabWidget->setTabText(docId, caller->title());
    }
    updateActions();
}

/*!
 * \brief Shows the \a info message from GraphicsView in the status bar.
 */
void MainWindow::onGraphicsViewInfo(const QString &info)
{
    statusBar()->showMessage(info);
}

/*!
 * \brief Closes documents, attempting to save changes, if any.
 * If user declyne request to save cahnges, returns \c false.
 */
bool MainWindow::prepareQuit()
{
    for (int i = 0; i < m_tabWidget->count(); ++i)
        if (!closeTab(i))
            return false;

    aadlinterface::AppOptions::MainWindow.State.write(saveState());
    aadlinterface::AppOptions::MainWindow.Geometry.write(saveGeometry());
    aadlinterface::AppOptions::MainWindow.LastTab.write(m_tabWidget->currentIndex());

    return true;
}

/*!
 * \brief Exports the document \a doc to the output file \a pathToSave using the template file \a templateToUse.
 * Returns \c true on succes.
 */
bool MainWindow::exportDocAsXml(document::AbstractTabDocument *doc, const QString &pathToSave,
                                const QString &templateToUse)
{
    if (!doc)
        return false;

    return app::XmlDocExporter::exportDocSilently(doc, pathToSave, templateToUse);
}

/*!
 * \brief Shows the preview dialog for exportin the \a doc to the output file \a pathToSave
 * using the template file \a templateToUse.
 * If a \a pathToSave or \a templateToUse is empty, user will be asked to select the file via QFileDialog.
 * Returns true if the preview dialog can not be shown.
 */
bool MainWindow::exportDocInteractive(document::AbstractTabDocument *doc, const QString &pathToSave,
                                      const QString &templateToUse)
{
    if (!doc)
        return false;

    return app::XmlDocExporter::exportDocInteractive(doc, this, pathToSave, templateToUse);
}

} // ns taste3
