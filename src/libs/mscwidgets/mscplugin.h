#pragma once

#include "graphicsview.h"
#include "plugin.h"

namespace msc {

class MSCPlugin : public shared::Plugin
{
    Q_OBJECT

public:
    explicit MSCPlugin(QObject *parent = 0);

    GraphicsView *graphicsView() override;
    void addToolBars(QMainWindow *window) override;

    QToolBar *mscToolBar() { return m_mscToolBar; }
    QToolBar *hierarchyToolBar() { return m_hierarchyToolBar; }

    void populateCommandLineArguments(shared::CommandLineParser *parser) const override;

private:
    GraphicsView *m_graphicsView;
    QToolBar *m_mscToolBar;
    QToolBar *m_hierarchyToolBar;
};

}
