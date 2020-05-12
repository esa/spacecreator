#pragma once

#include "graphicsview.h"
#include "plugin.h"

namespace msc {

class MSCPlugin : public shared::Plugin
{
    Q_OBJECT

public:
    explicit MSCPlugin(QObject *parent);

    GraphicsView *graphicsView() override;
    void addToolBars(QMainWindow *window) override;

    QToolBar *mscToolBar() { return m_mscToolBar; }
    QToolBar *hierarchyToolBar() { return m_hierarchyToolBar; }

private:
    GraphicsView *m_graphicsView;
    QToolBar *m_mscToolBar;
    QToolBar *m_hierarchyToolBar;
};

}
