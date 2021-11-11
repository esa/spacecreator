/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "editorcore.h"

#include "scversion.h"
#include "ui/graphicsviewbase.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QKeySequence>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QUndoStack>
#include <QUrl>

namespace shared {

EditorCore::EditorCore(QObject *parent)
    : QObject(parent)
{
}

EditorCore::~EditorCore() {}

QWidget *EditorCore::minimapView() const
{
    if (m_miniMap.isNull()) {
        const_cast<EditorCore *>(this)->setupMiniMap();
    }
    return m_miniMap;
}

QToolBar *EditorCore::mainToolBar()
{
    if (m_mainToolBar.isNull()) {
        m_mainToolBar = new QToolBar;
        m_mainToolBar->addAction(actionNewFile());
        m_mainToolBar->addAction(actionOpenFile());
        m_mainToolBar->addAction(actionSaveFile());
        m_mainToolBar->addSeparator();
        m_mainToolBar->addAction(actionUndo());
        m_mainToolBar->addAction(actionRedo());
        m_mainToolBar->setObjectName("mainToolBar");
    }
    return m_mainToolBar;
}

QAction *EditorCore::actionNewFile()
{
    if (m_actionNewFile == nullptr) {
        m_actionNewFile = new QAction(tr("&New file"), this);
        m_actionNewFile->setShortcut(QKeySequence::New);
        m_actionNewFile->setIcon(
                QIcon::fromTheme("document-new", QIcon(QLatin1String(":/sharedresources/icons/newfile.svg"))));
    }
    return m_actionNewFile;
}

QAction *EditorCore::actionOpenFile()
{
    if (m_actionOpenFile == nullptr) {
        m_actionOpenFile = new QAction(tr("&Open file..."), this);
        m_actionOpenFile->setShortcut(QKeySequence::Open);
        m_actionOpenFile->setIcon(
                QIcon::fromTheme("document-open", QIcon(QLatin1String(":/sharedresources/icons/open.svg"))));
    }
    return m_actionOpenFile;
}

QAction *EditorCore::actionSaveFile()
{
    if (m_actionSaveFile == nullptr) {
        m_actionSaveFile = new QAction(tr("&Save file"), this);
        m_actionSaveFile->setShortcut(QKeySequence::Save);
        m_actionSaveFile->setIcon(
                QIcon::fromTheme("document-save", QIcon(QLatin1String(":/sharedresources/icons/save.svg"))));
    }
    return m_actionSaveFile;
}

QAction *EditorCore::actionSaveFileAs()
{
    if (m_actionSaveFileAs == nullptr) {
        m_actionSaveFileAs = new QAction(tr("Save file &as..."), this);
        m_actionSaveFileAs->setIcon(QIcon::fromTheme("document-save-as"));
        m_actionSaveFileAs->setShortcut(QKeySequence::SaveAs);
    }
    return m_actionSaveFileAs;
}

QAction *EditorCore::actionQuit()
{
    if (m_actionQuit == nullptr) {
        m_actionQuit = new QAction(tr("&Quit"), this);
        m_actionQuit->setShortcut(QKeySequence::Quit);
    }
    return m_actionQuit;
}

QAction *EditorCore::actionUndo()
{
    if (m_actionUndo == nullptr) {
        m_actionUndo = undoStack()->createUndoAction(this);
        m_actionUndo->setIcon(QIcon::fromTheme("edit-undo", QIcon(QLatin1String(":/sharedresources/icons/undo.svg"))));
    }
    return m_actionUndo;
}

QAction *EditorCore::actionRedo()
{
    if (m_actionRedo == nullptr) {
        m_actionRedo = undoStack()->createRedoAction(this);
        m_actionRedo->setIcon(QIcon::fromTheme("edit-redo", QIcon(QLatin1String(":/sharedresources/icons/redo.svg"))));
    }
    return m_actionRedo;
}

QAction *EditorCore::actionToggleMinimap()
{
    if (m_actionToggleMinimap == nullptr) {
        m_actionToggleMinimap = new QAction(tr("&Mini map"), this);
        m_actionToggleMinimap->setCheckable(true);
        m_actionToggleMinimap->setChecked(true);
        m_actionToggleMinimap->setIcon(QIcon(QLatin1String(":/sharedresources/icons/minimap.svg")));
    }
    return m_actionToggleMinimap;
}

/*!
   Pops upd the about dialog with basic information about the application
 */
void EditorCore::showAboutDialog()
{
    QString info = QString("%1 %2 @%3").arg(qApp->applicationName(), qApp->applicationVersion(), spaceCreatorGitHash);
    QMessageBox::information(QApplication::activeWindow() ? QApplication::activeWindow() : nullptr, tr("About"), info);
}

void EditorCore::setupMiniMap()
{
    m_miniMap = new ui::MiniMap;
    m_miniMap->setupSourceView(chartView());
    m_miniMap->setVisible(actionToggleMinimap()->isChecked());
    connect(actionToggleMinimap(), &QAction::toggled, m_miniMap, &shared::ui::MiniMap::setVisible);
    connect(m_miniMap, &shared::ui::MiniMap::visibilityChanged, actionToggleMinimap(), &QAction::setChecked);
}

void EditorCore::showHelp()
{
    QDesktopServices::openUrl(helpPage());
}

QUrl EditorCore::helpPage() const
{
    return QUrl("https://taste.tuxfamily.org/wiki/index.php?title=Main_Page");
}

}
