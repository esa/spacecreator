#pragma once

#include "graphicsview.h"
#include "plugin.h"

namespace msc {

class MainWindow;

class MSCPlugin : public shared::Plugin
{
    Q_OBJECT

public:
    explicit MSCPlugin(QObject *parent = nullptr);

    void setPluginActive(bool active) override;

    GraphicsView *graphicsView() override;
    void addToolBars(QMainWindow *window) override;
    void addMenuFileActions(QMenu *menu, QMainWindow *window) override;
    void addMenuEditActions(QMenu *menu, QMainWindow *window) override;

    QToolBar *mscToolBar() { return m_mscToolBar; }
    QToolBar *hierarchyToolBar() { return m_hierarchyToolBar; }

    void populateCommandLineArguments(shared::CommandLineParser *parser) const override;

    QAction *actionCopy() { return m_actionCopy; }
    QAction *actionPaste() { return m_actionPaste; }

private:
    QAction *createActionCopy(MainWindow *window);
    QAction *createActionPaste(MainWindow *window);

    GraphicsView *m_graphicsView;
    QToolBar *m_mscToolBar;
    QToolBar *m_hierarchyToolBar;

    QAction *m_actionScreenshot = nullptr;
    QAction *m_editSeparator = nullptr;
    QAction *m_actionCopy = nullptr;
    QAction *m_actionPaste = nullptr;
};

}
