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
#include "document/documentsmanager.h"
#include "settings/appoptions.h"
#include "settings/settingsmanager.h"
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
#include <QTabWidget>
#include <QUndoGroup>

namespace taste3 {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_tabWidget(new QTabWidget(this))
    , m_docToolbar(new QToolBar(this))
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
    e->ignore();
    onQuitRequested();
}

void MainWindow::init()
{
    ui->setupUi(this);

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
    m_actQuit = m_menuFile->addAction(tr("Quit"), this, &MainWindow::onQuitRequested, QKeySequence::Quit);
}

void MainWindow::initMenuEdit()
{
    m_actUndo = m_undoGroup->createUndoAction(this, tr("Undo:"));
    m_actUndo->setShortcut(QKeySequence::Undo);

    m_actRedo = m_undoGroup->createRedoAction(this, tr("Redo:"));
    m_actRedo->setShortcut(QKeySequence::Redo);

    m_menuEdit = menuBar()->addMenu(tr("&Edit"));
    m_menuEdit->addAction(m_actUndo);
    m_menuEdit->addAction(m_actRedo);
    m_menuEdit->addSeparator();
}

void MainWindow::initMenuHelp()
{
    m_menuHelp = menuBar()->addMenu(tr("&Help"));
    m_actAbout = m_menuHelp->addAction(tr("About"), this, &MainWindow::onAboutRequested, QKeySequence::HelpContents);
}

void MainWindow::initConnections()
{
    connect(m_docsManager, &document::DocumentsManager::currentDocIdChanged, this, &MainWindow::onTabSwitched);
}

void MainWindow::onOpenFileRequested()
{
    showNIY(Q_FUNC_INFO);
}

void MainWindow::onCreateFileRequested()
{
    showNIY(Q_FUNC_INFO);
}

bool MainWindow::onCloseFileRequested()
{
    showNIY(Q_FUNC_INFO);
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

void MainWindow::onQuitRequested()
{
    AppOptions::MainWindow.State.write(saveState());
    AppOptions::MainWindow.Geometry.write(saveGeometry());
    AppOptions::MainWindow.LastTab.write(m_tabWidget->currentIndex());

    //    while(hasOpenDocs())
    //        if(!onCloseFileRequested())
    //            return;

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

void MainWindow::showNIY(const QString &caller)
{
    const QString &message = QString("Not implemented yet:\n%1").arg(caller);
    qDebug() << message;
    QMessageBox::information(this, "NIY", message);
}

void MainWindow::onTabSwitched(int tab)
{
    QUndoStack *currentStack { nullptr };
    if (document::AbstractTabDocument *doc = m_docsManager->docById(tab)) {
        doc->fillToolBar(m_docToolbar);
        m_docToolbar->show();
        currentStack = doc->commandsStack();
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

void MainWindow::initTabs()
{
    using namespace document;
    auto appendTab = [this](AbstractTabDocument *doc) { m_docsManager->addDocument(doc, new QGraphicsView(this)); };

    appendTab(new DataTabDocument(this));
    appendTab(new InterfaceTabDocument(this));
    appendTab(new DeploymentTabDocument(this));
    appendTab(new ConcurrencyTabDocument(this));
    appendTab(new AADLTabDocument(this));
    appendTab(new DataTabDocument(this));
    appendTab(new MSCTabDocument(this));
}

void MainWindow::initSettings()
{
    restoreGeometry(AppOptions::MainWindow.Geometry.read().toByteArray());
    restoreState(AppOptions::MainWindow.State.read().toByteArray());
    m_tabWidget->setCurrentIndex(AppOptions::MainWindow.LastTab.read().toInt());
}

void MainWindow::updateActions()
{
    {
        bool renderAvailable(false);
        if (document::AbstractTabDocument *doc = m_docsManager->currentDoc())
            if (QGraphicsScene *scene = doc->scene()) {
                renderAvailable = !scene->sceneRect().isEmpty() && !scene->items().isEmpty();
            }
        m_actSaveSceneRender->setEnabled(renderAvailable);
    }
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

    if (document::AbstractTabDocument *doc = m_docsManager->currentDoc())
        if (QGraphicsScene *scene = doc->scene()) {
            QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
            img.fill(Qt::transparent);
            QPainter p(&img);
            scene->render(&p);
            img.save(filePath);
        }
}

} // ns taste3
