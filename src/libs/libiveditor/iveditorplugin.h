#pragma once

#include "baseitems/graphicsview.h"
#include "plugin.h"

namespace aadlinterface {

class IVEditorPlugin : public shared::Plugin
{
public:
    explicit IVEditorPlugin(QObject *parent = 0);

    GraphicsView *graphicsView() override;
    void addToolBars(QMainWindow *window) override;

    QToolBar *docToolBar() { return m_docToolBar; }

private:
    GraphicsView *m_graphicsView;
    QToolBar *m_docToolBar;
};

}
