#include "plugin.h"

#include <QAction>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QUndoGroup>

namespace shared {

Plugin::Plugin(QObject *parent)
    : QObject(parent)
    , m_undoGroup(new QUndoGroup(this))
{
}

Plugin::~Plugin() { }

QToolBar *Plugin::mainToolBar()
{
    if (m_mainToolBar == nullptr) {
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

/*!
 * \brief initMenus Initialize the menus. Calls each menu initialization
 */
void Plugin::initMenus(QMainWindow *window)
{
    // Initialize the file menu
    auto menu = window->menuBar()->addMenu(tr("File"));

    menu->addAction(actionNewFile());
    menu->addAction(actionOpenFile());
    menu->addAction(actionSaveFile());
    menu->addAction(actionSaveFileAs());
    menu->addAction(actionCloseFile());
    menu->addSeparator();
    addMenuFileActions(menu, window);
    menu->addSeparator();
    menu->addAction(actionQuit());

    // Initialize the edit menu
    menu = window->menuBar()->addMenu(tr("Edit"));
    menu->addAction(actionUndo());
    menu->addAction(actionRedo());
    addMenuEditActions(menu, window);
}

QAction *Plugin::actionNewFile()
{
    if (m_actionNewFile == nullptr) {
        m_actionNewFile = new QAction(tr("&New file"), this);
        m_actionNewFile->setShortcut(QKeySequence::New);
        m_actionNewFile->setIcon(QIcon(QLatin1String(":/sharedresources/icons/newfile.svg")));
    }
    return m_actionNewFile;
}

QAction *Plugin::actionOpenFile()
{
    if (m_actionOpenFile == nullptr) {
        m_actionOpenFile = new QAction(tr("&Open file..."), this);
        m_actionOpenFile->setShortcut(QKeySequence::Open);
        m_actionOpenFile->setIcon(QIcon(QLatin1String(":/sharedresources/icons/open.svg")));
    }
    return m_actionOpenFile;
}

QAction *Plugin::actionSaveFile()
{
    if (m_actionSaveFile == nullptr) {
        m_actionSaveFile = new QAction(tr("&Save file"), this);
        m_actionSaveFile->setShortcut(QKeySequence::Save);
        m_actionSaveFile->setIcon(QIcon(QLatin1String(":/sharedresources/icons/save.svg")));
    }
    return m_actionSaveFile;
}

QAction *Plugin::actionSaveFileAs()
{
    if (m_actionSaveFileAs == nullptr) {
        m_actionSaveFileAs = new QAction(tr("Save file &as..."), this);
        m_actionSaveFileAs->setShortcut(QKeySequence::SaveAs);
    }
    return m_actionSaveFileAs;
}

QAction *Plugin::actionCloseFile()
{
    if (m_actionCloseFile == nullptr) {
        m_actionCloseFile = new QAction(tr("&Close file"), this);
        m_actionCloseFile->setShortcut(QKeySequence::Close);
    }
    return m_actionCloseFile;
}

QAction *Plugin::actionQuit()
{
    if (m_actionQuit == nullptr) {
        m_actionQuit = new QAction(tr("&Quit"), this);
        m_actionQuit->setShortcut(QKeySequence::Quit);
    }
    return m_actionQuit;
}

QAction *Plugin::actionUndo()
{
    if (m_actionUndo == nullptr) {
        m_actionUndo = m_undoGroup->createUndoAction(this);
        m_actionUndo->setShortcut(QKeySequence::Undo);
        m_actionUndo->setIcon(QIcon(QLatin1String(":/sharedresources/icons/undo.svg")));
    }
    return m_actionUndo;
}

QAction *Plugin::actionRedo()
{
    if (m_actionRedo == nullptr) {
        m_actionRedo = m_undoGroup->createRedoAction(this);
        m_actionRedo->setShortcut(QKeySequence::Redo);
        m_actionRedo->setIcon(QIcon(QLatin1String(":/sharedresources/icons/redo.svg")));
    }
    return m_actionRedo;
}

}
