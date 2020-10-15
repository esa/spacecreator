#pragma once

#include <QObject>

class QAction;
class QMainWindow;
class QMenu;
class QToolBar;
class QUndoStack;

namespace shared {

class CommandLineParser;

namespace ui {
class GraphicsViewBase;
class MiniMap;
}

class EditorCore : public QObject
{
    Q_OBJECT

public:
    explicit EditorCore(QObject *parent = nullptr);
    virtual ~EditorCore();

    // Override this to show or hide actions
    virtual void setPluginActive(bool active) = 0;

    QWidget *minimapView() const;
    virtual ui::GraphicsViewBase *chartView() = 0;
    virtual QToolBar *mainToolBar();
    virtual void addToolBars(QMainWindow *window) = 0;

    // Set up all the menus
    void initMenus(QMainWindow *window);
    virtual void addMenuFileActions(QMenu *menu, QMainWindow *window) = 0;
    virtual void addMenuEditActions(QMenu *menu, QMainWindow *window) = 0;
    virtual void addMenuViewActions(QMenu *menu, QMainWindow *window);
    virtual void addMenuHelpActions(QMenu *menu, QMainWindow *window) = 0;

    QAction *actionNewFile();
    QAction *actionOpenFile();
    QAction *actionSaveFile();
    QAction *actionSaveFileAs();
    QAction *actionOpenAadl();
    QAction *actionImport();
    QAction *actionExport();
    QAction *actionQuit();
    QAction *actionUndo();
    QAction *actionRedo();
    QAction *actionToggleMinimap();
    QAction *actionToggleE2EView();

    // Populate a CommandLineParser with the arguments this application can handle
    virtual void populateCommandLineArguments(CommandLineParser *parser) const = 0;

    void setupMiniMap();

    virtual QUndoStack *undoStack() const = 0;

    virtual bool renameAsnFile(const QString &oldName, const QString &newName) = 0;

    /// Returns the full file path of the file in this core
    virtual QString filePath() const = 0;
    /// Saves the file
    virtual bool save() = 0;

public Q_SLOTS:
    void showAboutDialog();

Q_SIGNALS:
    void editedExternally(shared::EditorCore *);

private:
    QMainWindow *m_mainWindow = { nullptr };
    QToolBar *m_mainToolBar { nullptr };

    ui::MiniMap *m_miniMap { nullptr };

    QAction *m_actionNewFile { nullptr };
    QAction *m_actionOpenFile { nullptr };
    QAction *m_actionSaveFile { nullptr };
    QAction *m_actionSaveFileAs { nullptr };
    QAction *m_actionCloseFile { nullptr };
    QAction *m_actionOpenAadl { nullptr };
    QAction *m_actionImport { nullptr };
    QAction *m_actionExport { nullptr };
    QAction *m_actionQuit { nullptr };
    QAction *m_actionUndo { nullptr };
    QAction *m_actionRedo { nullptr };
    QAction *m_actionToggleMinimap { nullptr };
    QAction *m_actionToggleE2EView { nullptr };
};

}
