#include "editorcore.h"

#include "minimap.h"
#include "ui/graphicsviewbase.h"

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QUndoGroup>

namespace shared {

EditorCore::EditorCore(QObject *parent)
    : QObject(parent)
    , m_undoGroup(new QUndoGroup(this))
{
}

EditorCore::~EditorCore() { }

QWidget *EditorCore::minimapView() const
{
    return m_miniMap;
}

QToolBar *EditorCore::mainToolBar()
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
void EditorCore::initMenus(QMainWindow *window)
{
    m_mainWindow = window;
    // Initialize the file menu
    auto menu = window->menuBar()->addMenu(tr("File"));

    menu->addAction(actionNewFile());
    menu->addAction(actionOpenFile());
    menu->addAction(actionSaveFile());
    menu->addAction(actionSaveFileAs());
    menu->addSeparator();
    menu->addAction(actionOpenAadl());
    menu->addSeparator();
    addMenuFileActions(menu, window);
    menu->addSeparator();
    menu->addAction(actionQuit());

    // Initialize the edit menu
    menu = window->menuBar()->addMenu(tr("Edit"));
    menu->addAction(actionUndo());
    menu->addAction(actionRedo());
    addMenuEditActions(menu, window);

    // Initialize the view menu
    menu = window->menuBar()->addMenu(tr("&View"));
    addMenuViewActions(menu, window);

    // Initialize the help menu
    menu = window->menuBar()->addMenu(tr("&Help"));
    addMenuHelpActions(menu, window);
    menu->addAction(tr("About"), this, &EditorCore::showAboutDialog);
    menu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
}

void EditorCore::addMenuViewActions(QMenu *menu, QMainWindow * /*window*/)
{
    menu->addAction(actionToggleMinimap());
    menu->addAction(actionToggleE2EView());
    menu->addSeparator();
}

QAction *EditorCore::actionNewFile()
{
    if (m_actionNewFile == nullptr) {
        m_actionNewFile = new QAction(tr("&New file"), this);
        m_actionNewFile->setShortcut(QKeySequence::New);
        m_actionNewFile->setIcon(QIcon(QLatin1String(":/sharedresources/icons/newfile.svg")));
    }
    return m_actionNewFile;
}

QAction *EditorCore::actionOpenFile()
{
    if (m_actionOpenFile == nullptr) {
        m_actionOpenFile = new QAction(tr("&Open file..."), this);
        m_actionOpenFile->setShortcut(QKeySequence::Open);
        m_actionOpenFile->setIcon(QIcon(QLatin1String(":/sharedresources/icons/open.svg")));
    }
    return m_actionOpenFile;
}

QAction *EditorCore::actionSaveFile()
{
    if (m_actionSaveFile == nullptr) {
        m_actionSaveFile = new QAction(tr("&Save file"), this);
        m_actionSaveFile->setShortcut(QKeySequence::Save);
        m_actionSaveFile->setIcon(QIcon(QLatin1String(":/sharedresources/icons/save.svg")));
    }
    return m_actionSaveFile;
}

QAction *EditorCore::actionSaveFileAs()
{
    if (m_actionSaveFileAs == nullptr) {
        m_actionSaveFileAs = new QAction(tr("Save file &as..."), this);
        m_actionSaveFileAs->setShortcut(QKeySequence::SaveAs);
    }
    return m_actionSaveFileAs;
}

QAction *EditorCore::actionOpenAadl()
{
    if (m_actionOpenAadl == nullptr) {
        m_actionOpenAadl = new QAction(tr("Open AADL file ..."), this);
    }
    return m_actionOpenAadl;
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
        m_actionUndo = m_undoGroup->createUndoAction(this);
        m_actionUndo->setShortcut(QKeySequence::Undo);
        m_actionUndo->setIcon(QIcon(QLatin1String(":/sharedresources/icons/undo.svg")));
    }
    return m_actionUndo;
}

QAction *EditorCore::actionRedo()
{
    if (m_actionRedo == nullptr) {
        m_actionRedo = m_undoGroup->createRedoAction(this);
        m_actionRedo->setShortcut(QKeySequence::Redo);
        m_actionRedo->setIcon(QIcon(QLatin1String(":/sharedresources/icons/redo.svg")));
    }
    return m_actionRedo;
}

QAction *EditorCore::actionToggleMinimap()
{
    if (m_actionToggleMinimap == nullptr) {
        m_actionToggleMinimap = new QAction(tr("&Mini map"), this);
        m_actionToggleMinimap->setCheckable(true);
    }
    return m_actionToggleMinimap;
}

QAction *EditorCore::actionToggleE2EView()
{
    if (m_actionToggleE2EView == nullptr) {
        m_actionToggleE2EView = new QAction(tr("&Show end to end dataflow"), this);
        m_actionToggleE2EView->setCheckable(true);
    }
    return m_actionToggleE2EView;
}

/*!
   Pops upd the about dialog with basic information about the application
 */
void EditorCore::showAboutDialog()
{
    QString info = QString("%1 %2").arg(qApp->applicationName(), qApp->applicationVersion());

#ifdef VS_BUILD_HASH
    info += QString(" @%1").arg(VS_BUILD_HASH);
#endif

    QMessageBox::information(m_mainWindow, tr("About"), info);
}

void EditorCore::setupMiniMap()
{
    m_miniMap = new ui::MiniMap;
    m_miniMap->setupSourceView(chartView());
    connect(actionToggleMinimap(), &QAction::toggled, m_miniMap, &shared::ui::MiniMap::setVisible);
    connect(m_miniMap, &shared::ui::MiniMap::visibilityChanged, actionToggleMinimap(), &QAction::setChecked);
}

}
