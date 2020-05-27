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

#include "aadlobjectfunctiontype.h"
#include "baseitems/common/aadlutils.h"
#include "commandsstack.h"
#include "common.h"
#include "context/action/actionsmanager.h"
#include "interface/interfacetabdocument.h"
#include "iveditorplugin.h"
#include "reports/bugreportdialog.h"
#include "settings/appoptions.h"
#include "ui_mainwindow.h"
#include "xmldocexporter.h"
#include "zoomcontroller.h"

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

namespace aadlinterface {

/*!
\class aadlinterface::MainWindow
\brief Main appllication window - the place to store and manage supported document types, import/export data,
process command line arguments and user actions.

\sa aadlinterface::AbstractTabDocument, shared::CommandLineParser
*/

MainWindow::MainWindow(aadlinterface::IVEditorPlugin *plugin, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_zoomCtrl(new ZoomController())
    , m_document(new InterfaceTabDocument(this))
    , m_plugin(plugin)
{
    ui->setupUi(this);

    m_document->init();
    setCentralWidget(m_document->view());

    statusBar()->addPermanentWidget(m_zoomCtrl);
    m_plugin->addToolBars(this);

    // Connect the actions
    connect(m_plugin->actionNewFile(), &QAction::triggered, this, &MainWindow::onCreateFileRequested);
    connect(m_plugin->actionOpenFile(), &QAction::triggered, this, &MainWindow::onOpenFileRequested);
    connect(m_plugin->actionSaveFile(), &QAction::triggered, this, [=]() { exportXml(); });
    connect(m_plugin->actionSaveFileAs(), &QAction::triggered, this, [=]() { exportXmlAs(); });
    connect(m_plugin->actionQuit(), &QAction::triggered, this, &MainWindow::onQuitRequested);

    // Register the actions to the action manager
    ActionsManager::registerAction(Q_FUNC_INFO, m_plugin->actionNewFile(), "Create file", "Create new empty file");
    ActionsManager::registerAction(Q_FUNC_INFO, m_plugin->actionOpenFile(), "Open file", "Show Open File dialog");
    ActionsManager::registerAction(Q_FUNC_INFO, m_plugin->actionQuit(), "Quit", "Quite the application");
    ActionsManager::registerAction(Q_FUNC_INFO, m_plugin->actionUndo(), "Undo", "Undo the last operation");
    ActionsManager::registerAction(Q_FUNC_INFO, m_plugin->actionRedo(), "Redo", "Redo the last undone operation");

    connect(m_document, &document::AbstractTabDocument::dirtyChanged, this, &MainWindow::onDocDirtyChanged);

    m_plugin->initMenus(this);

    QUndoStack *currentStack { nullptr };
    if (document::AbstractTabDocument *doc = m_document) {
        doc->fillToolBar(m_plugin->docToolBar());
        currentStack = doc->commandsStack();
        if (auto view = qobject_cast<aadlinterface::GraphicsView *>(doc->view())) {
            m_zoomCtrl->setView(view);
            connect(view, &aadlinterface::GraphicsView::mouseMoved, this, &MainWindow::onGraphicsViewInfo,
                    Qt::UniqueConnection);
        }
    }

    if (currentStack) {
        if (m_plugin->undoGroup()->stacks().contains(currentStack)) {
            m_plugin->undoGroup()->addStack(currentStack);
        }
        m_plugin->undoGroup()->setActiveStack(currentStack);
    } else {
        m_plugin->undoGroup()->removeStack(m_plugin->undoGroup()->activeStack());
    }

    aadlinterface::cmd::CommandsStack::setCurrent(currentStack);

    updateActions();

    initSettings();

    updateWindowTitle();
    connect(m_document, &InterfaceTabDocument::titleChanged, this, &MainWindow::updateWindowTitle);
}

/*!
 * \brief D-tor clears the stuff generated by QDesigner.
 */
MainWindow::~MainWindow()
{
    delete ui;
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
 * \brief Handler for File->open action.
 */
void MainWindow::onOpenFileRequested()
{
    const QString prevPath(m_document->path());
    const QString &fileName =
            QFileDialog::getOpenFileName(this, tr("Open file"), prevPath, m_document->supportedFileExtensions());
    if (!fileName.isEmpty() && closeFile()) {
        m_document->load(fileName);
    }
}

/*!
 * \brief Handler for File->create action.
 */
void MainWindow::onCreateFileRequested()
{
    if (closeFile()) {
        m_document->create();
    }
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

/*!
 * \brief Exports the document document to the output file \a savePath using the template file \a templatePath.
 * Returns \c true on succes.
 */
bool MainWindow::exportXml(const QString &savePath, const QString &templatePath)
{
    return XmlDocExporter::exportDocSilently(m_document, savePath, templatePath);
}

/*!
 * \brief Shows the preview dialog for exportin the document to the output file \a savePath
 * using the template file \a templatePath.
 * If a \a savePath or \a templatePath is empty, user will be asked to select the file via QFileDialog.
 * Returns true if the preview dialog can not be shown.
 */
bool MainWindow::exportXmlAs(const QString &savePath, const QString &templatePath)
{
    return XmlDocExporter::exportDocInteractive(m_document, this, savePath, templatePath);
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

void MainWindow::onReportRequested()
{
    QList<QPixmap> images;
    if (document::AbstractTabDocument *doc = m_document) {
        if (QGraphicsScene *scene = doc->scene()) {
            const QSize sceneSize = scene->sceneRect().size().toSize();
            if (!sceneSize.isNull()) {
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
    auto dialog = new reports::BugreportDialog(images, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}

void MainWindow::initSettings()
{
    restoreGeometry(aadlinterface::AppOptions::MainWindow.Geometry.read().toByteArray());
    restoreState(aadlinterface::AppOptions::MainWindow.State.read().toByteArray());
}

void MainWindow::updateActions()
{
    bool renderAvailable(false);
    if (QGraphicsScene *scene = m_document->scene()) {
        renderAvailable = !scene->sceneRect().isEmpty() && !scene->items().isEmpty();
    }

    m_plugin->actionSaveFile()->setEnabled(m_document->isDirty());
    m_plugin->actionSaveSceneRender()->setEnabled(renderAvailable);
}

/*!
 * \brief Handles startup options from command line
 * \a arg - the option to be processed,
 * \a value - its value.
 * Returns \c true if the option \a arg \a value is known and handled.
 */
bool MainWindow::processCommandLineArg(shared::CommandLineParser::Positional arg, const QString &value)
{
    switch (arg) {
    case shared::CommandLineParser::Positional::DropUnsavedChangesSilently: {
        m_dropUnsavedChangesSilently = true;
        return true;
    }
    case shared::CommandLineParser::Positional::OpenAADLXMLFile: {
        if (!value.isEmpty()) {
            if (document::AbstractTabDocument *doc = m_document) {
                return doc->load(value);
            }
        };

        return false;
    }
    case shared::CommandLineParser::Positional::OpenStringTemplateFile:
        if (!value.isEmpty())
            return exportXmlAs(QString(), value);
        return false;
    case shared::CommandLineParser::Positional::ExportToFile:
        if (!value.isEmpty())
            return exportXml(value);
        return false;
    case shared::CommandLineParser::Positional::ListScriptableActions: {
        ActionsManager::listRegisteredActions();
        QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
        return true;
    }
    default:
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

    if (QGraphicsScene *scene = m_document->scene()) {
        QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
        img.fill(Qt::transparent);
        QPainter p(&img);
        scene->render(&p);
        img.save(filePath);
    }
}

/*!
 * \brief Updates the title of a tab related to the updated doc.
 * (Adds or removes the "*" sign when doc is changed/saved).
 */
void MainWindow::onDocDirtyChanged(bool dirty)
{
    setWindowModified(dirty);
    updateActions();
}

/*!
 * \brief Shows the \a info message from GraphicsView in the status bar.
 */
void MainWindow::onGraphicsViewInfo(const QString &info)
{
    statusBar()->showMessage(info);
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(QString("Interface View Editor [%1][*]").arg(m_document->title()));
}

/*!
 * Performs check for unsaved changes, asking the user to save, if necessary.
 * Returns true if the file is closed.
 */
bool MainWindow::closeFile()
{
    if (m_document->isDirty() && !m_dropUnsavedChangesSilently) {
        const QMessageBox::StandardButtons btns(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
        auto btn = QMessageBox::question(this, tr("Document closing"),
                tr("There are unsaved changes.\nWould you like to save the document?"), btns);
        if (btn == QMessageBox::Save) {
            if (!exportXml()) {
                return false;
            }
        } else if (btn == QMessageBox::Cancel) {
            return false;
        }
    }

    m_document->close();

    return true;
}

/*!
 * \brief Closes documents, attempting to save changes, if any.
 * If user declyne request to save cahnges, returns \c false.
 */
bool MainWindow::prepareQuit()
{
    if (!closeFile()) {
        return false;
    }

    aadlinterface::AppOptions::MainWindow.State.write(saveState());
    aadlinterface::AppOptions::MainWindow.Geometry.write(saveGeometry());

    return true;
}

}
