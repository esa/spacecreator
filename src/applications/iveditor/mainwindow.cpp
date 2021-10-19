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

#include "asn1dialog.h"
#include "commands/cmdchangeasn1file.h"
#include "commandsstack.h"
#include "common.h"
#include "config/ivlibrarydialog.h"
#include "context/action/actionsmanager.h"
#include "endtoend/endtoendview.h"
#include "errorhub.h"
#include "interfacedocument.h"
#include "itemeditor/common/ivutils.h"
#include "ivappwidget.h"
#include "iveditorcore.h"
#include "ivexporter.h"
#include "ivfunctiontype.h"
#include "minimap.h"
#include "reports/bugreportdialog.h"
#include "settingsmanager.h"
#include "ui_mainwindow.h"
#include "zoomcontroller.h"

#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsView>
#include <QMessageBox>
#include <QStatusBar>
#include <QTabWidget>
#include <QTreeView>
#include <QUndoCommand>
#include <QUndoGroup>
#include <QWindow>
#include <iostream>

namespace ive {

/*!
\class ive::MainWindow
\brief Main application window - the place to store and manage supported document types, import/export data,
process command line arguments and user actions.

\sa ive::InterfaceDocument, shared::CommandLineParser
*/

MainWindow::MainWindow(ive::IVEditorCore *core, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_zoomCtrl(new ZoomController())
    , m_core(core)
{
    ui->setupUi(this);

    setCentralWidget(m_core->document()->view());

    statusBar()->addPermanentWidget(m_zoomCtrl);
    m_core->addToolBars(this);

    // Connect the actions
    connect(m_core->actionNewFile(), &QAction::triggered, this, &MainWindow::onCreateFileRequested);
    connect(m_core->actionOpenFile(), &QAction::triggered, this, &MainWindow::onOpenFileRequested);
    connect(m_core->actionSaveFile(), &QAction::triggered, this, [=]() { exportXml(); });
    connect(m_core->actionSaveFileAs(), &QAction::triggered, this, [=]() { exportXmlAs(); });
    connect(m_core->actionQuit(), &QAction::triggered, this, &MainWindow::onQuitRequested);
    connect(m_core->actionExportFunctions(), &QAction::triggered, this, &MainWindow::onExportFunctionsRequested);
    connect(m_core->actionExportType(), &QAction::triggered, this, &MainWindow::onExportTypeRequested);

    // Register the actions to the action manager
    ActionsManager::registerAction(Q_FUNC_INFO, m_core->actionNewFile(), "Create file", "Create new empty file");
    ActionsManager::registerAction(Q_FUNC_INFO, m_core->actionOpenFile(), "Open file", "Show Open File dialog");
    ActionsManager::registerAction(Q_FUNC_INFO, m_core->actionQuit(), "Quit", "Quit the application");
    m_core->registerBasicActions();

    connect(m_core->document(), &InterfaceDocument::dirtyChanged, this, &MainWindow::onDocDirtyChanged);

    initMenus();

    if (auto view = m_core->chartView()) {
        m_zoomCtrl->setView(view);
        connect(view, &ive::GraphicsView::mouseMoved, this, &MainWindow::onGraphicsViewInfo, Qt::UniqueConnection);
    }

    updateActions();

    initSettings();

    updateWindowTitle();
    connect(m_core->document(), &InterfaceDocument::titleChanged, this, &MainWindow::updateWindowTitle);

    m_core->setupMiniMap();

    connect(m_core->document()->exporter(), &IVExporter::exported, m_core->document(),
            &InterfaceDocument::onSavedExternally);

    // Create the E2E view and add the action
    auto endToEndView = new EndToEndView(m_core->document(), this);
    endToEndView->hide();
    connect(core->actionToggleE2EView(), &QAction::toggled, endToEndView, &QWidget::setVisible);
    connect(endToEndView, &EndToEndView::visibleChanged, core->actionToggleE2EView(), &QAction::setChecked);
    endToEndView->setVisible(core->actionToggleE2EView()->isChecked());

    connect(shared::ErrorHub::instance(), &shared::ErrorHub::errorAdded, this, [this](const shared::ErrorItem &error) {
        switch (error.m_type) {
        case shared::ErrorItem::TaskType::Error:
            QMessageBox::critical(this, tr("Error"), shared::ErrorHub::errorDescriptions().join(QLatin1Char('\n')));
            break;
        case shared::ErrorItem::TaskType::Warning:
            QMessageBox::warning(this, tr("Warning"), shared::ErrorHub::errorDescriptions().join(QLatin1Char('\n')));
            break;
        default:
            break;
        }
        shared::ErrorHub::clearErrors();
    });
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
    const QString prevPath(m_core->document()->path());
    const QString &fileName = QFileDialog::getOpenFileName(
            this, tr("Open file"), prevPath, m_core->document()->supportedFileExtensions());
    if (!fileName.isEmpty() && closeFile()) {
        m_core->document()->load(fileName);
    }
    updateActions();
}

/*!
 * \brief Handler for File->create action.
 */
void MainWindow::onCreateFileRequested()
{
    if (closeFile()) {
        m_core->document()->create();
    }
}

void MainWindow::onExportFunctionsRequested()
{
    m_core->document()->exportSelectedFunctions();
}

void MainWindow::onExportTypeRequested()
{
    m_core->document()->exportSelectedType();
}

/*!
 * \brief Exports the document document to the output file \a savePath using the template file \a templatePath.
 * Returns \c true on success.
 */
bool MainWindow::exportXml(const QString &savePath, const QString &templatePath)
{
    return m_core->document()->exporter()->exportDocSilently(m_core->document(), savePath, templatePath);
}

/*!
 * \brief Shows the preview dialog for exporting the document to the output file \a savePath
 * using the template file \a templatePath.
 * If a \a savePath or \a templatePath is empty, user will be asked to select the file via QFileDialog.
 * Returns true if the preview dialog can not be shown.
 */
bool MainWindow::exportXmlAs(const QString &savePath, const QString &templatePath)
{
    return m_core->document()->exporter()->exportDocInteractively(m_core->document(), savePath, templatePath);
}

void MainWindow::onQuitRequested()
{
    if (prepareQuit())
        qApp->quit();
}

void MainWindow::onReportRequested()
{
    QList<QPixmap> images;
    if (m_core->document() != nullptr) {
        if (QGraphicsScene *scene = m_core->document()->scene()) {
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
    restoreGeometry(shared::SettingsManager::load<QByteArray>(shared::SettingsManager::Common::Geometry));
    restoreState(shared::SettingsManager::load<QByteArray>(shared::SettingsManager::Common::State));

    const bool showMinimap = shared::SettingsManager::load<bool>(shared::SettingsManager::Common::ShowMinimap);
    m_core->actionToggleMinimap()->setChecked(showMinimap);
}

void MainWindow::updateActions()
{
    bool renderAvailable(false);
    if (QGraphicsScene *scene = m_core->document()->scene()) {
        renderAvailable = !scene->sceneRect().isEmpty() && !scene->items().isEmpty();
    }

    m_core->actionSaveFile()->setEnabled(m_core->document()->isDirty());
    m_actionSaveSceneRender->setEnabled(renderAvailable);
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
    case shared::CommandLineParser::Positional::OpenXMLFile: {
        if (!value.isEmpty() && m_core->document() != nullptr) {
            shared::ErrorHub::clearErrors();
            bool ok = m_core->document()->load(value);
            if (shared::ErrorHub::hasErrors()) {
                QMessageBox::warning(this, tr("File load warnings"), shared::ErrorHub::errorDescriptions().join("\n"));
            }
            return ok;
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
        QString list = ActionsManager::listRegisteredActions();
        std::cout << list.toStdString() << std::endl;
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
    QString fileName = QFileInfo(m_core->document()->path()).fileName();
    if (fileName.isEmpty()) {
        fileName = tr("Untitled");
    }
    setWindowTitle(QString("Interface View Editor [%1][*]").arg(fileName));
}

/*!
   Show the dialog to set the ASN1 file
 */
void MainWindow::openAsn1Dialog()
{
    Asn1Dialog dialog;
    QFileInfo fi(m_core->document()->asn1FilePath());
    dialog.setFile(fi);
    dialog.show();
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        if (m_core->document()->asn1FileName() != dialog.fileName()) {
            auto command = new cmd::CmdChangeAsn1File(m_core->document(), dialog.fileName());
            m_core->commandsStack()->push(command);
        }
    }
}

void MainWindow::editIVLibrary()
{
    IVLibraryDialog dialog;
    dialog.exec();
}

void MainWindow::initMenus()
{
    // Initialize the file menu
    auto menu = menuBar()->addMenu(tr("File"));
    menu->addAction(m_core->actionNewFile());
    menu->addAction(m_core->actionOpenFile());
    menu->addAction(m_core->actionSaveFile());
    menu->addAction(m_core->actionSaveFileAs());
    menu->addSeparator();
    menu->addSeparator();
    menu->addAction(m_core->actionExportFunctions());
    menu->addAction(m_core->actionExportType());
    menu->addSeparator();
    m_actionSaveSceneRender = menu->addAction(QIcon(QLatin1String(":/toolbar/icns/render.svg")), tr("Render Scene..."),
            m_core, &IVEditorCore::onSaveRenderRequested);
    ActionsManager::registerAction(
            Q_FUNC_INFO, m_actionSaveSceneRender, "Render", "Save current scene complete render.");
    m_actionShowAsnDialog = menu->addAction(tr("ASN1 dialog..."), this, &MainWindow::openAsn1Dialog);
    ActionsManager::registerAction(Q_FUNC_INFO, m_actionShowAsnDialog, "Asn1", "Edit the ASN1 file");

    menu->addSeparator();
    menu->addAction(m_core->actionQuit());

    // Initialize the edit menu
    menu = menuBar()->addMenu(tr("Edit"));
    QAction *undoAction = m_core->actionUndo();
    undoAction->setShortcut(QKeySequence::Undo);
    menu->addAction(undoAction);
    QAction *redoAction = m_core->actionRedo();
    redoAction->setShortcut(QKeySequence::Redo);
    menu->addAction(redoAction);
    QMenu *root = new QMenu(tr("Common Settings"));
    root->addActions(m_core->document()->customActions());
    auto action = root->addAction(tr("IV Library settings"), this, &MainWindow::editIVLibrary);
    ActionsManager::registerAction(Q_FUNC_INFO, action, "IV Library", "Edit IV Library pathes");

    menu->addMenu(root);

    // Initialize the view menu
    menu = menuBar()->addMenu(tr("&View"));
    menu->addAction(m_core->actionToggleMinimap());
    menu->addAction(m_core->actionToggleE2EView());

    // Initialize the help menu
    menu = menuBar()->addMenu(tr("&Help"));
    menu->addAction(tr("Help"), m_core, &shared::EditorCore::showHelp);
    menu->addSeparator();
    auto report = menu->addAction(tr("Send report..."), this, &MainWindow::onReportRequested);
    ActionsManager::registerAction(Q_FUNC_INFO, report, "Report", "Send the debug information");
    menu->addAction(tr("About"), m_core, &shared::EditorCore::showAboutDialog);
    menu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
}

/*!
 * Performs check for unsaved changes, asking the user to save, if necessary.
 * Returns true if the file is closed.
 */
bool MainWindow::closeFile()
{
    if (m_core->document()->isDirty() && !m_dropUnsavedChangesSilently) {
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

    m_core->document()->close();

    return true;
}

/*!
 * \brief Closes documents, attempting to save changes, if any.
 * If user decline request to save changes, returns \c false.
 */
bool MainWindow::prepareQuit()
{
    if (!closeFile()) {
        return false;
    }

    shared::SettingsManager::store<QByteArray>(shared::SettingsManager::Common::State, saveState());
    shared::SettingsManager::store<QByteArray>(shared::SettingsManager::Common::Geometry, saveGeometry());

    return true;
}

}
