#pragma once

#include "baseitems/graphicsview.h"
#include "plugin.h"

namespace aadlinterface {

class IVEditorPlugin : public shared::Plugin
{
public:
    explicit IVEditorPlugin(QObject *parent = 0);

    void setPluginActive(bool active) override;

    GraphicsView *graphicsView() override;
    void addToolBars(QMainWindow *window) override;
    void addMenuFileActions(QMenu *menu, QMainWindow *window) override;
    void addMenuEditActions(QMenu *menu, QMainWindow *window) override;

    QToolBar *docToolBar() { return m_docToolBar; }

    void populateCommandLineArguments(shared::CommandLineParser *parser) const override;

    QAction *actionSaveSceneRender() { return m_actionSaveSceneRender; }

private:
    GraphicsView *m_graphicsView;
    QToolBar *m_docToolBar;

    QAction *m_actionSaveSceneRender { nullptr };
};

}
