#pragma once

#include "baseitems/graphicsview.h"
#include "plugin.h"

namespace aadlinterface {

class IVEditorPlugin : public shared::Plugin
{
public:
    explicit IVEditorPlugin(QObject *parent = 0);
    ~IVEditorPlugin() override;

    GraphicsView *graphicsView() override;
    QVector<QToolBar *> additionalToolBars() override;

    QToolBar *docToolBar() { return m_docToolbar; }

private:
    GraphicsView *m_graphicsView;
    QToolBar *m_docToolbar;
};

}
