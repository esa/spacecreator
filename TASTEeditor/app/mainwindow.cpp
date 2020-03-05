/*
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
#include "app/common.h"
#include "app/context/action/actionsmanager.h"
#include "app/xmldocexporter.h"
#include "app/zoomcontroller.h"
#include "document/documentsmanager.h"
#include "document/tabdocumentfactory.h"
#include "logging/loghandler.h"
#include "reports/bugreportdialog.h"
#include "settings/appoptions.h"
#include "settings/settingsmanager.h"
#include "tab_aadl/aadlobjectfunctiontype.h"
#include "tab_aadl/aadltabdocument.h"
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (prepareQuit())
        QMainWindow::closeEvent(e);
    else
        e->ignore();
}

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

void MainWindow::initMenus()
{
    initMenuFile();
    initMenuEdit();
    initMenuHelp();
}

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

    common::registerAction(Q_FUNC_INFO, m_actOpenFile, "Open file", "Show Open File dialog");
    common::registerAction(Q_FUNC_INFO, m_actCreateFile, "Create file", "Create new empty file");
    common::registerAction(Q_FUNC_INFO, m_actCloseFile, "Close file", "Close current file");
    common::registerAction(Q_FUNC_INFO, m_actSaveSceneRender, "Render", "Save current scene complete render.");
    common::registerAction(Q_FUNC_INFO, m_actQuit, "Quit", "Quite the application");
}

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

    common::registerAction(Q_FUNC_INFO, m_actUndo, "Undo", "Undo the last operation");
    common::registerAction(Q_FUNC_INFO, m_actRedo, "Redo", "Redo the last undone operation");
}

void MainWindow::initMenuHelp()
{
    m_menuHelp = menuBar()->addMenu(tr("&Help"));
    m_actReport = m_menuHelp->addAction(tr("Send report..."), this, &MainWindow::onReportRequested);
    m_actAbout = m_menuHelp->addAction(tr("About"), this, &MainWindow::onAboutRequested, QKeySequence::HelpContents);

    common::registerAction(Q_FUNC_INFO, m_actReport, "Report", "Send the debug information");
    common::registerAction(Q_FUNC_INFO, m_actAbout, "About", "Show About dialog");
}

void MainWindow::initConnections()
{
    connect(m_docsManager, &document::DocumentsManager::currentDocIdChanged, this, &MainWindow::onTabSwitched);
}

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

void MainWindow::onCreateFileRequested()
{
    onCloseFileRequested();
    if (document::AbstractTabDocument *doc = currentDoc())
        doc->create();
}

bool MainWindow::onCloseFileRequested()
{
    if (document::AbstractTabDocument *doc = currentDoc())
        return closeTab(m_docsManager->docId(doc));

    return true;
}

bool MainWindow::closeTab(int id)
{
    if (document::AbstractTabDocument *doc = m_docsManager->docById(id)) {
        if (doc->isDirty() && !m_dropUnsavedChangesSilently) {
            const QMessageBox::StandardButtons btns(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
            const QMessageBox::StandardButton btn = QMessageBox::question(this, tr("Document closing"),
                                                                          tr("There are unsaved changes.\n"
                                                                             "Would you like to save the document?"),
                                                                          btns);
            switch (btn) {
            case QMessageBox::Save: {
                if (app::XmlDocExporter::canExportXml(doc)) {
                    if (!exportDocAsXml(doc))
                        return false;
                } else {
                    qWarning() << "Not implemented yet";
                }
                break;
            }
            case QMessageBox::Cancel: {
                return false;
            }
            default:
                break;
            }
        }

        doc->close();
    }

    return true;
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
 * @brief MainWindow::onExportAs slot to open Template Editor dialog
 */
bool MainWindow::onExportAs()
{
    return exportDocsAs();
}

/**
 * @brief MainWindow::onExportXml slot to export the current document to XML
 * with current/default values (the template and result file names)
 */
bool MainWindow::onExportXml()
{
    return exportCurrentDocAsXml();
}

bool MainWindow::exportCurrentDocAsXml(const QString &savePath, const QString &templatePath)
{
    return exportDocAsXml(currentDoc(), savePath, templatePath);
}

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
        m_zoomCtrl->setView(qobject_cast<GraphicsView *>(doc->view()));
    }

    if (currentStack) {
        if (m_undoGroup->stacks().contains(currentStack))
            m_undoGroup->addStack(currentStack);
        m_undoGroup->setActiveStack(currentStack);
    } else {
        m_undoGroup->removeStack(m_undoGroup->activeStack());
    }

    cmd::CommandsStack::setCurrent(currentStack);

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
    restoreGeometry(AppOptions::MainWindow.Geometry.read().toByteArray());
    restoreState(AppOptions::MainWindow.State.read().toByteArray());
    onTabSwitched(AppOptions::MainWindow.LastTab.read().toInt());
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
 * \brief MainWindow::processCommandLineArg handles initial option from command line
 * \returns true if an option is handled
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
    default:
        qWarning() << Q_FUNC_INFO << "Unhandled option:" << arg << value;
        break;
    }
    return false;
}

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

document::AbstractTabDocument *MainWindow::currentDoc() const
{
    return m_docsManager->currentDoc();
}

void MainWindow::onDocDirtyChanged(bool /*dirty*/)
{
    if (document::AbstractTabDocument *caller = qobject_cast<document::AbstractTabDocument *>(sender())) {
        int docId = m_docsManager->docId(caller);
        if (m_docsManager->isValidDocId(docId))
            m_tabWidget->setTabText(docId, caller->title());
    }
    updateActions();
}

bool MainWindow::prepareQuit()
{
    for (int i = 0; i < m_tabWidget->count(); ++i)
        if (!closeTab(i))
            return false;

    AppOptions::MainWindow.State.write(saveState());
    AppOptions::MainWindow.Geometry.write(saveGeometry());
    AppOptions::MainWindow.LastTab.write(m_tabWidget->currentIndex());

    return true;
}

bool MainWindow::exportDocAsXml(document::AbstractTabDocument *doc, const QString &pathToSave,
                                const QString &templateToUse)
{
    if (!doc)
        return false;

    return app::XmlDocExporter::exportDocSilently(doc, pathToSave, templateToUse);
}

bool MainWindow::exportDocInteractive(document::AbstractTabDocument *doc, const QString &pathToSave,
                                      const QString &templateToUse)
{
    if (!doc)
        return false;

    return app::XmlDocExporter::exportDocInteractive(doc, this, pathToSave, templateToUse);
}

} // ns taste3
