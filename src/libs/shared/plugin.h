#pragma once

#include <QObject>

class QToolBar;
class QAction;
class QUndoGroup;
class QMainWindow;
class QMenu;

namespace shared {

class CommandLineParser;

namespace ui {
class GraphicsViewBase;
}

class Plugin : public QObject
{
    Q_OBJECT

public:
    explicit Plugin(QObject *parent = nullptr);
    virtual ~Plugin();

    // Override this to show or hide actions
    virtual void setPluginActive(bool active) = 0;

    virtual ui::GraphicsViewBase *chartView() = 0;
    virtual QToolBar *mainToolBar();
    virtual void addToolBars(QMainWindow *window) = 0;

    // Set up all the menus
    void initMenus(QMainWindow *window);
    virtual void addMenuFileActions(QMenu *menu, QMainWindow *window) = 0;
    virtual void addMenuEditActions(QMenu *menu, QMainWindow *window) = 0;
    virtual void addMenuViewActions(QMenu *menu, QMainWindow *window) = 0;
    virtual void addMenuHelpActions(QMenu *menu, QMainWindow *window) = 0;

    QUndoGroup *undoGroup() { return m_undoGroup; }

    QAction *actionNewFile();
    QAction *actionOpenFile();
    QAction *actionSaveFile();
    QAction *actionSaveFileAs();
    QAction *actionQuit();
    QAction *actionUndo();
    QAction *actionRedo();

    // Populate a CommandLineParser with the arguments this application can handle
    virtual void populateCommandLineArguments(CommandLineParser *parser) const = 0;

private:
    QUndoGroup *m_undoGroup;

    QToolBar *m_mainToolBar { nullptr };

    QAction *m_actionNewFile { nullptr };
    QAction *m_actionOpenFile { nullptr };
    QAction *m_actionSaveFile { nullptr };
    QAction *m_actionSaveFileAs { nullptr };
    QAction *m_actionCloseFile { nullptr };
    QAction *m_actionQuit { nullptr };
    QAction *m_actionUndo { nullptr };
    QAction *m_actionRedo { nullptr };
};

}
