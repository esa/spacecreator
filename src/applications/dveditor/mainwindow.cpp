﻿/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "colors/colormanagerdialog.h"
#include "config/dvhwlibrarydialog.h"
#include "dvappmodel.h"
#include "dveditorcore.h"
#include "errorhub.h"
#include "settingsmanager.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>

namespace dve {

MainWindow::MainWindow(dve::DVEditorCore *core, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_core(core)
{
    ui->setupUi(this);
    initSettings();

    initActions();
    initMenus();

    setCentralWidget(core->mainwidget());
    addToolBar(core->toolBar());

    QString hwFile = shared::SettingsManager::load<QString>(shared::SettingsManager::DVE::HwLibraryFile, "");
    m_core->loadHWLibrary(hwFile);
}

/*!
 * \brief D-tor clears the stuff generated by QDesigner.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSaveFileRequested()
{
    m_core->save();
}

void MainWindow::onSaveFileAsRequested()
{
    m_core->saveAs();
}

void MainWindow::onOpenFileRequested()
{
    const QString prevPath(m_core->appModel()->path());
    const QString &fileName = QFileDialog::getOpenFileName(
            this, tr("Open file"), prevPath, m_core->appModel()->supportedFileExtensions());
    if (!fileName.isEmpty() && closeFile()) {
        shared::ErrorHub::clearErrors();
        m_core->appModel()->load(fileName);
        m_core->centerOnView();
        if (shared::ErrorHub::hasErrors()) {
            QMessageBox::warning(this, tr("File load warnings"), shared::ErrorHub::errorDescriptions().join("\n"));
        }
    }
    m_core->actionSaveFile()->setEnabled(m_core->appModel()->isDirty());
}

void MainWindow::showColorScheme()
{
    shared::ColorManagerDialog *dialog = new shared::ColorManagerDialog(window());
    dialog->setFilterGroup("DVE");
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void MainWindow::editHwLibrary()
{
    DVHWLibraryDialog dialog;
    int ret = dialog.exec();
    if (ret == QDialog::Accepted) {
        QString hwFile = shared::SettingsManager::load<QString>(shared::SettingsManager::DVE::HwLibraryFile, "");
        m_core->loadHWLibrary(hwFile);
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

void MainWindow::initActions()
{
    // Connect the core actions
    connect(m_core->actionOpenFile(), &QAction::triggered, this, &MainWindow::onOpenFileRequested);

    connect(m_core->actionSaveFile(), &QAction::triggered, this, &MainWindow::onSaveFileRequested);
    connect(m_core->actionSaveFileAs(), &QAction::triggered, this, &MainWindow::onSaveFileAsRequested);

    connect(m_core->actionQuit(), &QAction::triggered, this, &MainWindow::onQuitRequested);
}

void MainWindow::initMenus()
{
    // Initialize the file menu
    auto menu = menuBar()->addMenu(tr("File"));
    menu->addAction(m_core->actionOpenFile());
    menu->addAction(m_core->actionSaveFile());
    menu->addAction(m_core->actionSaveFileAs());
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
    menu->addSeparator();
    QAction *colorAction = menu->addAction(tr("Color Scheme..."));
    connect(colorAction, &QAction::triggered, this, &MainWindow::showColorScheme);
    QAction *hwAction = menu->addAction(tr("Hardware Library..."));
    connect(hwAction, &QAction::triggered, this, &MainWindow::editHwLibrary);

    // Initialize the help menu
    menu = menuBar()->addMenu(tr("&Help"));
    menu->addAction(tr("About"), m_core, &shared::EditorCore::showAboutDialog);
    menu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
}

void MainWindow::onQuitRequested()
{
    if (prepareQuit()) {
        qApp->quit();
    }
}

void MainWindow::initSettings()
{
    // Use the application settings, and not the common SpaceCreator settings
    QSettings settings;
    restoreGeometry(settings.value("Common/Geometry").toByteArray());
    restoreState(settings.value("Common/State").toByteArray());
}

bool MainWindow::closeFile()
{
    if (m_core->appModel()->isDirty()) {
        const QMessageBox::StandardButtons btns(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
        auto btn = QMessageBox::question(this, tr("Document closing"),
                tr("There are unsaved changes.\nWould you like to save the document?"), btns);
        if (btn == QMessageBox::Save) {
            // @todo implement save
        } else if (btn == QMessageBox::Cancel) {
            return false;
        }
    }

    m_core->appModel()->close();

    return true;
}

/*!
 * \brief Updates the title of a tab related to the updated doc.
 * (Adds or removes the "*" sign when doc is changed/saved).
 */
void MainWindow::onDocDirtyChanged(bool dirty)
{
    setWindowModified(dirty);
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(QString("Deployment View Editor [%1][*]").arg("DOCUMENT_NAME"));
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

    // Use the application settings, and not the common SpaceCreator settings
    QSettings settings;
    settings.setValue("Common/State", saveState());
    settings.setValue("Common/Geometry", saveGeometry());

    return true;
}

}
